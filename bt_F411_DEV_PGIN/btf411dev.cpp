#include <btf411dev.h>
#include <QFile>
#include <QDebug>
#include <QStringList>
#include <syserror.h>
#include <eventhandler.h>
#include <QDir>
#include <plantmessage.h>
#include <QStack>
#include <QPair>
#include <QDomDocument>
#include <f411xmlserializer.h>
#include <plantmgr.h>
#include <openmsg.h>


static const QString LUCI = "1";
static const QString STATO_LUCI = "#1";
static const QString RESOURCE_FOLDER = ":/files/resources/";
static const QString MSG_ACK = "*#*1##";

BtF411_dev::BtF411_dev(){

}

BtF411_dev::~BtF411_dev() {
    qDebug() << m_deviceID << " ~btf411_dev ";
}

void BtF411_dev::setResoucesFileName(QString fileName) {

    m_ResourcesFileName = RESOURCE_FOLDER + fileName;

}

void    BtF411_dev::setID(const QString& theID)  {
    m_deviceID = theID;
}

const QString&  BtF411_dev::getID(void)  {
    return(m_deviceID);
}

void    BtF411_dev::HandleMessage(const PlantMessage& /*theSrt*/, SysError& /*theErr*/)    {
}

void    BtF411_dev::setSuperior(const QSharedPointer<PlantNode>& /*theSuperior*/)    {

}

void    BtF411_dev::attachChild(const QSharedPointer<PlantNode>& /*theMsgHndObjRef*/)    {

}

unsigned int BtF411_dev::executeCmd(CMD_PRIO /*thePrio*/, const QByteArray& theCmd, QByteArray& theResult, SysError& theErr)  {

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

        qDebug() << m_deviceID << " LOAD XML " << m_ResourcesFileName;

        QFile inFile( m_ResourcesFileName );
        if( ! inFile.open( QIODevice::ReadOnly ) )
        {
            qDebug() << m_deviceID << " HostThread: Cant find file " << m_ResourcesFileName;
            return -1;
        }

        QByteArray block;

        block =  inFile.readAll();

        qDebug() << m_deviceID << "FileSIze: " << inFile.size();
        qDebug() << m_deviceID << "Blocksize:" << block.size();

        theResult = block;
    }
        break;
    case 3: //  CLICKED...
    {
        qDebug() << m_deviceID << " CLICKED " + list2[1] << " ID " << this->getID();

        PlantMessage msg;
        msg.setValue(PlantMessage::WHO, PlantMessage::LIGHT);
        msg.setType(PlantMessage::EVENT);

        QString qStr;
        QString btn = list2[1];
        if (m_ledAddress.contains(btn)) {
            if (m_ledAddress[btn]->isConfigured()) {
                m_ledAddress[btn]->setStatus(m_ledAddress[btn]->getstatus() == 0 ? 1 : 0);
                msg.setValue(PlantMessage::WHAT, QString::number(m_ledAddress[btn]->getstatus()));
                msg.setValue(PlantMessage::WHERE, m_ledAddress[btn]->getA() + m_ledAddress[btn]->getPl() );
                msg.setSourceAddr(QPair<QString, QString>(PlantMessage::INTERNAL, PlantMessage::DEVICE), true);
                msg.setDestAddr(QPair<QString, QString>(PlantMessage::INTERNAL, PlantMessage::GATEWAY), true);
                qStr = btn + ":" + QString::number(m_ledAddress[btn]->getstatus());
            } else {
                theErr = SysError(SysError::F411_SLOT_NOT_CONFIGURED, "Slot is unconfigured");
                return(0);
            }
        }

        QByteArray qBary;
        qBary.append(qStr);

        m_theHnd->handleEvent(EventHandler::NORMAL,  qBary);

        emit ReleaseMessage(msg, theErr);

    }
        break;
    case 4: // setup parameter request
    {
        QDomDocument xml;
        F411xmlserializer::serialize(m_deviceID, m_ledAddress, this->getClassName(), m_mode, this->getBusName(), xml, theErr);
        theResult = xml.toByteArray();
    }
        break;
    case 5:
    {
        QDomDocument xml;
        xml.setContent(list2[1]);
        QString busName;
        F411xmlserializer::deserialize(xml, m_ledAddress, m_mode, busName, theErr);

        if(!theErr.isError()) {

            //            if (getBusName().compare(busName) != 0) {
            // If Device is set over an other bus, change the connection between SIGNAl and SLOT
            qDebug() << "old BUS: " << getBusName();
            disconnect(PlantMgr::getInstance().getPlant(PlantMgr::getInstance().getCurrPlantName())->getBus(getBusName()).data(),
                       SIGNAL(ReleaseMessage(PlantMessage&, SysError&)), this, SLOT(ReceiveMessage(PlantMessage&, SysError&)));
            disconnect(this, SIGNAL(ReleaseMessage(PlantMessage&, SysError&)),
                       PlantMgr::getInstance().getPlant(PlantMgr::getInstance().getCurrPlantName())->getBus(getBusName()).data(),
                       SLOT(ReceiveMessage(PlantMessage&, SysError&)));

            this->setBusName(busName);

            qDebug() << "current BUS: " << getBusName();
            connect(PlantMgr::getInstance().getPlant(PlantMgr::getInstance().getCurrPlantName())->getBus(getBusName()).data(),
                    SIGNAL(ReleaseMessage(PlantMessage&, SysError&)), this, SLOT(ReceiveMessage(PlantMessage&, SysError&)));
            connect(this, SIGNAL(ReleaseMessage(PlantMessage&, SysError&)),
                    PlantMgr::getInstance().getPlant(PlantMgr::getInstance().getCurrPlantName())->getBus(getBusName()).data(),
                    SLOT(ReceiveMessage(PlantMessage&, SysError&)));
            //            }

        }

        foreach(QSharedPointer<LedStatus> led, m_ledAddress) {
            if(led->isConfigured())
            {
                QString qStr;
                qStr = led->getId() + ":" + QString::number(led->getstatus());
                QByteArray qBary;
                qBary.append(qStr);
                m_theHnd->handleEvent(EventHandler::NORMAL,  qBary);
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

bool    BtF411_dev::cancelCmd(unsigned int /*theCmdId*/) {
    return(false);
}


void    BtF411_dev::registerEventHnd(EventHandler* theHnd, SysError& /*theErr*/) {
    m_theHnd = theHnd;


}

void    BtF411_dev::unRegisterEventHnd(EventHandler* /*theHnd*/) {

}

bool    BtF411_dev::setApl(QSharedPointer<LedStatus> led, bool theVal)   {

    if (led->isConfigured()) {

        led->setStatus(theVal ? 1 : 0);

        QString qStr = led->getId() + ":" + QString::number(led->getstatus());

        QByteArray qBary;
        qBary.append(qStr);

        m_theHnd->handleEvent(EventHandler::NORMAL,  qBary);
        return(true);
    }

    return(false);
}

QList<bool>    BtF411_dev::getApl(const QString& /*theAddress*/)    {
    //TODO
    return(QList<bool>());
}

void  BtF411_dev::ReceiveMessage( PlantMessage &theMsg, SysError &/*theErr*/)
{
    qDebug() << className() << " " << m_deviceID << ": Receive Message from Bus" ;
    QPair<QString, QString>  destAddr = theMsg.getDestAddr();
    if (destAddr.first.compare(PlantMessage::INTERNAL) == 0 && destAddr.second.compare(PlantMessage::DEVICE) == 0)
    {
        qDebug() << PlantMessage::WHO << theMsg.getValue(PlantMessage::WHO);
        qDebug() << PlantMessage::WHAT << theMsg.getValue(PlantMessage::WHAT);
        qDebug() << PlantMessage::WHERE << theMsg.getValue(PlantMessage::WHERE);

        if ( !execCommand(theMsg) ) {
            qDebug() << getClassName() << " " << m_deviceID << ": Discart Message, it is not for me" ;
        }
    } else {
        qDebug() << getClassName() << " " << m_deviceID << ": Discart Message, it is not for me" ;
    }
}

bool BtF411_dev::findLed(QString address,QSharedPointer<LedStatus> &led) {
    QMap<QString, QSharedPointer <LedStatus> >::iterator  iter = m_ledAddress.begin();
    while (iter != m_ledAddress.end()) {
        qDebug() << iter.value()->getId();
        if(address.compare(QString(iter.value()->getA() + iter.value()->getPl())) == 0) {
            led = iter.value();
            return(true);
        }
        iter++;
    }
    return (false);
}

bool BtF411_dev::execCommand(PlantMessage &theMsg) {
    SysError theErr;
    if((theMsg.getValues().contains(PlantMessage::WHO) && theMsg.getValue(PlantMessage::WHO).compare(LUCI) == 0) &&
            theMsg.getValues().contains(PlantMessage::WHAT) &&
            theMsg.getValues().contains(PlantMessage::WHERE)) {

        bool valid;
        int value = theMsg.getValue(PlantMessage::WHERE).toInt(&valid, 10);
        if (valid) {
            if ((value == 0) && (m_mode.compare(LedStatus::PUL) != 0)) {
                sendACK(theMsg);
                foreach (QSharedPointer<LedStatus> led, m_ledAddress) {
                    if (setApl(led, (theMsg.getValue(PlantMessage::WHAT).compare("1") == 0))) {

                    }
                }
                PlantMessage rspMessage;
                rspMessage.setId(theMsg.getId());
                rspMessage.setValues(theMsg.getValues());
                rspMessage.setDestAddr(QPair<QString, QString>(PlantMessage::INTERNAL, PlantMessage::GATEWAY));
                rspMessage.setSourceAddr(QPair<QString, QString>(PlantMessage::INTERNAL, PlantMessage::DEVICE), true);
                rspMessage.setType(PlantMessage::COMMAND);
                emit ReleaseMessage(rspMessage, theErr);
            } else if ((value >= LedStatus::getMinA() && value <= LedStatus::getMaxA()) && (m_mode.compare(LedStatus::PUL) != 0)) {
                sendACK(theMsg);
                foreach (QSharedPointer<LedStatus> led, m_ledAddress) {
                    if(led->getA().compare(theMsg.getValue(PlantMessage::WHERE)) == 0) {
                        if (setApl(led, (theMsg.getValue(PlantMessage::WHAT).compare("1") == 0))) {

                        }
                    }
                }
                PlantMessage rspMessage;
                rspMessage.setId(theMsg.getId());
                rspMessage.setValues(theMsg.getValues());
                rspMessage.setDestAddr(QPair<QString, QString>(PlantMessage::INTERNAL, PlantMessage::GATEWAY));
                rspMessage.setSourceAddr(QPair<QString, QString>(PlantMessage::INTERNAL, PlantMessage::DEVICE), true);
                rspMessage.setType(PlantMessage::COMMAND);
                emit ReleaseMessage(rspMessage, theErr);
            } else if (value >= ((LedStatus::getMinA() * 10) + LedStatus::getMinPL()) && value <= ((LedStatus::getMaxA()*10) + LedStatus::getMaxPL())) {
                QSharedPointer<LedStatus> led;
                if(findLed(theMsg.getValue(PlantMessage::WHERE), led))
                {
                    sendACK(theMsg);
                    setApl(led, (theMsg.getValue(PlantMessage::WHAT).compare("1") == 0));
                    PlantMessage rspMessage;
                    rspMessage.setId(theMsg.getId());
                    rspMessage.setValues(theMsg.getValues());
                    rspMessage.setDestAddr(QPair<QString, QString>(PlantMessage::INTERNAL, PlantMessage::GATEWAY));
                    rspMessage.setSourceAddr(QPair<QString, QString>(PlantMessage::INTERNAL, PlantMessage::DEVICE), true);
                    rspMessage.setType(PlantMessage::COMMAND);
                    emit ReleaseMessage(rspMessage, theErr);

                }
            } else {
                return(false);
            }
        } else if(theMsg.getValue(PlantMessage::WHERE).at(0) == '#') {
            bool valid;
            int value = theMsg.getValue(PlantMessage::WHERE).remove('#').toInt(&valid, 10);
            if (value >= LedStatus::getMinGRP() && value <= LedStatus::getMaxGRP()){
                sendACK(theMsg);
                foreach (QSharedPointer<LedStatus> led, m_ledAddress) {
                    qDebug() << led->getGroup().toInt();
                    if(led->getGroup().toInt() == value) {
                        if (setApl(led, (theMsg.getValue(PlantMessage::WHAT).compare("1") == 0))) {

                        }
                    }
                }
                PlantMessage rspMessage;
                rspMessage.setId(theMsg.getId());
                rspMessage.setValues(theMsg.getValues());
                rspMessage.setDestAddr(QPair<QString, QString>(PlantMessage::INTERNAL, PlantMessage::GATEWAY));
                rspMessage.setSourceAddr(QPair<QString, QString>(PlantMessage::INTERNAL, PlantMessage::DEVICE), true);
                rspMessage.setType(PlantMessage::COMMAND);
                emit ReleaseMessage(rspMessage, theErr);
            }
        }

        return(true);

    // Status request lighting
    } else if ((theMsg.getValues().contains(PlantMessage::WHO) && theMsg.getValue(PlantMessage::WHO).compare(STATO_LUCI) == 0) &&
               theMsg.getValues().contains(PlantMessage::WHERE)) {
        bool valid;
        int value = theMsg.getValue(PlantMessage::WHERE).toInt(&valid, 10);
        if (valid) {
            if ((value == 0) && (m_mode.compare(LedStatus::PUL) != 0)) {
                qDebug() << className() << " " << m_deviceID << ": Request status frame Generale" ;
                foreach (QSharedPointer<LedStatus> led, m_ledAddress) {
                    if (led->isConfigured()) {
                        PlantMessage rspMessage;
                        rspMessage.setId(theMsg.getId());
                        rspMessage.setValue(PlantMessage::WHO, LUCI);
                        rspMessage.setValue(PlantMessage::WHAT, QString::number(led->getstatus()));
                        rspMessage.setValue(PlantMessage::WHERE, led->getA() + led->getPl());

                        // Retrive open message
                        QString openString;
                        OpenMsg::parse(rspMessage, openString);
                        rspMessage.setValue(PlantMessage::OPEN, openString);

                        rspMessage.setDestAddr(QPair<QString, QString>(theMsg.getSourceAddr().first, theMsg.getSourceAddr().second), true);
                        rspMessage.setSourceAddr(QPair<QString, QString>(PlantMessage::INTERNAL, PlantMessage::DEVICE), true);
                        rspMessage.setType(PlantMessage::EVENT);
                        emit ReleaseMessage(rspMessage, theErr);

                        rspMessage.setDestAddr(QPair<QString, QString>(PlantMessage::INTERNAL, PlantMessage::GATEWAY), true);
                        emit ReleaseMessage(rspMessage, theErr);
                    }
                }

            } else if ((value >= LedStatus::getMinA() && value <= LedStatus::getMaxA()) && (m_mode.compare(LedStatus::PUL) != 0)) {
                qDebug() << className() << " " << m_deviceID << ": Request status frame Ambiente" ;
                foreach (QSharedPointer<LedStatus> led, m_ledAddress) {
                    if(led->getA().compare(theMsg.getValue(PlantMessage::WHERE)) == 0) {
                        if (led->isConfigured()) {
                            PlantMessage rspMessage;
                            rspMessage.setId(theMsg.getId());
                            rspMessage.setValue(PlantMessage::WHO, LUCI);
                            rspMessage.setValue(PlantMessage::WHAT, QString::number(led->getstatus()));
                            rspMessage.setValue(PlantMessage::WHERE, led->getA() + led->getPl());
                            rspMessage.setDestAddr(QPair<QString, QString>(theMsg.getSourceAddr().first, theMsg.getSourceAddr().second), true);
                            rspMessage.setSourceAddr(QPair<QString, QString>(PlantMessage::INTERNAL, PlantMessage::DEVICE), true);
                            // Retrive open message
                            QString openString;
                            OpenMsg::parse(rspMessage, openString);
                            rspMessage.setValue(PlantMessage::OPEN, openString);

                            rspMessage.setType(PlantMessage::EVENT);
                            emit ReleaseMessage(rspMessage, theErr);

                            rspMessage.setDestAddr(QPair<QString, QString>(PlantMessage::INTERNAL, PlantMessage::GATEWAY), true);
                            emit ReleaseMessage(rspMessage, theErr);
                        }
                    }
                }
            } else if (value >= ((LedStatus::getMinA() * 10) + LedStatus::getMinPL()) && value <= ((LedStatus::getMaxA()*10) + LedStatus::getMaxPL())) {
                qDebug() << className() << " " << m_deviceID << ": Request status frame PL" ;
                QSharedPointer<LedStatus> led;
                if(findLed(theMsg.getValue(PlantMessage::WHERE), led))
                {
                    PlantMessage rspMessage;
                    rspMessage.setId(theMsg.getId());
                    rspMessage.setValue(PlantMessage::WHO, LUCI);
                    rspMessage.setValue(PlantMessage::WHAT, QString::number(led->getstatus()));
                    rspMessage.setValue(PlantMessage::WHERE, theMsg.getValue(PlantMessage::WHERE));
                    rspMessage.setDestAddr(QPair<QString, QString>(theMsg.getSourceAddr().first, theMsg.getSourceAddr().second), true);
                    rspMessage.setSourceAddr(QPair<QString, QString>(PlantMessage::INTERNAL, PlantMessage::DEVICE), true);
                    // Retrive open message
                    QString openString;
                    OpenMsg::parse(rspMessage, openString);
                    rspMessage.setValue(PlantMessage::OPEN, openString);

                    rspMessage.setType(PlantMessage::EVENT);
                    emit ReleaseMessage(rspMessage, theErr);

                    rspMessage.setDestAddr(QPair<QString, QString>(PlantMessage::INTERNAL, PlantMessage::GATEWAY), true);
                    emit ReleaseMessage(rspMessage, theErr);
                }
            } else {
                return(false);
            }
        } else if(theMsg.getValue(PlantMessage::WHERE).at(0) == '#') {
            bool valid;
            int value = theMsg.getValue(PlantMessage::WHERE).remove('#').toInt(&valid, 10);
            if (value >= LedStatus::getMinGRP() && value <= LedStatus::getMaxGRP()){
                qDebug() << className() << " " << m_deviceID << ": Request status frame Group" ;
                foreach (QSharedPointer<LedStatus> led, m_ledAddress) {
                    qDebug() << led->getGroup().toInt();
                    if(led->getGroup().toInt() == value) {
                        if (led->isConfigured()) {
                            PlantMessage rspMessage;
                            rspMessage.setId(theMsg.getId());
                            rspMessage.setValue(PlantMessage::WHO, LUCI);
                            rspMessage.setValue(PlantMessage::WHAT, QString::number(led->getstatus()));
                            rspMessage.setValue(PlantMessage::WHERE, led->getA() + led->getPl());
                            rspMessage.setDestAddr(QPair<QString, QString>(theMsg.getSourceAddr().first, theMsg.getSourceAddr().second), true);
                            rspMessage.setSourceAddr(QPair<QString, QString>(PlantMessage::INTERNAL, PlantMessage::DEVICE), true);
                            // Retrive open message
                            QString openString;
                            OpenMsg::parse(rspMessage, openString);
                            rspMessage.setValue(PlantMessage::OPEN, openString);

                            rspMessage.setType(PlantMessage::EVENT);
                            emit ReleaseMessage(rspMessage, theErr);

                            rspMessage.setDestAddr(QPair<QString, QString>(PlantMessage::INTERNAL, PlantMessage::GATEWAY), true);
                            emit ReleaseMessage(rspMessage, theErr);
                        }
                    }
                }
            }
        }

        return(true);
    } else {
        return(false);
    }
}

QDomDocument BtF411_dev::save(SysError &theErr) {

    qDebug() << BtF411_dev::className() << " - save - " << m_deviceID;
    QDomDocument xml;
    F411xmlserializer::serialize(m_deviceID, m_ledAddress, this->getClassName(), m_mode, this->getBusName(), xml, theErr);

    return(xml);

}

QString BtF411_dev::getModeTypeValues() {
    QString rtnValue = LedStatus::MODETYPE[0];
    for (int i = 1; i < LedStatus::MODETYPE.size(); i++)
        rtnValue = rtnValue + "," + LedStatus::MODETYPE[i];

    return rtnValue;
}

void BtF411_dev::sendACK(PlantMessage &theMsg){
    SysError theErr;
    PlantMessage rspACKmsg;
    rspACKmsg.setValue(PlantMessage::OPEN, MSG_ACK);
    rspACKmsg.setSourceAddr(QPair<QString, QString>(PlantMessage::INTERNAL, PlantMessage::DEVICE), true);
    rspACKmsg.setDestAddr(QPair<QString, QString>(theMsg.getSourceAddr().first, theMsg.getSourceAddr().second), true);
    rspACKmsg.setType(PlantMessage::FEED_BACK);
    emit ReleaseMessage(rspACKmsg, theErr);
}
