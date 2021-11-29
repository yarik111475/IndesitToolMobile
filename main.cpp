#include <QQmlContext>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QTranslator>

#include "SQLiteManager.h"
#include "FileHandler.h"
#include "SettingsHandler.h"
#include "BluetoothScanner.h"
#include "EepReaderHandler.h"
#include "EepWriterHandler.h"

#ifdef Q_OS_ANDROID
#include <QtAndroid>

//"android.permission.WRITE_SETTINGS"
//"android.permission.ACCESS_FINE_LOCATION"
//"android.permission.ACCESS_BACKGROUND_LOCATION"

const QVector<QString> permissions({"android.permission.BLUETOOTH",
                                    "android.permission.BLUETOOTH_ADMIN",
                                    "android.permission.WRITE_EXTERNAL_STORAGE",
                                    "android.permission.READ_EXTERNAL_STORAGE",
                                    "android.permission.ACCESS_COARSE_LOCATION"});
#endif


int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);
    QGuiApplication::setApplicationName("Service Tool Mobile");
    QGuiApplication::setApplicationVersion("1.2.0.0");
    QGuiApplication::setOrganizationName("Home Appliances Tools");

    qmlRegisterType<SettingsHandler>("com.ServiceToolMobile",1,0,"SettingsHandler");
    qmlRegisterType<BluetoothScanner>("com.ServiceToolMobile", 1, 0, "BluetoothScanner");
    qmlRegisterType<SQLiteManager>("com.ServiceToolMobile", 1, 0, "SQLiteManager");



#if defined (Q_OS_ANDROID)
    //Request requiered permissions at runtime
    for(const QString &permission : permissions){
        auto result = QtAndroid::checkPermission(permission);
        if(result == QtAndroid::PermissionResult::Denied){
            auto resultHash = QtAndroid::requestPermissionsSync(QStringList({permission}));
            if(resultHash[permission] == QtAndroid::PermissionResult::Denied)
                return 0;
        }
    }
#endif

    FileHandler* pFileHandler=new FileHandler;
    EepReaderHandler eepReaderHandler(pFileHandler);
    EepWriterHandler eepWriterHandler(pFileHandler);

    QQmlApplicationEngine engine;
    QQmlContext* ctx=engine.rootContext();

    ctx->setContextProperty("fileHandler", pFileHandler);
    ctx->setContextProperty("eepReaderHandler", &eepReaderHandler);
    ctx->setContextProperty("eepWriterHandler", &eepWriterHandler);


    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty()){
        return -1;
    }

    QTranslator translator;
    bool result = translator.load(":/lang/ServiceToolMobile_ru.qm");
    qDebug()<<result;
    qApp->installTranslator(&translator);

    engine.retranslate();

    return app.exec();
}
