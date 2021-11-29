#ifndef SETTINGSHANDLER_H
#define SETTINGSHANDLER_H

#include <QObject>
#include <QSettings>
#include <QGuiApplication>
#include <QtBluetooth>

#include "Functions.h"


class SettingsHandler : public QObject
{
    Q_OBJECT
private:
    bool m_isActivated;
    bool m_bluetoothConnected;
    QString m_applianceName;

public:
    inline void setBluetoothConnected(bool connected){
        m_bluetoothConnected=connected;
        emit isBluetoothConnectedChanged(m_bluetoothConnected);
    }
    inline bool getBluetoothConnected() const{
        return m_bluetoothConnected;
    }

    inline void setIsActivated(bool activated){
        m_isActivated=activated;
        emit isActivatedChanged(m_isActivated);
    }
    inline bool getIsActivated() const{
        return m_isActivated;
    }

    inline void setApplianceName(const QString& appliance){
        m_applianceName=appliance;
        emit isApplianceNameChanged(m_applianceName);
    }
    inline QString getApplianceName() const{
        return m_applianceName;
    }


public:
    explicit SettingsHandler(QObject *parent = nullptr);
    ~SettingsHandler();

    Q_PROPERTY(bool bluetoothConnected
               READ getBluetoothConnected
               WRITE setBluetoothConnected
               NOTIFY isBluetoothConnectedChanged)

    Q_PROPERTY(bool isActivated
               READ getIsActivated
               WRITE setIsActivated
               NOTIFY isActivatedChanged)

    Q_PROPERTY(QString applianceName
               READ getApplianceName
               WRITE setApplianceName
               NOTIFY isApplianceNameChanged)

    Q_INVOKABLE
    QString getApplicationName()const;
    Q_INVOKABLE
    QString getApplicationVersion()const;
    Q_INVOKABLE
    void requestActivation(const QString& keyBluetoothAddress);

signals:
    void isBluetoothConnectedChanged(bool connected);
    void isActivatedChanged(bool activated);
    void isLanguageNameChanged(const QString& languageName);
    void isApplianceNameChanged(const QString& applianceName);
};

#endif // SETTINGSHANDLER_H
