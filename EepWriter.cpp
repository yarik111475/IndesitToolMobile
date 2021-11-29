#include "EepWriter.h"

EepWriter::EepWriter(QObject *parent) : QObject(parent)
{
    QObject::connect(&m_IODevice, &IODevice::signalDisconnected,
                     this, &EepWriter::signalIODeviceDisconnected);
}

EepWriter::~EepWriter()
{
    if(m_pApplianceParser){
        delete m_pApplianceParser;
    }
}

void EepWriter::init(const QString& bluetoothAddress, ApplianceParser *pApplianceParser, const QByteArray eepData)
{
    m_eepData.append(eepData);
    m_pApplianceParser=pApplianceParser;
    m_bluetoothAddress=bluetoothAddress;
}

void EepWriter::slotWriteEep()
{
    try{
        m_IODevice.init(m_bluetoothAddress);
        bool result=m_IODevice.connect();
        if(!result){
            throw QObject::tr("Can not open I/O device");
        }
        readAppliance();
        checkEepSize();
        writeEep();
    }
    catch(QString exceptionMessage){
        emit signalException(exceptionMessage);
    }
    catch(...){
        emit signalException(QObject::tr("Generic write failure"));
    }
    m_IODevice.disconnect();
}
