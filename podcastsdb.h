#ifndef PODCASTSDB_H
#define PODCASTSDB_H

#include <QThread>
#include <QtSQL>
#include <QUrl>

#include "rssfeeditem.h"
#include "RSSUpdater.h"
#include "downloadqueue.h"

class  podcastsDB : public QThread
{
    Q_OBJECT
public:
    explicit podcastsDB(QObject *parent = 0);
    ~podcastsDB();

    void run ();

    bool exists (QUrl);

	void feedQuery( QSqlQuery &query, QUrl &url );

	void append (QUrl);
public slots:
    void startUpdate (QUrl src);
    void addFeed (rssFeedItem item, QUrl src);
    void finishUpdate (QUrl src);

    void removeItem (rssFeedItem item, QUrl src);
    void update(QUrl);
    void updateAll(void);
    void updateNext(void);
    void updateImage (QUrl src, QString imageUrl);
    void updateTitle (QUrl src, QString newTitle);
signals:
    void updateUrl (QUrl);
    void addDownloadInQueue (QString url, QString dst);
private:
    QSqlDatabase dbase;
    QSqlQuery allRecords;
    rssUpdater upa;
    DownloadQueue dq;

    void initMeta(void);
    QString imgName (QUrl rssUrl);
};
extern podcastsDB pdb;

#endif // PODCASTSDB_H
