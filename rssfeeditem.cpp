#include <QUrl>
#include <QVariant>
#include <QSqlRecord>

#include "rssfeeditem.h"

rssFeedItem::rssFeedItem (QSqlRecord * f)
{
    if (f)
    {
        mp3Url      = f->value("url").toUrl();
        link        = f->value("link").toUrl();

        localMp3    = f->value("mp3").toString();
        title       = f->value("title").toString();
        description = f->value("description").toString();
        GUID        = f->value("GUID").toString();
        duration    = f->value("duration").toDouble();
        creation    = f->value("created").toDateTime();
        showNotes   = f->value("shownotes").toString();

        played      = f->value("played").toBool();
        downloaded  = f->value("downloaded").toBool();
        ustareet    = f->value("ustareet").toDateTime();
        src         = f->value("src").toUrl();
        stillInFeed = f->value("stillInFeed").toBool();
    }
}

void rssFeedItem::bindItem (QSqlQuery & query)
{
    query.bindValue(":mp3",         localMp3);
    query.bindValue(":url",         mp3Url.toString());
    query.bindValue(":played",      played);
    query.bindValue(":downloaded",  downloaded);
    query.bindValue(":created",     creation);
    query.bindValue(":duration",    duration);
    query.bindValue(":description", description);
    query.bindValue(":shownotes",   showNotes);
    query.bindValue(":ustareet",    ustareet);
    query.bindValue(":stillInFeed", stillInFeed);
    query.bindValue(":link",        link.toString());
    query.bindValue(":updating",    false);
    query.bindValue(":title",       title);
    query.bindValue(":GUID",        GUID);



}
