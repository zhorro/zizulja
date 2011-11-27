#include <QtSql>
#include <cstdlib>
#include <cmath>

#include "podcastsdb.h"
#include "rssfeeditem.h"
#include "zibazing.h"

podcastsDB pdb;
podcastsDB::podcastsDB(QObject *parent) :
    QThread(parent),
    upa(this),
    dq(this)
{
    connect(&upa, SIGNAL(startUpdate(QUrl)), this, SLOT(startUpdate (QUrl)));
    connect(&upa, SIGNAL(gotNewRss(rssFeedItem, QUrl)), this, SLOT(addFeed(rssFeedItem, QUrl)));
    connect(&upa, SIGNAL(doneUpdate(QUrl)), this, SLOT(finishUpdate (QUrl)));
    connect(&upa, SIGNAL(gotFeedImage(QUrl, QString)), this, SLOT(updateImage (QUrl, QString)));
    connect(&upa, SIGNAL(gotFeedTitle(QUrl, QString)), this, SLOT(updateTitle (QUrl, QString)));

    connect(this, SIGNAL(addDownloadInQueue(QString, QString)), &dq, SLOT(addDownload(QString, QString)));
    connect(this, SIGNAL(updateUrl(QUrl)), &upa, SLOT(update (QUrl)));
}

void podcastsDB::run ()
{
    workingFolder = QDir().currentPath(); //TODO: Переделать на чтение из QSetings

    dbase = QSqlDatabase::addDatabase("QSQLITE");
    dbase.setDatabaseName(ziba.dbaseName());

    if (!dbase.open()) {
        // ААаааа!!   Где моя базочка!!!
        qDebug() << "ААаааа!!   Где моя базочка!!!";
        throw QString ("Не открывается база по адресу %1").arg(ziba.dbaseName());
    }

    QSqlQuery queryOfCreatingPeredachki(dbase);
    if (queryOfCreatingPeredachki.exec (
                "CREATE TABLE peredachki ("
                "id                 INTEGER     PRIMARY KEY,"
                "mp3                TEXT,"
                "url                TEXT,"
                "played             BOOLEAN,"
                "downloaded         BOOLEAN,"
                "created            TIME,"
                "duration           DOUBLE,"
                "lastPlayedPosition DOUBLE,"
                "description        TEXT,"
                "shownotes          TEXT,"
                "ustareet           TIME,"
                "src                TEXT,"
                "stillInFeed        BOOLEAN,"
                "link               TEXT,"
                "updating           BOOLEAN,"
                "title              TEXT,"
                "GUID               TEXT"
                ")"))
            qDebug () << "TABLE peredachki successfuly created";
    else
            qDebug () << "TABLE peredachki not created: " << queryOfCreatingPeredachki.lastError ();

//CREATE TABLE sources (title text,"rss_url" text,"index" integer NOT NULL PRIMARY KEY AUTOINCREMENT,icon text,zivka integer)
    QSqlQuery queryOfCreatingSources(dbase);
    if (queryOfCreatingSources.exec (
                "CREATE TABLE sources ("
                "id                 INTEGER     PRIMARY KEY,"
                "title              TEXT,"
                "rss_url            TEXT,"
                "icon               TEXT,"
                "zivka              INTEGER"
                ")"))
            qDebug () << "TABLE sources successfuly created";
    else
            qDebug () << "TABLE sources not created: " << queryOfCreatingSources.lastError ();
}

podcastsDB::~podcastsDB()
{
    dbase.close();
    QSqlDatabase::removeDatabase(ziba.dbaseName());
    exit();
    wait();
}

bool podcastsDB::exists (QUrl url)
{
    QSqlQuery query = feedQuery(url);

    if (query.next())
        return true;
    return false;
}

void podcastsDB::append (QUrl url)
{
    QSqlQuery query (dbase);
    query.prepare("INSERT INTO sources (rss_url)"
                  "VALUES(:url)");
    query.bindValue(":url", url.toString());
    if (!query.exec())
        qDebug() << "error appending. SQL:" << query.executedQuery();
}

void podcastsDB::addFeed (rssFeedItem item, QUrl src)
{
    QSqlQuery query (dbase);
    query.prepare("SELECT * FROM peredachki WHERE "
                  "src = :src & url = :url");
    query.bindValue(":src", src.toString());
    query.bindValue(":url", item.mp3Url.toString());
    if (!query.exec())
        qDebug() << "error finding. SQL:" << query.executedQuery();

    if (query.next())
    {
        query.record().field("stillInFeed").setValue(true);
        query.record().field("updating").setValue(false);
    }
    else
    {
        QSqlQuery query (dbase);

        query.prepare("INSERT INTO peredachki "
                      " (mp3, url, played, downloaded, created, duration, description, shownotes, ustareet, stillInFeed, link, updating, title, GUID) "
                      "VALUES "
                      "(:mp3,:url,:played,:downloaded,:created,:duration,:description,:shownotes,:ustareet,:stillInFeed,:link,:updating,:title,:GUID)");

        item.bindItem(query);

        if (!query.exec())
            qDebug() << "error inserting. SQL:" << query.executedQuery();

    }
}

void podcastsDB::startUpdate (QUrl src)
{
    QSqlQuery query (dbase);
    query.prepare("UPDATE peredachki "
                  "SET updating=:newval "
                  "WHERE src = :src");
    query.bindValue(":newval", true);
    query.bindValue(":src", src.toString());
    if (!query.exec())
        qDebug() << "error finding. SQL:" << query.executedQuery();
}

void podcastsDB::finishUpdate (QUrl src)
{
    QSqlQuery query (dbase);
    query.prepare("SELECT * FROM peredachki WHERE "
                  "src = :src & updating = :updating");
    query.bindValue(":src", src.toString());
    query.bindValue(":updating", true);
    if (!query.exec())
        qDebug() << "error finding. SQL:" << query.executedQuery();
    while (query.next())
    {
        QSqlRecord record = query.record();
        rssFeedItem rss (&record);
        removeItem (rss, src);
    }

    updateNext();
}

void podcastsDB::removeItem (rssFeedItem item, QUrl src)
{
    if (!item.localMp3.isEmpty())
    {
        QFile file (item.localMp3);
        file.remove();
    }
    QSqlQuery query (dbase);
    query.prepare("DELETE * FROM peredachki WHERE "
                  "src = :src & updating = :updating");
    query.bindValue(":src", src.toString());
    query.bindValue(":url", item.mp3Url);
    query.exec();
}

void podcastsDB::update(QUrl u)
{
    if (!upa.isRunning())
        upa.start();

    if (!exists(u))
        append(u);

    emit updateUrl(u);
}

void podcastsDB::updateAll(void)
{
    allRecords = QSqlQuery (dbase);
    allRecords.prepare("SELECT * FROM sources"); // Всех
    if (!allRecords.exec())
        qDebug() << "error finding. SQL:" << allRecords.executedQuery();
    updateNext();
}

void podcastsDB::updateNext(void)
{
    if (allRecords.next())
        update(allRecords.record().field("rss_url").value().toUrl());
}

void podcastsDB::updateImage (QUrl src, QString imgUrl)
{
    QFileInfo dst (workingFolder, imgName(src));
    emit addDownloadInQueue (imgUrl, dst.absoluteFilePath());
}

void podcastsDB::updateTitle (QUrl src, QString newTitle)
{
    QSqlQuery query = feedQuery (src);
    if (query.next())
        query.record().setValue("title",newTitle);
    else
        qDebug () << "shift happen";
}

QString podcastsDB::imgName (QUrl rssUrl)
{
    QSqlQuery query (dbase);
    query.prepare("SELECT * FROM sources WHERE "
                  "rss_url = :url;");
    query.bindValue(":url", rssUrl.toString());
    if (!query.exec())
        qDebug() << "error finding. SQL:" << query.executedQuery();

    if (query.next())
    {
        QString res = query.record().value("icon").toString();
        if (!res.isEmpty())
            return res;
        // Генерируем имя иконки из урла
        qDebug () << rssUrl.encodedQuery();
        QFileInfo fi (rssUrl.encodedQuery());
        qDebug() << fi.fileName();
        while (1)
        {
            QSqlQuery queryIfSuchNameEists (dbase);
            queryIfSuchNameEists.prepare("SELECT * FROM sources WHERE "
                                         "rss_url = :url;"
                                         "icon = :icon;");
            queryIfSuchNameEists.bindValue(":url", rssUrl.toString());
            queryIfSuchNameEists.bindValue(":icon", fi.baseName());
            if (!queryIfSuchNameEists.exec())
                qDebug() << "error finding. SQL:" << queryIfSuchNameEists.executedQuery();
            if (!queryIfSuchNameEists.next())
            {
                query.record().setValue("icon", fi.fileName());
                return fi.fileName();

            }
            fi.setFile(fi.bundleName()+QString("(%1)").arg(std::rand()%10) + fi.suffix());
            qDebug() << fi.fileName();
        }

    }
    else
        throw QString ("Обращение к несозданному полю");
}

QSqlQuery podcastsDB::feedQuery(QUrl url)
{
    QSqlQuery query(dbase);
    query.prepare("SELECT * FROM sources WHERE "
                  "rss_url = :url;");
    query.bindValue(":url", url.toString());
    if (!query.exec())
        qDebug() << "error finding. SQL:" << query.executedQuery();
    return query;
}

QSqlQuery podcastsDB::feedView ()
{
    QSqlQuery query(dbase);
    query.prepare("SELECT rss_url FROM sources");
    if (!query.exec())
        qDebug() << "error finding. SQL:" << query.executedQuery();
    return query;
}


