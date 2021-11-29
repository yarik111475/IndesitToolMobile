#ifndef FACTORIES_H
#define FACTORIES_H
#include "Functions.h"

#include "EepReader.h"
#include "EepWriter.h"
#include "EepParser.h"
#include "FaultParser.h"
#include "ApplianceParser.h"

#include "Evo1EepParser.h"
#include "Evo1EepReader.h"
#include "Evo1EepWriter.h"
#include "Evo1FaultParser.h"
#include "Evo1ApplianceParser.h"

#include "Evo2FaultParser.h"
#include "Evo2ApplianceParser.h"
#include "Evo2EepReader.h"
#include "Evo2EepParser.h"
#include "Evo2EepWriter.h"

#include "ArcadiaEepReader.h"
#include "ArcadiaFaultParser.h"
#include "ArcadiaApplianceParser.h"
#include "ArcadiaEepWriter.h"
#include "ArcadiaEepParser.h"


class Factories
{
public:
    Factories();
    static FaultParser* createFaultParser(const QString& deviceType);
    static EepParser* createEepParser(const QString& deviceType);
    static EepReader* createEepReader(const QString& deviceType);
    static EepWriter* createEepWriter(const QString& deviceType);
    static ApplianceParser* createApplianceParser(const QString& deviceType);
};

#endif // FACTORIES_H
