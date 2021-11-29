#ifndef APPLICATIONHANDLER_H
#define APPLICATIONHANDLER_H

#include <QObject>
#include <QtBluetooth>

class ApplicationHandler : public QObject
{
    Q_OBJECT
public:
    explicit ApplicationHandler(QObject *parent = nullptr);

signals:

public slots:

};

#endif // APPLICATIONHANDLER_H
