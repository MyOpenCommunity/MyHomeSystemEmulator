#include <bt_scs_bus_pgin.h>
#include <btscsbus.h>
#include <QtPlugin>

Bt_scs_BUS_PGIN::Bt_scs_BUS_PGIN()
{
    ;
}


QSharedPointer<PlantBus>    Bt_scs_BUS_PGIN::createBus(const QString& /*theKind*/, SysError& /*theErr*/)  {
     QSharedPointer<PlantBus> plantBusRef = QSharedPointer<PlantBus>(new Btscsbus());

     return(plantBusRef);
}

const QString Bt_scs_BUS_PGIN::getFactoryID(void) {
    return(Btscsbus::className().toStdString().c_str());
}

Q_EXPORT_PLUGIN2(bt_scs_BUS_PGIN, Bt_scs_BUS_PGIN)
