#include "ArcadiaEepParser.h"

ArcadiaEepParser::ArcadiaEepParser()
{

}

void ArcadiaEepParser::checkEepData(const QByteArray &eepData)
{
    const quint16 CORRECTION_VALUE=2;
    const quint16 INFO_SIZE=6;

    m_eepInfoStartAddress=(quint8)eepData.at(0) * 0x100 +
            (quint8)eepData.at(1);
    m_eepInfoStartAddress=m_eepInfoStartAddress * CORRECTION_VALUE + CORRECTION_VALUE;

    if(m_eepInfoStartAddress >= eepData.size()-25){
        m_eepModel.IsModelValid=false;
        return;
    }

    QStringList list;
    for(int i=m_eepInfoStartAddress; i<m_eepInfoStartAddress + INFO_SIZE; i++){
        list.push_back(QString::number(eepData.at(i),16).rightJustified(2, '0'));
    }
    foreach(QString l, list){
        m_eepMarker+=l;
    }
    if(!m_eepMarkersMap.contains(m_eepMarker)){
        m_eepModel.IsModelValid=false;
        return;
    }
    m_eepModel.IsModelValid=true;
}

void ArcadiaEepParser::checkEepChecksum(const QByteArray &eepData)
{
    if(!m_eepModel.IsModelValid){
        return;
    }

    quint16 checksumAddress=(quint8)eepData.at(18) * 0x100 + (quint8)eepData.at(19);
    checksumAddress=checksumAddress * 2;
    if(eepData.size() < (checksumAddress+2)){
        m_eepModel.IsModelValid=false;
        return;
    }
    m_eepModel.FileChecksum=(quint8)eepData.at(checksumAddress) * 0x100 +
            (quint8)eepData.at(checksumAddress+1);

    m_eepModel.RealChecksum=getSimpleChecksum(eepData, checksumAddress);

    if(m_eepModel.RealChecksum!=m_eepModel.FileChecksum){    
        m_eepModel.RealChecksum=getCrcChecksum(eepData, checksumAddress);
        if(m_eepModel.RealChecksum!=m_eepModel.FileChecksum){
            m_eepModel.IsChecksumValid=false;
        }
        else{
            m_eepModel.IsChecksumValid=true;
        }
    }
    else{
        m_eepModel.IsChecksumValid=true;
    }
    m_eepModel.Data.append(eepData);
}

void ArcadiaEepParser::setEepModel(const QByteArray &eepData)
{
    if(!m_eepModel.IsModelValid){
        return;
    }
    QString model="";
    const quint16 MODEL_CORRECTION_INDEX=18;
    quint16 modelInformationAddress=m_eepInfoStartAddress + MODEL_CORRECTION_INDEX;
    model=QString(eepData.mid(modelInformationAddress,14)).trimmed().replace("/","").replace("\0", "");
    m_eepModel.Model=model;
}

void ArcadiaEepParser::setEepCode(const QByteArray &eepData)
{
    if(!m_eepModel.IsModelValid){
        return;
    }
    QString product="";
    const quint16 PRODUCT_CORRECTION_INDEX=12;
    quint16 productInformationAddress=m_eepInfoStartAddress + PRODUCT_CORRECTION_INDEX;
    for(int i=0;i<6;i++){
       QString productItem=QString::number((quint8)eepData.at(productInformationAddress + i),16).rightJustified(2, '0');
       product+=productItem;
    }
    m_eepModel.Code=product.mid(0, 11);
}

void ArcadiaEepParser::setEepSerial(const QByteArray &eepData)
{
    if(!m_eepModel.IsModelValid){
        return;
    }
    QString serial="";
    const quint16 SERIAL_CORRECTION_INDEX=6;
    quint16 serialInformationAddresss=m_eepInfoStartAddress + SERIAL_CORRECTION_INDEX;
    for(int i=0;i<6;i++){
        QString serialItem=QString::number((quint8)eepData.at(serialInformationAddresss + i),16).rightJustified(2, '0');
        serial+=serialItem;
    }
    m_eepModel.Serial=serial.mid(0, 9);
}
