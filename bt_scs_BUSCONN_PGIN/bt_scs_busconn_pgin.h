#ifndef BT_SCS_BUSCONN_PGIN_H
#define BT_SCS_BUSCONN_PGIN_H

#include "bt_scs_BUSCONN_PGIN_global.h"
#include <busconnfactory.h>
#include <plantbus.h>

class Bt_scs_BUSCONN_PGIN: public QObject, public BusConnFactory {

public:

    Q_OBJECT
    Q_INTERFACES(BusConnFactory)
    Q_PLUGIN_METADATA(IID "it.bticino.myopen.BusConnFactoryInterface/1.0")
public:
    Bt_scs_BUSCONN_PGIN();
    ~Bt_scs_BUSCONN_PGIN(){}

    QSharedPointer<PlantBusConn> createBusConn(const QString& theKind, SysError& theErr);
    QString                getFactoryID(void);


};

#endif // BT_SCS_BUSCONN_PGIN_H
