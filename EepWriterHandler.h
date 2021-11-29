#ifndef EEPWRITERHANDLER_H
#define EEPWRITERHANDLER_H
#include <QtCore>
#include <QObject>

#include "FileHandler.h"
#include "IODevice.h"
#include "Functions.h"
#include "Factories.h"
#include "ApplianceModel.h"
#include "ApplianceParser.h"

class EepWriterHandler : public QObject
{
    Q_OBJECT
private:
    EepWriter* m_pEepWriter;
    FileHandler* m_pFileHandler;
    ApplianceModel m_applianceModel;
    QString m_applianceName;

public:
    explicit EepWriterHandler(FileHandler* pFileHandler, QObject *parent = nullptr);
    Q_INVOKABLE
    void init(const QString &applianceName, const QString& bluetoothAddress);

signals:
    void signalReadApplianceOK(const ApplianceModel applianceModel);
    void signalEraseEep();
    void signalWriteEepStart(quint16 size);
    void signalWriteEepStep(quint16 step);
    void signalWriteEepStop();
    void signalException(const QString& message);
    void signalIODeviceDisconnected();

public slots:
    void slotWriteEep();

private slots:
    void slotWriteEepStop();
    void slotReadApplianceOK(const ApplianceModel &applianceModel);
    void slotException(const QString& message);
    void slotIODeviceDisconnected();

};

#endif // EEPWRITERHANDLER_H
