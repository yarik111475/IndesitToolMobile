#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <QtCore>

#ifdef Q_OS_ANDROID
#include <QtAndroid>
#include <QAndroidJniObject>
#include <QAndroidJniEnvironment>
#endif

#include "EepModel.h"


class Functions
{
public:
    enum DeviceType{
        EVO_1,
        EVO_2,
        ARCADIA
    };
    Functions();
    static quint8 getRequestChecksum(const QByteArray& request);
    static DeviceType getDeviceType(const QString stringType);
    static QByteArray eepToBinToEep(const QByteArray& eepData);
    static QStringList createSRecordFirmware(const QByteArray& firmwareData, const QByteArray& eepData);
    static QByteArray createBinaryFirmware(const QByteArray& firmwareData, const QByteArray& eepData);
    static void changeEepModelSerial(EepModel* pEepModel, quint64 serial, quint16 correctionValue);

#ifdef Q_OS_ANDROID
    static QString getDeviceImei();
#endif

    Q_INVOKABLE
    static bool requestAndroidPermissions(const QString& permission);
};

#endif // FUNCTIONS_H
