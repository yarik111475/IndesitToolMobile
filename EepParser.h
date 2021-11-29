#ifndef EEPPARSER_H
#define EEPPARSER_H
#include <QtCore>

#include "EepModel.h"


class EepParser
{
public:
    EepParser();
    EepModel getEepModel() const;
    virtual ~EepParser(){}
    virtual void parseEep(const QByteArray& eepData);
protected:
    EepModel m_eepModel;
    QString m_eepMarker;
    quint16 m_eepInfoStartAddress;
    QMap<QString, quint8> m_eepMarkersMap;

    quint16 getCrcChecksum(const QByteArray& eepData, quint16 checksumAddress);
    quint16 getSimpleChecksum(const QByteArray& eepData, quint16 checksumAddress);

    virtual void checkEepData(const QByteArray& eepData)=0;
    virtual void checkEepChecksum(const QByteArray& eepData)=0;
    virtual void setEepModel(const QByteArray& eepData)=0;
    virtual void setEepCode(const QByteArray& eepData)=0;
    virtual void setEepSerial(const QByteArray& eepData)=0;
};

#endif // EEPPARSER_H
