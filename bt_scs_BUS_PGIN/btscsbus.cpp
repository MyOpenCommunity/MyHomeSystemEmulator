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
