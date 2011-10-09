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
        explicit fileDownloader(QString rssAddress = "", QString fileName = "", QObject *parent = 0);
        bool isActive() {return active;}

signals:
        void done();
		void progress(float complete); //TODO: Нужно эмитить

public slots:
        void updateAddress (QUrl rssAddress);
        void updateFileName(QString newFileName);
		void fetch();
		void metaDataChanged();
        void finished();
		void readyRead();
		void error(QNetworkReply::NetworkError);

private:
	void get(const QUrl &url);

	QUrl address;
	// Нужны для парсинга урлов
	QXmlStreamReader xml;
	QString currentTag;
	QString linkString;
	QString titleString;

	QNetworkAccessManager manager;
	QNetworkReply *currentReply;
        QFile file;
        void setDone(bool newState);
        bool active;
};
#endif // PodcastDownloader_H
