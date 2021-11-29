#ifndef EEPWRITER_H
#define EEPWRITER_H
#include <QtCore>
#include <QObject>

#include "IODevice.h"
#include "Functions.h"
#include "ApplianceModel.h"
#include "ApplianceParser.h"

class EepWriter : public QObject
{
    Q_OBJECT
protected:
    QString m_bluetoothAddress;
    IODevice m_IODevice;
    ApplianceModel m_ApplianceModel;
    ApplianceParser* m_pApplianceParser;

    QByteArray m_applianceData;
    QByteArray m_lookUpTableData;
    QByteArray m_lookUpTableDataSizesData;
    QByteArray m_eepData;
public:
    explicit EepWriter(QObject *parent = nullptr);
    virtual ~EepWriter();
    void init(const QString& bluetoothAddress, ApplianceParser* pApplianceParser, const QByteArray eepData);

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

protected:
    virtual void readAppliance()=0;
    virtual void checkEepSize()=0;
    virtual void writeEep()=0;
};

#endif // EEPWRITER_H
