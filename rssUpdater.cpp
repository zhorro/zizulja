#include <QtCore>
#include <QtGui>
#include <QtNetwork>
#include <QUrl>
#include <QUrlInfo>

#include <QFile>
#include "rssUpdater.h"

rssUpdater::rssUpdater(QObject *parent) :
    QThread(parent), currentReply(0)
{
    xml.setNamespaceProcessing(true);
}

void rssUpdater::update (QUrl rssAddress)
{
    busy = true;
    emit startUpdate(rssAddress);
    address = rssAddress;
    fetch();
}

void rssUpdater::get(const QUrl &url)
{
    qDebug() << "Updating from url " << url.toString();
    QNetworkRequest request(url);
    if (currentReply) {
        currentReply->disconnect(this);
        currentReply->deleteLater();

        if (rssFile.isOpen())
        {
            rssFile.close();
            rssFile.remove();
        }
    }


    static int idx = 0;
    rssFile.setFileName(QString("rss_%1.xml").arg(idx++));
    rssFile.open(QIODevice::WriteOnly);

    currentReply = manager.get(request);
    connect(currentReply, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(currentReply, SIGNAL(metaDataChanged()), this, SLOT(metaDataChanged()));
    connect(currentReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));
    connect(currentReply, SIGNAL(finished ()), this, SLOT(finished()));
}

void rssUpdater::fetch()
{
    xml.clear();
    get(address);
}

void rssUpdater::readyRead()
{
    int statusCode = currentReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (statusCode >= 200 && statusCode < 300) {
        QByteArray data = currentReply->readAll();
        xml.addData(data);
        rssFile.write(data);
        parseXml();
    }
}

void rssUpdater::finished()
{
    rssFile.close();
    emit doneUpdate (address);
    busy = false;
    exit();
}

void rssUpdater::parseXml()
{

    while (!xml.atEnd())
    {
        // TODO: При обнаружении ссылки на картинку подкаста нужно зафигачить ее в базу
        xml.readNext();

        switch (processingItem)
        {
            case parsingOther :
                if (xml.isStartElement())
                {
                    if (xml.name().toString() == "item")
                        processingItem = parsingItem;
                    else
                        if (xml.name().toString() == "image")
                            processingItem = parsingImage;
                        else
                            processingItem = parsingOther;
                }
                break;
            case parsingImage:
            case parsingItem :
                if (xml.isStartElement())
                {
                    currentTag = xml.name().toString();
                    if (currentTag == "enclosure")
                        rss.mp3Url.setUrl(xml.attributes().value("url").toString());
                }
                else if (xml.isEndElement())
                {
                    if (currentTag == "url" && processingItem==parsingImage)
                    {
                        emit gotFeedImage (address, unknownString);
                        break;
                    }
                    if (currentTag == "title" && processingItem==parsingImage)
                    {
                        emit gotFeedTitle (address, unknownString);
                        break;
                    }
                    processingItem = parsingOther;

                    if (currentTag == "title")
                        rss.title = unknownString;
                    else if (currentTag == "link")
                        rss.link = unknownString;
                    else if (currentTag == "guid")
                        rss.GUID = unknownString;
                    else if (currentTag == "pubDate")
                        rss.creation.fromString(unknownString);
                    else if (currentTag == "description")
                        rss.description = unknownString;

                    // Элемент прочтён
                    unknownString.clear();
                    if (xml.name().toString()=="item")
                    {
                        processingItem = parsingOther;
                        emit gotNewRss (rss, address);
                    }

                }
                else if (xml.isCharacters() && !xml.isWhitespace())
                {
                    unknownString += xml.text().toString();
                }
                break;
        }

        if (xml.error() && xml.error() != QXmlStreamReader::PrematureEndOfDocumentError) {
            qWarning() << "XML ERROR:" << xml.lineNumber() << ": " << xml.errorString();
        }
    }
}

void rssUpdater::error(QNetworkReply::NetworkError)
{
    qWarning("error retrieving RSS feed");
    currentReply->disconnect(this);
    currentReply->deleteLater();
    currentReply = 0;
}

void rssUpdater::metaDataChanged()
{
    QUrl redirectionTarget = currentReply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
    if (redirectionTarget.isValid()) {
        get(redirectionTarget);
    }
}

bool rssUpdater::isBusy()
{
    return busy;
}

void rssUpdater::run(void)
{
    qDebug() << "Starting RSS Updater";
    exec();
}
