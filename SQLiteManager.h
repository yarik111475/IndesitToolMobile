#ifndef SQLITEMANAGER_H
#define SQLITEMANAGER_H

#include <QObject>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QApplication>

#include "EepModel.h"
#include "Functions.h"


class SQLiteManager : public QObject
{
    Q_OBJECT
private:
    const QString FAULT_TABLE_NAME="FAULT_DESCRIPTION_TABLE";
    const QString EEP_TABLE_NAME="EEP_DATA_TABLE";;
    bool m_isDatabaseCorrect;
    QString m_directory;
    QString m_deviceType;
    QStringList m_filesList;
    QString m_databasePath;
public:
    explicit SQLiteManager(const QString& databaseName="stmbase.sqlite", QObject *parent = nullptr);
    QString getFaultDescription(quint16 deviceTypeCode, quint16 eepAreaSize, const QString& languageCode, const quint16 &code, const quint16 &subcode);

    Q_INVOKABLE
    QVariant getEepModelsList(const QString& applianceName);
    Q_INVOKABLE
    QByteArray getEepDataByDescriptors(const QString& applianceName, const QString& name, const QString& code, const QString serial);
    Q_INVOKABLE
    void addEepModel(const QString& applianceName, const EepModel model);


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
    void isDatabaseCorrectChanged(bool isCorrect);
};

#endif // SQLITEMANAGER_H
