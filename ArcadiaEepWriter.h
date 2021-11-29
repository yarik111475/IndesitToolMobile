#ifndef ARCADIAEEPWRITER_H
#define ARCADIAEEPWRITER_H
#include "EepWriter.h"


class ArcadiaEepWriter : public EepWriter
{
private:
    const int LOOP_DELAY=150;
public:
    ArcadiaEepWriter(QObject* parent=nullptr);

protected:
    virtual void readAppliance();
    virtual void checkEepSize();
    virtual void writeEep();

private:
    void eraseEep(quint8 mcuTypeCode);
    void eraseHCS08Series();
    void eraseH8Series();
    void eraseRXSeries();
    void eraseMCFSeries();

    quint16 m_lookUpTableSize;
    void readLookUpTableData();
    void readLookUpTableDataSizesData();
};

#endif // ARCADIAEEPWRITER_H
