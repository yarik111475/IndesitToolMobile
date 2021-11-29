#ifndef ARCADIAFAULTPARSER_H
#define ARCADIAFAULTPARSER_H
#include "FaultParser.h"


class ArcadiaFaultParser : public FaultParser
{
public:
    ArcadiaFaultParser();
    virtual void parseLastFault(const char lastFaultData);
    virtual void parseFaultsList(const QByteArray& faultsData);

    virtual void parseLastFaultFromEepData(){}
    virtual void parseFailtsListFromEepData(){}
};

#endif // ARCADIAFAULTPARSER_H
