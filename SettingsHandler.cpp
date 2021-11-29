
#include "Functions.h"
#include "SettingsHandler.h"
#include "RegistrationManager.h"

#ifdef Q_OS_ANDROID
#include <QtAndroid>
#endif

SettingsHandler::SettingsHandler(QObject *parent) : QObject(parent)
{
    setIsActivated(false);
    setBluetoothConnected(false);
    setApplianceName("ARCADIA");
}

SettingsHandler::~SettingsHandler()
{
}

QString SettingsHandler::getApplicationName() const
{
    return QGuiApplication::applicationName();
}

QString SettingsHandler::getApplicationVersion() const
{
    return QGuiApplication::applicationVersion();
}

void SettingsHandler::requestActivation(const QString& keyBluetoothAddress)
{
    RegistrationManager regManager(keyBluetoothAddress);
    setIsActivated(regManager.getRegistrationResult());
    qDebug()<<"activation requested";
}
