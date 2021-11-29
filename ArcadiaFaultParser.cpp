#include "ArcadiaFaultParser.h"

ArcadiaFaultParser::ArcadiaFaultParser()
{

}

void ArcadiaFaultParser::parseLastFault(const char lastFaultData)
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

void ArcadiaFaultParser::parseFaultsList(const QByteArray &faultsData)
{
    QByteArray tempRawFaultsData;
    if(m_deviceTypeCode!=0x02){
        tempRawFaultsData.append(faultsData.mid(0,3));
        tempRawFaultsData[1]=static_cast<quint8>(0x00);
    }
    else if(m_eepAreaSize==0x2000){
        tempRawFaultsData.append(faultsData.mid(0,15));
    }
    else{
        tempRawFaultsData.append(faultsData);
    }

    for(int i=0; i<tempRawFaultsData.size(); i+=3){
        quint8 rawCode=static_cast<quint8>(tempRawFaultsData.at(i));
        QString code=QString::number(rawCode, 16).rightJustified(2,'0');
        if(!isDigits(code)){
            code=QString::number(rawCode, 10).rightJustified(2, '0');
        }

        quint8 rawSubcode=static_cast<quint8>(tempRawFaultsData.at(i+1));
        QString subCode=QString::number(rawSubcode, 16).rightJustified(2, '0');
        if(!isDigits(subCode)){
            subCode=QString::number(rawSubcode, 10).rightJustified(2, '0');
        }


        quint8 rawOccurence=static_cast<quint8>(tempRawFaultsData.at(i+2));
        QString occurence=QString::number(rawOccurence, 16).rightJustified(2, '0');
        if(!isDigits(occurence)){
            occurence=QString::number(rawOccurence, 10).rightJustified(2, '0');
        }

        if(code=="00"){
            subCode="00";
            occurence="00";
        }
        bool ok=false;
        int intCode=code.toInt(&ok);
        if(ok){
            if(intCode>18){
                code="00";
                subCode="00";
                occurence="00";
            }
        }

        FaultModel faultModel; 
        faultModel.FaultCode=code;
        faultModel.SubCode=subCode;
        faultModel.Occurence=occurence;
        faultModel.Description=sqliteManager.getFaultDescription(m_deviceTypeCode,m_eepAreaSize,m_languageCode,code.toInt(),subCode.toInt());

        faultModel.IsFaultClear=(faultModel.FaultCode=="00");

        faultModelsList.push_back(faultModel);
    }
}
