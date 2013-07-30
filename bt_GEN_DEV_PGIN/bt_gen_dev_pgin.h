#ifndef BT_GEN_DEV_PGIN_H
#define BT_GEN_DEV_PGIN_H

#include <bt_gen_DEV_PGIN_global.h>
#include <devicefactory.h>

// Genric devices plugin
class Bt_GEN_DEV_PGIN : public QObject, public DeviceFactory{

    Q_OBJECT
    Q_INTERFACES(DeviceFactory)

public:
    Bt_GEN_DEV_PGIN();
    ~Bt_GEN_DEV_PGIN(){}

    QSharedPointer<PlantDevice>         createDevice(const QString& theKind, SysError& theErr);
    const QString                       getFactoryID(void);

};

#endif // BT_GEN_DEV_PGIN_H
