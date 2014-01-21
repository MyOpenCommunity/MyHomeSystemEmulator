#include <bt_plant_pgin.h>
#include <btplant.h>
#include <QtPlugin>

Bt_PLANT_PGIN::Bt_PLANT_PGIN()
{
    ;
}


QSharedPointer<Plant>       Bt_PLANT_PGIN::createPlant(const QString& /*theKind*/, SysError& /*theErr*/)    {

    QSharedPointer<Plant> plantRef = QSharedPointer<Plant>(new BtPlant());

    return(plantRef);
}


const QString Bt_PLANT_PGIN::getFactoryID(void) {
    return(BtPlant::className().toStdString().c_str());
}

