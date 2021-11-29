#ifndef APPLIANCEPARSER_H
#define APPLIANCEPARSER_H
#include <QtCore>

#include "ApplianceModel.h"


class ApplianceParser
{
protected:
    QByteArray m_applianceData;
    ApplianceModel m_applianceModel;
    QByteArray m_lookUpTableData;
    QByteArray m_lookUpTableDataSizesData;
public:
    ApplianceParser();
    void initApplianceParser(const QByteArray& applianceData, const QByteArray& lookUpTableData,
                             const QByteArray& lookUpTableDataSizesData);
    virtual ApplianceModel createApplianceModel()=0;
    virtual~ApplianceParser(){}
};

#endif // APPLIANCEPARSER_H
