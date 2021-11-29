#include "Evo1EepWriter.h"

Evo1EepWriter::Evo1EepWriter()
{

}

void Evo1EepWriter::readAppliance()
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

void Evo1EepWriter::checkEepSize()
{
    //Check if raw eep size is correct for target device (and fill it wth 0xFF if size is less)
    quint16 eepAreaSize=m_ApplianceModel.EepAreaSize;
    if(m_eepData.size() > eepAreaSize){
        throw QObject::tr("Uncorrect eep size for device Eeprom");
    }
    else if(m_eepData.size() < eepAreaSize){
        QByteArray correctiontBlock;
        correctiontBlock.fill(0xFF,eepAreaSize-m_eepData.size());
        m_eepData.append(correctiontBlock);
    }
}

void Evo1EepWriter::writeEep()
{
    //EVO_1 standart write request to write eep data
    //0xA5, <address_high> (0x40), <address_low> (0x00), <data> (0x00, 0x00), <checksum> (0x00)
    //const quint16 PAGE_SIZE=0x2;
    const quint16 RESPONSE_SIZE=1;
    quint8 addressHigh=0x040;
    quint16 eepAreaSize=m_ApplianceModel.EepAreaSize;
    quint8 addressLow=0x00;
    QByteArray eepData=Functions::eepToBinToEep(m_eepData);

    setWriteProtect(false);
    emit signalWriteEepStart(eepAreaSize);

    for(int i=0; i<eepAreaSize; i+=0x02){
        QByteArray request;
        QByteArray response;
        QByteArray eepBlock=eepData.mid(i,0x02);

        request.push_back((char)0xA5);
        request.push_back(addressHigh);
        request.push_back(addressLow);

        request.push_back(eepBlock.at(0));
        request.push_back(eepBlock.at(1));

        request.push_back(Functions::getRequestChecksum(request));

        IODevice::IOResult result=m_IODevice.sendRequest(&request, RESPONSE_SIZE,true);
        if(result==IODevice::RESPONSE_FAIL){
            throw QObject::tr("Failure writing data to device Eeprom.\n"
                              "Maybe device not connected");
        }

        addressLow=(quint8)(addressLow+1);
        if(addressLow==0x00){
            addressHigh=(quint8)(addressHigh + 1);
        }


        emit signalWriteEepStep(0x02);
    }

    setWriteProtect(true);
    emit signalReadApplianceOK(m_ApplianceModel);
    emit signalWriteEepStop();
}

void Evo1EepWriter::setWriteProtect(bool state)
{
    //EVO_1 standart request to disable write protect
    //0xA5, 0x2F, 0x10, 0xC9, 0x0F, <checksum> (0xBC)

    //EVO_1 standart request to enable write protect
    //0xA5, 0x2F, 0x10, 0x00, 0x0F, <checksum> (0xF3)

    QByteArray request;
    QByteArray response;
    const quint16 WP_RESPONSE_SIZE=1;

    request.push_back((char)0xA5);
    request.push_back((char)0x2F);
    request.push_back((char)0x10);

    //if WP must be ON
    if(state){
        request.push_back((char)0x00);
    }
    //if WP must be OFF
    else{
        request.push_back((char)0xC9);
    }
    request.push_back((char)0x0F);

    request.push_back(Functions::getRequestChecksum(request));

    IODevice::IOResult result=m_IODevice.sendRequest(&request, WP_RESPONSE_SIZE, true);
    if(result==IODevice::RESPONSE_FAIL){
        throw QObject::tr("Can not operate with eeprom write protect");
    }
}
