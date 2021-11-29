#ifndef IODEVICE_H
#define IODEVICE_H

#include <QObject>
#include <QtBluetooth>

class IODevice : public QObject
{
    Q_OBJECT
public:
    enum IOResult{
        RESPONSE_OK,
        RESPONSE_FAIL
    };

private:
    QBluetoothLocalDevice* m_pLocalDevice;

    QBluetoothSocket* m_pSocket;
    QString m_bluetoothAddress;
    QBluetoothServiceInfo m_bluetoothServiceInfo;

    QEventLoop m_connectLoop;
    QByteArray m_outBuffer;
    int m_responseSize;

public:
    explicit IODevice(QObject *parent = nullptr);
    ~IODevice();
    void init(const QString& bluetoothAddress);
    virtual IODevice::IOResult sendRequest(const QByteArray* pRequest, int responseSize, int loopDelay=200, bool writeByteToByte=false);
    QByteArray getResponse() const;
    bool connect();
    void disconnect();

private slots:
    void slotSocketConnected();
    void slotSocketDisconnected();
    void slotSocketReadyRead();
    void slotSocketError(QBluetoothSocket::SocketError error);

signals:
    void signalConnected();
    void signalDisconnected();
    void signalReadCompleted();
};


#endif // IODEVICE_H
