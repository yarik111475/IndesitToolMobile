#ifndef EEPREADER_H
#define EEPREADER_H

#include <QObject>

#include "IODevice.h"
#include "Functions.h"
#include "ApplianceModel.h"
#include "ApplianceParser.h"

class EepReader : public QObject
{
    Q_OBJECT
protected:
    QString m_bluetoothAddress;
    IODevice m_IODevice;
    ApplianceModel m_ApplianceModel;
    ApplianceParser* m_pApplianceParser;
    quint8 m_lastFaultData;
    QByteArray m_applianceData;
    QByteArray m_lookUpTableData;
    QByteArray m_lookUpTableDataSizesData;
    QByteArray m_faultsBlock;
    QByteArray m_eepData;

public:
    explicit EepReader(QObject *parent = nullptr);
    void init(QString bluetoothAddress, ApplianceParser* pApplianceParser);
    virtual ~EepReader();

signals:
    void signalReadApplianceOK(const ApplianceModel applianceModel);
    void signalReadEepStart(quint16 size);
    void signalReadEepStep(quint16 step);
    void signalReadEepStop(const QByteArray& eepData);
    void signalReadLastFaultOK(quint16 deviceTypeCode, quint16 eepAreaSize, const quint8 lastFault);
    void signalReadFaultsBlockOK(quint16 deviceTypeCode, quint16 eepAreaSize, const QByteArray& faultsBlock);
    void signalException(const QString& message);
    void signalIODeviceDisconnected();

public slots:
    void slotReadEep();

protected:
    virtual void readAppliance()=0;
    virtual void readEep()=0;
    virtual void readLastFault()=0;
    virtual void readFaultsBlock()=0;

};

#endif // EEPREADER_H
