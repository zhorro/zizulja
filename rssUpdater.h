#ifndef RSSCONTAINER_H
#define RSSCONTAINER_H

#include <QThread>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QXmlStreamReader>
#include <QSemaphore>
#include <QFile>

#include "rssfeeditem.h"

static const int maximumRssFeedUpdates = 5;

class rssUpdater : public QThread
{
    Q_OBJECT
public:
    void run(void);
    explicit rssUpdater(QObject *parent = 0);
    bool isBusy();

signals:
    void startUpdate (QUrl src);
    void gotNewRss (rssFeedItem rss, QUrl src);
    void gotFeedImage (QUrl src, QString imageUrl);
    void gotFeedTitle (QUrl src, QString title);
    void doneUpdate (QUrl src);

public slots:
    void update (QUrl rssAddress);
    void fetch();
    void metaDataChanged();
    void finished();
    void readyRead();
    void error(QNetworkReply::NetworkError);

private:
    void parseXml();
    void get(const QUrl&);

    QUrl address;
    rssFeedItem rss;
    // Нужны для парсинга урлов
    QXmlStreamReader xml;
    QString currentTag;
    QString linkString;
    QString titleString;
    QString unknownString;

    QNetworkAccessManager manager;
    QNetworkReply *currentReply;
    enum parsingStates {
        parsingOther,
        parsingItem,
        parsingImage,
        parsingTitle
    };
    parsingStates processingItem;
    bool busy;

    QFile rssFile;

};
#endif // RSSCONTAINER_H
