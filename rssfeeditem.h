#ifndef RSSFEEDITEM_H
#define RSSFEEDITEM_H

#include <QUrl>
#include <QString>
#include <QTime>
#include <QDateTime>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QVariant>

class rssFeedItem
{
public:
	rssFeedItem (QSqlRecord * src = 0);
    void bindItem (QSqlQuery & query);

    QUrl mp3Url;
    QUrl link;   // —сылка на описание

    QString localMp3;
    QString title;
    QString description;
    QString GUID;
    QString showNotes;
    double duration;
    QDateTime creation;

    bool played;
    bool downloaded;
    QDateTime ustareet;
    QUrl src;
    bool stillInFeed;
};

Q_DECLARE_METATYPE(rssFeedItem)

#endif // RSSFEEDITEM_H
