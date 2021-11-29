#ifndef FAULTPARSER_H
#define FAULTPARSER_H
#include <QtCore>

#include "FaultModel.h"
#include "SQLiteManager.h"


class FaultParser
{
protected:
    bool isDigits(const QString str);
    FaultModel lastFaultModel;
    QList<FaultModel> faultModelsList;

    quint16 m_deviceTypeCode;
    quint16 m_eepAreaSize;
    QString m_languageCode;
    SQLiteManager sqliteManager;
    QByteArray m_eepData;
public:
    FaultParser();
    virtual ~FaultParser(){}
    FaultModel getLastFaultModel();
    QList<FaultModel> getFaultModelsList();

    virtual void initFaultParser(quint16 deviceTypeCode, quint16 eepSize, const QString& languageCode);
    virtual void parseLastFault(const char lastFaultData)=0;
    virtual void parseFaultsList(const QByteArray& faultsData)=0;

    virtual void initForParseEepData(quint16 deviceTypeCode, quint16 eepSize, const QString& languageCode, const QByteArray& eepData);
    virtual void parseLastFaultFromEepData()=0;
    virtual void parseFailtsListFromEepData()=0;
};

#endif // FAULTPARSER_H
