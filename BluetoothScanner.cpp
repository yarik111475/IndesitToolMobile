#include "BluetoothScanner.h"

void BluetoothScanner::slotFinished()
{
    if(m_pLocalDevice){
        delete m_pLocalDevice;
        m_pLocalDevice=nullptr;
    }
    if(m_pDiscoveryAgent){
        delete m_pDiscoveryAgent;
        m_pDiscoveryAgent=nullptr;
    }
    emit signalScanException(QObject::tr("No bluetooth devices found"));
}

void BluetoothScanner::slotCanceled()
{
    if(m_pLocalDevice){
        delete m_pLocalDevice;
        m_pLocalDevice=nullptr;
    }
    if(m_pDiscoveryAgent){
        delete m_pDiscoveryAgent;
        m_pDiscoveryAgent=nullptr;
    }
}

void BluetoothScanner::slotDeviceDiscovered(const QBluetoothDeviceInfo &info)
{
    QString deviceName=info.name().trimmed();
    if(deviceName.startsWith("Indesit")){
        m_bluetoothAddressesMap.insert(info.name().trimmed(), info.address().toString());
        m_pDiscoveryAgent->stop();
        emit signalScanStop();
    }
}

void BluetoothScanner::slotError(QBluetoothDeviceDiscoveryAgent::Error error)
{
    qDebug()<<"bluetooth scanner error: " << error;
    emit signalScanException(QObject::tr("Generic blueutooth scan error"));
}

BluetoothScanner::BluetoothScanner(QObject *parent) : QObject(parent),
    m_pLocalDevice(nullptr), m_pDiscoveryAgent(nullptr)
{
}

BluetoothScanner::~BluetoothScanner()
{

}

QStringList BluetoothScanner::getBluetoothDevicesList() const
{
    return m_bluetoothAddressesMap.keys();
}

void BluetoothScanner::slotStartScan()
{
    if(m_pLocalDevice){
        delete m_pLocalDevice;
        m_pLocalDevice=nullptr;
    }

    m_pLocalDevice=new QBluetoothLocalDevice(this);
    if(!m_pLocalDevice->isValid()){
        emit signalScanException(QObject::tr("Bluetooth adapter not found"));
        delete m_pLocalDevice;
        m_pLocalDevice=nullptr;
        return;
    }

    m_pLocalDevice->powerOn();
    m_pLocalDevice->setHostMode(QBluetoothLocalDevice::HostDiscoverable);

    m_pDiscoveryAgent=new QBluetoothDeviceDiscoveryAgent(this);

    QObject::connect(m_pDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
                     this, &BluetoothScanner::slotDeviceDiscovered);
    QObject::connect(m_pDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished,
                     this, &BluetoothScanner::slotFinished);
    QObject::connect(m_pDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::canceled,
                     this, &BluetoothScanner::slotCanceled);
    QObject::connect(m_pDiscoveryAgent, QOverload<QBluetoothDeviceDiscoveryAgent::Error>::of(&QBluetoothDeviceDiscoveryAgent::error),
                     this, &BluetoothScanner::slotError);

    m_pDiscoveryAgent->start();
    emit signalScanStart();
}

void BluetoothScanner::slotStartConnect(const QString& bluetoothName)
{
    if(!m_bluetoothAddressesMap.contains(bluetoothName)){
        emit signalScanException(QObject::tr("Wrong bluetooth device name"));
        return;
    }

    IODevice ioDevice;
    setBluetoothAddress(m_bluetoothAddressesMap.value(bluetoothName));

    ioDevice.init(m_bluetoothAddressesMap.value(bluetoothName));

    emit signalConnectStart();
    bool connected=false;
    connected=ioDevice.connect();
    QThread::msleep(2000);
    ioDevice.disconnect();
    emit signalConnectStop(connected);
}

