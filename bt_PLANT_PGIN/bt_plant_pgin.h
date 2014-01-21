#ifndef BT_PLANT_PGIN_H
#define BT_PLANT_PGIN_H

#include "bt_PLANT_PGIN_global.h"
#include <plantfactory.h>


class  Bt_PLANT_PGIN : public QObject, public PlantFactory

{
    Q_OBJECT
    Q_INTERFACES(PlantFactory)
    Q_PLUGIN_METADATA(IID "it.bticino.myopen.PlantFactoryInterface/1.0")

public:
    Bt_PLANT_PGIN();
    ~Bt_PLANT_PGIN(){}

    QSharedPointer<Plant>        createPlant(const QString& theKind, SysError& theErr);
    const QString                 getFactoryID(void);

};




#endif // BT_PLANT_PGIN_H
