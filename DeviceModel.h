#ifndef DEVICEMODEL_H
#define DEVICEMODEL_H
#include <QtCore>


class DeviceModel
{
public:
    DeviceModel();
    QString DeviceType;
    QString McuType;
    QString FirmwareVersion;
    quint16 EepAreaAddress;
    quint16 EepAreaSize;
    quint16 SettingsAreaPageSize;
    quint16 SettingsPageNumber;
    quint16 LastFaultAddress;
    quint16 CurrentFaultAddress;
};
Q_DECLARE_METATYPE(DeviceModel)

#endif // DEVICEMODEL_H
