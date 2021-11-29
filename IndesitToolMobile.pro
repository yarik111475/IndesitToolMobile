QT += quick bluetooth sql widgets core gui
android: QT += androidextras
#CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

TRANSLATIONS +=ServiceToolMobile_ru.ts\
               ServiceToolMobile_en.ts

SOURCES += main.cpp \
    ApplianceModel.cpp \
    ApplianceParser.cpp \
    ApplicationHandler.cpp \
    ArcadiaApplianceParser.cpp \
    ArcadiaEepParser.cpp \
    ArcadiaEepReader.cpp \
    ArcadiaEepWriter.cpp \
    ArcadiaFaultParser.cpp \
    BluetoothScanner.cpp \
    DatabaseProxyModel.cpp \
    DeviceModel.cpp \
    DeviceParser.cpp \
    EepModel.cpp \
    EepParser.cpp \
    EepProxyModel.cpp \
    EepReader.cpp \
    EepReaderHandler.cpp \
    EepWriter.cpp \
    EepWriterHandler.cpp \
    Evo1ApplianceParser.cpp \
    Evo1EepParser.cpp \
    Evo1EepReader.cpp \
    Evo1EepWriter.cpp \
    Evo1FaultParser.cpp \
    Evo2ApplianceParser.cpp \
    Evo2EepParser.cpp \
    Evo2EepReader.cpp \
    Evo2EepWriter.cpp \
    Evo2FaultParser.cpp \
    Factories.cpp \
    FaultModel.cpp \
    FaultParser.cpp \
    FileHandler.cpp \
    Functions.cpp \
    IODevice.cpp \
    RegistrationManager.cpp \
    SQLiteManager.cpp \
    SettingsHandler.cpp \
    SimpleCrypt.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    ApplianceModel.h \
    ApplianceParser.h \
    ApplicationHandler.h \
    ArcadiaApplianceParser.h \
    ArcadiaEepParser.h \
    ArcadiaEepReader.h \
    ArcadiaEepWriter.h \
    ArcadiaFaultParser.h \
    BluetoothScanner.h \
    DatabaseProxyModel.h \
    DeviceModel.h \
    DeviceParser.h \
    EepModel.h \
    EepParser.h \
    EepProxyModel.h \
    EepReader.h \
    EepReaderHandler.h \
    EepWriter.h \
    EepWriterHandler.h \
    Evo1ApplianceParser.h \
    Evo1EepParser.h \
    Evo1EepReader.h \
    Evo1EepWriter.h \
    Evo1FaultParser.h \
    Evo2ApplianceParser.h \
    Evo2EepParser.h \
    Evo2EepReader.h \
    Evo2EepWriter.h \
    Evo2FaultParser.h \
    Factories.h \
    FaultModel.h \
    FaultParser.h \
    FileHandler.h \
    Functions.h \
    IODevice.h \
    RegistrationManager.h \
    SQLiteManager.h \
    SettingsHandler.h \
    SimpleCrypt.h

DISTFILES += \
    android/AndroidManifest.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew \
    android/gradlew.bat \
    android/res/values/libs.xml

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
