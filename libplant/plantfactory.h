#ifndef PLANTFACTORY_H
#define PLANTFACTORY_H


#include <QSharedPointer>
#include <QtPlugin>
#include <plant.h>

class PlantFactory
{

public:

    virtual ~PlantFactory(){}

    virtual QSharedPointer<Plant>   createPlant(const QString& theKind, SysError& theErr)=0;
    virtual const QString             getFactoryID(void)=0;
    
};

Q_DECLARE_INTERFACE(PlantFactory, "it.bticino.myopen.PlantFactoryInterface/1.0")

#endif // PLANTFACTORY_H
