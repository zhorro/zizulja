#include "downloadqueue.h"
#include "fileDownloader.h"

DownloadQueue::DownloadQueue(QObject *parent) :
    QObject(parent)
{
    setMaximumDownloads(6);
}

void DownloadQueue::setMaximumDownloads (int size)
{
    while (downloaderArmy.size()<size) // Если нехватает, то создать
    {
        // Добавить качалок
        fileDownloader * D = new fileDownloader(&manager, this);
        connect (D, SIGNAL(done()), this, SLOT(findOutWhoIsDone()));
        downloaderArmy.append(D);
    }

    foreach (fileDownloader * d, downloaderArmy) // Похерачили по всем
    {
        if (downloaderArmy.size()==size) // А Вот тута! Если  размерчег стал в самый раз хватит
            break;
        if (!d->isActive()) // Иначе гасим неактивных
        {
            downloaderArmy.remove(downloaderArmy.indexOf(d));
            d->deleteLater();
        }
    }
    // Если все были активные, то никто не удалится.
}
void DownloadQueue::findOutWhoIsDone()
{
    if (fileDownloader * src = static_cast<fileDownloader*>(QObject::sender ()))
    {
        emit downloadDone(src->initialUrl.toString (), src->fileName, src->status);
    }
}

void DownloadQueue::checkQueue(QString url, QString fileName, int status)
{
    emit downloadDone(url, fileName, status);
    while (!queue.isEmpty()) // Пока есть в очереди добавлять
    {
        QPair<QString, QString> SS = queue.dequeue();
        if (!addDownload(SS.first, SS.second))  // Если не добавляется
            break;                              //  - прекратить
    }
}

bool DownloadQueue::addDownload(QString url, QString fileName)
{
    // Если армия свободна, качать ею. Если несвободна - добавить в очередь
    if (isOnline())
    foreach (fileDownloader * d, downloaderArmy)
        if (!d->isActive())
        {
            d->getFromAddress (url, fileName);
            return true;
        }
    queue.append (qMakePair(url, fileName));
    return false;
}

bool DownloadQueue::isOnline()
{
    return true; // TODO: Вобще то нужно и проверять, как там оно...
}
