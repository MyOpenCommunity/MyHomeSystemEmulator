#ifndef BT_CAM_DEV_PGIN_H
#define BT_CAM_DEV_PGIN_H

#include "bt_CAM_DEV_PGIN_global.h"
#include <devicefactory.h>


class Bt_CAM_DEV_PGIN : public QObject, public DeviceFactory{
    Q_OBJECT
    Q_INTERFACES(DeviceFactory)
public:
    Bt_CAM_DEV_PGIN();
    ~Bt_CAM_DEV_PGIN(){}

    QSharedPointer<PlantDevice>     createDevice(const QString& theKind, SysError& theErr);
    const QString                    getFactoryID(void);
};

#endif // BT_CAM_DEV_PGIN_H
