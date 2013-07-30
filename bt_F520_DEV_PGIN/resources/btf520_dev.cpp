#include "btf520_dev.h"
#include <QStringList>
//#include <QDebug>
//#include <QFile>
#include <QDir>
#include <f520xmlserializer.h>
#include <plantmgr.h>
#include <plantmessage.h>
//#include <openmsg.h>

static const QString ENEGY = "18";
static const QString STATO_ENEGY = "#18";
static const QString MSG_ACK = "*#*1##";

static const QString RESOURCE_FOLDER = ":/files/resources/";
static const QString RESOURCE_XML = "F520.xml";
//static const QString BUS_PREFIX = "Bus_";
//static const QString LOCAL_BUS = "4";

//static const QString MSG_ACK = "*#*1##";
//static const QString MSG_NACK = "*#*0##";
//static const QString GENERALE = "0";

//const QList<QString> FEED_BACK_FRAMES = QList<QString>() << MSG_ACK << MSG_NACK;

static const int DAILY_TOTALIZER = 57;
static const int AVARAGE_MONTHLY = 58;
static const int MONTHLY_TOTALIZER = 59;

BtF520_dev::BtF520_dev()
{
    m_f520Status = QSharedPointer<F520Status>(new F520Status());
}

BtF520_dev::~BtF520_dev() {
    qDebug() << BtF520_dev::className() << " Destroy " << m_deviceID;
    PlantMgr::getInstance().getPlant(PlantMgr::getInstance().getCurrPlantName())->removeBusConn(m_localBus);
}

void    BtF520_dev::setID(const QString& theID)  {
    m_deviceID = theID;
}

const QString&  BtF520_dev::getID(void)  {
    return(m_deviceID);
}

void    BtF520_dev::HandleMessage(const PlantMessage& /*theSrt*/, SysError& /*theErr*/)    {
}

QDomDocument BtF520_dev::save(SysError &theErr) {

    QDomDocument xml;
    F520xmlserializer::serialize(m_deviceID, m_f520Status, xml, false, theErr);

    return(xml);

}

void    BtF520_dev::setSuperior(const QSharedPointer<PlantNode>& /*theSuperior*/)    {

}

void    BtF520_dev::attachChild(const QSharedPointer<PlantNode>& /*theMsgHndObjRef*/)    {

}

unsigned int BtF520_dev::executeCmd(CMD_PRIO /*thePrio*/, const QByteArray& theCmd, QByteArray& theResult, SysError& theErr)  {
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
        F520xmlserializer::serialize(m_deviceID, m_f520Status, xml, true, theErr);
        theResult = xml.toByteArray();
    }
        break;
    case 5:
    {
        QDomDocument xml;
        xml.setContent(list2[1]);
        F520xmlserializer::deserialize(xml, m_f520Status, theErr);

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

bool    BtF520_dev::cancelCmd(unsigned int /*theCmdId*/) {
    return(false);
}


void    BtF520_dev::registerEventHnd(EventHandler* theHnd, SysError& /*theErr*/) {
    m_theHnd = theHnd;


}

void    BtF520_dev::unRegisterEventHnd(EventHandler* /*theHnd*/) {

}

void    BtF520_dev::ReceiveMessage( PlantMessage &theMsg, SysError &theErr) {

    qDebug() << className() << " " << m_deviceID << ": Receive Message from Bus" ;
    QPair<QString, QString>  destAddr = theMsg.getDestAddr();
    if (destAddr.first.compare(PlantMessage::INTERNAL) == 0 && destAddr.second.compare(PlantMessage::DEVICE) == 0)
    {
        qDebug() << PlantMessage::WHO << theMsg.getValue(PlantMessage::WHO);
        qDebug() << PlantMessage::WHAT << theMsg.getValue(PlantMessage::WHAT);
        qDebug() << PlantMessage::WHERE << theMsg.getValue(PlantMessage::WHERE);

        if (((theMsg.getValues().contains(PlantMessage::WHO) && theMsg.getValue(PlantMessage::WHO).compare(ENEGY) == 0) ||
             (theMsg.getValues().contains(PlantMessage::WHO) && theMsg.getValue(PlantMessage::WHO).compare(STATO_ENEGY) == 0) ) &&
                theMsg.getValues().contains(PlantMessage::WHAT) &&
                theMsg.getValues().contains(PlantMessage::WHERE) &&
                !theMsg.getValue(PlantMessage::WHERE).compare(m_f520Status->getWhere())) {
            qDebug() << className() << " " << m_deviceID << ": It is an energy message" ;

            if (theMsg.getValues()[PlantMessage::WHO].startsWith("#"))
                requestFrame(theMsg);
            else
                commandFrame(theMsg);
        }

    } else {
        qDebug() << className() << " " << m_deviceID << ": Discart Message, it is not for me" ;
    }

}

void BtF520_dev::commandFrame(PlantMessage &theMsg) {
    qDebug() << className() << " " << m_deviceID << ": It is command frame" ;
    SysError theErr;

    QStringList whatValue = theMsg.getValue(PlantMessage::WHAT).split('#');


    switch (whatValue.at(0).toInt()) {
    case DAILY_TOTALIZER:
    {
        sendACK(theMsg);
        QStringList energyVal = F520xmlserializer::query(m_f520Status, whatValue.at(2), whatValue.at(1), theErr).split('.');
        PlantMessage rspMessage;
        rspMessage.setDestAddr(QPair<QString, QString>(PlantMessage::INTERNAL, PlantMessage::GATEWAY));
        rspMessage.setType(PlantMessage::EVENT);
        rspMessage.setValue(PlantMessage::WHO, STATO_ENEGY);
        rspMessage.setValue(PlantMessage::WHERE, theMsg.getValue(PlantMessage::WHERE));
        rspMessage.setSourceAddr(QPair<QString, QString>(PlantMessage::INTERNAL, PlantMessage::DEVICE), true);

        QString what = "511#" + whatValue.at(1) + "#" + whatValue.at(2);
        rspMessage.setValue(PlantMessage::DIMENSION, what);

        rspMessage.setId(theMsg.getId());

        int dayTotalUnit = 0;
        for(int index = 0; index < energyVal.length(); index++) {
            int dimIndex = 1;
            rspMessage.setValue(PlantMessage::DIM_VALUE + dimIndex, QString::number(index + 1));
            dimIndex++;
            rspMessage.setValue(PlantMessage::DIM_VALUE + dimIndex, energyVal.at(index));
            dimIndex++;
            dayTotalUnit = dayTotalUnit + energyVal.at(index).toInt();
            emit ReleaseMessage(rspMessage, theErr);
        }
        int dimIndex = 1;
        rspMessage.setValue(PlantMessage::DIM_VALUE + dimIndex, QString::number(25));
        dimIndex++;
        rspMessage.setValue(PlantMessage::DIM_VALUE + dimIndex, QString::number(dayTotalUnit));
        emit ReleaseMessage(rspMessage, theErr);
    }
        break;
    case MONTHLY_TOTALIZER:
    {
        sendACK(theMsg);
        QStringList energyVal = F520xmlserializer::query(m_f520Status, whatValue.at(1), theErr).split('.');
        PlantMessage rspMessage;
        rspMessage.setDestAddr(QPair<QString, QString>(PlantMessage::INTERNAL, PlantMessage::GATEWAY));
        rspMessage.setType(PlantMessage::EVENT);
        rspMessage.setValue(PlantMessage::WHO, STATO_ENEGY);
        rspMessage.setValue(PlantMessage::WHERE, theMsg.getValue(PlantMessage::WHERE));
        rspMessage.setSourceAddr(QPair<QString, QString>(PlantMessage::INTERNAL, PlantMessage::DEVICE), true);

        QString what = "513#" + whatValue.at(1);
        rspMessage.setValue(PlantMessage::DIMENSION, what);

        rspMessage.setId(theMsg.getId());


        for(int index = 0; index < energyVal.length(); index++) {
            int dimIndex = 1;
            rspMessage.setValue(PlantMessage::DIM_VALUE + dimIndex, QString::number(index + 1));
            dimIndex++;
            rspMessage.setValue(PlantMessage::DIM_VALUE + dimIndex, energyVal.at(index));
            dimIndex++;
            emit ReleaseMessage(rspMessage, theErr);
        }

    }
        break;
    case AVARAGE_MONTHLY:
    {
        sendACK(theMsg);
        QStringList energyVal = F520xmlserializer::avarageQuery(m_f520Status, whatValue.at(1), theErr).split('.');
        PlantMessage rspMessage;
        rspMessage.setDestAddr(QPair<QString, QString>(PlantMessage::INTERNAL, PlantMessage::GATEWAY));
        rspMessage.setType(PlantMessage::EVENT);
        rspMessage.setValue(PlantMessage::WHO, STATO_ENEGY);
        rspMessage.setValue(PlantMessage::WHERE, theMsg.getValue(PlantMessage::WHERE));
        rspMessage.setSourceAddr(QPair<QString, QString>(PlantMessage::INTERNAL, PlantMessage::DEVICE), true);

        QString what = "512#" + whatValue.at(1);
        rspMessage.setValue(PlantMessage::DIMENSION, what);

        rspMessage.setId(theMsg.getId());

        int dayTotalUnit = 0;
        for(int index = 0; index < energyVal.length(); index++) {
            int dimIndex = 1;
            rspMessage.setValue(PlantMessage::DIM_VALUE + dimIndex, QString::number(index + 1));
            dimIndex++;
            rspMessage.setValue(PlantMessage::DIM_VALUE + dimIndex, energyVal.at(index));
            dimIndex++;
            dayTotalUnit = dayTotalUnit + energyVal.at(index).toInt();
            emit ReleaseMessage(rspMessage, theErr);
        }

        int dimIndex = 1;
        rspMessage.setValue(PlantMessage::DIM_VALUE + dimIndex, QString::number(25));
        dimIndex++;
        rspMessage.setValue(PlantMessage::DIM_VALUE + dimIndex, QString::number(dayTotalUnit));
        emit ReleaseMessage(rspMessage, theErr);

    }
        break;
    default:
        break;
    }
}

void BtF520_dev::requestFrame(PlantMessage &theMsg) {
    qDebug() << className() << " " << m_deviceID << ": It is requestFrame" ;
}


void BtF520_dev::sendACK(PlantMessage &theMsg){
    SysError theErr;
    PlantMessage rspACKmsg;
    rspACKmsg.setValue(PlantMessage::OPEN, MSG_ACK);
    rspACKmsg.setSourceAddr(QPair<QString, QString>(PlantMessage::INTERNAL, PlantMessage::DEVICE), true);
    rspACKmsg.setDestAddr(QPair<QString, QString>(theMsg.getSourceAddr().first, theMsg.getSourceAddr().second), true);
    rspACKmsg.setType(PlantMessage::FEED_BACK);
    emit ReleaseMessage(rspACKmsg, theErr);
}
