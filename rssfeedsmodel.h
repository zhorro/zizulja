#ifndef RSSFEEDSMODEL_H
#define RSSFEEDSMODEL_H

#include <QSqlQueryModel>


class rssFeedsModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit rssFeedsModel(QObject *parent = 0);

signals:

public slots:

};

#endif // RSSFEEDSMODEL_H
