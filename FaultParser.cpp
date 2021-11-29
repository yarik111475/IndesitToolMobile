#include "FaultParser.h"

bool FaultParser::isDigits(const QString str)
{
    bool result=false;
    foreach(char ch, str.toStdString()){
        if(isdigit(ch)){
            result=true;
        }
        else{
            result=false;
        }
    }
    return result;
}

FaultParser::FaultParser()
{

}

FaultModel FaultParser::getLastFaultModel()
{
    return lastFaultModel;
}

QList<FaultModel> FaultParser::getFaultModelsList()
{
    return faultModelsList;
}

void FaultParser::initFaultParser(quint16 deviceTypeCode, quint16 eepSize, const QString &languageCode)
{
    m_deviceTypeCode=deviceTypeCode;
    m_eepAreaSize=eepSize;
    m_languageCode=languageCode;
}

void FaultParser::initForParseEepData(quint16 deviceTypeCode, quint16 eepSize, const QString &languageCode, const QByteArray &eepData)
{
    m_deviceTypeCode=deviceTypeCode;
    m_eepAreaSize=eepSize;
    m_languageCode=languageCode;
    m_eepData.append(eepData);
}
