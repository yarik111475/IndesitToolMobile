#include "ArcadiaEepReader.h"

ArcadiaEepReader::ArcadiaEepReader(QObject *parent):EepReader(parent)
{
    m_lookUpTableSize=0;
}

void ArcadiaEepReader::readAppliance()
{
    //ARCADIA standart request to read appliance data:
    //0xA5, 0xEE, 0x02, 0x95, 0x49, 0x02, 0x90, 0x20, 0x25
    //Standart response size: 45 bytes (prefix+body+checksum)
    const quint16 PREFIX_SIZE=7;
    const quint16 BLOCK_SIZE=32;
    const quint16 CHECKSUM_SIZE=1;
    IODevice::IOResult result=IODevice::RESPONSE_FAIL;

    QByteArray request;
    QByteArray response;

    //request body
    request.append((char)0xA5);
    request.append((char)0xEE);
    request.append((char)0x02);
    request.append((char)0x95);
    request.append((char)0x49);
    request.append((char)0x02);
    request.append((char)0x90);
    request.append((char)0x20);
    request.append((char)0x25);

    result=m_IODevice.sendRequest(&request, PREFIX_SIZE+BLOCK_SIZE+CHECKSUM_SIZE, 300);
    if(result==IODevice::RESPONSE_FAIL){
        throw QObject::tr("Failure read information data.\n"
                          "Maybe device not connected");
    }
    response=m_IODevice.getResponse();

    m_applianceData=response.mid(PREFIX_SIZE, BLOCK_SIZE);

    readLookUpTableData();

    readLookUpTableDataSizesData();

    m_pApplianceParser->initApplianceParser(m_applianceData, m_lookUpTableData, m_lookUpTableDataSizesData);
    m_ApplianceModel=m_pApplianceParser->createApplianceModel();
}

void ArcadiaEepReader::readEep()
{
    //ARCADIA standart request to read variant data:
    //0xA5 0xEE 0x02 0x95 0x3F 0x05 0x90 <low_start_address> <high_start_address> <lowEneAddress> <lowHighAddress> <checksum>
    //Standart response size: 264 bytes (prefix+body+checksum)
    const quint16 PREFIX_SIZE=7;
    const quint16 BLOCK_SIZE=256;
    const quint16 CHECKSUM_SIZE=1;

    quint16 eepAreaAddress=m_ApplianceModel.EepAreaAddress;
    quint16 eepAreaSize=m_ApplianceModel.EepAreaSize;
    quint16 pageSize=0xFF;

    emit signalReadEepStart(eepAreaSize);

    for(int i=0; i<eepAreaSize/pageSize; i++){
        QByteArray request;
        QByteArray response;
        //request body
        request.append((char)0xA5);
        request.append((char)0xEE);
        request.append((char)0x02);
        request.append((char)0x95);
        request.append((char)0x3F);
        request.append((char)0x05);
        request.append((char)0x90);

        //request read start address
        request.append((quint8)eepAreaAddress);
        request.append((quint8)((eepAreaAddress >> 8) + i));

        //request read end address
        request.append((quint8)(pageSize));
        request.append((quint8)((eepAreaAddress >> 8) + i));

        //request checksum
        request.append((quint8)(Functions::getRequestChecksum(request)));

        IODevice::IOResult result=m_IODevice.sendRequest(&request, PREFIX_SIZE+BLOCK_SIZE+CHECKSUM_SIZE, 300);
        if(result==IODevice::RESPONSE_FAIL){
            throw QObject::tr("Failure read eep data.\n"
                              "Maybe device not connected");
        }

        response=m_IODevice.getResponse();

        m_eepData.append(response.mid(PREFIX_SIZE, BLOCK_SIZE));
        emit signalReadEepStep(pageSize);
    }

    emit signalReadApplianceOK(m_ApplianceModel);
    emit signalReadEepStop(m_eepData);
}

void ArcadiaEepReader::readLastFault()
{
    //ARCADIA standart request to read last failure data:
    //0xA5 0xEE 0x02 0x95 0x3F 0x05 0x90 <low_start_address> <high_start_address> <lowEneAddress> <lowHighAddress> <checksum>
    //Standart response size: 9 bytes (prefix+body+checksum)
    const quint16 PREFIX_SIZE=7;
    const quint16 BLOCK_SIZE=1;
    const quint16 CHECKSUM_SIZE=1;

    quint16 lastFaultAddress=m_ApplianceModel.LastFaultAddress;
    quint8 lowLastFaultAddress=(quint8)lastFaultAddress;
    quint8 highLastFaultAddress=(quint8)(lastFaultAddress >> 8);

    QByteArray request;
    QByteArray response;

    //request body
    request.append((char)0xA5);
    request.append((char)0xEE);
    request.append((char)0x02);
    request.append((char)0x95);
    request.append((char)0x3F);
    request.append((char)0x05);
    request.append((char)0x90);

    //request read start address
    request.append(lowLastFaultAddress);
    request.append(highLastFaultAddress);

    //request read end address
    request.append(lowLastFaultAddress);
    request.append(highLastFaultAddress);


    //request checksum
    request.append((quint8)(Functions::getRequestChecksum(request)));

    IODevice::IOResult result=m_IODevice.sendRequest(&request, PREFIX_SIZE+BLOCK_SIZE+CHECKSUM_SIZE, 300);
    if(result==IODevice::RESPONSE_FAIL){
        throw QObject::tr("Failure reading last fault data.\n"
                          "Maybe device not connected");
    }
    response=m_IODevice.getResponse();

    m_lastFaultData=response.at(PREFIX_SIZE);
    quint16 deviceTypeCode=m_ApplianceModel.DeviceTypeCode;
    quint16 eepAreaSize=m_ApplianceModel.EepAreaSize;

    emit signalReadLastFaultOK(deviceTypeCode, eepAreaSize, m_lastFaultData);
}

void ArcadiaEepReader::readFaultsBlock()
{
    //ARCADIA standart request to read failures data:
    //0xA5 0xEE 0x02 0x95 0x3F 0x05 0x90 <low_start_address> <high_start_address> <lowEneAddress> <lowHighAddress> <checksum>
    //Standart response size: 37 bytes (prefix+body+checksum)
    const quint16 PREFIX_SIZE=7;
    const quint16 BLOCK_SIZE=30;
    const quint16 CHECKSUM_SIZE=1;

    quint16 faultsBlockAddressStart=m_ApplianceModel.FaultBlockAddress;
    quint16 faultsBlockAddressEnd=faultsBlockAddressStart + BLOCK_SIZE - CHECKSUM_SIZE;

    QByteArray request;
    QByteArray response;

    //request body
    request.append((char)0xA5);
    request.append((char)0xEE);
    request.append((char)0x02);
    request.append((char)0x95);
    request.append((char)0x3F);
    request.append((char)0x05);
    request.append((char)0x90);

    //request read start address
    request.append((quint8)faultsBlockAddressStart);
    request.append((quint8)(faultsBlockAddressStart >> 8));

    //request read end address
    request.append((quint8)faultsBlockAddressEnd);
    request.append((quint8)(faultsBlockAddressEnd >> 8));

    //request checksum
    request.append((quint8)(Functions::getRequestChecksum(request)));

    IODevice::IOResult result=m_IODevice.sendRequest(&request, PREFIX_SIZE+BLOCK_SIZE+CHECKSUM_SIZE, 300);
    if(result==IODevice::RESPONSE_FAIL){
        throw QObject::tr("Failure reading faults data.\n"
                          "Maybe device not connected");
    }
    response=m_IODevice.getResponse();

    m_faultsBlock=response.mid(PREFIX_SIZE, BLOCK_SIZE);
    quint16 deviceTypeCode=m_ApplianceModel.DeviceTypeCode;
    quint16 eepAreaSize=m_ApplianceModel.EepAreaSize;

    emit signalReadFaultsBlockOK(deviceTypeCode,eepAreaSize,m_faultsBlock);
}

void ArcadiaEepReader::readLookUpTableData()
{
    //ARCADIA standart request to read LookUp table:
    //0xA5 0xEE 0x02 0x95 0x3F 0x05 0x90 <low_start_address> <high_start_address> <lowEneAddress> <lowHighAddress> <checksum>
    const quint16 PREFIX_SIZE=7;
    const quint16 CHECKSUM_SIZE=1;
    IODevice::IOResult result=IODevice::RESPONSE_FAIL;

    QByteArray request;
    QByteArray response;

    quint16 lookUpTableAddressStart=(quint8)m_applianceData.at(10) * 0x100 +
            (quint8)m_applianceData.at(11);

    m_lookUpTableSize=(quint8)m_applianceData.at(13) * 2;
    quint16 lookUpTableAddressEnd=lookUpTableAddressStart + m_lookUpTableSize-1;

    //request body
    request.push_back((char)0xA5);
    request.push_back((char)0xEE);
    request.push_back((char)0x02);
    request.push_back((char)0x95);
    request.push_back((char)0x3F);
    request.push_back((char)0x05);
    request.push_back((char)0x90);

    //request read start address
    quint8 hightLookUpTableAddressStart1=(quint8)(lookUpTableAddressStart >> 8);
    quint8 lowLookUpTableAddressEnd1=(quint8)lookUpTableAddressStart;
    request.push_back(lowLookUpTableAddressEnd1);
    request.push_back(hightLookUpTableAddressStart1);

    //request read end address
    quint8 hightLookUpTableAddressStart2=(quint8)(lookUpTableAddressEnd >> 8);
    quint8 lowLookUpTableAddressEnd2=(quint8)lookUpTableAddressEnd;
    request.push_back(lowLookUpTableAddressEnd2);
    request.push_back(hightLookUpTableAddressStart2);

    //request checksum
    request.push_back(Functions::getRequestChecksum(request));

    result=m_IODevice.sendRequest(&request, PREFIX_SIZE + m_lookUpTableSize + CHECKSUM_SIZE, 300);
    if(result!=IODevice::RESPONSE_OK){
        throw QObject::tr("Failure read lookup table data.\n"
                          "Maybe device not connected");
    }

    response=m_IODevice.getResponse();

    m_lookUpTableData.append(response.mid(PREFIX_SIZE, m_lookUpTableSize));
}

void ArcadiaEepReader::readLookUpTableDataSizesData()
{
    //ARCADIA standart request to read variant data:
    //0xA5 0xEE 0x02 0x95 0x3F 0x05 0x90 <low_start_address> <high_start_address> <lowEneAddress> <lowHighAddress> <checksum>
    const quint16 PREFIX_SIZE=7;
    const quint16 CHECKSUM_SIZE=1;
    IODevice::IOResult result=IODevice::RESPONSE_FAIL;

    QByteArray request;
    QByteArray response;

    quint16 lookUpTableDataSizesAddress=(quint8)m_lookUpTableData.at(m_lookUpTableSize-2) * 0x100
            + (quint8)m_lookUpTableData.at(m_lookUpTableSize-1);
    quint16 lookUpTableDataSizesSize=(quint8)m_applianceData.at(13);
    quint16 lookUpTableDataSizesAddressEnd=lookUpTableDataSizesAddress + lookUpTableDataSizesSize - 1;

    //request body
    request.push_back((char)0xA5);
    request.push_back((char)0xEE);
    request.push_back((char)0x02);
    request.push_back((char)0x95);
    request.push_back((char)0x3F);
    request.push_back((char)0x05);
    request.push_back((char)0x90);

    //request read start address
    quint8 hightLookUpTablDataSizeseAddressStart1=(quint8)(lookUpTableDataSizesAddress >> 8);
    quint8 lowLookUpTableDataSizesAddressEnd1=(quint8)lookUpTableDataSizesAddress;
    request.push_back(lowLookUpTableDataSizesAddressEnd1);
    request.push_back(hightLookUpTablDataSizeseAddressStart1);


    //request read end address
    quint8 hightLookUpTableDataSizesAddressStart2=(quint8)(lookUpTableDataSizesAddressEnd >> 8);
    quint8 lowLookUpTableDataSizesAddressEnd2=(quint8)lookUpTableDataSizesAddressEnd;
    request.push_back(lowLookUpTableDataSizesAddressEnd2);
    request.push_back(hightLookUpTableDataSizesAddressStart2);

    //request checksum
    request.push_back(Functions::getRequestChecksum(request));


    result=m_IODevice.sendRequest(&request, PREFIX_SIZE+lookUpTableDataSizesSize+CHECKSUM_SIZE, 300);
    if(result!=IODevice::RESPONSE_OK){
        throw QObject::tr("Failure read lookup table data.\n"
                          "Maybe device not connected");
    }

    response=m_IODevice.getResponse();

    m_lookUpTableDataSizesData.append(response.mid(PREFIX_SIZE, lookUpTableDataSizesSize));
}
