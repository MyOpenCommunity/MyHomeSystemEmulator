#ifndef BT_OWN_GTW_PGIN_H
#define BT_OWN_GTW_PGIN_H


#include <gatewayfactory.h>
#include "bt_own_GTW_PGIN_global.h"


class  Bt_own_GTW_PGIN : public QObject, public GatewayFactory

{
    Q_OBJECT
    Q_INTERFACES(GatewayFactory)
    Q_PLUGIN_METADATA(IID "it.bticino.myopen.GatewayFactoryInterface/1.0")

public:
    Bt_own_GTW_PGIN();
    ~Bt_own_GTW_PGIN(){}

    QSharedPointer<PlantGateway>    createGateway(const QString& theKind, SysError& theErr);
    const QString                    getFactoryID(void);

};
#endif // BT_OWN_GTW_PGIN_H
