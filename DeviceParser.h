#ifndef DEVICEPARSER_H
#define DEVICEPARSER_H
#include <QtCore>

#include "DeviceModel.h"


class DeviceParser
{
private:
    QByteArray m_firmwareData;
    QByteArray m_lookUpTableData;
    QByteArray m_lookUpTableDataSizesData;
public:
    DeviceParser();
    void initDeviceParser(const QByteArray &firmwareData=QByteArray(),
                          const QByteArray &lookUpTableData=QByteArray(),
                          const QByteArray &lookUpTableDataSizesData=QByteArray());

    virtual DeviceModel createDeviceModel()=0;
};

#endif // DEVICEPARSER_H
