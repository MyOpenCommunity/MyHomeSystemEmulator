#ifndef BUSFACTORY_H
#define BUSFACTORY_H

#include <QtPlugin>
#include <QSharedPointer>
#include <plantbus.h>

class BusFactory
{
public:
    virtual ~BusFactory(){}

    virtual QSharedPointer<PlantBus>        createBus(const QString& theKind, SysError& theErr)=0;
    virtual const QString                     getFactoryID(void)=0;

};

Q_DECLARE_INTERFACE(BusFactory, "it.bticino.myopen.BusFactoryInterface/1.0")

#endif // BUSFACTORY_H
