#ifndef BLUETOOTHSCANNER_H
#define BLUETOOTHSCANNER_H

#include <QtCore>
#include <QObject>
#include <QtBluetooth>

#include "IODevice.h"
#include "SettingsHandler.h"

class BluetoothScanner : public QObject
{
    Q_OBJECT
private:
    QString m_bluetoothAddress;
    QMap<QString, QString> m_bluetoothAddressesMap;
    QBluetoothLocalDevice* m_pLocalDevice;
    QBluetoothDeviceDiscoveryAgent* m_pDiscoveryAgent;
    SettingsHandler* m_pSettingsHandler;

private slots:   
    void slotFinished();
    void slotCanceled();
    void slotDeviceDiscovered(const QBluetoothDeviceInfo& info);
    void slotError(QBluetoothDeviceDiscoveryAgent::Error error);

public:
    explicit BluetoothScanner(QObject *parent = nullptr);

    Q_PROPERTY(QString bluetoothAddress
               READ getBluetoothAddress
               WRITE setBluetoothAddress
               NOTIFY bluetoothAddressChanged)

    inline QString getBluetoothAddress() const{
        return m_bluetoothAddress;
    }

    inline void setBluetoothAddress(const QString& bluetoothAddress){
        m_bluetoothAddress=bluetoothAddress;
        emit bluetoothAddressChanged(m_bluetoothAddress);
    }

    ~BluetoothScanner();
    Q_INVOKABLE
    QStringList getBluetoothDevicesList()const;
public slots:
    void slotStartScan();
    void slotStartConnect(const QString &bluetoothName);

signals:
    void signalScanStart();
    void signalScanStop();
    void signalScanException(const QString& message);

    void signalConnectStart();
    void signalConnectStop(bool connected);

    void bluetoothAddressChanged(const QString& bluetoothAddress);
};

#endif // BLUETOOTHSCANNER_H
