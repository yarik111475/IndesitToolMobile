#include "FileHandler.h"
#include "EepParser.h"
#include "Factories.h"
#include "Functions.h"

FileHandler::FileHandler(QObject *parent) : QObject(parent), m_isDatabaseCorrect(false)
{
    Model.reset();
    setIsDatabaseCorrect(m_sqliteManger.getIsDatabaseCorrect());
}

bool FileHandler::isEepModelValid() const
{
    return Model.IsModelValid;
}

QString FileHandler::getEepModelName() const
{
    if(Model.IsModelValid){
        QString eepModelName=QString("%1_%2_%3").arg(Model.Model).arg(Model.Code).arg(Model.Serial);
        return eepModelName;
    }
    return QString();
}

void FileHandler::saveEepModelData(const QString& applianceName, const QString &eepModelName, const QString &extenshion)
{
    if(Model.IsModelValid){
        if(applianceName!="ARCADIA"){
            if(extenshion.contains("eep")){
                QByteArray eepData=Functions::eepToBinToEep(Model.Data);
                Model.Data.clear();
                Model.Data.append(eepData);
            }
        }
        QString path=QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
        QFile file(path + "/" + eepModelName + extenshion.mid(1,4));
        if(file.open(QIODevice::WriteOnly)){
            file.write(Model.Data, Model.Data.size());
            file.close();
            emit signalSaveEepDataOK();
        }
        else{
            emit signalException(QObject::tr("Cannot open file to save eep data"));
            return;
        }
    }
    else{
        emit signalException(QObject::tr("No eep data exists to save"));
    }
}

QVariant FileHandler::getCurrentFilesList(const QString& applianceName,const QString &extension)
{
    QString path=QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    QDir dir(path);
    dir.setNameFilters(QStringList()<<extension);
    QStringList entries=dir.entryList(QDir::Files);

    if(entries.isEmpty()){
        return QVariant();
    }
    QList<EepModel> modelsList;
    EepParser* pEepParser=Factories::createEepParser(applianceName);
    foreach(QString entry, entries){
        QFile file(path + "/" + entry);
        if(file.open(QIODevice::ReadOnly)){
            QByteArray eepData=file.readAll();
            file.close();

            if(applianceName!="ARCADIA"){
                if(extension.contains("eep")){
                    eepData=Functions::eepToBinToEep(eepData);
                }
            }

            pEepParser->parseEep(eepData);
            EepModel eepModel=pEepParser->getEepModel();
            if(eepModel.IsModelValid){
                eepModel.Data.clear();
                modelsList.push_back(eepModel);
            }
        }
    }
    delete pEepParser;
    pEepParser=nullptr;

    return QVariant::fromValue(modelsList);
}

void FileHandler::openEepModelData(const QString& applianceName, const QString &eepModelName)
{
    QString downloadLocation=QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    QFileInfo fileInfo(eepModelName);
    QString suffix=fileInfo.suffix();

    QFile file(downloadLocation + "/" + eepModelName);
    if(file.open(QIODevice::ReadOnly)){
        QByteArray eepData=file.readAll();
        file.close();

        if(applianceName!="ARCADIA"){
            if(suffix.contains("eep")){
                eepData=Functions::eepToBinToEep(eepData);
            }
        }

        EepParser* pEepParser=Factories::createEepParser(applianceName);
        pEepParser->parseEep(eepData);
        Model=pEepParser->getEepModel();
        if(Model.IsModelValid){
            emit signalOpenEepDataOK(Model);
        }
        else{
            emit signalException(QObject::tr("Current file not correct\nTry to open other eep file"));
        }
    }
    else{
        emit signalException(QObject::tr("Can not open data file"));
    }
}

void FileHandler::openEepModelDataFromDatabase(const QString &applianceName, const QString &name, const QString &code, const QString &serial)
{
    if(!m_sqliteManger.getIsDatabaseCorrect()){
        emit signalException(QObject::tr("Database failure"));
        return;
    }

    try{
        QByteArray eepData=m_sqliteManger.getEepDataByDescriptors(applianceName, name,code,serial);
        EepParser* pEepParser=Factories::createEepParser(applianceName);
        pEepParser->parseEep(eepData);
        EepModel tempEepModel=pEepParser->getEepModel();
        delete pEepParser;
        pEepParser=nullptr;

        if(tempEepModel.IsModelValid){
            Model=tempEepModel;
            emit signalOpenEepDataOK(Model);
        }
        else{
            emit signalException(QObject::tr("Current file not correct\nTry to open other eep file"));
        }
    }
    catch(const QString& ex){
    }
    catch(...){
        emit signalException(QObject::tr("Generic database failure"));
    }
}

QVariant FileHandler::getEepModelsList(const QString &applianceName)
{
    try{
        return  m_sqliteManger.getEepModelsList(applianceName);
    }
    catch(const QString& ex){
        emit signalException(ex);
    }
    catch(...){
        emit signalException(QObject::tr("Generic database failure"));
    }
    return QVariant();
}

void FileHandler::addEepModel(const QString &applianceNamel)
{
    try{
        if(Model.IsModelValid){
            m_sqliteManger.addEepModel(applianceNamel, Model);
            emit signalAddEepDataOK();
        }
    }
    catch(const QString& ex){
        emit signalException(ex);
    }
    catch(...){
        emit signalException(QObject::tr("Generic database failure"));
    }
}
