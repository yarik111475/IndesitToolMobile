#include "Factories.h"

Factories::Factories()
{

}

FaultParser *Factories::createFaultParser(const QString &deviceType)
{
    FaultParser* faultParser=nullptr;
    Functions::DeviceType type=Functions::getDeviceType(deviceType);
    switch(type){
    case Functions::ARCADIA:
        faultParser=new ArcadiaFaultParser;
        break;
    case Functions::EVO_2:
        faultParser=new Evo2FaultParser;
        break;
    case Functions::EVO_1:
        faultParser=new Evo1FaultParser;
        break;
    default:
        throw QObject::tr("Unknown device type");
    }
    return faultParser;
}

EepParser *Factories::createEepParser(const QString &deviceType)
{
    EepParser* pEepParser=nullptr;
    Functions::DeviceType type=Functions::getDeviceType(deviceType);
    switch(type){
    case Functions::ARCADIA:
        pEepParser=new ArcadiaEepParser;
        break;
    case Functions::EVO_2:
        pEepParser=new Evo2EepParser;
        break;
    case Functions::EVO_1:
        pEepParser=new Evo1EepParser;
        break;
    default:
        throw QObject::tr("Unknown device type");
    }
    return pEepParser;
}

EepReader *Factories::createEepReader(const QString &deviceType)
{
    EepReader* pEepReader=nullptr;
    Functions::DeviceType type=Functions::getDeviceType(deviceType);
    switch(type){
    case Functions::ARCADIA:
        pEepReader=new ArcadiaEepReader;
        break;
    case Functions::EVO_2:
        pEepReader=new Evo2EepReader;
        break;
    case Functions::EVO_1:
        pEepReader=new Evo1EepReader;
        break;
    default:
        throw QObject::tr("Unknown device type");
    }
    return pEepReader;
}

EepWriter *Factories::createEepWriter(const QString &deviceType)
{
    EepWriter* pEepWriter=nullptr;
    Functions::DeviceType type=Functions::getDeviceType(deviceType);
    switch(type){
    case Functions::ARCADIA:
        pEepWriter=new ArcadiaEepWriter;
        break;
    case Functions::EVO_2:
        pEepWriter=new Evo2EepWriter;
        break;
    case Functions::EVO_1:
        pEepWriter=new Evo1EepWriter;
        break;
    default:
        throw QObject::tr("Unknown device type");
    }
    return pEepWriter;
}

ApplianceParser *Factories::createApplianceParser(const QString &deviceType)
{
    ApplianceParser* pApplianceParser=nullptr;
    Functions::DeviceType type=Functions::getDeviceType(deviceType);
    switch(type){
    case Functions::ARCADIA:
        pApplianceParser=new ArcadiaApplianceParser;
        break;
    case Functions::EVO_2:
        pApplianceParser=new Evo2ApplianceParser;
        break;
    case Functions::EVO_1:
        pApplianceParser=new Evo1ApplianceParser;
        break;
    default:
        throw QObject::tr("Unknown device type");
    }
    return pApplianceParser;
}
