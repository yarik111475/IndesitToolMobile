#include "ArcadiaApplianceParser.h"

ArcadiaApplianceParser::ArcadiaApplianceParser()
{
    m_eepSizesMap.insert(0x38, 0x1000);  //4 Кб
    m_eepSizesMap.insert(0x48, 0x1400);  //5 Кб
    m_eepSizesMap.insert(0x07, 0x1400);  //5 Кб
    m_eepSizesMap.insert(0x58, 0x1800);  //6 Кб
    m_eepSizesMap.insert(0x68, 0x1c00);  //7 Кб
    m_eepSizesMap.insert(0x78, 0x2000);  //8 Кб
    m_eepSizesMap.insert(0x98, 0x2800);  //10 Кб
    m_eepSizesMap.insert(0x05, 0x4000);  //16 Кб
    m_eepSizesMap.insert(0x79, 0x4000);  //16 кб
    m_eepSizesMap.insert(0x39, 0x4000);  //16 кб
    m_eepSizesMap.insert(0x6a, 0x7000);  //28 кб


    m_mcuTypesMap.insert(0x00, "Motorola GB60/GB60A");
    m_mcuTypesMap.insert(0x40, "Motorola GT60/GT60A");
    m_mcuTypesMap.insert(0x51, "Renesas HD36079");
    m_mcuTypesMap.insert(0x71, "Renesas RX Series");
    m_mcuTypesMap.insert(0x81, "Renesas RX Series");
    m_mcuTypesMap.insert(0xE1, "Renesas RX Series");
    m_mcuTypesMap.insert(0xD1, "Renesas RX Series");
    m_mcuTypesMap.insert(0x60, "Motorola MCF52AG128");

    m_deviceTypesMap.insert(0x0128, "Dishwash");                     //DeviceTypeCode = 0x01  DeviceSubTypeCode =0x28
    m_deviceTypesMap.insert(0x0129, "Dishwash");                     //DeviceTypeCode = 0x01  DeviceSubTypeCode =0x29
    m_deviceTypesMap.insert(0x012A, "Dishwash");                     //DeviceTypeCode = 0x01  DeviceSubTypeCode =0x2A
    m_deviceTypesMap.insert(0x012B, "Dishwash");                     //DeviceTypeCode = 0x01  DeviceSubTypeCode =0x2A
    m_deviceTypesMap.insert(0x012D, "Dishwash");                     //DeviceTypeCode = 0x01  DeviceSubTypeCode =0x2D

    m_deviceTypesMap.insert(0x0228, "Wash Arcadia");             //DeviceTypeCode = 0x02  DeviceSubTypeCode =0x28
    m_deviceTypesMap.insert(0x0229, "Wash Arcadia-2");           //DeviceTypeCode = 0x02  DeviceSubTypeCode =0x29
    m_deviceTypesMap.insert(0x022B, "Wash Arcadia-2");           //DeviceTypeCode = 0x02  DeviceSubTypeCode =0x2B
    m_deviceTypesMap.insert(0x022D, "Wash Arcadia-3/Windy");     //DeviceTypeCode = 0x02  DeviceSubTypeCode =0x2D
    m_deviceTypesMap.insert(0x02AC, "Wash");                     //DeviceTypeCode = 0x02  DeviceSubTypeCode =0xAC

    m_deviceTypesMap.insert(0x0300, "Cook");                     //DeviceTypeCode = 0x03  DeviceSubTypeCode =0x00
    m_deviceTypesMap.insert(0x0328, "Cook");                     //DeviceTypeCode = 0x03  DeviceSubTypeCode =0x28
    m_deviceTypesMap.insert(0x032A, "Cook");                     //DeviceTypeCode = 0x03  DeviceSubTypeCode =0x2A
    m_deviceTypesMap.insert(0x032D, "Cook");                     //DeviceTypeCode = 0x03  DeviceSubTypeCode =0x2D

    m_deviceTypesMap.insert(0x0020, "Cool");                     //DeviceTypeCode = 0x00  DeviceSubTypeCode =0x20
    m_deviceTypesMap.insert(0x002A, "Cool");                     //DeviceTypeCode = 0x00  DeviceSubTypeCode =0x2A
    m_deviceTypesMap.insert(0x002D, "Cool");                     //DeviceTypeCode = 0x00  DeviceSubTypeCode =0x2D
    m_deviceTypesMap.insert(0x002E, "Cool");                     //DeviceTypeCode = 0x00  DeviceSubTypeCode =0x2E

}

ApplianceModel ArcadiaApplianceParser::createApplianceModel()
{
    //eep area size code
    m_applianceModel.EepAreaSizeCode=(quint8)m_applianceData.at(12);

    //eep area size
    m_applianceModel.EepAreaSize=m_eepSizesMap.contains(m_applianceModel.EepAreaSizeCode) ?
                m_eepSizesMap.value(m_applianceModel.EepAreaSizeCode) :
                0x8000;

    //eep area address
    m_applianceModel.EepAreaAddress=(quint8)m_applianceData.at(8) * 0x100 +
            (quint8)m_applianceData.at(9);

    //last fault address
    m_applianceModel.LastFaultAddress=(quint8)m_applianceData.at(28) * 0x100 +
            (quint8)m_applianceData.at(29);

    //fault block address
    m_applianceModel.FaultBlockAddress=(quint8)m_applianceData.at(26) * 0x100 +
            (quint8)m_applianceData.at(27);

    //mcu type code
    m_applianceModel.McuTypeCode=(quint8)m_applianceData.at(0);

    //mcu type
    m_applianceModel.McuType=m_mcuTypesMap.contains(m_applianceModel.McuTypeCode) ?
                m_mcuTypesMap.value(m_applianceModel.McuTypeCode) :
                QObject::tr("Unknown mcu");

    //appliance type code
    m_applianceModel.DeviceTypeCode=(quint8)m_applianceData.at(1);

    //appliance type subcode
    m_applianceModel.DeviceSubTypeCode=(quint8)m_applianceData.at(2);

    //full appliance type code
    quint32 fullDeviceTypeCode=m_applianceModel.DeviceTypeCode * 0x100 + m_applianceModel.DeviceSubTypeCode;

    //appliance type
    m_applianceModel.DeviceType=m_deviceTypesMap.contains(fullDeviceTypeCode) ?
                m_deviceTypesMap.value(fullDeviceTypeCode) :
                QObject::tr("Unknown appliance");

    //firmware version
    QStringList list;
    list.push_back(QString::number(m_applianceData.at(3),16).rightJustified(2, '0'));
    list.push_back(QString::number(m_applianceData.at(4),16).rightJustified(2, '0'));
    list.push_back(QString::number(m_applianceData.at(5),16).rightJustified(2, '0'));
    m_applianceModel.FirmwareVersion=list.join('.');

    return m_applianceModel;
}
