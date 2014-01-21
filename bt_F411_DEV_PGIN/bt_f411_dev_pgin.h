#ifndef BT_F411_DEV_PGIN_H
#define BT_F411_DEV_PGIN_H

#include "bt_F411_DEV_PGIN_global.h"
#include <devicefactory.h>





class  Bt_F411_DEV_PGIN : public QObject, public DeviceFactory

{
    Q_OBJECT
    Q_INTERFACES(DeviceFactory)
    Q_PLUGIN_METADATA(IID "it.bticino.myopen.DeviceFactoryInterface/1.0")

public:
    Bt_F411_DEV_PGIN();
    ~Bt_F411_DEV_PGIN(){}

    QSharedPointer<PlantDevice>     createDevice(const QString& theKind, SysError& theErr);
    const QString                   getFactoryID(void);

};

#endif // BT_F411_DEV_PGIN_H
