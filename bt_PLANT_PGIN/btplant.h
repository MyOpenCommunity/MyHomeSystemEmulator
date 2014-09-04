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

#ifndef BTPLANT_H
#define BTPLANT_H

#include <QObject>
#include <plant.h>

class PlantMessage;

class BtPlant : public Plant
{
public:

    static QString const className() { return "BtPlant"; }

    BtPlant();

    ~BtPlant();

    void                                            setID(const QString& theID);
    const QString&                                  getID(void);

    void                                            addBus(const QSharedPointer<PlantBus>& theBus);
    void                                            removeBus(const QString& theID);
            QSharedPointer<PlantBus>                getBus(const QString& theID);
            QList< QSharedPointer<PlantBus> >       getBusList(void);

    void                                            addDevice(const QSharedPointer<PlantDevice>& theDevice);
    bool removeDevice(const QString& theID);
            QSharedPointer<PlantDevice>             getDevice(const QString& theID);
            QList< QSharedPointer<PlantDevice> >    getDeviceList(void);

    void                                            addBusConn(const QSharedPointer<PlantBusConn>& theBusConn);
    bool removeBusConn(const QString& theID);
            QSharedPointer<PlantBusConn>            getBusConn(const QString& theID);
            QList< QSharedPointer<PlantBusConn> >   getBusConnList(void);

    void addGateway(const QSharedPointer<PlantGateway>& theGateway, SysError &theErr);
    bool removeGateway(const QString& theID);
            QSharedPointer<PlantGateway>            getGateway(const QString& theID);
            QList< QSharedPointer<PlantGateway> >   getGatewayList(void);

    void                                            HandleMessage(const PlantMessage& theSrt, SysError& theErr);

    unsigned int                                    executeCmd(CMD_PRIO thePrio, const QByteArray& theCmd, QByteArray& theResult, SysError& theErr);

    bool                                            cancelCmd(unsigned int theCmdId);

    void                                            init(SysError& theErr);

    void                                            registerEventHnd(EventHandler* theHnd, SysError& theErr);

    void                                            unRegisterEventHnd(EventHandler* theHnd);

    bool                                            setApl(const QString& theAddress, bool theVal);

    QList<bool>                                     getApl(const QString& theAddress);

    bool                                            execCommand(PlantMessage &theMsg);

    QDomDocument                                    save(SysError &theErr);


private:
    QString m_plantID;
    QMap<QString, QSharedPointer<PlantBus> >        m_plantBusList;
    QMap<QString, QSharedPointer<PlantDevice> >     m_plantDeviceList;
    QMap<QString, QSharedPointer<PlantBusConn> >    m_plantBusConnList;
    QMap<QString, QSharedPointer<PlantGateway> >    m_plantGatewayList;


};

#endif // BTPLANT_H
