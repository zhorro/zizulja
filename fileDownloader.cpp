#include <QtCore>
#include <QtGui>
#include <QtNetwork>
#include <QUrl>
#include "fileDownloader.h"

//TODO: Добавить обработку ошибок
//TODO: Добавить хераченье прогресса
//TODO: Добавить возобновление скачивания ? В самом конце, ибо глючная весчь

fileDownloader::fileDownloader(QNetworkAccessManager * networkManager, QObject *parent) :
    QObject(parent), currentReply(0), active(false), manager(networkManager)
{

}

void fileDownloader::setActive(bool newState)
{
    if (active && !newState)
        emit done();
    active = newState;
}

void fileDownloader::getFromAddress (QUrl rssAddress, QString fileName)
{
    initialUrl = address = rssAddress;
    status = notFinished;
    if (!fileName.isEmpty ())
        openFile(fileName);
    get(QUrl(address));
}

void fileDownloader::openFile(QString fileName)
{
    this->fileName = fileName;
    qDebug() << "starting download file: " << fileName;
    file.setFileName (fileName);
    bool res = file.open(QIODevice::WriteOnly);
    if (res)
    {
        status = ioErrors;
        throw QString ("file %1 : error opening : %2").arg(fileName).arg(file.errorString ());
    }
}

void fileDownloader::get(const QUrl &url)
{
    //TODO: Возможно тут нужно добавить ожидение WiFi -
    // курить QNetworkConfigurationManager. Но скорее всего не нужно плодить сущностей,
    // и такой переход правильнее всего будет добавить выше, в очереди
    setActive(true);
    QNetworkRequest request(url);
    if (currentReply)
    {
        currentReply->disconnect(this);
        currentReply->deleteLater();
    }
    currentReply = manager->get(request);

    qDebug () << connect(currentReply, SIGNAL(finished ()), this, SLOT(finished ()));
    qDebug () << connect(currentReply, SIGNAL(readyRead()), this, SLOT(readyRead()));
    qDebug () << connect(currentReply, SIGNAL(metaDataChanged()), this, SLOT(metaDataChanged()));
    qDebug () << connect(currentReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));
}


void fileDownloader::readyRead()
{
    if (!file.isOpen()) // Если файло еще не использовался
    {
        // Парсим наконец то имя файла, который в нас прилител
        openFile(address.toLocalFile ()); // BUG: не верю, что всё может быть так просто!
    }

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
    setActive(false);
    status = ok;
}

void fileDownloader::error(QNetworkReply::NetworkError)
{
    qWarning("error retrieving RSS feed");
    currentReply->disconnect(this);
    currentReply->deleteLater();
    currentReply = 0;
    file.close();
    setActive(false);
}

void fileDownloader::metaDataChanged()
{
    QUrl redirectionTarget = currentReply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
    if (redirectionTarget.isValid()) {
        get(redirectionTarget);
    }
}
