#ifndef EEOREADERHANDLER_H
#define EEOREADERHANDLER_H

#include <QObject>
#include "EepReader.h"
#include "IODevice.h"
#include "Functions.h"
#include "Factories.h"
#include "ApplianceModel.h"
#include "ApplianceParser.h"

#include "EepModel.h"
#include "EepParser.h"
#include "EepProxyModel.h"

#include "FaultParser.h"
#include "FaultModel.h"

#include "FileHandler.h"


class EepReaderHandler : public QObject
{
    Q_OBJECT
private:
    EepReader* m_pEepReader;
    ApplianceModel m_applianceModel;
    FileHandler* m_pFileHandler;
    QString m_applianceName;
    bool m_writeCheck;

public:
    explicit EepReaderHandler(FileHandler* pFileHandler, QObject *parent = nullptr);
    Q_INVOKABLE
    void init(const QString& applianceName, const QString& bluetoothAddress, bool writeCheck=false);

signals:
    void signalReadEepStart(quint16 size);
    void signalReadEepStep(quint16 step);
    void signalReadEepStop(const EepProxyModel& model);

    void signalReadLastFaultOK(const FaultModel& faultModel);
    void signalReadFaultsBlockOK(const QList<FaultModel> faultsList);
;
    void signalException(const QString& message);
    void signalIODeviceDisconnected();

public slots:
    void slotReadEep(bool writeCheck=false);

private slots:
    void slotReadEepStop(const QByteArray& eepData);
    void slotReadLastFaultOK(quint16 deviceTypeCode, quint16 eepAreaSize, const quint8 lastFault);
    void slotReadFaultsBlockOK(quint16 deviceTypeCode, quint16 eepAreaSize, const QByteArray& faultsBlock);
    void slotReadApplianceOK(const ApplianceModel& applianceModel);
    void slotException(const QString& message);
    void slotIODeviceDisconnected();

};

#endif // EEOREADERHANDLER_H
