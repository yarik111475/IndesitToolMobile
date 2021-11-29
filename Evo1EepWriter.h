#ifndef EVO1EEPWRITER_H
#define EVO1EEPWRITER_H
#include "EepWriter.h"


class Evo1EepWriter : public EepWriter
{
public:
    Evo1EepWriter();
    virtual void readAppliance();
    virtual void checkEepSize();
    virtual void writeEep();

    void setWriteProtect(bool state);
};

#endif // EVO1EEPWRITER_H
