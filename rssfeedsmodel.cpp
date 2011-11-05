#include <QDebug>
#include <QSqlError>
#include <QSqlQueryModel>

#include "rssfeedsmodel.h"
#include "podcastsdb.h"

rssFeedsModel::rssFeedsModel(QObject *parent):
    QSqlQueryModel(parent)
{
    setQuery(pdb.feedView());
}


