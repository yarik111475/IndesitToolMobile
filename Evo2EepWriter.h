#ifndef EVO2EEPWRITER_H
#define EVO2EEPWRITER_H
#include "EepWriter.h"


class Evo2EepWriter : public EepWriter
{
public:
    Evo2EepWriter(QObject* parent=nullptr);

protected:
    virtual void readAppliance();
    virtual void checkEepSize();
    virtual void writeEep();

    void setWriteProtect(bool state);
};

#endif // EVO2EEPWRITER_H
