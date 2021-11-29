#ifndef REGISTRATIONMANAGER_H
#define REGISTRATIONMANAGER_H

#include <random>
#include <QtCore>
#include <QFile>
#include <QObject>
#include <QSettings>
#include <QtAlgorithms>
#include <QApplication>
#include "SimpleCrypt.h"

class RegistrationManager : public QObject
{
    Q_OBJECT
private:
    const quint16 RAND_SIZE=0xFE;
    const quint16 MIN_RANGE=0x00;
    const quint16 MAX_RANGE=0xFF;
    const quint16 BUFFER_LENGTH_SIZE=2;

    const quint64 ENCRYPT_KEY=0x0F2AD4A4ACB9F023;
    const quint64 DECRYPT_KEY=0x0D563F0CEA826901;

    QString m_keyBluetoothAddress;

    SimpleCrypt m_cryptor;

    const QString m_requestFileName="stmkey.req";
    const QString m_responseFileName="stmkey.resp";

    std::random_device m_rd;
    QByteArray getRandomArray();
    void createRequestData();
public:
    explicit RegistrationManager(const QString& keyBluetoothAddress,
                                 QObject *parent = nullptr);

    bool getRegistrationResult();
};

#endif // REGISTRATIONMANAGER_H
