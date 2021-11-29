#ifndef EVO1FAULTPARSER_H
#define EVO1FAULTPARSER_H
#include "FaultParser.h"


class Evo1FaultParser : public FaultParser
{
public:
    Evo1FaultParser();
    virtual void parseLastFault(const char lastFaultData);
    virtual void parseFaultsList(const QByteArray& faultsData);

    virtual void parseLastFaultFromEepData();
    virtual void parseFailtsListFromEepData();
};

#endif // EVO1FAULTPARSER_H
