#include <QtCore>
#include <QtGui>
#include <QtNetwork>
#include <QUrl>
#include <QUrlInfo>

#include <QFile>
#include "rssUpdater.h"

rssUpdater::rssUpdater(QObject *parent) :
    QThread(parent)
{
    // connect(dqueue, SIGNAL(downloadDone(QString, QString, int)), this, )
}

void rssUpdater::update (QUrl rssAddress)
{
    emit startUpdate(rssAddress);

}

void rssUpdater::run(void)
{
    qDebug() << "Starting RSS Updater";
    exec();
}
