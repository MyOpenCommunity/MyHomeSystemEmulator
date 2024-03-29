#ifndef BT_F520_DEV_PGIN_H
#define BT_F520_DEV_PGIN_H

#include "bt_F520_DEV_PGIN_global.h"
#include <devicefactory.h>

class Bt_F520_DEV_PGIN : public QObject, public DeviceFactory {
public:

    Q_OBJECT
    Q_INTERFACES(DeviceFactory)
public:
    Bt_F520_DEV_PGIN();
    ~Bt_F520_DEV_PGIN(){}

    QSharedPointer<PlantDevice>     createDevice(const QString& theKind, SysError& theErr);
    const QString                   getFactoryID(void);
};

#endif // BT_F520_DEV_PGIN_H
