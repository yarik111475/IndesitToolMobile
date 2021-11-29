#ifndef EVO1EEPPARSER_H
#define EVO1EEPPARSER_H
#include "EepParser.h"


class Evo1EepParser : public EepParser
{
public:
    Evo1EepParser();
    virtual void checkEepData(const QByteArray& eepData);
    virtual void checkEepChecksum(const QByteArray& eepData);
    virtual void setEepModel(const QByteArray& eepData);
    virtual void setEepCode(const QByteArray& eepData);
    virtual void setEepSerial(const QByteArray& eepData);
};

#endif // EVO1EEPPARSER_H
