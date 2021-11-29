#ifndef EVO2EEPREADER_H
#define EVO2EEPREADER_H
#include "EepReader.h"


class Evo2EepReader : public EepReader
{
private:
    const int LOOP_DELAY=250;
    QMap<QString, QByteArray> m_currentFaultRequestMap;
public:
    Evo2EepReader();
    virtual ~Evo2EepReader(){}

protected:
    virtual void readAppliance();
    virtual void readEep();
    virtual void readLastFault();
    virtual void readFaultsBlock();
};

#endif // EVO2EEPREADER_H
