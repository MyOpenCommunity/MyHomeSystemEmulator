#ifndef PLANTGATEWAY_H
#define PLANTGATEWAY_H

#include <plantnode.h>
#include <cmdexecutor.h>
#include <QDebug>

class PlantGateway : public PlantNode
{
public:

    virtual ~PlantGateway(){ qDebug() << "~PlantGateway" ;}



};


#endif // PLANTGATEWAY_H
