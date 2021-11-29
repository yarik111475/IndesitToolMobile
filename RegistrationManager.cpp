#include "RegistrationManager.h"
#include "SimpleCrypt.h"


QByteArray RegistrationManager::getRandomArray()
{
    QByteArray randBuffer;
    std::uniform_int_distribution<> uid(MIN_RANGE, MAX_RANGE);
    for(int i=0;i<RAND_SIZE;i++){
        randBuffer.push_back(uid(m_rd));
    }
    return randBuffer;
}

void RegistrationManager::createRequestData()
{
    QByteArray baseRandBuffer=getRandomArray();

    QByteArray keyBluetoothAddressBuffer=m_keyBluetoothAddress.toUtf8();
    quint16 keyBluetoothAddressBufferSize=keyBluetoothAddressBuffer.size();

    QByteArray tempBuffer;
    tempBuffer.append((quint8)keyBluetoothAddressBufferSize >> 8);
    tempBuffer.append((quint8)keyBluetoothAddressBufferSize);
    tempBuffer.append(keyBluetoothAddressBuffer);





    quint16 offset=keyBluetoothAddressBufferSize + BUFFER_LENGTH_SIZE;
    tempBuffer.append(baseRandBuffer.mid(offset, baseRandBuffer.size()-offset));

    m_cryptor.setKey(ENCRYPT_KEY);
    QByteArray requestBuffer = m_cryptor.encryptToByteArray(tempBuffer);

    QString path=QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    QString requestFilePath=path + "/" + m_requestFileName;
    QFile file(requestFilePath);
    if(file.open(QIODevice::WriteOnly)){
        file.write(requestBuffer);
        file.close();
    }   
}

RegistrationManager::RegistrationManager(const QString &keyBluetoothAddress,
                                         QObject *parent) : QObject(parent)
{
    m_keyBluetoothAddress=keyBluetoothAddress;
}

bool RegistrationManager::getRegistrationResult()
{
    bool result=false;
    QString path=QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    QString responseFilePath=path + "/" + m_responseFileName;
    if(!QFile::exists(responseFilePath)){
        createRequestData();
        return false;
    }

    QByteArray responseFileBuffer;
    QFile responseFile(responseFilePath);
    if(responseFile.open(QIODevice::ReadOnly)){
        responseFileBuffer=responseFile.readAll();
        responseFile.close();

        m_cryptor.setKey(DECRYPT_KEY);
        QByteArray uncipheredRegFileBuffer=m_cryptor.decryptToByteArray(responseFileBuffer);

        quint16 keyBluetootAddresshBufferSize=(quint8)uncipheredRegFileBuffer.at(0) * 0x100 +
                (quint8)uncipheredRegFileBuffer.at(1);
        QByteArray keyBluetoothAddressBuffer=uncipheredRegFileBuffer.mid(BUFFER_LENGTH_SIZE, keyBluetootAddresshBufferSize);



        QString keyBluetoothAddress(keyBluetoothAddressBuffer);

        result=m_keyBluetoothAddress==keyBluetoothAddress;
    }
    if(!result){
        createRequestData();
    }
    return result;
}



