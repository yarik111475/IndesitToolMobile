#include "EepParser.h"

EepParser::EepParser()
{
    m_eepMarkersMap.insert("030609101111", 0);
    m_eepMarkersMap.insert("030609101211", 0);
    m_eepMarkersMap.insert("030609101212", 0);
    m_eepMarkersMap.insert("030609101213", 0);
    m_eepMarkersMap.insert("030609101214", 0);
    m_eepMarkersMap.insert("04070A111315", 2);
}

EepModel EepParser::getEepModel() const
{
    return m_eepModel;
}

void EepParser::parseEep(const QByteArray &eepData)
{
    m_eepMarker="";
    m_eepModel.reset();
    try{
        checkEepData(eepData);
        checkEepChecksum(eepData);
        setEepModel(eepData);
        setEepCode(eepData);
        setEepSerial(eepData);
    }
    catch(...){
        m_eepModel.IsModelValid=false;
    }
}

quint16 EepParser::getCrcChecksum(const QByteArray &eepData, quint16 checksumAddress)
{
    quint16 checkSum=0;
    for(int i=0;i<checksumAddress; i++){
        checkSum = (quint16)(checkSum ^(quint8)eepData.at(i));
        checkSum =(quint16)(2 * checkSum) | ((checkSum & 0x8000) !=0 ? 1 : 0);
    }
    return checkSum;
}

quint16 EepParser::getSimpleChecksum(const QByteArray &eepData, quint16 checksumAddress)
{
    quint16 checkSum=0;
    for(int i=0;i<checksumAddress; i++){
        checkSum+=(quint8)eepData.at(i);
    }
    return checkSum;
}
