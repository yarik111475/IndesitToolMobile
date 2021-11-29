#ifndef EEPPROXYMODEL_H
#define EEPPROXYMODEL_H
#include <QtCore>


class EepProxyModel
{
    Q_GADGET
public:
    EepProxyModel();
    QString DeviceType;
    QString McuType;
    QString FirmwareVersion;
    QString EepAreaSize;

    Q_PROPERTY(QString DeviceType MEMBER DeviceType)
    Q_PROPERTY(QString McuType MEMBER McuType)
    Q_PROPERTY(QString FirmwareVersion MEMBER FirmwareVersion)
    Q_PROPERTY(QString EepAreaSize MEMBER EepAreaSize)


    QString Model;
    QString Code;
    QString Serial;
    bool IsChecksumValid;
    bool IsModelValid;

    Q_PROPERTY(QString Model MEMBER Model)
    Q_PROPERTY(QString Code MEMBER Code)
    Q_PROPERTY(QString Serial MEMBER Serial)
    Q_PROPERTY(bool IsChecksumValid MEMBER IsChecksumValid)
    Q_PROPERTY(bool IsModelValid MEMBER IsModelValid)

};
Q_DECLARE_METATYPE(EepProxyModel)

#endif // EEPPROXYMODEL_H
