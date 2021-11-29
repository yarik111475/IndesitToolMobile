#include "Evo2FaultParser.h"

Evo2FaultParser::Evo2FaultParser()
{

}

void Evo2FaultParser::parseLastFault(const char lastFaultData)
{
    QString faultCode=QString::number(lastFaultData, 16).rightJustified(2, '0');
    if(!isDigits(faultCode)){
        faultCode=QString::number(lastFaultData, 10).rightJustified(2, '0');
    }
    lastFaultModel.FaultCode=faultCode;
    lastFaultModel.SubCode="00";
    lastFaultModel.Occurence="--";
    lastFaultModel.Description=sqliteManager.getFaultDescription(m_deviceTypeCode,m_eepAreaSize,m_languageCode,faultCode.toInt(),0);

    lastFaultModel.IsFaultClear=(lastFaultModel.FaultCode=="00");
}

void Evo2FaultParser::parseFaultsList(const QByteArray &faultsData)
{

    quint8 faultIndex=1;
    for(int i=0; i<faultsData.size(); i+=2){
        FaultModel faultModel;
        faultModel.FaultCode=QString::number(faultIndex++, 10).rightJustified(2,'0');
        faultModel.SubCode="--";
        faultModel.Occurence=QString::number(0xFFFF - ((quint8)faultsData.at(i) *
                                                       0x100 + (quint8)faultsData.at(i+1)), 10).rightJustified(2,'0');
        faultModel.Description=sqliteManager.getFaultDescription(m_deviceTypeCode,m_eepAreaSize,m_languageCode,
                                                                 faultModel.FaultCode.toInt(), 0);
        faultModel.IsFaultClear=(faultModel.Occurence=="00");

        faultModelsList.push_back(faultModel);
    }
}

void Evo2FaultParser::parseLastFaultFromEepData(){
    parseLastFault(0x00);
}

void Evo2FaultParser::parseFailtsListFromEepData()
{
    const quint8 ADDRESS_INDEX=0x0E;
    const quint8 CORRECTION_VALUE=0xA2;
    const quint8 FAULTS_BLOCK_SIZE=0x20;
    QByteArray faultsBlock;

    quint16 faultsBlockStartAddress=((quint8)m_eepData.at(ADDRESS_INDEX) * 0x100 +
                                     (quint8)m_eepData.at(ADDRESS_INDEX+1)) * 2 + CORRECTION_VALUE;
    if(m_eepData.size() < (faultsBlockStartAddress + FAULTS_BLOCK_SIZE)){
        faultsBlock.fill(0xFF, FAULTS_BLOCK_SIZE);
    }
    else{
        faultsBlock.append(m_eepData.mid(faultsBlockStartAddress, FAULTS_BLOCK_SIZE));
    }
    parseFaultsList(faultsBlock);
}
