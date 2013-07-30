#ifndef DEVICEFACTORY_H
#define DEVICEFACTORY_H

#include <QSharedPointer>
#include <plantdevice.h>

class DeviceFactory
{
public:
    virtual ~DeviceFactory(){}

    virtual QSharedPointer<PlantDevice>     createDevice(const QString& theKind, SysError& theErr)=0;
    virtual const QString                     getFactoryID(void)=0;
};

Q_DECLARE_INTERFACE(DeviceFactory, "it.bticino.myopen.DeviceFactoryInterface/1.0")

#endif // DEVICEFACTORY_H
