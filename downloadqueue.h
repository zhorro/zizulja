#ifndef DOWNLOADQUEUE_H
#define DOWNLOADQUEUE_H

#include <QObject>
#include <QVector>
#include <QString>
#include <QQueue>
#include <QPair>

class fileDownloader;
class DownloadQueue : public QObject
{
    Q_OBJECT
public:
    explicit DownloadQueue(QObject *parent = 0);
    void setMaximumDownloads (int size = 5);
signals:

public slots:
    bool addDownload(QString url, QString fileName);
private:
    QVector <fileDownloader*> downloaderArmy;
    QQueue <QPair <QString, QString> > queue;
public slots:
    void checkQueue();
};


#endif // DOWNLOADQUEUE_H
