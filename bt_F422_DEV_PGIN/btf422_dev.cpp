#include "btf422_dev.h"
#include <QStringList>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <f422xmlserializer.h>
#include <plantmgr.h>
#include <plantmessage.h>
#include <openmsg.h>

static const QString RESOURCE_FOLDER = ":/files/resources/";
static const QString RESOURCE_XML = "F422.xml";
static const QString BUS_PREFIX = "Bus_";
static const QString LOCAL_BUS = "4";

static const QString MSG_ACK = "*#*1##";
static const QString MSG_NACK = "*#*0##";
static const QString GENERALE = "0";

const QList<QString> FEED_BACK_FRAMES = QList<QString>() << MSG_ACK << MSG_NACK;

BtF422_dev::BtF422_dev()
{
    m_f422Status = QSharedPointer<F422Status>(new F422Status(""));

}

BtF422_dev::~BtF422_dev() {
    qDebug() << BtF422_dev::className() << " Destroy " << m_deviceID;
    PlantMgr::getInstance().getPlant(PlantMgr::getInstance().getCurrPlantName())->removeBusConn(m_localBus);
}

void    BtF422_dev::setID(const QString& theID)  {
    m_deviceID = theID;
}

const QString&  BtF422_dev::getID(void)  {
    return(m_deviceID);
}

void    BtF422_dev::HandleMessage(const PlantMessage& /*theSrt*/, SysError& /*theErr*/)    {
}

QDomDocument BtF422_dev::save(SysError &theErr) {

    QDomDocument xml;
    F422xmlserializer::serialize(m_deviceID, xml, m_f422Status, theErr);

    return(xml);

}

void    BtF422_dev::setSuperior(const QSharedPointer<PlantNode>& /*theSuperior*/)    {

}

void    BtF422_dev::attachChild(const QSharedPointer<PlantNode>& /*theMsgHndObjRef*/)    {

}

unsigned int BtF422_dev::executeCmd(CMD_PRIO /*thePrio*/, const QByteArray& theCmd, QByteArray& theResult, SysError& theErr)  {
    // Da sistemare con il file di risorse...
    QString rec(theCmd);
    QStringList list2 = rec.split("#", QString::SkipEmptyParts);
    int scelta = list2[0].toInt();
    switch(scelta)
    {
    case 1: // LOAD IMAGE   1#file_name
    {
        QString filename = RESOURCE_FOLDER + list2[1];
        qDebug() << m_deviceID << " LOAD IMAGE " << filename;
        QFile inFile( filename );
        if( ! inFile.open( QIODevice::ReadOnly ) )
        {
            qDebug() << m_deviceID << " HostThread: Cant find file " << filename;
            return -1;
        }

        QByteArray block;
        block =  inFile.readAll();

        qDebug() << m_deviceID << " FileSIze: " << inFile.size();
        qDebug() << m_deviceID << " Blocksize:" << block.size();

        theResult = block;
    }
        break;
    case 2: // LOAD XML FILE 2#
    {
        QString str = QDir::currentPath();


        qDebug() << m_deviceID << " Current path = " << str;

        QString filename = RESOURCE_FOLDER + RESOURCE_XML;
        qDebug() << m_deviceID << " LOAD XML " << filename;

        QFile inFile( filename );
        if( ! inFile.open( QIODevice::ReadOnly ) )
        {
            qDebug() << m_deviceID << " HostThread: Cant find file " << filename;
            return -1;
        }

        QByteArray block;

        block =  inFile.readAll();

        qDebug() << m_deviceID << "FileSIze: " << inFile.size();
        qDebug() << m_deviceID << "Blocksize:" << block.size();

        theResult = block;
    }
        break;
    case 4: // setup parameter request
    {
        QDomDocument xml;
        F422xmlserializer::serialize(m_deviceID, xml, m_f422Status, theErr);
        theResult = xml.toByteArray();
    }
        break;
    case 5:
    {
        QDomDocument xml;
        xml.setContent(list2[1]);
        F422xmlserializer::deserialize(xml, m_f422Status, theErr);

        if (!theErr.isError() && m_f422Status->isConfigured()) {

            m_localBus = BUS_PREFIX + m_f422Status->getI3() + m_f422Status->getI4();
            if (!(m_localBus.compare(m_oldLocalBus) == 0)) {

                if(m_oldLocalBus.compare("") != 0 && !m_oldLocalBus.isEmpty()) {
                    disconnect(PlantMgr::getInstance().getPlant(PlantMgr::getInstance().getCurrPlantName())->getBusConn(m_oldLocalBus).data(),
                               SIGNAL(ReleaseMessage(PlantMessage&, SysError&)), this, SLOT(ReceiveMsgLocalBUS(PlantMessage&, SysError&)));
                    PlantMgr::getInstance().getPlant(PlantMgr::getInstance().getCurrPlantName())->removeBusConn(m_oldLocalBus);
                }

                qDebug() << "BUS length: " << PlantMgr::getInstance().getBusConnType().values().length();
                if (!PlantMgr::getInstance().getPlant(PlantMgr::getInstance().getCurrPlantName())->getBusConn(m_localBus).isNull()) {
                    qDebug() << "BUS: " << m_localBus << " alrady exist";
                } else {
                    QSharedPointer<PlantBusConn> localBus = PlantMgr::getInstance().createBusConn(PlantMgr::getInstance().getBusConnType().values()[0],theErr);
                    if(!theErr.isError()) {
                        localBus->setID(m_localBus);
                        PlantMgr::getInstance().getPlant(PlantMgr::getInstance().getCurrPlantName())->addBusConn(localBus);
                    }
                    else
                        return(0);
                }

                connect(PlantMgr::getInstance().getPlant(PlantMgr::getInstance().getCurrPlantName())->getBusConn(m_localBus).data(),
                        SIGNAL(ReleaseMessage(PlantMessage&, SysError&)), this, SLOT(ReceiveMsgLocalBUS(PlantMessage&, SysError&)));

                m_oldLocalBus = m_localBus;
            }
        }
    }
        break;
    default:
    {
        qDebug() << m_deviceID << " NOT HANDLED";
    }
        break;
    }

    return(0);
}

bool    BtF422_dev::cancelCmd(unsigned int /*theCmdId*/) {
    return(false);
}


void    BtF422_dev::registerEventHnd(EventHandler* theHnd, SysError& /*theErr*/) {
    m_theHnd = theHnd;


}

void    BtF422_dev::unRegisterEventHnd(EventHandler* /*theHnd*/) {

}

void    BtF422_dev::ReceiveMessage( PlantMessage &theMsg, SysError &theErr) {

    qDebug() << className() << ": Receive Message from Bus" ;
    PlantMessage localMsg = theMsg;
    QPair<QString, QString>  destAddr = localMsg.getDestAddr();

    QSharedPointer<PlantBus> currentSecondBus = PlantMgr::getInstance().getPlant(PlantMgr::getInstance().getCurrPlantName())->getBus(BUS_PREFIX + m_f422Status->getI3() + m_f422Status->getI4());

    if (destAddr.first.compare(PlantMessage::INTERNAL) == 0 && destAddr.second.compare(PlantMessage::DEVICE) == 0)
    {
        // controll if message is form me
        QStringList where = localMsg.getValue(PlantMessage::WHERE).split("#", QString::SkipEmptyParts);
        if(where.at(0).compare(GENERALE) == 0) {
            qDebug() << className() << ": It is general message, send to local bus";
            currentSecondBus->ReceiveMessage(theMsg, theErr);
            return;
        }
        if (where.length() < 3) {
            qDebug() << className() << ": Discart Message, it is not for me" ;
            return;
        }
        if (where.at(1).compare(LOCAL_BUS) != 0) {
            qDebug() << className() << ": Discart Message, it is not for me" ;
            return;
        }
        if (where.at(2).compare(m_f422Status->getI3() + m_f422Status->getI4()) != 0) {
            qDebug() << className() << ": Discart Message, it is not for me" ;
            return;
        }

        qDebug() << className() << "Pre - OPEN MSG: " << localMsg.getValue(PlantMessage::OPEN);

        // Remove Local Bus identifier from open message
        QString openMsg = localMsg.getValue(PlantMessage::OPEN);
        openMsg = openMsg.replace(localMsg.getValue(PlantMessage::WHERE), where.at(0));

        // Remove Local Bus identifier from open message where
        localMsg.setValue(PlantMessage::OPEN, openMsg);
        localMsg.setValue(PlantMessage::WHERE, where.at(0));

        qDebug() << className() << "Post - OPEN MSG: " << localMsg.getValue(PlantMessage::OPEN);


        // Send message to my local Bus
        qDebug() << className() << "Message for device on my local BUS" ;

        currentSecondBus->ReceiveMessage(localMsg, theErr);
    } else {
        qDebug() << className() << ": Discart Message, it is not for me" ;
    }

}

void    BtF422_dev::ReceiveMsgLocalBUS( PlantMessage &theMsg, SysError &theErr) {
    qDebug() << className() << " ReceiveMsgLocalBUS" ;

    if(theMsg.getDestAddr().second.compare(PlantMessage::DEVICE) != 0) {

        QString openStr;
        if (theMsg.getValue(PlantMessage::OPEN).compare("") == 0) {
            OpenMsg::parse(theMsg, openStr);
        } else {
            openStr = theMsg.getValue(PlantMessage::OPEN);
        }
        PlantMessage currMsg = theMsg;
        if (!FEED_BACK_FRAMES.contains(openStr)) {
            OpenMsg::parse(openStr, currMsg);
            QString where = currMsg.getValue(PlantMessage::WHERE);
            if(where.compare(GENERALE) != 0) {
                where = where + "#4#" + m_f422Status->getI3() + m_f422Status->getI4();
                currMsg.setValue(PlantMessage::WHERE, where);
                QString tmp;
                OpenMsg::parse(currMsg, tmp);
                currMsg.setValue(PlantMessage::OPEN, tmp);
            }
        }
        qDebug() << className() << "Message for Gateway" ;
        emit ReleaseMessage(currMsg, theErr);
    }
}
