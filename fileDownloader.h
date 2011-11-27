#ifndef PodcastDownloader_H
#define PodcastDownloader_H

#include <QObject>
#include <QFile>
#include <QUrl>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QXmlStreamReader>

class fileDownloader : public QObject
{
    Q_OBJECT
public:
    QUrl initialUrl;
    QString fileName;
    enum {ok, badUrl, ioErrors, notFinished} status;

    explicit fileDownloader(QNetworkAccessManager * manager, QObject *parent = 0);
    bool isActive() {return active;}

signals:
    void done();
    void progress(float complete); //TODO: Чего то было, но кануло в кодировке

public slots:
    void getFromAddress (QUrl rssAddress, QString newFileName = "");

protected slots:
    void metaDataChanged();
    void finished();
    void readyRead();
    void error(QNetworkReply::NetworkError);
    void get(const QUrl &url);
private:

    bool active;
    QUrl address;

    QNetworkAccessManager * manager;
    QNetworkReply *currentReply;
    QFile file;
    QByteArray data;

    void setActive(bool newState);
    void openFile(QString fileName);
};
#endif // PodcastDownloader_H
