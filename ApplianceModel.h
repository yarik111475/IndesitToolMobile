#ifndef APPLIANCEMODEL_H
#define APPLIANCEMODEL_H
#include <QtCore>
#include <QtQml>


class ApplianceModel
{
    Q_GADGET
public:
    explicit ApplianceModel();
    quint16 DeviceTypeCode;
    quint16 DeviceSubTypeCode;
    QString DeviceType;
    QString DeviceSubType;
    quint8 McuTypeCode;
    QString McuType;
    QString FirmwareVersion;
    quint16 EepAreaAddress;
    quint8 EepAreaSizeCode;
    quint16 EepAreaSize;
    quint16 SettingsAreaPageSize;
    quint16 SettingsPageNumber;
    quint16 LastFaultAddress;
    quint16 FaultBlockAddress;

    Q_PROPERTY(quint16 DeviceTypeCode MEMBER DeviceTypeCode)
    Q_PROPERTY(quint16 DeviceSubTypeCode MEMBER DeviceSubTypeCode)
    Q_PROPERTY(QString DeviceType MEMBER DeviceType)
    Q_PROPERTY(QString DeviceSubType MEMBER DeviceSubType)
    Q_PROPERTY(quint8 McuTypeCode MEMBER McuTypeCode)
    Q_PROPERTY(QString McuType MEMBER McuType)
    Q_PROPERTY(QString FirmwareVersion MEMBER FirmwareVersion)
    Q_PROPERTY(quint16 EepAreaAddress MEMBER EepAreaAddress)
    Q_PROPERTY(quint8 EepAreaSizeCode MEMBER EepAreaSizeCode)
    Q_PROPERTY(quint16 EepAreaSize MEMBER EepAreaSize)
    Q_PROPERTY(quint16 SettingsAreaPageSize MEMBER SettingsAreaPageSize)
    Q_PROPERTY(quint16 SettingsPageNumber MEMBER SettingsPageNumber)
    Q_PROPERTY(quint16 LastFaultAddress MEMBER LastFaultAddress)
    Q_PROPERTY(quint16 FaultBlockAddress MEMBER FaultBlockAddress)

    ApplianceModel& operator=(const ApplianceModel& other);
};


Q_DECLARE_METATYPE(ApplianceModel)

#endif // APPLIANCEMODEL_H
