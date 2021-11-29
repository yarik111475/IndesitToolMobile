#ifndef ARCADIAAPPLIANCEPARSER_H
#define ARCADIAAPPLIANCEPARSER_H
#include "ApplianceParser.h"


class ArcadiaApplianceParser : public ApplianceParser
{
private:
    QMap<quint8, quint16> m_eepSizesMap;
    QMap<char, QString> m_mcuTypesMap;
    QMap<quint16, QString> m_deviceTypesMap;


public:
    ArcadiaApplianceParser();
    virtual ApplianceModel createApplianceModel();
};

#endif // ARCADIAAPPLIANCEPARSER_H
