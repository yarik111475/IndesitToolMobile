#include "ArcadiaEepWriter.h"

ArcadiaEepWriter::ArcadiaEepWriter(QObject *parent):EepWriter(parent)
{
    m_lookUpTableSize=0;
}

void ArcadiaEepWriter::readAppliance()
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

    result=m_IODevice.sendRequest(&request,PREFIX_SIZE+BLOCK_SIZE+CHECKSUM_SIZE);
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

void ArcadiaEepWriter::checkEepSize()
{
    //Check if raw variant size is correct for target device (and fill it wth 0xFF if size is less)
    quint16 eepAreaSize=m_ApplianceModel.EepAreaSize;
    if(m_eepData.size() > eepAreaSize){
        throw QObject::tr("Uncorrect eep size for device MCU");
    }
    else if(m_eepData.size() < eepAreaSize){
        QByteArray correctiontBlock;
        correctiontBlock.fill(0xFF,eepAreaSize-m_eepData.size());
        m_eepData.append(correctiontBlock);
    }
}

void ArcadiaEepWriter::writeEep()
{
    quint8 mcuTypeCode=m_ApplianceModel.McuTypeCode;
    emit signalEraseEep();
    eraseEep(mcuTypeCode);

    //ARCADIA standart write reqest to write variant data:
    //0xA5, 0xEE, 0x02, 0x95, 0x4F, 0x43, 0x90, <low_start_address> <high_start_address> <data_body> <checksum>

    quint16 eepAreaSize=m_ApplianceModel.EepAreaSize;
    quint16 eepAreaAddress=m_ApplianceModel.EepAreaAddress;

    quint8 highEepAreaAddress=(quint8)(eepAreaAddress >> 8);
    quint8 lowEepAreaAddress=(quint8)eepAreaAddress;
    //const int PAGE_SIZE=64; //0x40;
    const int RESPONSE_SIZE=11;

    emit signalWriteEepStart(eepAreaSize);


    for(int i=0; i < eepAreaSize/0x40; i++){
        QByteArray request;
        //request body
        request.append((char)0xA5);
        request.append((char)0xEE);
        request.append((char)0x02);
        request.append((char)0x95);
        request.append((char)0x4F);
        request.append((char)0x43);
        request.append((char)0x90);

        //write start address
        request.append(lowEepAreaAddress);
        request.append(highEepAreaAddress);

        //writing data
        QByteArray eepDataBlock;
        eepDataBlock.append(m_eepData.mid(i * 0x40, 0x40));

        request.append(eepDataBlock);
        request.append(Functions::getRequestChecksum(request));

        QByteArray response;
        IODevice::IOResult result=m_IODevice.sendRequest(&request, RESPONSE_SIZE, LOOP_DELAY);
        if(result==IODevice::RESPONSE_FAIL){
            throw QObject::tr("Failure writing eep data to device MCU.\n"
                              "Maybe device not connected");
        }
        response=m_IODevice.getResponse();

        lowEepAreaAddress=(quint8)(lowEepAreaAddress + 0x40);
        if(lowEepAreaAddress==0){
            highEepAreaAddress=(quint8)(highEepAreaAddress+1);
        }

        emit signalWriteEepStep(0x40);

        QThread::msleep(100);
    }
    emit signalReadApplianceOK(m_ApplianceModel);
    emit signalWriteEepStop();
}

void ArcadiaEepWriter::eraseEep(quint8 mcuTypeCode)
{
    //ARCADIA standart cleas statistics request:
    //0xA5, 0xEE, 0x02, 0x9D, 0x5F, 0x02, 0x90, 0x02, 0x25
    QByteArray request;
    QByteArray response;
    const quint16 STATISTICS_RESPONSE_SIZE=1;
    const quint16 END_RESPONSE_SIZE=11;

    //request body
    request.append((char)0xA5);
    request.append((char)0xEE);
    request.append((char)0x02);
    request.append((char)0x9D);
    request.append((char)0x5F);
    request.append((char)0x02);
    request.append((char)0x90);
    request.append((char)0x02);
    request.append((char)0x25);


    IODevice::IOResult result=m_IODevice.sendRequest(&request, STATISTICS_RESPONSE_SIZE, LOOP_DELAY);
    if(result!=IODevice::RESPONSE_OK){
        throw QObject::tr("Failure reset device faults.\n"
                          "Maybe device not connected");
    }
    response=m_IODevice.getResponse();

    request.clear();
    response.clear();

    switch(mcuTypeCode){
    case 0x00:
    case 0x40:
        eraseHCS08Series();
        break;
    case 0x51:
        eraseH8Series();
        break;
    case 0x71:
    case 0x81:
        eraseRXSeries();
        break;
    case 0x60:
        eraseMCFSeries();
        break;
    default:
        eraseHCS08Series();
        break;
    }

    //ARCADIA standart end erase request:
    //0xA5, 0xEE, 0x02, 0x95, 0x53, 0x02, 0x90, 0x53, 0x62

    //request body
    request.append((char)0xA5);
    request.append((char)0xEE);
    request.append((char)0x02);
    request.append((char)0x95);
    request.append((char)0x53);
    request.append((char)0x02);
    request.append((char)0x90);
    request.append((char)0x53);
    request.append((char)0x62);

    QByteArray endEraseResponse;
    result=m_IODevice.sendRequest(&request, END_RESPONSE_SIZE, LOOP_DELAY);
    if(result!=IODevice::RESPONSE_OK){
        throw QObject::tr("Failure reset device statistics.\n"
                          "Maybe device not connected");
    }
}

void ArcadiaEepWriter::eraseHCS08Series()
{
    //Delay for correct erase process
    QThread::msleep(2000);

    //ARCADIA standart request to erase Motorola HCS08 mcu series:
    //0xA5 0xEE 0x02 0x95 0x4E 0x03 0x90 <low_start_address> <high_start_address> <checksum>
    const quint16 RESPONSE_SIZE=11;
    //const quint16 PAGE_SIZE=0x200;

    quint16 eepAreaSize=m_ApplianceModel.EepAreaSize;
    quint16 eepAreaAddress=m_ApplianceModel.EepAreaAddress;

    quint8 lowEepAreaAddress=(quint8)eepAreaAddress;
    quint8 highEepAreaAddress=(quint8)(eepAreaAddress >> 8);

    for(int i=0; i < eepAreaSize/0x200; i++){
        QByteArray request;

        //request body
        request.append((char)0xA5);
        request.append((char)0xEE);
        request.append((char)0x02);
        request.append((char)0x95);
        request.append((char)0x4E);
        request.append((char)0x03);
        request.append((char)0x90);

        //request addresses
        request.append(lowEepAreaAddress);
        request.append(highEepAreaAddress);

        //request checksum
        request.append((quint8)Functions::getRequestChecksum(request));

        QByteArray response;
        IODevice::IOResult result=m_IODevice.sendRequest(&request, RESPONSE_SIZE, LOOP_DELAY);
        if(result!=IODevice::RESPONSE_OK){
            throw QObject::tr("Failure erasing device MCU.\n"
                              "Maybe device not connected");
        }
        highEepAreaAddress=(quint8)(highEepAreaAddress+2);
        QThread::msleep(50);
    }
}

void ArcadiaEepWriter::eraseH8Series()
{
    //Delay for correct erase process
    QThread::msleep(2000);

    //ARCADIA standart request to erase Renesas H8 MCU series:
    //0xA5, 0xEE, 0x02, 0x95, 0x4E, 0x03, 0x90, <low_start_address> <high_start_address> <checksum>
    const int RESPONSE_SIZE=11;
    quint16 eepAreaSize=m_ApplianceModel.EepAreaSize;
    Q_UNUSED(eepAreaSize)

    quint16 eepAreaAddress=m_ApplianceModel.EepAreaAddress;

    quint8 lowEepAreaAddress=(quint8)eepAreaAddress;
    quint8 highEepAreaAddress=(quint8)(eepAreaAddress >> 8);

    QByteArray request;

    //request body
    request.append((char)0xA5);
    request.append((char)0xEE);
    request.append((char)0x02);
    request.append((char)0x95);
    request.append((char)0x4E);
    request.append((char)0x03);
    request.append((char)0x90);

    //request addresses
    request.append(lowEepAreaAddress);
    request.append(highEepAreaAddress);

    //request checksum
    request.append((quint8)Functions::getRequestChecksum(request));

    QByteArray response;
    IODevice::IOResult result=m_IODevice.sendRequest(&request,RESPONSE_SIZE, LOOP_DELAY);
    response=m_IODevice.getResponse();

    if(result!=IODevice::RESPONSE_OK){
        throw QObject::tr("Failure erasing device MCU.\n"
                          "Maybe device not connected");
    }
}

void ArcadiaEepWriter::eraseRXSeries()
{
    //Delay for correct erase process
    QThread::msleep(2000);

    //ARCADIA standart request to erase Renesas RX mcu series:
    //0xA5 0xEE 0x02 0x95 0x4E 0x03 0x90 <low_start_address> <high_start_address> <checksum>

    const quint16 RESPONSE_SIZE=11;
    //const quint16 PAGE_SIZE=0x400;

    quint16 eepAreaSize=m_ApplianceModel.EepAreaSize;
    quint16 eepAreaAddress=m_ApplianceModel.EepAreaAddress;

    quint8 lowEepAreaAddress=(quint8)eepAreaAddress;
    quint8 highEepAreaAddress=(quint8)(eepAreaAddress >> 8);

    for(int i=0; i < eepAreaSize/0x400; i++){
        QByteArray request;

        //request body
        request.append((char)0xA5);
        request.append((char)0xEE);
        request.append((char)0x02);
        request.append((char)0x95);
        request.append((char)0x4E);
        request.append((char)0x03);
        request.append((char)0x90);

        //request addresses
        request.append(lowEepAreaAddress);
        request.append(highEepAreaAddress);

        //request checksum
        request.append((quint8)Functions::getRequestChecksum(request));

        QByteArray response;
        IODevice::IOResult result=m_IODevice.sendRequest(&request, RESPONSE_SIZE, LOOP_DELAY);
        if(result!=IODevice::RESPONSE_OK){
            throw QObject::tr("Failure erasing device MCU.\n"
                              "Maybe device not connected");
        }

        highEepAreaAddress=(quint8)(highEepAreaAddress+4);
        QThread::msleep(50);
    }
}

void ArcadiaEepWriter::eraseMCFSeries()
{
    //Delay for correct erase process
    QThread::msleep(2000);

    //ARCADIA standart request to erase Renesas RX mcu series:
    //0xA5 0xEE 0x02 0x95 0x4E 0x03 0x90 <low_start_address> <high_start_address> <checksum>

    const quint16 RESPONSE_SIZE=11;
    //const quint16 PAGE_SIZE=0x400;

    quint16 eepAreaSize=m_ApplianceModel.EepAreaSize;
    quint16 eepAreaAddress=m_ApplianceModel.EepAreaAddress;

    quint8 lowEepAreaAddress=(quint8)eepAreaAddress;
    quint8 highEepAreaAddress=(quint8)(eepAreaAddress >> 8);

    for(int i=0; i < eepAreaSize/0x400; i++){
        QByteArray request;

        //request body
        request.append((char)0xA5);
        request.append((char)0xEE);
        request.append((char)0x02);
        request.append((char)0x95);
        request.append((char)0x4E);
        request.append((char)0x03);
        request.append((char)0x90);

        //request addresses
        request.append(lowEepAreaAddress);
        request.append(highEepAreaAddress);

        //request checksum
        request.append((quint8)Functions::getRequestChecksum(request));

        QByteArray response;
        IODevice::IOResult result=m_IODevice.sendRequest(&request, RESPONSE_SIZE, LOOP_DELAY);
        if(result!=IODevice::RESPONSE_OK){
            throw QObject::tr("Failure erasing device MCU.\n"
                              "Maybe device not connected");
        }

        highEepAreaAddress=(quint8)(highEepAreaAddress+4);
        QThread::msleep(50);
    }
}

void ArcadiaEepWriter::readLookUpTableData()
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

    result=m_IODevice.sendRequest(&request, PREFIX_SIZE + m_lookUpTableSize + CHECKSUM_SIZE);
    if(result!=IODevice::RESPONSE_OK){
        throw QObject::tr("Failure read lookup table data.\n"
                          "Maybe device not connected");
    }
    response=m_IODevice.getResponse();

    m_lookUpTableData.append(response.mid(PREFIX_SIZE, m_lookUpTableSize));
}

void ArcadiaEepWriter::readLookUpTableDataSizesData()
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


    result=m_IODevice.sendRequest(&request, PREFIX_SIZE+lookUpTableDataSizesSize+CHECKSUM_SIZE);
    if(result!=IODevice::RESPONSE_OK){
        throw QObject::tr("Failure read lookup table data.\n"
                          "Maybe device not connected");
    }
    response=m_IODevice.getResponse();
    m_lookUpTableDataSizesData.append(response.mid(PREFIX_SIZE, lookUpTableDataSizesSize));
}
