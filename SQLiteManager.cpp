/*
 * FaultDescriptionsTable structure
 * ------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * Id int | DeviceTypeCode int | EepAreaSize int  | Code QString | SubCode QString | RU QString (languageCode for Russian) | EN QString (languageCode for English)
 * ------------------------------------------------------------------------------------------------------------------------------------------------------------------
 *        |                    |                  |              |                 |                                       |
 * ------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * */

#include <QSqlQuery>
#include <QSqlError>
#include "EepParser.h"
#include "Factories.h"
#include "SQLiteManager.h"


SQLiteManager::SQLiteManager(const QString &databaseName, QObject *parent) : QObject(parent), m_isDatabaseCorrect(false)
{
    QString downloadPath=QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    m_databasePath=downloadPath + "/" + databaseName;
    if(QFile::exists(m_databasePath)){
        setIsDatabaseCorrect(true);
        return;
    }
    setIsDatabaseCorrect(false);

    QString eepQueryText="CREATE TABLE IF NOT EXISTS 'EEP_DATA_TABLE' "
            "('Id' INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, "
            "'ApplianceName' TEXT, 'Name' TEXT, 'Code' TEXT, 'Serial' TEXT, 'Data' BLOB )";

    if(!QFile::exists(m_databasePath)){
        QString connectionName;
        {
            QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE","SQLITE");
            db.setDatabaseName(m_databasePath);
            bool ok=db.open();

            if(ok){
                connectionName=db.connectionName();
                QSqlQuery query(db);
                query.prepare(eepQueryText);
                if(query.exec()){
                    setIsDatabaseCorrect(true);
                }
                else{
                    qDebug()<<query.lastError().text();
                }
            }
            else{
                qDebug()<< QObject::tr("Cannot open database");
            }
        }
        QSqlDatabase::removeDatabase(connectionName);
    }
}

QString SQLiteManager::getFaultDescription(quint16 deviceTypeCode, quint16 eepAreaSize, const QString &languageCode,
                                             const quint16 &code, const quint16 &subcode)
{
    Q_UNUSED(eepAreaSize)

    QString faultDescription=QObject::tr("Database failure\nDescription not available");
    QFile file(":/database/faults.db");
    QTemporaryFile* pTemporaryFile=QTemporaryFile::createNativeFile(file);

    if(pTemporaryFile){
        QString connectionName;
        QString databasePath=pTemporaryFile->fileName();
        {
            QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE","SQLITE");
            db.setDatabaseName(databasePath);
            bool ok=db.open();
            if(!ok){
                faultDescription=QObject::tr("Database failure\nDescription not available");
            }
            else{
                connectionName=db.connectionName();
                QSqlQuery query(db);
                query.prepare("SELECT * FROM " + FAULT_TABLE_NAME + " "
                                                                    " WHERE DeviceTypeCode=:DeviceTypeCode"
                                                                    " AND Code=:Code AND SubCode=:SubCode");
                query.bindValue(":DeviceTypeCode", deviceTypeCode);
                query.bindValue(":Code", code);
                query.bindValue(":SubCode", subcode);
                if(query.exec()){
                    while(query.next()){
                        QString description=query.value(languageCode).toString();
                        if(!description.isEmpty()){
                            faultDescription=description;
                        }
                    }
                }
            }
        }
        QSqlDatabase::removeDatabase(connectionName);
    }
    delete pTemporaryFile;
    return faultDescription;
}

QVariant SQLiteManager::getEepModelsList(const QString &applianceName)
{ 

    QList<EepModel> eepModels;
    QString connectionName;

    {
        QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE","SQLITE");
        db.setDatabaseName(m_databasePath);
        bool ok=db.open();
        connectionName=db.connectionName();
        if(!ok){
            throw QObject::tr("Cannot open database");
        }
        else{
            QSqlQuery query(db);
            query.prepare("SELECT Name, Code, Serial FROM " + EEP_TABLE_NAME + " WHERE ApplianceName=:applianceName ORDER BY Name");
            query.bindValue(":applianceName", applianceName);

            if(query.exec()){
                while(query.next()){
                    EepModel eepModel;
                    eepModel.Model=query.value("Name").toString();
                    eepModel.Code=query.value("Code").toString();
                    eepModel.Serial=query.value("Serial").toString();

                    eepModels.push_back(eepModel);
                }
            }
        }
    }
    QSqlDatabase::removeDatabase(connectionName);
    return QVariant::fromValue(eepModels);
}

QByteArray SQLiteManager::getEepDataByDescriptors(const QString &applianceName, const QString &name, const QString &code, const QString serial)
{
    QByteArray eepData;
    QString connectionName;

    {
        QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE","SQLITE");
        db.setDatabaseName(m_databasePath);
        bool ok=db.open();
        connectionName=db.connectionName();

        if(!ok){
            throw QObject::tr("Cannot open database");
        }
        else{
            QSqlQuery query(db);
            query.prepare("SELECT Data FROM " + EEP_TABLE_NAME + " "
                          "WHERE ApplianceName=:applianceName "
                          "AND Name=:name "
                          "AND Code=:code "
                          "AND Serial=:serial");

            query.bindValue(":applianceName", applianceName);
            query.bindValue(":name", name);
            query.bindValue(":code", code);
            query.bindValue(":serial", serial);
            if(query.exec()){
                while(query.next()){
                    QByteArray data=query.value("Data").toByteArray();
                    eepData.append(data);
                    break;
                }
            }
            else{
                throw QObject::tr("Uncorrect eep data descriptors");
            }
        }
    }
    QSqlDatabase::removeDatabase(connectionName);
    return eepData;
}

void SQLiteManager::addEepModel(const QString &applianceName, const EepModel model)
{
    QString connectionName;
    {
        QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE","SQLITE");
        db.setDatabaseName(m_databasePath);
        bool ok=db.open();
        connectionName=db.connectionName();
        if(!ok){
            throw QObject::tr("Cannot open database");
        }
        else{
            QSqlQuery query(db);

            query.prepare("SELECT * FROM " + EEP_TABLE_NAME + " WHERE "
                          "ApplianceName=:applianceName "
                          "AND Name=:name "
                          "AND Code=:code "
                          "AND Serial=:serial");
            query.bindValue(":applianceName", applianceName);
            query.bindValue(":name", model.Model);
            query.bindValue(":code", model.Code);
            query.bindValue(":serial", model.Serial);

            bool queryOk=query.exec();
            if(queryOk && query.next()){
                QSqlDatabase::removeDatabase(connectionName);
                throw QObject::tr("Same eep data file\nexists in database");
            }

            query.clear();
            query.prepare("INSERT INTO " + EEP_TABLE_NAME + " (ApplianceName, Name, Code, Serial, Data) "
                          "VALUES(:applianceName, :name, :code, :serial, :data)");
            query.bindValue(":applianceName", applianceName);
            query.bindValue(":name", model.Model);
            query.bindValue(":code", model.Code);
            query.bindValue(":serial", model.Serial);
            query.bindValue(":data", model.Data);
            if(!query.exec()){
                QSqlDatabase::removeDatabase(connectionName);
                throw QObject::tr("Failure add eep data\nto database");
            }
        }
    }
    QSqlDatabase::removeDatabase(connectionName);
}

