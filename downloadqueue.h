#ifndef DOWNLOADQUEUE_H
#define DOWNLOADQUEUE_H

#include <QObject>
#include <QVector>
#include <QString>
#include <QQueue>
#include <QPair>
#include <QNetworkAccessManager>

class fileDownloader;
class DownloadQueue : public QObject
{
    Q_OBJECT
public:
    explicit DownloadQueue(QObject *parent = 0);
    void setMaximumDownloads (int size = 5);
signals:
    void downloadDone(QString url, QString fileName, int status);

public slots:
    void findOutWhoIsDone();
    bool addDownload(QString url, QString fileName);
private:
    QVector <fileDownloader*> downloaderArmy;
    QQueue <QPair <QString, QString> > queue;
    QNetworkAccessManager manager;
    bool isOnline();
public slots:
    void checkQueue(QString url, QString fileName, int status);
};


#endif // DOWNLOADQUEUE_H
