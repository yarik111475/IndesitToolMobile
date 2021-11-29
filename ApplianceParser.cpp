#include "ApplianceParser.h"

ApplianceParser::ApplianceParser()
{

}

void ApplianceParser::initApplianceParser(const QByteArray &applianceData, const QByteArray &lookUpTableData,
                                          const QByteArray &lookUpTableDataSizesData)
{
    m_applianceData.append(applianceData);
    m_lookUpTableData.append(lookUpTableData);
    m_lookUpTableDataSizesData.append(lookUpTableDataSizesData);
}
