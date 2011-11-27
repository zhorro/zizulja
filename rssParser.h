#ifndef RSSCONTAINER_H
#define RSSCONTAINER_H

#include <QUrl>
#include <QDomDocument>
#include <QFile>
#include <QByteArray>
#include <QVector>

#include "rssfeeditem.h"

class rssParser
{
public:
    rssParser();
    bool update (QByteArray xml);

    QVector <rssFeedItem> rss;
    QString imageUrl;
    QString title;
    QUrl    link;

private:
    QDomDocument doc;
    bool errorParsingXML;

    void extractItems();
    void extractImage();
    void extractTitle();
    rssFeedItem parseItem( const QDomNode n );
    QDateTime timeParser(QString);
};

#endif // RSSCONTAINER_H
