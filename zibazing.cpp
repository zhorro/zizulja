#include "zibazing.h"
#include <QFile>
#include <QDebug>
#include <QCoreApplication>

zibazing ziba;
zibazing::zibazing(QObject *parent) :
    QObject(parent),
    set ("RB", "Zyazyulya", this)
{
}

QDir zibazing::podcastsDir ()
{
    QDir l_podcastsDir (set.value("podcastsDir", QDir::homePath()+"/Podcasts").toString());
    set.setValue("podcastsDir", l_podcastsDir.absolutePath());
    return l_podcastsDir;
}

int  zibazing::updateEveryHours ()
{
    int l_updateEveryHours = set.value("updateEveryHours", 24).toInt();
    set.setValue("updateEveryHours", l_updateEveryHours);
    return l_updateEveryHours;
}


QDateTime zibazing::nextUpdate()
{
    QDateTime l_nextUpdate = set.value("nextUpdate", QVariant(QDateTime::currentDateTime().addSecs(3600*updateEveryHours()))).toDateTime();
    set.setValue("nextUpdate", l_nextUpdate);
    return l_nextUpdate;
}

bool zibazing::downloadShowsAutomatically()
{
    bool l_downloadShowsAutomatically = set.value("downloadShowsAutomatically", false).toBool ();
    set.setValue("downloadShowsAutomatically", l_downloadShowsAutomatically);
    return l_downloadShowsAutomatically;
}

int  zibazing::deleteOldShowsInADays()
{
    int l_deleteOldShowsInADays = set.value("deleteOldShowsInADays", 7).toInt ();
    set.setValue("deleteOldShowsInADays", l_deleteOldShowsInADays);
    return l_deleteOldShowsInADays;
}

int  zibazing::markShowsAsOldInADays()
{
    int l_markShowsAsOldInADay = set.value("markShowsAsOldInADay", -1).toInt ();
    set.setValue("markShowsAsOldInADay", l_markShowsAsOldInADay);
    return l_markShowsAsOldInADay;
}

void zibazing::updateDone ()
{

}

QString zibazing::dbaseName()
{
    QString l_DBaseName = set.value("DBaseName", QVariant(QString())).toString();
    if (l_DBaseName.isEmpty())
    {
        // TODO: Решить таки, где хранить базу!
        // l_DBaseName = QDir::homePath()+"/ziba.sqlite";
        l_DBaseName = qApp->applicationDirPath() + "/ziba.sqlite";
        qDebug() << "Creating dBase at: " << l_DBaseName;
        // Copy default db
        if (QFile::copy ( ":/default/podcast_db.sqlite", l_DBaseName ))
            set.setValue("DBaseName", l_DBaseName);
    }
    return l_DBaseName;
}
