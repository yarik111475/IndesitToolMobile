#include "ApplianceModel.h"

ApplianceModel::ApplianceModel()
{
}

ApplianceModel &ApplianceModel::operator=(const ApplianceModel &other)
{
    if(this!=&other){
        DeviceTypeCode=other.DeviceTypeCode;
        DeviceSubTypeCode=other.DeviceSubTypeCode;
        DeviceType=other.DeviceType;
        DeviceSubType=other.DeviceSubType;
        McuTypeCode=other.McuTypeCode;
        McuType=other.McuType;
        FirmwareVersion=other.FirmwareVersion;
        EepAreaAddress=other.EepAreaAddress;
        EepAreaSizeCode=other.EepAreaSizeCode;
        EepAreaSize=other.EepAreaSize;
        SettingsAreaPageSize=other.SettingsAreaPageSize;
        SettingsPageNumber=other.SettingsPageNumber;
        LastFaultAddress=other.LastFaultAddress;
        FaultBlockAddress=other.FaultBlockAddress;
    }
    return *this;
}
