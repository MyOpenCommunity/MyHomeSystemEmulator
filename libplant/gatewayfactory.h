#ifndef GATEWAYFACTORY_H
#define GATEWAYFACTORY_H

#include <QtPlugin>
#include <plantgateway.h>
#include <QSharedPointer>

class GatewayFactory
{
public:
    virtual ~GatewayFactory(){}

    virtual QSharedPointer<PlantGateway>    createGateway(const QString& theKind, SysError& theErr)=0;
    virtual const QString                     getFactoryID(void)=0;
};

Q_DECLARE_INTERFACE(GatewayFactory, "it.bticino.myopen.GatewayFactoryInterface/1.0")

#endif // GATEWAYFACTORY_H
