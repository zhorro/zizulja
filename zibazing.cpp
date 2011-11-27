#include "zibazing.h"
#include <QFile>
#include <QFileInfo>
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
    QDateTime l_nextUpdate = QDateTime::currentDateTime().addSecs(3600*updateEveryHours());
    set.setValue("nextUpdate", l_nextUpdate);
}

QString zibazing::dbaseName()
{
    // Переделать на создание новой, пустой базы данных. А то вдруг там бинарно не совместимая гадость?
    QString dbName = podcastsDir().cleanPath("podcast_db.sqlite");
    qDebug() << "using db name: " << dbName;
    return dbName;
}
