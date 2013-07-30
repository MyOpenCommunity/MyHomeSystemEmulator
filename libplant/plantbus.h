#ifndef PLANTBUS_H
#define PLANTBUS_H

#include "libplant_global.h"
#include <plantnode.h>
#include <QString>
#include <QtPlugin>
#include <cmdexecutor.h>
#include <QSharedPointer>



class PlantBus : public PlantNode
{
public:
    virtual                 ~PlantBus(){}



};



#endif // PLANTBUS_H
