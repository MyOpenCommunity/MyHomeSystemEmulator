#ifndef BUSCONNFACTORY_H
#define BUSCONNFACTORY_H

#include <QSharedPointer>
#include <plantbusconn.h>
#include <QtPlugin>

class BusConnFactory
{
public:
    virtual ~BusConnFactory(){}

    virtual QSharedPointer<PlantBusConn>    createBusConn(const QString& theKind, SysError& theErr)=0;
    virtual QString                         getFactoryID(void)=0;
};

Q_DECLARE_INTERFACE(BusConnFactory, "it.bticino.myopen.BusConnFactoryInterface/1.0")


#endif // BUSCONNFACTORY_H
