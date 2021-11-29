#ifndef EVO1EEPREADER_H
#define EVO1EEPREADER_H
#include "EepReader.h"


class Evo1EepReader : public EepReader
{
public:
    Evo1EepReader();
    virtual ~Evo1EepReader(){}

    virtual void readAppliance();
    virtual void readEep();
    virtual void readLastFault();
    virtual void readFaultsBlock();
};

#endif // EVO1EEPREADER_H
