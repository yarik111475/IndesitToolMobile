#include "Evo2ApplianceParser.h"

Evo2ApplianceParser::Evo2ApplianceParser()
{

}

ApplianceModel Evo2ApplianceParser::createApplianceModel()
{
    //eep area size code
    m_applianceModel.EepAreaSizeCode=0x0;

    //eep area size
    m_applianceModel.EepAreaSize=0x2000;

    //eep area address
    m_applianceModel.EepAreaAddress=0x0;

    //last fault address
    m_applianceModel.LastFaultAddress=0x0;

    //fault block address
    m_applianceModel.FaultBlockAddress=0x0;

    //mcu type code
    m_applianceModel.McuTypeCode=0x00;

    //mcu type
    m_applianceModel.McuType="Renesas HD64F3664/94";

    //appliance type code
    m_applianceModel.DeviceTypeCode=0xFF;       //DeviceTypeCode = 0xFF

    //appliance type subcode
    m_applianceModel.DeviceSubTypeCode=0x01;    //DeviceSubTypeCode =0x01

    //appliance type
    m_applianceModel.DeviceType="Wash Evo-2";

    //firmware version
    QStringList list;
    if(m_applianceData.isEmpty()){
        m_applianceModel.FirmwareVersion=QObject::tr("Unknown FW version");
    }
    else{
        foreach(quint8 ch, m_applianceData){
            list.push_back(QString::number(ch,16).rightJustified(2, '0'));
        }
        m_applianceModel.FirmwareVersion=list.join('.');
    }

    return m_applianceModel;
}
