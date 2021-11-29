#include "Evo1EepParser.h"

Evo1EepParser::Evo1EepParser()
{

}

void Evo1EepParser::checkEepData(const QByteArray &eepData)
{
    if(eepData.size() > 0x800){
        m_eepModel.IsModelValid=false;
        return;
    }

    const quint16 CORRECTION_VALUE=0;
    m_eepInfoStartAddress=(quint8)eepData.at(0) * 0x100 +
            (quint8)eepData.at(1);
    m_eepInfoStartAddress=m_eepInfoStartAddress * 2 + CORRECTION_VALUE;

    if(m_eepInfoStartAddress!=0x24){
        m_eepModel.IsModelValid=false;
        return;
    }

    QStringList list;
    for(int i=m_eepInfoStartAddress; i<m_eepInfoStartAddress+6; i++){
        list.push_back(QString::number(eepData.at(i),16).rightJustified(2, '0'));
    }
    foreach(QString l, list){
        m_eepMarker+=l;
    }

    if(!m_eepMarker.startsWith("030609")){
        m_eepModel.IsModelValid=false;
        return;
    }
    m_eepModel.IsModelValid=true;
}

void Evo1EepParser::checkEepChecksum(const QByteArray &eepData)
{
    if(!m_eepModel.IsModelValid){
        return;
    }

    quint16 checksumAddress=(quint8)eepData.at(18) * 0x100
            + (quint8)eepData.at(19);

    checksumAddress=checksumAddress * 2;
    if(eepData.size() < (checksumAddress + 2)){
        m_eepModel.IsModelValid=false;
        return;
    }
    m_eepModel.FileChecksum=(quint8)eepData.at(checksumAddress) * 0x100 +
            (quint8)eepData.at(checksumAddress+1);

    m_eepModel.RealChecksum=getSimpleChecksum(eepData, checksumAddress);
    if(m_eepModel.RealChecksum!=m_eepModel.FileChecksum){
        m_eepModel.IsChecksumValid=false;
    }
    else{
        m_eepModel.IsChecksumValid=true;
    }
    m_eepModel.Data.append(eepData);
}

void Evo1EepParser::setEepModel(const QByteArray &eepData)
{
    if(!m_eepModel.IsModelValid){
        return;
    }
    QString model="";
    const quint16 MODEL_CORRECTION_INDEX=18;
    quint16 modelInformationAddress=m_eepInfoStartAddress+MODEL_CORRECTION_INDEX;
    model=QString(eepData.mid(modelInformationAddress,14)).trimmed().replace("/","").replace("\0", "");
    m_eepModel.Model=model;
}

void Evo1EepParser::setEepCode(const QByteArray &eepData)
{
    if(!m_eepModel.IsModelValid){
        return;
    }
    QString code="";
    const quint16 PRODUCT_CORRECTION_INDEX=12;
    quint16 productInformationAddress=m_eepInfoStartAddress+PRODUCT_CORRECTION_INDEX;
    for(int i=0;i<6;i++){
        QString codeItem=QString::number((quint8)eepData.at(productInformationAddress + i),16).rightJustified(2, '0');
        code+=codeItem;
    }
    m_eepModel.Code=code.mid(0,11);
}

void Evo1EepParser::setEepSerial(const QByteArray &eepData)
{
    if(!m_eepModel.IsModelValid){
        return;
    }
    QString serial="";
    const quint16 SERIAL_CORRECTION_INDEX=6;
    quint16 serialInformationAddresss=m_eepInfoStartAddress+SERIAL_CORRECTION_INDEX;
    for(int i=0;i<6;i++){
        QString serialItem=QString::number((quint8)eepData.at(serialInformationAddresss + i),16).rightJustified(2, '0');
        serial+=serialItem;
    }
    m_eepModel.Serial=serial.mid(0, 9);
}
