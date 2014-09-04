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

#include <btplant.h>
#include <QString>
#include <syserror.h>
#include <QDebug>

static const QString PLANT = "plant";
static const QString ID = "id";

BtPlant::BtPlant()
{
    ;
}


BtPlant::~BtPlant() {
    ;
}

void BtPlant::setID(const QString& theID)   {
    m_plantID = theID;
}


const QString& BtPlant::getID(void) {
    return(m_plantID);
}


void BtPlant::addBus(const QSharedPointer<PlantBus>& theBus)    {
    m_plantBusList.insert(theBus->getID(), theBus);
}

void BtPlant::removeBus(const QString& theID)   {
    m_plantBusList.remove(theID);
}

QSharedPointer<PlantBus> BtPlant::getBus(const QString& theID)  {
    QSharedPointer<PlantBus> retVal;

    QMap<QString, QSharedPointer<PlantBus> >::iterator theIter = m_plantBusList.find(theID);

    if(theIter != m_plantBusList.end()) {
        retVal = theIter.value();
    } else {
        retVal = getBusConn(theID);
    }
    return(retVal);
}

QList< QSharedPointer<PlantBus> >    BtPlant::getBusList(void)   {
    return(m_plantBusList.values());
}

void    BtPlant::addDevice(const QSharedPointer<PlantDevice>& theDevice)   {
    m_plantDeviceList.insert(theDevice->getID(), theDevice);

    QMap<QString, QSharedPointer<PlantBus> >::iterator theIter = m_plantBusList.find(theDevice->getBusName());

    if(theIter != m_plantBusList.end()) {
        connect(theIter.value().data(), SIGNAL(ReleaseMessage(PlantMessage&, SysError&)), theDevice.data(), SLOT(ReceiveMessage(PlantMessage&, SysError&)));
        connect(theDevice.data(), SIGNAL(ReleaseMessage(PlantMessage&, SysError&)), theIter.value().data(), SLOT(ReceiveMessage(PlantMessage&, SysError&)));
    } else {
        QMap<QString, QSharedPointer<PlantBusConn> >::iterator theIter = m_plantBusConnList.find(theDevice->getBusName());
        if(theIter != m_plantBusConnList.end()) {
            connect(theIter.value().data(), SIGNAL(ReleaseMessage(PlantMessage&, SysError&)), theDevice.data(), SLOT(ReceiveMessage(PlantMessage&, SysError&)));
            connect(theDevice.data(), SIGNAL(ReleaseMessage(PlantMessage&, SysError&)), theIter.value().data(), SLOT(ReceiveMessage(PlantMessage&, SysError&)));
        }
    }
}

bool    BtPlant::removeDevice(const QString& theID) {
    if (m_plantDeviceList.contains(theID))
    {
        qDebug() << "Removing device " << theID;
        m_plantDeviceList.remove(theID);
        return(true);
    }
    return(false);
}

QSharedPointer<PlantDevice>   BtPlant::getDevice(const QString& theID)  {
    QSharedPointer<PlantDevice> retVal;

    QMap<QString, QSharedPointer<PlantDevice> >::iterator theIter = m_plantDeviceList.find(theID);

    if(theIter != m_plantDeviceList.end())  {
        retVal = theIter.value();
    }

    return(retVal);
}

QList< QSharedPointer<PlantDevice> > BtPlant::getDeviceList(void)   {
    return(m_plantDeviceList.values());
}

void    BtPlant::addBusConn(const QSharedPointer<PlantBusConn>& theBusConn) {
    qDebug() << "Add Bus conn: " << theBusConn;
    if (!m_plantBusConnList.contains(theBusConn->getID()))
        m_plantBusConnList.insert(theBusConn->getID(), theBusConn);
}

bool     BtPlant::removeBusConn(const QString& theID)   {
    qDebug() << "Remove Bus conn: " << theID;
    if(m_plantBusConnList.contains(theID))
    {
        m_plantBusConnList.remove(theID);
        return(true);
    }
    return(false);
}

QSharedPointer<PlantBusConn>  BtPlant::getBusConn(const QString& theID) {
    QSharedPointer<PlantBusConn> retVal;

    QMap<QString, QSharedPointer<PlantBusConn> >::iterator theIter = m_plantBusConnList.find(theID);

    if(theIter != m_plantBusConnList.end()) {
        return(theIter.value());
    }

    return(retVal);
}

QList< QSharedPointer<PlantBusConn> >     BtPlant::getBusConnList(void)  {
    return(m_plantBusConnList.values());
}

void     BtPlant::addGateway(const QSharedPointer<PlantGateway>& theGateway, SysError &theErr)    {
    if (m_plantGatewayList.size() > 0)
    {
        qDebug() << "A Gateway is already present in the plant ... ";
        theErr = SysError(SysError::GWT_OVERFLOW, "A gateway is already present in the plant ...");
        return;
    }

    m_plantGatewayList.insert(theGateway->getID(), theGateway);
    QMap<QString, QSharedPointer<PlantBus> >::iterator theIter = m_plantBusList.find(theGateway->getBusName());

    if(theIter != m_plantBusList.end()) {
        qDebug() << "Set Signal between gateway and bus";
        if (connect(theIter.value().data(), SIGNAL(ReleaseMessage(PlantMessage&, SysError&)), theGateway.data(), SLOT(ReceiveMessage(PlantMessage&, SysError&)))) {
            qDebug() << "gateway Connacted";
        }
        if (connect(theGateway.data(), SIGNAL(ReleaseMessage(PlantMessage&, SysError&)), theIter.value().data(), SLOT(ReceiveMessage(PlantMessage&, SysError&)))) {
            qDebug() << "bus Connacted to the gateway";
        }
    }
}

bool     BtPlant::removeGateway(const QString& theID)   {
    if (m_plantGatewayList.contains(theID))
    {
        qDebug() << "Remove " << m_plantGatewayList.remove(theID) << " Gateway";
        return(true);
    }
    return(false);
}

QSharedPointer<PlantGateway>  BtPlant::getGateway(const QString& theID) {
    QSharedPointer<PlantGateway>  retVal;

    QMap<QString, QSharedPointer<PlantGateway> >::iterator theIter = m_plantGatewayList.find(theID);

    if(theIter != m_plantGatewayList.end()) {
        retVal=theIter.value();
    }

    return(retVal);
}

QList< QSharedPointer<PlantGateway> >     BtPlant::getGatewayList(void)  {
    return(m_plantGatewayList.values());
}

void BtPlant::HandleMessage(const PlantMessage& /*theSrt*/, SysError& /*theErr*/)   {

}

unsigned int BtPlant::executeCmd(CMD_PRIO /*thePrio*/, const QByteArray& /*theCmd*/, QByteArray& /*theResult*/, SysError& /*theErr*/)   {

    return(0);
}

bool BtPlant::cancelCmd(unsigned int /*theCmdId*/)  {
    return(false);
}

void BtPlant::init(SysError& /*theErr*/)
{

}

void BtPlant::registerEventHnd(EventHandler* /*theHnd*/, SysError& /*theErr*/)
{

}

void BtPlant::unRegisterEventHnd(EventHandler* /*theHnd*/)
{

}

bool BtPlant::setApl(const QString& /*theAddress*/, bool /*theVal*/)
{
    return false;
}

QList<bool> BtPlant::getApl(const QString& /*theAddress*/)
{
    QList<bool> res;
    return res;
}

bool BtPlant::execCommand(PlantMessage &/*theMsg*/)
{
    return false;
}

QDomDocument BtPlant::save(SysError &theErr) {

    QMap<QString, QDomDocument> busesList;
    QDomDocument plantXml;
    QDomElement plant = plantXml.createElement(PLANT);
    plantXml.appendChild(plant);
    plant.setAttribute(ID, m_plantID);

    foreach (QSharedPointer<PlantBus> bus, m_plantBusList) {
        QDomDocument doc = bus->save(theErr);
        plant.appendChild(doc);
        busesList.insert(bus->getID(), doc);
    }
    foreach (QSharedPointer<PlantBusConn> busConn, m_plantBusConnList) {
        QDomDocument doc = busConn->save(theErr);
        plant.appendChild(doc);
        busesList.insert(busConn->getID(), doc);
    }

    foreach (QSharedPointer<PlantGateway> gwt, m_plantGatewayList) {
        busesList[gwt->getBusName()].firstChild().appendChild(gwt->save(theErr));
    }

    foreach (QSharedPointer<PlantDevice> device, m_plantDeviceList) {
        busesList[device->getBusName()].firstChild().appendChild(device->save(theErr));
    }

    return(plantXml);
}
