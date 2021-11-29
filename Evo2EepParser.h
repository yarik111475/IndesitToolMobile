#ifndef EVO2EEPPARSER_H
#define EVO2EEPPARSER_H
#include "EepParser.h"


class Evo2EepParser : public EepParser
{
public:
    Evo2EepParser();
    virtual void checkEepData(const QByteArray& eepData);
    virtual void checkEepChecksum(const QByteArray& eepData);
    virtual void setEepModel(const QByteArray& eepData);
    virtual void setEepCode(const QByteArray& eepData);
    virtual void setEepSerial(const QByteArray& eepData);
};

#endif // EVO2EEPPARSER_H
