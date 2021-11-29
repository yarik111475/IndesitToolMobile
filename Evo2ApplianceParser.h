#ifndef EVO2APPLIANCEPARSER_H
#define EVO2APPLIANCEPARSER_H
#include "ApplianceParser.h"


class Evo2ApplianceParser : public ApplianceParser
{
public:
    Evo2ApplianceParser();
    virtual ApplianceModel createApplianceModel();
};

#endif // EVO2APPLIANCEPARSER_H
