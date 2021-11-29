#include "IODevice.h"

IODevice::IODevice(QObject *parent) : QObject(parent)
{
    m_pSocket=nullptr;
    m_responseSize=0;
}

IODevice::~IODevice()
{
    if(m_pLocalDevice){
        delete m_pLocalDevice;
        m_pLocalDevice=nullptr;
    }
    if(m_pSocket){
        delete m_pSocket;
        m_pSocket=nullptr;
    }
}

void IODevice::init(const QString &bluetoothAddress)
{
    m_bluetoothAddress=bluetoothAddress;
}

IODevice::IOResult IODevice::sendRequest(const QByteArray *pRequest, int responseSize, int loopDelay, bool writeByteToByte)
{
    const int DELAY_SIZE=15;
    const int MAX_TRY_COUNT=8;

    for(int i=0; i<MAX_TRY_COUNT; i++){
        m_outBuffer.clear();
        m_responseSize=responseSize;

        //write to port byte-to-byte(EVO_1)
        if(writeByteToByte){
            for(int i=0; i<pRequest->size(); i++){
                QByteArray temp;
                temp.push_back(pRequest->at(i));
                m_pSocket->write(temp.data(), 1);
                m_pSocket->waitForBytesWritten(10);
                QThread::msleep(DELAY_SIZE);
            }
        }
        //write to port all data(for EVO_2 & ARCADIA)
        else{
            m_pSocket->write(pRequest->data(), pRequest->size());
        }

        QEventLoop readLoop;

        //timer for wait async operation completed...
        QTimer::singleShot(loopDelay, &readLoop, &QEventLoop::quit);

        //wait for async read operation completed...
        readLoop.exec();

        if(m_outBuffer.size()==m_responseSize){
            return IODevice::RESPONSE_OK;
        }
    }
    return IODevice::RESPONSE_FAIL;
}

QByteArray IODevice::getResponse() const
{
    return m_outBuffer;
}

bool IODevice::connect()
{
    m_pLocalDevice=new QBluetoothLocalDevice(this);
    if(m_pLocalDevice->isValid()){
        m_pLocalDevice->powerOn();
        m_pLocalDevice->setHostMode(QBluetoothLocalDevice::HostDiscoverable);

        m_pSocket=new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol, this);

        QObject::connect(m_pSocket, &QBluetoothSocket::connected,
                         this, &IODevice::slotSocketConnected);
        QObject::connect(m_pSocket, &QBluetoothSocket::disconnected,
                         this, &IODevice::slotSocketDisconnected);
        QObject::connect(m_pSocket, &QBluetoothSocket::readyRead,
                         this, &IODevice::slotSocketReadyRead);
        QObject::connect(m_pSocket, QOverload<QBluetoothSocket::SocketError>::of(&QBluetoothSocket::error),
                         this, &IODevice::slotSocketError);

        m_pSocket->setPreferredSecurityFlags(QBluetooth::NoSecurity);
        m_pSocket->connectToService(QBluetoothAddress(m_bluetoothAddress),
                                    QBluetoothUuid(QBluetoothUuid::SerialPort),
                                    QIODevice::ReadWrite);

        //timer for wait async operation completed...;
        QTimer::singleShot(30000, &m_connectLoop, &QEventLoop::quit);

        //wait for async read operation completed...
        m_connectLoop.exec();

        if(m_pSocket->state()==QBluetoothSocket::ConnectedState){
            emit signalConnected();
            return true;
        }
    }
    return false;
}

void IODevice::disconnect()
{
    if(m_pSocket){
        if(m_pSocket->state()==QBluetoothSocket::ConnectedState){
            m_pSocket->disconnectFromService();
        }
    }
}

void IODevice::slotSocketConnected()
{
    m_connectLoop.quit();
}

void IODevice::slotSocketDisconnected()
{
    emit signalDisconnected();
}

void IODevice::slotSocketReadyRead()
{
    m_outBuffer.append(m_pSocket->readAll());
}

void IODevice::slotSocketError(QBluetoothSocket::SocketError error)
{
    qDebug()<<"socket error: " <<error;
}

