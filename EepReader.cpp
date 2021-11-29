#include "EepReader.h"

EepReader::EepReader(QObject *parent) : QObject(parent)
{
    QObject::connect(&m_IODevice, &IODevice::signalDisconnected,
                     this, &EepReader::signalIODeviceDisconnected);
}

void EepReader::init(QString bluetoothAddress, ApplianceParser *pApplianceParser)
{
    m_bluetoothAddress=bluetoothAddress;
    m_pApplianceParser=pApplianceParser;
}

EepReader::~EepReader()
{
    if(m_pApplianceParser){
        delete m_pApplianceParser;
    }
}

void EepReader::slotReadEep()
{
    try{
        m_IODevice.init(m_bluetoothAddress);
        bool result=m_IODevice.connect();
        if(!result){
            throw QObject::tr("Can not open I/O device");
        }
        readAppliance();
        readEep();
        readLastFault();
        readFaultsBlock();
    }
    catch(QString exceptionMessage){
        emit signalException(exceptionMessage);
    }
    catch(...){
        emit signalException(QObject::tr("Generic read failure"));
    }
    m_IODevice.disconnect();
}
