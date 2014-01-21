#ifndef BT_SCS_BUS_PGIN_H
#define BT_SCS_BUS_PGIN_H

#include "bt_scs_BUS_PGIN_global.h"

#include <busfactory.h>

class  Bt_scs_BUS_PGIN : public QObject, public BusFactory

{
    Q_OBJECT
    Q_INTERFACES(BusFactory)
    Q_PLUGIN_METADATA(IID "it.bticino.myopen.BusFactoryInterface/1.0")

public:
    Bt_scs_BUS_PGIN();
    ~Bt_scs_BUS_PGIN(){}

    QSharedPointer<PlantBus>    createBus(const QString& theKind, SysError& theErr);
    const QString                getFactoryID(void);

};




#endif // BT_SCS_BUS_PGIN_H
