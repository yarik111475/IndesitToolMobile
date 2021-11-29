#ifndef FILEHANDLER_H
#define FILEHANDLER_H
#include <QtCore>
#include <QObject>

#include "EepModel.h"
#include "EepParser.h"
#include "EepModel.h"
#include "Functions.h"
#include "Factories.h"
#include "SQLiteManager.h"

class FileHandler : public QObject
{
    Q_OBJECT
private:
    SQLiteManager m_sqliteManger;
    bool m_isDatabaseCorrect;
public:
    EepModel Model;
    Q_PROPERTY(EepModel Model MEMBER Model)
public:
    explicit FileHandler(QObject *parent = nullptr);
    ~FileHandler(){}
    Q_INVOKABLE
    bool isEepModelValid()const;
    Q_INVOKABLE
    QString getEepModelName() const;
    Q_INVOKABLE
    void saveEepModelData(const QString& applianceName, const QString &eepModelName, const QString &extenshion);
    Q_INVOKABLE
    QVariant getCurrentFilesList(const QString& applianceName, const QString &extension);
    Q_INVOKABLE
    void openEepModelData(const QString& applianceName, const QString& eepModelName);

    Q_INVOKABLE
    void openEepModelDataFromDatabase(const QString& applianceName,const QString& name,const QString& code,const QString& serial);

    Q_INVOKABLE
    QVariant getEepModelsList(const QString& applianceName);

    Q_INVOKABLE
    void addEepModel(const QString& applianceNamel);

    Q_PROPERTY(bool isDatabaseCorrect
               READ getIsDatabaseCorrect
               WRITE setIsDatabaseCorrect
               NOTIFY isDatabaseCorrectChanged)

    inline bool getIsDatabaseCorrect()const{
        return m_isDatabaseCorrect;
    }
    inline void setIsDatabaseCorrect(bool isCorrect){
        m_isDatabaseCorrect=isCorrect;
        emit isDatabaseCorrectChanged(m_isDatabaseCorrect);
    }

signals:
    void signalSaveEepDataOK();
    void signalOpenEepDataOK(const EepModel& model);
    void signalAddEepDataOK();
    void signalException(const QString& message);
    void isDatabaseCorrectChanged(bool isCorrect);

};

#endif // FILEHANDLER_H
