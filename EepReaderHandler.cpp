#include "EepReaderHandler.h"
#include "Functions.h"

EepReaderHandler::EepReaderHandler(FileHandler *pFileHandler, QObject *parent) : QObject(parent),
    m_pEepReader(nullptr),
    m_pFileHandler(nullptr),
    m_writeCheck(false)
{
    m_pFileHandler=pFileHandler;
}

void EepReaderHandler::init(const QString& applianceName, const QString& bluetoothAddress, bool writeCheck)
{
    m_writeCheck=writeCheck;
    m_applianceName=applianceName;

    ApplianceParser* pApplianceParser=Factories::createApplianceParser(m_applianceName);
    m_pEepReader=Factories::createEepReader(m_applianceName);
    m_pEepReader->init(bluetoothAddress, pApplianceParser);

    QObject::connect(m_pEepReader, &EepReader::signalReadEepStart,
                     this, &EepReaderHandler::signalReadEepStart);
    QObject::connect(m_pEepReader, &EepReader::signalReadEepStep,
                     this, &EepReaderHandler::signalReadEepStep);
    QObject::connect(m_pEepReader, &EepReader::signalReadEepStep,
                     this, &EepReaderHandler::signalReadEepStep);
    QObject::connect(m_pEepReader, &EepReader::signalReadEepStop,
                     this, &EepReaderHandler::slotReadEepStop);

    QObject::connect(m_pEepReader, &EepReader::signalIODeviceDisconnected,
                     this, &EepReaderHandler::slotIODeviceDisconnected);


    QObject::connect(m_pEepReader, &EepReader::signalException,
                     this, &EepReaderHandler::slotException);

    QObject::connect(m_pEepReader, &EepReader::signalReadApplianceOK,
                     this, &EepReaderHandler::slotReadApplianceOK);

    QObject::connect(m_pEepReader, &EepReader::signalReadLastFaultOK,
                     this, &EepReaderHandler::slotReadLastFaultOK);
    QObject::connect(m_pEepReader, &EepReader::signalReadFaultsBlockOK,
                     this, &EepReaderHandler::slotReadFaultsBlockOK);
}

void EepReaderHandler::slotReadEep(bool writeCheck)
{
    if(m_pEepReader){
        if(writeCheck){
            QThread::msleep(2000);
        }
        m_pEepReader->slotReadEep();
    }
}

void EepReaderHandler::slotReadEepStop(const QByteArray &eepData)
{
    EepParser* pEepParser=Factories::createEepParser(m_applianceName);
    pEepParser->parseEep(eepData);
    if(m_writeCheck){
        EepModel tempEepModel=pEepParser->getEepModel();
        if(tempEepModel==m_pFileHandler->Model){
            m_pFileHandler->Model=tempEepModel;
        }
        else{
            emit signalException(QObject::tr("Failure when write eep data.\nTry to write eep data again"));
            if(pEepParser){
                delete pEepParser;
                pEepParser=nullptr;
            }
            return;
        }
    }
    else{
        m_pFileHandler->Model=pEepParser->getEepModel();
        if(pEepParser){
            delete pEepParser;
            pEepParser=nullptr;
        }
    }

    EepProxyModel eepProxyModel;
    eepProxyModel.DeviceType=m_applianceModel.DeviceType;
    eepProxyModel.McuType=m_applianceModel.McuType;
    eepProxyModel.FirmwareVersion=m_applianceModel.FirmwareVersion;
    eepProxyModel.EepAreaSize=QString::number(m_applianceModel.EepAreaSize/1024) + " Kb";

    eepProxyModel.Model=m_pFileHandler->Model.Model;
    eepProxyModel.Code=m_pFileHandler->Model.Code;
    eepProxyModel.Serial=m_pFileHandler->Model.Serial;
    eepProxyModel.IsModelValid=m_pFileHandler->Model.IsModelValid;
    eepProxyModel.IsChecksumValid=m_pFileHandler->Model.IsChecksumValid;

    emit signalReadEepStop(eepProxyModel);
}

void EepReaderHandler::slotReadLastFaultOK(quint16 deviceTypeCode, quint16 eepAreaSize, const quint8 lastFault)
{
    FaultParser* pFaultParser=Factories::createFaultParser(m_applianceName);
    pFaultParser->initFaultParser(deviceTypeCode, eepAreaSize, "RU");
    pFaultParser->parseLastFault(lastFault);
    FaultModel lastFaultModel=pFaultParser->getLastFaultModel();
    emit signalReadLastFaultOK(lastFaultModel);

    delete pFaultParser;
    pFaultParser=nullptr;
}

void EepReaderHandler::slotReadFaultsBlockOK(quint16 deviceTypeCode, quint16 eepAreaSize, const QByteArray &faultsBlock)
{
    FaultParser* pFaultParser=Factories::createFaultParser(m_applianceName);
    pFaultParser->initFaultParser(deviceTypeCode,eepAreaSize, "RU");
    pFaultParser->parseFaultsList(faultsBlock);
    QList<FaultModel> faultsList=pFaultParser->getFaultModelsList();
    emit signalReadFaultsBlockOK(faultsList);

    delete pFaultParser;
    pFaultParser=nullptr;
}

void EepReaderHandler::slotReadApplianceOK(const ApplianceModel &applianceModel)
{
    m_applianceModel=applianceModel;
}

void EepReaderHandler::slotException(const QString& message)
{
    if(m_pEepReader){
        m_pEepReader->deleteLater();
    }
    emit signalException(message);
}

void EepReaderHandler::slotIODeviceDisconnected()
{
    if(m_pEepReader){
        m_pEepReader->deleteLater();
    }
    emit signalIODeviceDisconnected();
}
