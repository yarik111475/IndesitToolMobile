#ifndef DATABASEPROXYMODEL_H
#define DATABASEPROXYMODEL_H

#include <QObject>

class DatabaseProxyModel
{
    Q_GADGET
public:
    explicit DatabaseProxyModel();
    QString Name;
    QString Code;
    QString Serial;

    Q_PROPERTY(QString Name MEMBER Name)
    Q_PROPERTY(QString Code MEMBER Code)
    Q_PROPERTY(QString Serial MEMBER Serial)

signals:

};
Q_DECLARE_METATYPE(DatabaseProxyModel)

#endif // DATABASEPROXYMODEL_H
