#ifndef EVO2FAULTPARSER_H
#define EVO2FAULTPARSER_H
#include "FaultParser.h"


class Evo2FaultParser : public FaultParser
{
public:
    Evo2FaultParser();
    virtual void parseLastFault(const char lastFaultData);
    virtual void parseFaultsList(const QByteArray& faultsData);

    virtual void parseLastFaultFromEepData();
    virtual void parseFailtsListFromEepData();
};

#endif // EVO2FAULTPARSER_H
