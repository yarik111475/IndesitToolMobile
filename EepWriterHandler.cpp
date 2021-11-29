#include "EepWriterHandler.h"

EepWriterHandler::EepWriterHandler(FileHandler *pFileHandler, QObject *parent) : QObject(parent),
    m_pEepWriter(nullptr),
    m_pFileHandler(nullptr)
{
    m_pFileHandler=pFileHandler;
}

void EepWriterHandler::init(const QString &applianceName, const QString &bluetoothAddress )
{
    if(!m_pFileHandler->isEepModelValid()){
        emit signalException(QObject::tr("First you must open eep data file to write"));
    }
    else{
        m_applianceName=applianceName;
        ApplianceParser* pApplianceParser=Factories::createApplianceParser(m_applianceName);
        m_pEepWriter=Factories::createEepWriter(m_applianceName);
        m_pEepWriter->init(bluetoothAddress,pApplianceParser, m_pFileHandler->Model.Data);

        QObject::connect(m_pEepWriter, &EepWriter::signalEraseEep,
                         this, &EepWriterHandler::signalEraseEep);
        QObject::connect(m_pEepWriter, &EepWriter::signalWriteEepStart,
                         this, &EepWriterHandler::signalWriteEepStart);
        QObject::connect(m_pEepWriter, &EepWriter::signalWriteEepStep,
                         this, &EepWriterHandler::signalWriteEepStep);
        QObject::connect(m_pEepWriter, &EepWriter::signalWriteEepStop,
                         this, &EepWriterHandler::slotWriteEepStop);

        QObject::connect(m_pEepWriter, &EepWriter::signalIODeviceDisconnected,
                         this, &EepWriterHandler::slotIODeviceDisconnected);

        QObject::connect(m_pEepWriter, &EepWriter::signalException,
                         this, &EepWriterHandler::slotException);

        QObject::connect(m_pEepWriter, &EepWriter::signalReadApplianceOK,
                         this, &EepWriterHandler::slotReadApplianceOK);
    }
}

void EepWriterHandler::slotWriteEep()
{
    if(m_pEepWriter){
        m_pEepWriter->slotWriteEep();
    }
}

void EepWriterHandler::slotWriteEepStop()
{
    emit signalWriteEepStop();
}

void EepWriterHandler::slotReadApplianceOK(const ApplianceModel &applianceModel)
{
    m_applianceModel=applianceModel;
}

void EepWriterHandler::slotException(const QString &message)
{
    if(m_pEepWriter){
        m_pEepWriter->deleteLater();
    }
    emit signalException(message);
}

void EepWriterHandler::slotIODeviceDisconnected()
{
    if(m_pEepWriter){
        m_pEepWriter->deleteLater();
    }
    emit signalIODeviceDisconnected();
}


