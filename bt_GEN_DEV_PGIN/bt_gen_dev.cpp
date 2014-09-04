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
#include <bt_gen_dev.h>
#include <gen_xmlserializer.h>
#include <genmsgworker.h>
#include <QThread>
#include <plantmgr.h>


static const QString LUCI = "1";
static const QString RESOURCE_FOLDER = ":/files/resources/";
static const QString RESOURCE_XML = "GEN.xml";
static const QString MSG_ACK = "*#*1##";

BtGEN_dev::BtGEN_dev(){


}

BtGEN_dev::~BtGEN_dev() {
    qDebug() << "~BtGEN_dev ";
}

void    BtGEN_dev::setID(const QString& theID)  {
    m_deviceID = theID;
}

const QString&  BtGEN_dev::getID(void)  {
    return(m_deviceID);
}

void    BtGEN_dev::HandleMessage(const PlantMessage& theSrt, SysError& theErr)    {
}

void    BtGEN_dev::setSuperior(const QSharedPointer<PlantNode>& theSuperior)    {

}

void    BtGEN_dev::attachChild(const QSharedPointer<PlantNode>& theMsgHndObjRef)    {

}

unsigned int BtGEN_dev::executeCmd(CMD_PRIO thePrio, const QByteArray& theCmd, QByteArray& theResult, SysError& theErr)  {

    // Da sistemare con il file di risorse...
    QString rec(theCmd);
    QStringList list2 = rec.split("#", QString::SkipEmptyParts);
    int scelta = list2[0].toInt();
    switch(scelta)
    {
    case 1: // LOAD IMAGE   1#file_name
    {
        QString filename = RESOURCE_FOLDER + list2[1];
        qDebug() << "LOAD IMAGE " << filename;
        QFile inFile( filename );
        if( ! inFile.open( QIODevice::ReadOnly ) )
        {
            qDebug() << "HostThread: Cant find file " << filename;
            return -1;
        }

        QByteArray block;
        block =  inFile.readAll();

        qDebug() << "FileSIze: " << inFile.size();
        qDebug() << "Blocksize:" << block.size();

        theResult = block;
    }
        break;
    case 2: // LOAD XML FILE 2#
    {
        QString str = QDir::currentPath();


        qDebug() << "Current path = " << str;

        QString filename = RESOURCE_FOLDER + RESOURCE_XML;
        qDebug() << "LOAD XML " << filename;
        QFile inFile( filename );
        if( ! inFile.open( QIODevice::ReadOnly ) )
        {
            qDebug() << "HostThread: Cant find file " << filename;
            return -1;
        }

        QByteArray block;

        block =  inFile.readAll();

        qDebug() << "FileSIze: " << inFile.size();
        qDebug() << "Blocksize:" << block.size();

        theResult = block;
    }
        break;
    case 3: //  CLICKED...
    {
        qDebug() << "CLICKED " + list2[1] << " ID " << this->getID();

        QString theFrame = list2[1];
        if (m_scenariosMap.contains(theFrame)) {

            PlantMessage theMsg;
            GenMsgWorker *work = new GenMsgWorker(m_theHnd, m_scenariosMap[theFrame], theMsg);
            connect(work, SIGNAL(ReleaseResponse(QObject *)), this, SLOT(ReceiveResponce(QObject *)));

            work->start();

        } else {
            theErr = SysError(SysError::GEN_TRIGGER_MSG_NOT_FOUND, "GEN_TRIGGER_MSG_NOT_FOUND");
        }

    }
        break;
    case 4: // foreword parameter request
    {
        qDebug() << "foreword parameter request";
        QDomDocument xml;
        GEN_xmlserializer::serialize(m_deviceID, this->getBusName(), m_xmlSrcFile, xml, theErr);
        theResult = xml.toByteArray();
    }
        break;
    case 5:
    {
        qDebug() << "setup parameter request";
        QDomDocument xml;
        xml.setContent(list2[1]);
        QString busName;
        GEN_xmlserializer::deserialize(xml, m_xmlSrcFile, busName, theErr);

        if(!theErr.isError()) {

            if (getBusName().compare(busName) != 0) {
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
            }

            if(!m_xmlSrcFile.isEmpty() && m_xmlSrcFile.compare("") != 0)
            {

                QString filename = m_xmlSrcFile;
                qDebug() << "LOAD XML " << filename;
                QFile inFile( filename );
                if( ! inFile.open( QIODevice::ReadOnly ) )
                {
                    qDebug() << "HostThread: Cant find file " << filename;
                    return -1;
                }
                QDomDocument scenariosXml;
                if (scenariosXml.setContent(inFile.readAll())) {
                    GEN_xmlserializer::loadScenarios(scenariosXml, m_scenariosMap, theErr);
                } else {
                    qDebug() << this->className() << " ERROR, xml is not valid";
                    theErr = SysError(SysError::GEN_XML_WAS_WRONG_FORMAT, "GEN_XML_WAS_WRONG_FORMAT");
                }
                inFile.close();
            }
        }
    }
        break;
    default:
    {
        qDebug() << "NOT HANDLED";
    }
        break;
    }

    return(0);
}

bool    BtGEN_dev::cancelCmd(unsigned int theCmdId) {
    return(false);
}


void    BtGEN_dev::registerEventHnd(EventHandler* theHnd, SysError& theErr) {
    m_theHnd = theHnd;


}

void    BtGEN_dev::unRegisterEventHnd(EventHandler* theHnd) {

}

void  BtGEN_dev::ReceiveMessage( PlantMessage &theMsg, SysError &theErr)
{
    qDebug() << className() << ": Receive Message from Bus" ;
    QPair<QString, QString>  destAddr = theMsg.getDestAddr();
    if (destAddr.first.compare(PlantMessage::INTERNAL) == 0 && destAddr.second.compare(PlantMessage::DEVICE) == 0)
    {
        qDebug() << PlantMessage::WHO << theMsg.getValue(PlantMessage::WHO);
        qDebug() << PlantMessage::WHAT << theMsg.getValue(PlantMessage::WHAT);
        qDebug() << PlantMessage::WHERE << theMsg.getValue(PlantMessage::WHERE);

        if ( !execCommand(theMsg, theErr) ) {
            qDebug() << className() << ": Discart Message, it is not for me" ;
        }
    } else {
        qDebug() << className() << ": Discart Message, it is not for me" ;
    }
}

bool BtGEN_dev::execCommand(PlantMessage &theMsg, SysError &theErr) {

    qDebug() << "Generic Device receive message: ";
    QString theFrame = theMsg.getValue(PlantMessage::OPEN);
    if (m_scenariosMap.contains(theFrame)) {

        GenMsgWorker *work = new GenMsgWorker(m_theHnd, m_scenariosMap[theFrame], theMsg);
        connect(work, SIGNAL(ReleaseResponse(QObject *)), this, SLOT(ReceiveResponce(QObject *)));

        work->start();

    } else {
        theErr = SysError(SysError::GEN_TRIGGER_MSG_NOT_FOUND, "GEN_TRIGGER_MSG_NOT_FOUND");
        return(false);
    }
    return true;
}

QDomDocument BtGEN_dev::save(SysError &theErr) {

    QDomDocument xml;
    GEN_xmlserializer::serialize(m_deviceID, this->getBusName(), m_xmlSrcFile, xml, theErr);

    return(xml);
}

void BtGEN_dev::ReceiveResponce(QObject *msg) {


    PlantResponse *myyy = dynamic_cast<PlantResponse *> (msg);
    PlantMessage plantMsg = myyy->getRspMessage();
    SysError err = myyy->getCurrentErr();
    qDebug() << "Send ReceiveResponce: " << myyy->getRspMessage().getValue(PlantMessage::OPEN);
    emit ReleaseMessage(plantMsg, err);
}
