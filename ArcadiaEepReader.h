#ifndef ARCADIAEEPREADER_H
#define ARCADIAEEPREADER_H
#include "EepReader.h"


class ArcadiaEepReader : public EepReader
{
public:
    ArcadiaEepReader(QObject* parent=nullptr);
    virtual ~ArcadiaEepReader(){};
protected:
    virtual void readAppliance();
    virtual void readEep();
    virtual void readLastFault();
    virtual void readFaultsBlock();

private:
    quint16 m_lookUpTableSize;
    void readLookUpTableData();
    void readLookUpTableDataSizesData();
};

#endif // ARCADIAEEPREADER_H
