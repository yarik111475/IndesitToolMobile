#include "DeviceParser.h"

DeviceParser::DeviceParser()
{

}

void DeviceParser::initDeviceParser(const QByteArray &firmwareData,
                                    const QByteArray &lookUpTableData,
                                    const QByteArray &lookUpTableDataSizesData)
{
    m_firmwareData.append(firmwareData);
    m_lookUpTableData.append(lookUpTableData);
    m_lookUpTableDataSizesData.append(lookUpTableDataSizesData);
}


