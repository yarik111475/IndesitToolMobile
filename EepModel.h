#ifndef EEPMODEL_H
#define EEPMODEL_H
#include <QtCore>


class EepModel
{
    Q_GADGET
public:
    EepModel();
    QString Model;
    QString Code;
    QString Serial;
    quint16 FileChecksum;
    quint16 RealChecksum;
    bool IsChecksumValid;
    bool IsModelValid;
    QByteArray Data;

    Q_PROPERTY(QString Model MEMBER Model)
    Q_PROPERTY(QString Code MEMBER Code)
    Q_PROPERTY(QString Serial MEMBER Serial)
    Q_PROPERTY(quint16 FileChecksum MEMBER FileChecksum)
    Q_PROPERTY(quint16 RealChecksum MEMBER RealChecksum)
    Q_PROPERTY(bool IsChecksumValid MEMBER IsChecksumValid)
    Q_PROPERTY(bool IsModelValid MEMBER IsModelValid)
    Q_PROPERTY(QByteArray Data MEMBER Data)

    void reset();
    bool operator==(const EepModel& Model)const;
    bool operator!=(const EepModel& other) const;
    EepModel& operator=(const EepModel& other);

};
Q_DECLARE_METATYPE(EepModel)

#endif // EEPMODEL_H
