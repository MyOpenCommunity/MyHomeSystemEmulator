#ifndef BT_PLANT_PGIN_H
#define BT_PLANT_PGIN_H

#include <plantfactory.h>
#include "bt_PLANT_PGIN_global.h"


class  Bt_PLANT_PGIN : public QObject, public PlantFactory

{
    Q_OBJECT
    Q_INTERFACES(PlantFactory)

public:
    Bt_PLANT_PGIN();
    ~Bt_PLANT_PGIN(){}

    QSharedPointer<Plant>        createPlant(const QString& theKind, SysError& theErr);
    const QString                 getFactoryID(void);

};




#endif // BT_PLANT_PGIN_H
