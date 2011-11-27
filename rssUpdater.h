#ifndef RSSCONTAINER_H
#define RSSCONTAINER_H

#include <QThread>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QDomDocument>
#include <QSemaphore>
#include <QFile>
#include <QByteArray>

#include "rssfeeditem.h"
#include "downloadqueue.h"

static const int maximumRssFeedUpdates = 5;

class rssUpdater : public QThread
{
    Q_OBJECT
public:
    void run(void);
    explicit rssUpdater(QObject *parent = 0);

signals:
    void startUpdate (QUrl src);
    void gotNewRss (rssFeedItem rss, QUrl src);
    void gotFeedImage (QUrl src, QString imageUrl);
    void gotFeedTitle (QUrl src, QString title);
    void doneUpdate (QUrl src);

public slots:
    void update (QUrl rssAddress);
private:
    DownloadQueue dqueue;
};
#endif // RSSCONTAINER_H
