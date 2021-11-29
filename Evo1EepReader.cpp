#include "Evo1EepReader.h"

Evo1EepReader::Evo1EepReader()
{

}

void Evo1EepReader::readAppliance()
{
    //EVO_1 standart request to check connection
    //0xA5 0x2F 0x10 0xC9 0x0F <checksum> (0xBC)

    //EVO_1 standart request to read firmware version (type_001)
    //0xA5 0x10 0x32 0x33 0x00 <checksum> (0x1A)

    quint16 PRE_RESPONSE_SIZE=1;
    quint16 RESPONSE_SIZE=7;
    quint16 PREFIX_SIZE=4;

    QByteArray request;
    QByteArray response;

    request.push_back((char)0xA5);
    request.push_back((char)0x2F);
    request.push_back((char)0x10);
    request.push_back((char)0xC9);
    request.push_back((char)0x0F);
    request.push_back((char)Functions::getRequestChecksum(request));

    IODevice::IOResult result=m_IODevice.sendRequest(&request, PRE_RESPONSE_SIZE, true);
    if(result!=IODevice::RESPONSE_FAIL){

        request.clear();
        response.clear();

        request.push_back((char)0xA5);
        request.push_back((char)0x10);
        request.push_back((char)0x32);
        request.push_back((char)0x33);
        request.push_back((char)0x00);
        request.push_back((char)Functions::getRequestChecksum(request));

        result=m_IODevice.sendRequest(&request, RESPONSE_SIZE, true);

        if(result!=IODevice::RESPONSE_FAIL){
            response=m_IODevice.getResponse();
            m_applianceData.append(response.mid(PREFIX_SIZE,2));
        }
    }

    m_pApplianceParser->initApplianceParser(m_applianceData, m_lookUpTableData, m_lookUpTableDataSizesData);
    m_ApplianceModel=m_pApplianceParser->createApplianceModel();
}

void Evo1EepReader::readEep()
{
    //EVO_1 standart request to read eeprom content
    //0xA5 0x30 0x00 0x3F 0x00 <checksum> (0x14)

    const int PREFIX_SIZE=4;
    const int BLOCK_SIZE=256;
    const int CHECKSUM_SIZE=1;
    //const int PAGE_SIZE=0x100;

    quint16 eepAreaSize=m_ApplianceModel.EepAreaSize;

    quint8 blockAddress=(quint8)0x30;
    quint8 highAddress=0;
    quint8 lowAddress=(quint8)0x7F;

    QByteArray tempEepData;

    emit signalReadEepStart(eepAreaSize);

    for(int i=0; i<eepAreaSize/0x100; i++){
        QByteArray request;
        QByteArray response;

        //request body
        request.append((char)0xA5);

        //request read addresses
        request.push_back(blockAddress);
        request.push_back(highAddress);
        request.push_back(lowAddress);

        //request end
        request.push_back((char)0x00);

        //request checksum
        request.append(Functions::getRequestChecksum(request));

        IODevice::IOResult result=m_IODevice.sendRequest(&request, PREFIX_SIZE+BLOCK_SIZE+CHECKSUM_SIZE, true);
        if(result==IODevice::RESPONSE_FAIL){
            throw QObject::tr("Failure read eep data.\n"
                              "Maybe device not connected");
        }
        response=m_IODevice.getResponse();

        //request addresses correction
        lowAddress=(quint8)(lowAddress + 0x80);
        if(lowAddress==0xFF){
            highAddress=(quint8)(highAddress + 0x80);
        }
        if(lowAddress==0x7F){
            blockAddress=(quint8)(blockAddress + 1);
            highAddress=(char)0x00;
        }

        emit signalReadEepStep(0x100);

        tempEepData.append(response.mid(PREFIX_SIZE, BLOCK_SIZE));
    }

    quint16 deviceTypeCode=m_ApplianceModel.DeviceTypeCode;
    Q_UNUSED(deviceTypeCode)

    m_eepData=Functions::eepToBinToEep(tempEepData);

    emit signalReadApplianceOK(m_ApplianceModel);
    emit signalReadEepStop(m_eepData);
}

void Evo1EepReader::readLastFault()
{
    //EVO_1 standart request to read last fault
    //0xA5 0x1F 0x21 0x21 0x0F <checksum> (0x15)
    const quint16 RESPONSE_SIZE=6;
    const quint16 PREFIX_SIZE=4;

    QByteArray request;
    QByteArray response;

    request.push_back((char)0xA5);
    request.push_back((char)0x1F);
    request.push_back((char)0x21);
    request.push_back((char)0x21);
    request.push_back((char)0x0F);
    request.push_back((char)Functions::getRequestChecksum(request));

    IODevice::IOResult result=m_IODevice.sendRequest(&request ,RESPONSE_SIZE, true);
    if(result!=IODevice::RESPONSE_FAIL){
        response=m_IODevice.getResponse();
        m_lastFaultData=response.at(PREFIX_SIZE);
    }
    else{
        m_lastFaultData=0x00;
    }

    quint16 deviceTypeCode=m_ApplianceModel.DeviceTypeCode;
    quint16 eepAreaSize=m_ApplianceModel.EepAreaSize;
    emit signalReadLastFaultOK(deviceTypeCode, eepAreaSize, m_lastFaultData);
}

void Evo1EepReader::readFaultsBlock()
{
    const quint8 ADDRESS_INDEX=0x0E;
    const quint8 CORRECTION_VALUE=0xA2;
    const quint8 FAULTS_BLOCK_SIZE=0x20;

    quint16 faultsBlockStartAddress=((quint8)m_eepData.at(ADDRESS_INDEX) * 0x100 +
                                     (quint8)m_eepData.at(ADDRESS_INDEX+1)) * 2 + CORRECTION_VALUE;
    if(m_eepData.size() < (faultsBlockStartAddress + FAULTS_BLOCK_SIZE)){
        m_faultsBlock.fill(0xFF, FAULTS_BLOCK_SIZE);
    }
    else{
        m_faultsBlock.append(m_eepData.mid(faultsBlockStartAddress, FAULTS_BLOCK_SIZE));
    }

    quint16 deviceTypeCode=m_ApplianceModel.DeviceTypeCode;
    quint16 eepAreaSize=m_ApplianceModel.EepAreaSize;
    emit signalReadFaultsBlockOK(deviceTypeCode, eepAreaSize, m_faultsBlock);
}
