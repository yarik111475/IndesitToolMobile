#ifndef ARCADIAEEPPARSER_H
#define ARCADIAEEPPARSER_H
#include "EepParser.h"


class ArcadiaEepParser : public EepParser
{
public:
    ArcadiaEepParser();
    virtual void checkEepData(const QByteArray& eepData);
    virtual void checkEepChecksum(const QByteArray& eepData);
    virtual void setEepModel(const QByteArray& eepData);
    virtual void setEepCode(const QByteArray& eepData);
    virtual void setEepSerial(const QByteArray& eepData);
};

#endif // ARCADIAEEPPARSER_H
