#include "Functions.h"

Functions::Functions()
{

}

quint8 Functions::getRequestChecksum(const QByteArray &request)
{
    quint32 rawChecksum=0;
    for(int i=0;i<request.size();i++){
        rawChecksum+=(quint8)(request.at(i));
    }
    return static_cast<quint8>(rawChecksum);
}

Functions::DeviceType Functions::getDeviceType(const QString deviceType)
{
    QMap<QString, DeviceType> typesMap;
    typesMap.insert("ARCADIA", DeviceType::ARCADIA);
    typesMap.insert("EVO_1", DeviceType::EVO_1);
    typesMap.insert("EVO_2", DeviceType::EVO_2);
    if(!typesMap.contains(deviceType)){
        throw QObject::tr("Unknown device type");
    }
    return typesMap.value(deviceType);
}

QByteArray Functions::eepToBinToEep(const QByteArray &eepData)
{
    if(eepData.size()==0){
        throw QObject::tr("Source data must not be empty");
    }
    QByteArray inSource(eepData);
    if((inSource.size()%2)!=0){
        inSource.push_back((char)0xFF);
    }
    QByteArray outSource;
    for(int i=0;i<inSource.size();i+=2){
        quint16 word=(quint8)inSource.at(i) * 0x100 + (quint8)inSource.at(i+1);
        outSource.push_back((quint8)word);
        outSource.push_back((quint16)word >> 8);
    }
    return outSource;
}

QStringList Functions::createSRecordFirmware(const QByteArray &firmwareData, const QByteArray &eepData)
{ 
    const quint32 START_ADDRESS=0x1080;
    const quint32 MIDDLE_ADDRESS=0x17F0;
    const quint32 START_ADDRESS_INCREMENT=0x3C;
    const quint16 LINE_SIZE=0x10;
    const quint16 LINE_SIZE_INCREMENT=0x03;

    QStringList outStringList;
    QByteArray outBuffer=createBinaryFirmware(firmwareData, eepData);

    for(int i=START_ADDRESS; i<outBuffer.size(); i+=LINE_SIZE){
        if(i==MIDDLE_ADDRESS){
            i+=START_ADDRESS_INCREMENT;
        }
        quint32 lineLength=(outBuffer.size() - i) >= LINE_SIZE ? LINE_SIZE : outBuffer.count() - i;

        QByteArray lineBuffer=outBuffer.mid(i,lineLength);
        QString lineString="S1";
        lineString.push_back(QString::number(lineLength + LINE_SIZE_INCREMENT,16).rightJustified(2,'0').toUpper());

        lineString.push_back(QString::number(i,16).rightJustified(4, '0').toUpper());

        for(int j=0;j<lineBuffer.size(); j++){
            lineString.push_back(QString::number((quint8)lineBuffer.at(j),16).rightJustified(2, '0').toUpper());
        }

        quint16 checksum=0;
        for(int n=0; n < lineBuffer.size(); n++){
            checksum+=lineBuffer.at(n);
        }
        checksum=checksum + lineLength + LINE_SIZE_INCREMENT;
        checksum=checksum + (quint8)(i >> 8);
        checksum=checksum + (quint8)i;
        checksum=(quint8)(0xFF - (quint8)checksum);

        lineString.push_back(QString::number(checksum, 16).rightJustified(2, '0').toUpper());
        outStringList.push_back(lineString);
    }
    return outStringList;
}

QByteArray Functions::createBinaryFirmware(const QByteArray &firmwareData, const QByteArray &eepData)
{
    const quint32 EEP_START_ADDRESS=0x1A00;
    int eepSize=eepData.size();
    int firmwareSize=firmwareData.size();

    QByteArray outBuffer;
    outBuffer.append(firmwareData.mid(0,EEP_START_ADDRESS));
    outBuffer.append(eepData);
    outBuffer.append(firmwareData.mid(EEP_START_ADDRESS+eepSize,firmwareSize-(EEP_START_ADDRESS+eepSize)));
    return outBuffer;
}

void Functions::changeEepModelSerial(EepModel *pEepModel, quint64 serial, quint16 correctionValue)
{
    QString strSerial=QString::number(serial).rightJustified(9,'0').leftJustified(12,'0');
    QByteArray serialBuffer;
    for(int i=0;i<strSerial.size();i+=2){
        bool ok=false;
        quint8 serialItem=strSerial.mid(i,2).toInt(&ok,16);
        if(ok){
            serialBuffer.push_back(serialItem);
        }
    }

    const quint16 SERIAL_CORRECTION_INDEX=6;

    quint16 eepSerialStartAddress=(quint8)pEepModel->Data.at(0) * 0x100 +
            (quint8)pEepModel->Data.at(1);
    eepSerialStartAddress=eepSerialStartAddress * 2 + correctionValue + SERIAL_CORRECTION_INDEX;

    QByteArray newEepData=pEepModel->Data.mid(0,eepSerialStartAddress);
    newEepData.append(serialBuffer);
    newEepData.append(pEepModel->Data.mid(eepSerialStartAddress+SERIAL_CORRECTION_INDEX,
                                        pEepModel->Data.size()-(eepSerialStartAddress+SERIAL_CORRECTION_INDEX)));
    pEepModel->Serial=strSerial.mid(0,9);
    pEepModel->Data.clear();
    pEepModel->Data.append(newEepData);
}
#ifdef Q_OS_ANDROID
QString Functions::getDeviceImei()
{
    QAndroidJniEnvironment environment;
    jclass contextClass=environment->FindClass("android/content/Context");
    jfieldID fieldID=environment->GetStaticFieldID(contextClass,"TELEPHONY_SERVICE","Ljava/lang/String");
    jstring telephonyManagerType=(jstring)environment->GetStaticObjectField(contextClass, fieldID);

    jclass telephonyManagerClass=environment->FindClass("android/telephony/TelefonyManager");
    jmethodID methodID=environment->GetMethodID(contextClass,
                                                "GetSystemService",
                                                "(Ljava/lang/string;)Ljava/lang/Object");
    QAndroidJniObject qtActivityObj=QAndroidJniObject::callStaticObjectMethod("org/qtproject/qt5/android/QtNative",
                                                                              "activity",
                                                                              "()Landroid/app/Activity");
    jobject telephonyManager=environment->CallObjectMethod(qtActivityObj.object<jobject>(),
                                                           methodID, telephonyManagerType);
    methodID=environment->GetMethodID(telephonyManagerClass, "getDeviceId",
                                      "()Ljava/lang/String");
    jstring jstr=(jstring)environment->CallObjectMethod(telephonyManager,
                                                        methodID);

    jsize length=environment->GetStringUTFLength(jstr);
    char* buffer_deviceID=new char[32];
    environment->GetStringUTFRegion(jstr, 0, length, buffer_deviceID);
    QString imei(buffer_deviceID);
    delete []buffer_deviceID;
    return imei;

}
#endif

bool Functions::requestAndroidPermissions(const QString &permission)
{
#ifdef Q_OS_ANDROID
   /*Permissions list
    * "android.permission.BLUETOOTH_ADMIN",
    * "android.permission.BLUETOOTH_PRIVILEGED",
    * "android.permission.WRITE_EXTERNAL_STORAGE",
    * "android.permission.READ_EXTERNAL_STORAGE"});
    */

    auto result = QtAndroid::checkPermission(permission);
    if(result == QtAndroid::PermissionResult::Denied){
        auto resultHash = QtAndroid::requestPermissionsSync(QStringList({permission}));
        if(resultHash[permission] == QtAndroid::PermissionResult::Denied)
            return false;
    }
#endif
    return true;
}
