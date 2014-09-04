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

#ifndef PLANTMGR_H
#define PLANTMGR_H

#include "libplant_global.h"
#include <busfactory.h>
#include <busconnfactory.h>
#include <devicefactory.h>
#include <gatewayfactory.h>
#include <plantfactory.h>
#include <cmdexecutor.h>
#include <QList>
#include <QSharedPointer>
#include <QList>
#include <QString>
#include <QFile>

//  Forward Declaration

//  @@
//  We have to also consider how to persist from the PlantMgr all the plant
//  informations that have to be retrieved each time a plant is loaded

class LIBPLANTSHARED_EXPORT PlantMgr : public CmdExecutor
{
public:

    ~PlantMgr();
    static PlantMgr&                        getInstance();

    void                                    setPluginPath(const QString& thePath);
    void                                    loadPlugin(void);

    //  Plant related services
    QSharedPointer<Plant>                   addPlant(const QString& theKind, const QString& theID, SysError& theErr);
    void                                    deletePlant(const QString& theId);
    QSharedPointer<Plant>                   getPlant(const QString& theId);
    QList<QSharedPointer<Plant> >    getPlantList(void);


    //  Plant related factory interface
    QSharedPointer<Plant>           createPlant(const QString& theKind, SysError& theErr);
    QSharedPointer<PlantDevice>     createDevice(const QString& theKind, SysError& theErr);
    QSharedPointer<PlantBus>        createBus(const QString& theKind, SysError& theErr);
    QSharedPointer<PlantBusConn>    createBusConn(const QString& theKind, SysError& theErr);
    QSharedPointer<PlantGateway>    createGateway(const QString& theKind, SysError& theErr);

    QByteArray                      savePlant(const QString& platId, SysError &theErr);




    QMap<QString, QString>          getDeviceType();
    QMap<QString, QString>          getGatewayType();
    QMap<QString, QString>          getBusType();
    QMap<QString, QString>          getBusConnType();
    QMap<QString, QString>          getPlantType();
    const QString                   getBuses();

    void                            activeLog(QString active);




    //  Inherithed from CmdExecutor Class
    unsigned int    executeCmd(CMD_PRIO thePrio, const QByteArray& theCmd, QByteArray& theResult, SysError& theErr);
    bool            cancelCmd(unsigned int theCmdId);

    inline void setCurrPlantName(QString plantName) { m_currPlantName = plantName; }

    inline QString getCurrPlantName() {return(m_currPlantName); }

    static QString logIsActive;

private:
    //  Plant Manager is a Singleton object
    PlantMgr();
    PlantMgr(const PlantMgr& theOrg);
    PlantMgr& operator=(const PlantMgr& theRVal);

    //  Managed plgin
    enum    PGIN_TYPE {NONE=0, PLANT, DEVICE, BUS, BUSCONN, GATEWAY};

    void addPluginFactory(const QString& libName, PlantMgr::PGIN_TYPE theType);


    //  Plugin Folder
    QString                         m_pluginPath;

    //  Plugin related factory list
    QMap<QString, PlantFactory*>    m_plantFactList;
    QMap<QString, DeviceFactory*>   m_devFactList;
    QMap<QString, BusFactory*>      m_busFactList;
    QMap<QString, BusConnFactory*>  m_busConnFactList;
    QMap<QString, GatewayFactory*>  m_gatewayFactList;

    //  Naming extension for plugin components
    const char* const        PLANT_EXT;
    const char* const        GW_EXT;
    const char* const        DEV_EXT;
    const char* const        BUS_EXT;
    const char* const        BUSC_EXT;

    //  Plant container
    QMap<QString, QSharedPointer<Plant> > m_plantList;
    QString                               m_currPlantName;

};

#endif // PLANTMGR_H
