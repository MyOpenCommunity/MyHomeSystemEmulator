#ifndef PLANTBUSCONN_H
#define PLANTBUSCONN_H

#include <QtPlugin>
#include <plantnode.h>
#include <cmdexecutor.h>
#include <plantbus.h>

class PlantBusConn : public PlantBus
{
public:

    virtual                 ~PlantBusConn(){}


    virtual void            connectBus(const QString& theFatherBusID,  const QString& theChildBusID, SysError& theErr)=0;
    virtual void            disconnectBus(void)=0;
};

Q_DECLARE_INTERFACE(PlantBusConn, "it.bticino.myopen.PlantBusConnInterface/1.0")

#endif // PLANTBUSCONN_H
