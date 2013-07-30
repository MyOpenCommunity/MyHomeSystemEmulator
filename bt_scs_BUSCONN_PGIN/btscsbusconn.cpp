#include "btscsbusconn.h"
#include <QDebug>
#include <busconnxml.h>

BtScsBusConn::~BtScsBusConn()   {
    ;
}

BtScsBusConn::BtScsBusConn()    {
    ;
}

void    BtScsBusConn::setID(const QString& theID)  {
    m_busID = theID;
}

const QString&  BtScsBusConn::getID(void)  {
    return(m_busID);
}

void    BtScsBusConn::HandleMessage(const PlantMessage& /*theSrt*/, SysError& /*theErr*/)    {

}

unsigned int BtScsBusConn::executeCmd(CMD_PRIO /*thePrio*/, const QByteArray& /*theCmd*/, QByteArray& /*theResult*/, SysError& /*theErr*/)  {
    return(0);
}

bool    BtScsBusConn::cancelCmd(unsigned int /*theCmdId*/) {
    return(false);
}


void  BtScsBusConn::ReceiveMessage(PlantMessage &theMsg, SysError &theErr)
{
    qDebug() << "Btscsbus: " << m_busID << " Receive Message" ;
    emit ReleaseMessage(theMsg, theErr);
}

void BtScsBusConn::init(SysError& /*theErr*/)
{

}

void BtScsBusConn::registerEventHnd(EventHandler* /*theHnd*/, SysError& /*theErr*/)
{

}

void BtScsBusConn::unRegisterEventHnd(EventHandler* /*theHnd*/)
{

}

bool BtScsBusConn::setApl(const QString& /*theAddress*/, bool /*theVal*/)
{
    return false;
}

QList<bool> BtScsBusConn::getApl(const QString& /*theAddress*/)
{
    QList<bool> res;
    return res;
}

bool BtScsBusConn::execCommand(PlantMessage &/*theMsg*/)
{
    return false;
}

QDomDocument BtScsBusConn::save(SysError &/*theErr*/) {

    QDomDocument xml;

    BusConnXml::serialize(m_busID, xml);

    return(xml);
}
