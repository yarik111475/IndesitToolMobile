#ifndef FAULTMODEL_H
#define FAULTMODEL_H
#include <QtCore>


class FaultModel
{
    Q_GADGET
public:
    FaultModel();
public:
    bool IsFaultClear;
    QString FaultCode;
    QString SubCode;
    QString Occurence;
    QString Description;

    Q_PROPERTY(bool IsFaultClear MEMBER IsFaultClear)
    Q_PROPERTY(QString FaultCode MEMBER FaultCode)
    Q_PROPERTY(QString SubCode MEMBER SubCode)
    Q_PROPERTY(QString Occurence MEMBER Occurence)
    Q_PROPERTY(QString Description MEMBER Description)

    FaultModel operator=(const FaultModel& other);
};

Q_DECLARE_METATYPE(FaultModel)

#endif // FAULTMODEL_H
