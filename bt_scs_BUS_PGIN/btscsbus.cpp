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

#include <btscsbus.h>
#include <QDebug>
#include <busxmlserializer.h>

Btscsbus::~Btscsbus()   {
    ;
}

Btscsbus::Btscsbus()    {
    ;
}

void    Btscsbus::setID(const QString& theID)  {
    m_busID = theID;
}

const QString&  Btscsbus::getID(void)  {
    return(m_busID);
}

void    Btscsbus::HandleMessage(const PlantMessage& /*theSrt*/, SysError& /*theErr*/)    {

}

unsigned int Btscsbus::executeCmd(CMD_PRIO /*thePrio*/, const QByteArray& /*theCmd*/, QByteArray& /*theResult*/, SysError& /*theErr*/)  {
    return(0);
}

bool    Btscsbus::cancelCmd(unsigned int /*theCmdId*/) {
    return(false);
}


void  Btscsbus::ReceiveMessage(PlantMessage &theMsg, SysError &theErr)
{
    qDebug() << "Btscsbus: " << m_busID << " Receive Message" ;
    emit ReleaseMessage(theMsg, theErr);
}

void Btscsbus::init(SysError& /*theErr*/)
{

}

void Btscsbus::registerEventHnd(EventHandler* /*theHnd*/, SysError& /*theErr*/)
{

}

void Btscsbus::unRegisterEventHnd(EventHandler* /*theHnd*/)
{

}

bool Btscsbus::setApl(const QString& /*theAddress*/, bool /*theVal*/)
{
    return false;
}

QList<bool> Btscsbus::getApl(const QString& /*theAddress*/)
{
    QList<bool> res;
    return res;
}

bool Btscsbus::execCommand(PlantMessage &/*theMsg*/)
{
    return false;
}

QDomDocument Btscsbus::save(SysError &/*theErr*/) {

    QDomDocument xml;

    BusXmlSerializer::serialize(m_busID, xml);

    return(xml);
}
