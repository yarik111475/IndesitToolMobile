#ifndef EVO1APPLIANCEPARSER_H
#define EVO1APPLIANCEPARSER_H
#include "ApplianceParser.h"


class Evo1ApplianceParser : public ApplianceParser
{
public:
    Evo1ApplianceParser();
    virtual ApplianceModel createApplianceModel();
};

#endif // EVO1APPLIANCEPARSER_H
