#include "Evo2EepWriter.h"

Evo2EepWriter::Evo2EepWriter(QObject *parent):EepWriter(parent)
{

}

void Evo2EepWriter::readAppliance()
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
    IODevice::IOResult result=m_IODevice.sendRequest(&request, RESPONSE_SIZE);
    response=m_IODevice.getResponse();

    if(result==IODevice::RESPONSE_OK){
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
            result=m_IODevice.sendRequest(&request, RESPONSE_SIZE);
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
            result=m_IODevice.sendRequest(&request, RESPONSE_SIZE);
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
        result=m_IODevice.sendRequest(&request, RESPONSE_SIZE);
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

            result=m_IODevice.sendRequest(&request, RESPONSE_SIZE);
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

void Evo2EepWriter::checkEepSize()
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

void Evo2EepWriter::writeEep()
{
    //EVO_2 standart write request to write eep data
    //0xA5 0xEE 0x02 0x91 0x40 0x23 0xA0 <low_start_address> <high_start_address> <data_body> <checksum>
    quint16 eepAreaSize=m_ApplianceModel.EepAreaSize;
    quint16 eepAreaAddress=m_ApplianceModel.EepAreaAddress;

    quint8 highEepAreaAddress=(quint8)(eepAreaAddress >> 8);
    quint8 lowEepAreaAddress=(quint8)eepAreaAddress;
    //const int PAGE_SIZE=0x20;
    const int RESPONSE_SIZE=1;

    QByteArray tempEepData;
    tempEepData.append(Functions::eepToBinToEep(m_eepData));

    setWriteProtect(false);
    emit signalWriteEepStart(eepAreaSize);

    for(int i=0; i<eepAreaSize/0x20; i++){
        QByteArray request;
        //request body
        request.append((char)0xA5);
        request.append((char)0xEE);
        request.append((char)0x02);
        request.append((char)0x91);
        request.append((char)0x40);
        request.append((char)0x23);
        request.append((char)0xA0);

        //write start address
        request.append(lowEepAreaAddress);
        request.append(highEepAreaAddress);

        //writing data
        QByteArray eepDataBlock;
        eepDataBlock.append(tempEepData.mid(i * 0x20, 0x20));

        request.append(eepDataBlock);
        request.append(Functions::getRequestChecksum(request));

        QByteArray response;
        IODevice::IOResult result=m_IODevice.sendRequest(&request, RESPONSE_SIZE);
        response=m_IODevice.getResponse();

        if(result==IODevice::RESPONSE_FAIL){
            throw QObject::tr("Failure writing data to device Eeprom.\n"
                              "Maybe device not connected");
        }
        lowEepAreaAddress=(quint8)(lowEepAreaAddress + (0x20/2));
        if(lowEepAreaAddress==0){
            highEepAreaAddress=(quint8)(highEepAreaAddress+1);
        }

        emit signalWriteEepStep(0x20);

        QThread::msleep(100);
    }

    setWriteProtect(true);
    emit signalReadApplianceOK(m_ApplianceModel);
    emit signalWriteEepStop();
}

void Evo2EepWriter::setWriteProtect(bool state)
{
    //EVO_2 standart request to disable eeprom write protect
    //type 1
    //0xA5 0xEE 0x02 0x93 0x20 0x05 0x0C 0x20 0x00 0xDB 0x00 <checksum>(0x54)
    //type 2
    //0xA5 0xEE 0x02 0x93 0x20 0x05 0x90 0x20 0x00 0xDB 0x00 <checksum>(0xD8)


    //EVO_2 standart request to enable eeprom write protect
    //type 1
    //0xA5 0xEE 0x02 0x93 0x20 0x05 0x0C 0x20 0x00 0x00 0x00 <checksum>
    //type 2

    QByteArray request;
    QByteArray response;
    const quint8 RESPONSE_SIZE=1;

    request.push_back((char)0xA5);
    request.push_back((char)0xEE);
    request.push_back((char)0x02);
    request.push_back((char)0x93);
    request.push_back((char)0x20);
    request.push_back((char)0x05);
    request.push_back((char)0x0C);
    request.push_back((char)0x20);

    //if WP must be ON
    if(state){
        request.push_back((char)0x00);
        request.push_back((char)0x00);
        request.push_back((char)0x00);
    }
    //if WP must be OFF
    else{
        request.push_back((char)0x00);
        request.push_back((char)0xDB);
        request.push_back((char)0x00);
    }
    request.push_back(Functions::getRequestChecksum(request));


    IODevice::IOResult result=m_IODevice.sendRequest(&request, RESPONSE_SIZE);
    response=m_IODevice.getResponse();

    if(result==IODevice::RESPONSE_FAIL){
        //if WP must be ON
        if(state){
            request[6]=(quint8)0x90;
            request[11]=(quint8)0x00;
            request[11]=(Functions::getRequestChecksum(request));
        }
        //if WP must be OFF
        else{
            request[6]=(quint8)0x90;
            request[11]=(quint8)0x00;
            request[11]=(Functions::getRequestChecksum(request));
        }

        result=m_IODevice.sendRequest(&request, RESPONSE_SIZE);
        response=m_IODevice.getResponse();

        if(result==IODevice::RESPONSE_FAIL){
            throw QObject::tr("Can not operate with eeprom write protect");
        }
    }
}
