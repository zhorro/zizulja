#include <QtCore>
#include <QtGui>
#include <QtNetwork>
#include <QUrl>
#include "fileDownloader.h"

//TODO: Добавить обработку ошибок
//TODO: Добавить хераченье прогресса
//TODO: Добавить возобновление скачивания

fileDownloader::fileDownloader(QString podcastUrl, QString newFileName, QObject *parent) :
    QObject(parent), currentReply(0), active(false)
{
    updateAddress(podcastUrl);
    updateFileName(newFileName);
}

void fileDownloader::setDone(bool newState)
{
    if (active && !newState)
        emit done();
    active = newState;
}

void fileDownloader::updateAddress (QUrl rssAddress)
{
    address = rssAddress;
}

void fileDownloader::updateFileName(QString newFileName)
{
    file.setFileName(newFileName);
}

/*
Starts the network request and connects the needed signals
*/
void fileDownloader::get(const QUrl &url)
{
    setDone(true);
    QNetworkRequest request(url);
    if (currentReply)
    {
        currentReply->disconnect(this);
        currentReply->deleteLater();
    }
    currentReply = manager.get(request);

    qDebug () << connect(currentReply, SIGNAL(finished ()), this, SLOT(finished ()));
    qDebug () << connect(currentReply, SIGNAL(readyRead()), this, SLOT(readyRead()));
    qDebug () << connect(currentReply, SIGNAL(metaDataChanged()), this, SLOT(metaDataChanged()));
    qDebug () << connect(currentReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));
}


void fileDownloader::fetch()
{
    if (!file.isOpen()) // Если файло еще не использовался
        file.open(QIODevice::WriteOnly);

    QUrl url(address);
    get(url);
}

/*
Reads data received from the RDF source.

We read all the available data, and pass it to the XML
stream reader. Then we call the XML parsing function.
*/

void fileDownloader::readyRead()
{
    if (!file.isOpen()) // Если файло еще не использовался
        qWarning("Файл не открыт!");

    int statusCode = currentReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (statusCode >= 200 && statusCode < 300)
        while (currentReply->bytesAvailable() > 0)
        {
            QByteArray data = currentReply->read(128);
            file.write(data);
        }
}

void fileDownloader::finished()
{
    file.close();
    setDone(false);
}

void fileDownloader::error(QNetworkReply::NetworkError)
{
    qWarning("error retrieving RSS feed");
    currentReply->disconnect(this);
    currentReply->deleteLater();
    currentReply = 0;
    file.close();
    setDone(false);
}

void fileDownloader::metaDataChanged()
{
    QUrl redirectionTarget = currentReply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
    if (redirectionTarget.isValid()) {
        get(redirectionTarget);
    }
}
