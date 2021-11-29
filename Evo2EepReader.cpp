#include "Evo2EepReader.h"

Evo2EepReader::Evo2EepReader()
{
    QByteArray requestBuffer;

    requestBuffer.push_back((char)0x0C);
    requestBuffer.push_back((char)0x1E);
    requestBuffer.push_back((char)0x1F);
    requestBuffer.push_back((char)0x1F);
    requestBuffer.push_back((char)0x0F);

    m_currentFaultRequestMap.insert("02.71",requestBuffer);
    m_currentFaultRequestMap.insert("02.74",requestBuffer);

    requestBuffer.clear();

    requestBuffer.push_back((char)0x90);
    requestBuffer.push_back((char)0x1E);
    requestBuffer.push_back((char)0x21);
    requestBuffer.push_back((char)0x21);
    requestBuffer.push_back((char)0x0F);

    m_currentFaultRequestMap.insert("08.44.00", requestBuffer);
    m_currentFaultRequestMap.insert("08.55.00", requestBuffer);
    m_currentFaultRequestMap.insert("09.21.00", requestBuffer);

    requestBuffer.clear();

    requestBuffer.push_back((char)0x0C);
    requestBuffer.push_back((char)0x1E);
    requestBuffer.push_back((char)0x21);
    requestBuffer.push_back((char)0x21);
    requestBuffer.push_back((char)0x0F);

    m_currentFaultRequestMap.insert("08.66.00", requestBuffer);

    requestBuffer.clear();
}

void Evo2EepReader::readAppliance()
{
    //EVO_2 standart request for 3664 mcu
    //0xA5, 0xEE, 0x02, 0x93, 0x10, 0x05, | 0x0C, 0x10, 0xF1, 0xF1, 0x00, 0x3B
    //EVO_2 standart request for 3694 mcu
    //0xA5, 0xEE, 0x02, 0x93, 0x10, 0x05, | 0x90, 0x10, 0xF1, 0xF1, 0x00, 0xBF
    //response size is 9

    //EVO_2 standart request for 3664 mcu to read firmware version (for 2.74 firmware version only):
    //0xA5, 0xEE, 0x02, 0x93, 0x10, 0x05, | 0x0C, 0x10, 0x36, 0x37, 0x00, 0xC6

    //EVO_2 standart request for 3664 mcu to read firmware version (for other versions)
    //0xA5, 0xEE, 0x02, 0x93, 0x10, 0x05, | 0x0C, 0x10, 0xEC, 0xEE, 0x00, 0x33

    //EVO_2 standart request for 3694 mcu to read firmware version
    //0xA5, 0xEE, 0x02, 0x93, 0x10, 0x05, | 0x90, 0x10, 0xEC, 0xEE, 0x00, 0xB7

    //EVO_2 standart request to read eeprom
    //0xA5, 0xEE, 0x02, 0x91, 0x30, 0x05, 0xA0, 0x00, 0x00, 0x00, 0x00, 0xFB

    quint16 RESPONSE_SIZE=9;
    quint16 PREFIX_SIZE=7;
    QByteArray request;
    QByteArray response;

    request.append((char)0xA5);
    request.append((char)0xEE);
    request.append((char)0x02);
    request.append((char)0x93);
    request.append((char)0x10);
    request.append((char)0x05);
    request.append((char)0x0C);
    request.append((char)0x10);
    request.append((char)0xF1);
    request.append((char)0xF1);
    request.append((char)0x00);
    request.append(Functions::getRequestChecksum(request));

    //try to connect to 3664 mcu
    IODevice::IOResult result=m_IODevice.sendRequest(&request, RESPONSE_SIZE, LOOP_DELAY);
    if(result==IODevice::RESPONSE_OK){
        response=m_IODevice.getResponse();

        if(response.at(7)==0){
            request.remove(6, 6);
            request.append((char)0x0C);
            request.append((char)0x10);
            request.append((char)0x36);
            request.append((char)0x37);
            request.append((char)0x00);
            request.append(Functions::getRequestChecksum(request));

            RESPONSE_SIZE=10;
            response.clear();

            //try to read firmware version for 3664 mcu (for 2.74 firmware version only)
            result=m_IODevice.sendRequest(&request, RESPONSE_SIZE, LOOP_DELAY);
            response=m_IODevice.getResponse();

            if(result==IODevice::RESPONSE_FAIL){
                //return;
            }
        }
        else{
            request.remove(6, 6);
            request.append((char)0x0C);
            request.append((char)0x10);
            request.append((char)0xEC);
            request.append((char)0xEE);
            request.append((char)0x00);
            request.append(Functions::getRequestChecksum(request));

            RESPONSE_SIZE=11;
            response.clear();

            //try to read firmware version for 3664 mcu (for other firmware versions)
            result=m_IODevice.sendRequest(&request, RESPONSE_SIZE, LOOP_DELAY);
            response=m_IODevice.getResponse();

            if(result==IODevice::RESPONSE_FAIL){
                //return;
            }
        }
        int counter=response.at(6)==2 ? response.at(6) : 3;
        m_applianceData.append(response.mid(PREFIX_SIZE, counter));
        //return;
    }
    //try to connect to 3694 mcu
    else{
        request.remove(6, 6);
        request.append((char)0x90);
        request.append((char)0x10);
        request.append((char)0xF1);
        request.append((char)0xF1);
        request.append((char)0x00);
        request.append(Functions::getRequestChecksum(request));

        RESPONSE_SIZE=9;
        response.clear();

        //try read firmware version for 3694 mcu
        result=m_IODevice.sendRequest(&request, RESPONSE_SIZE, LOOP_DELAY);
        response=m_IODevice.getResponse();

        if(result==IODevice::RESPONSE_OK){
            request.remove(6, 6);
            request.append((char)0x90);
            request.append((char)0x10);
            request.append((char)0xEC);
            request.append((char)0xEE);
            request.append((char)0x00);
            request.append(Functions::getRequestChecksum(request));

            RESPONSE_SIZE=11;
            response.clear();

            result=m_IODevice.sendRequest(&request, RESPONSE_SIZE, LOOP_DELAY);
            response=m_IODevice.getResponse();

            if(result==IODevice::RESPONSE_OK){
                int counter=response.at(6)== 3 ? response.at(6) : 2;
                m_applianceData.append(response.mid(PREFIX_SIZE, counter));
                //return;
            }
        }
    }
    m_pApplianceParser->initApplianceParser(m_applianceData, m_lookUpTableData, m_lookUpTableDataSizesData);
    m_ApplianceModel=m_pApplianceParser->createApplianceModel();
}

void Evo2EepReader::readEep()
{
    //EVO_2 standart request to read eeprom data
    //0xA5, 0xEE, 0x02, 0x91, 0x30, 0x05, 0xA0, 0x00, 0x00, 0x3F, 0x00, 0xFB
    const int PREFIX_SIZE=7;
    const int BLOCK_SIZE=128;
    const int CHECKSUM_SIZE=1;
    //const int PAGE_SIZE=0x80;

    quint16 eepAreaSize=m_ApplianceModel.EepAreaSize;

    quint8 lowAddressStart=0;
    quint8 highAddressStart=0;
    quint8 lowAddressEnd=(quint8)0x3F;
    quint8 highAddressEnd=0;

    QByteArray tempEepData;

    emit signalReadEepStart(eepAreaSize);

    for(int i=0; i<eepAreaSize/0x80; i++){
        QByteArray request;
        QByteArray response;

        //request body
        request.append((char)0xA5);
        request.append((char)0xEE);
        request.append((char)0x02);
        request.append((char)0x91);
        request.append((char)0x30);
        request.append((char)0x05);
        request.append((char)0xA0);

        //request read start address
        request.append(lowAddressStart);
        request.append(highAddressStart);

        //request read end address
        request.append(lowAddressEnd);
        request.append(highAddressEnd);

        //request checksum
        request.append(Functions::getRequestChecksum(request));

        IODevice::IOResult result=m_IODevice.sendRequest(&request, PREFIX_SIZE+BLOCK_SIZE+CHECKSUM_SIZE, LOOP_DELAY);
        response=m_IODevice.getResponse();

        if(result==IODevice::RESPONSE_FAIL){
            throw QObject::tr("Failure read eep data.\n"
                              "Maybe device not connected");
        }

        //request addresses correction
        lowAddressEnd=(quint8)(lowAddressEnd + 0x40);
        if(lowAddressEnd==0x3F){
            highAddressEnd=(quint8)(highAddressEnd + 1);
        }

        lowAddressStart=(quint8)(lowAddressStart + 0x40);
        if(lowAddressStart==0){
            highAddressStart=(quint8)(highAddressStart + 1);
        }

        emit signalReadEepStep(0x80);

        tempEepData.append(response.mid(PREFIX_SIZE, BLOCK_SIZE));
    }

    quint16 deviceTypeCode=m_ApplianceModel.DeviceTypeCode;
    Q_UNUSED(deviceTypeCode)

    m_eepData=Functions::eepToBinToEep(tempEepData);

    emit signalReadApplianceOK(m_ApplianceModel);
    emit signalReadEepStop(m_eepData);
}

void Evo2EepReader::readLastFault()
{
    //EVO_2 standart request to read last failure data
    //0xA5, 0xEE, 0x02, 0x93, 0x10, 0x05
    QByteArray request;
    QByteArray response;
    const quint16 RESPONSE_SIZE=9;
    const quint16 FAILURE_INDEX=7;
    QString firmwareVersion=m_ApplianceModel.FirmwareVersion;

    request.append((char)0xA5);
    request.append((char)0xEE);
    request.append((char)0x02);
    request.append((char)0x93);
    request.append((char)0x10);
    request.append((char)0x05);

    if(m_currentFaultRequestMap.contains(firmwareVersion)){
        request.append(m_currentFaultRequestMap.value(firmwareVersion));
    }
    else{
        request.append(m_currentFaultRequestMap.value("02.71"));
    }
    request.append(Functions::getRequestChecksum(request));

    IODevice::IOResult result=m_IODevice.sendRequest(&request, RESPONSE_SIZE, LOOP_DELAY);
    response=m_IODevice.getResponse();

    if(result==IODevice::RESPONSE_OK){
        m_lastFaultData=response.at(FAILURE_INDEX) <= 18 ? response.at(FAILURE_INDEX) : 0;
    }
    else{
        m_lastFaultData=0x00;
    }

    quint16 deviceTypeCode=m_ApplianceModel.DeviceTypeCode;
    quint16 eepAreaSize=m_ApplianceModel.EepAreaSize;
    emit signalReadLastFaultOK(deviceTypeCode, eepAreaSize, m_lastFaultData);
}

void Evo2EepReader::readFaultsBlock()
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
