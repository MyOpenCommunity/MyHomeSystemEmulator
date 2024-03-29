/* This file is part of My Home System Emulator
 *
 * My Home System Emulator is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.

 * My Home System Emulator is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with My Home System Emulator.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PLANT_H
#define PLANT_H

#include "libplant_global.h"
#include <cmdexecutor.h>
#include <plantbus.h>
#include <plantdevice.h>
#include <plantgateway.h>
#include <plantbusconn.h>
#include <plantnode.h>
#include <QList>
#include <QSharedPointer>
#include <QMap>



// class LIBPLANTSHARED_EXPORT Plant /*: public CmdExecutor */
class  Plant : public PlantNode
{
public:

    //Plant();
    virtual ~Plant(){}

    virtual void                                            addBus(const QSharedPointer<PlantBus>& theBus)=0;
    virtual void                                            removeBus(const QString& theID)=0;
    virtual       QSharedPointer<PlantBus>                  getBus(const QString& theID)=0;
    virtual       QList< QSharedPointer<PlantBus> >         getBusList(void)=0;

    virtual void                                            addDevice(const QSharedPointer<PlantDevice>& theDevice)=0;
    virtual bool                                            removeDevice(const QString& theID)=0;
    virtual       QSharedPointer<PlantDevice>               getDevice(const QString& theID)=0;
    virtual       QList< QSharedPointer<PlantDevice> >      getDeviceList(void)=0;

    virtual void                                            addBusConn(const QSharedPointer<PlantBusConn>& theBus)=0;
    virtual bool                                            removeBusConn(const QString& theID)=0;
    virtual       QSharedPointer<PlantBusConn>              getBusConn(const QString& theID)=0;
    virtual       QList< QSharedPointer<PlantBusConn> >     getBusConnList(void)=0;

    virtual void                                            addGateway(const QSharedPointer<PlantGateway>& theBus, SysError &theErr)=0;
    virtual bool                                            removeGateway(const QString& theID)=0;
    virtual       QSharedPointer<PlantGateway>              getGateway(const QString& theID)=0;
    virtual       QList< QSharedPointer<PlantGateway> >     getGatewayList(void)=0;


    /*
    unsigned int    executeCmd(CMD_PRIO thePrio, const QByteArray& theCmd, QByteArray& theResult, SysError& theErr);
    bool            cancelCmd(unsigned int theCmdId);
    */
};


//Q_DECLARE_INTERFACE(Plant, "it.bticino.myopen.PlantInterface/1.0")

#endif // PLANT_H
