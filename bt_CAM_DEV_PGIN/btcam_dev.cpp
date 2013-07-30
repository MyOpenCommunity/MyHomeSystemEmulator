#include "btcam_dev.h"
#include <QDebug>
#include <QStringList>
#include <QString>
#include <QFile>
#include <QDir>
#include <plantmessage.h>
#include <cam_xmlserializer.h>


static const QString RESOURCE_FOLDER = ":/files/resources/";
static const QString RESOURCE_XML = "CAM.xml";
static const int VIDEO_DOOR_ENTRY_SYS = 6;

static const QString MSG_ACK = "*#*1##";

static const int CAM_ACTIVE = 1;
static const int CAM_INACTIVE = 0;

static const int CAMERA_TIMEOUT = 60000;

Btcam_dev::Btcam_dev()
{
    m_camStatus = QSharedPointer<CameraStatus>(new CameraStatus("C1"));
    m_imageCounter = 0;
}


Btcam_dev::~Btcam_dev() {
    qDebug() << m_deviceID << " ~Btcam_dev ";
}


void    Btcam_dev::setID(const QString& theID)  {
    m_deviceID = theID;
}

const QString&  Btcam_dev::getID(void)  {
    return(m_deviceID);
}

void    Btcam_dev::HandleMessage(const PlantMessage& /*theSrt*/, SysError& /*theErr*/)    {
}

void    Btcam_dev::setSuperior(const QSharedPointer<PlantNode>& /*theSuperior*/)    {

}

void    Btcam_dev::attachChild(const QSharedPointer<PlantNode>& /*theMsgHndObjRef*/)    {

}

unsigned int Btcam_dev::executeCmd(CMD_PRIO /*thePrio*/, const QByteArray& theCmd, QByteArray& theResult, SysError& theErr)  {

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
        CAM_XmlSerializer::serialize(m_deviceID, xml, m_camStatus, theErr);
        theResult = xml.toByteArray();
    }
        break;
    case 5:
    {
        QDomDocument xml;
        xml.setContent(list2[1]);
        CAM_XmlSerializer::deserialize(xml, m_camStatus, theErr);
        if(m_camStatus->isConfigured())
        {
            QString qStr;
            qStr = m_camStatus->getId() + ":" + QString::number(m_camStatus->getstatus());
            QByteArray qBary;
            qBary.append(qStr);
            m_theHnd->handleEvent(EventHandler::NORMAL,  qBary);
        } else {
            QString qStr;
            qStr = m_camStatus->getId() + ":2";
            QByteArray qBary;
            qBary.append(qStr);
            m_theHnd->handleEvent(EventHandler::NORMAL,  qBary);
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

bool    Btcam_dev::cancelCmd(unsigned int /*theCmdId*/) {
    return(false);
}


void    Btcam_dev::registerEventHnd(EventHandler* theHnd, SysError& /*theErr*/) {
    m_theHnd = theHnd;
}

void    Btcam_dev::unRegisterEventHnd(EventHandler* /*theHnd*/) {

}

void    Btcam_dev::ReceiveMessage(PlantMessage &theMsg, SysError &theErr)
{
    qDebug() << className() << " " << m_deviceID << ": Receive Message from Bus" ;
    QPair<QString, QString>  destAddr = theMsg.getDestAddr();
    if (destAddr.first.compare(PlantMessage::INTERNAL) == 0 && destAddr.second.compare(PlantMessage::DEVICE) == 0)
    {
        qDebug() << PlantMessage::WHO << theMsg.getValue(PlantMessage::WHO);
        qDebug() << PlantMessage::WHAT << theMsg.getValue(PlantMessage::WHAT);
        qDebug() << PlantMessage::WHERE << theMsg.getValue(PlantMessage::WHERE);

        if ( !execCommand(theMsg, theErr) ) {
            qDebug() << className() << " " << m_deviceID << ": Discart Message, it is not for me" ;
        } else {
            PlantMessage rspACKmsg;
            rspACKmsg.setValue(PlantMessage::OPEN, MSG_ACK);
            rspACKmsg.setSourceAddr(QPair<QString, QString>(PlantMessage::INTERNAL, PlantMessage::DEVICE), true);
            rspACKmsg.setDestAddr(QPair<QString, QString>(theMsg.getSourceAddr().first, theMsg.getSourceAddr().second), true);
            rspACKmsg.setType(PlantMessage::FEED_BACK);
            emit ReleaseMessage(rspACKmsg, theErr);
        }
    } else {
        qDebug() << className() << " " << m_deviceID << ": Discart Message, it is not for me" ;
    }
}

bool Btcam_dev::execCommand(PlantMessage &theMsg, SysError &theErr)
{
    if( theMsg.getValues().contains(PlantMessage::WHO) && (theMsg.getValues()[PlantMessage::WHO]).toInt() == VIDEO_DOOR_ENTRY_SYS &&
            theMsg.getValues().contains(PlantMessage::WHAT))
    {
        bool valid;
        if (m_camStatus->isConfigured())
        {
            bool ok;
            int valore = (theMsg.getValues()[PlantMessage::WHAT]).toUInt(&ok,16);
            if (ok)
                switch(valore)
                {
                case 0:
                {
                    if (theMsg.getValues().contains(PlantMessage::WHERE))
                    {
                        if(m_camStatus->isConfigured())
                        {
                            int destAPl = theMsg.getValue(PlantMessage::WHERE).toInt(&valid, 10);
                            if (valid && (m_camStatus->getaddrCam().toInt() + 4000) == destAPl)
                            {
                                qDebug() << "Activate the cam ... ";
                                m_camStatus->setStatus(CAM_ACTIVE);

                                QString qStr;
                                qStr = m_camStatus->getId() + ":" + QString::number(m_camStatus->getstatus());
                                QByteArray qBary;
                                qBary.append(qStr);
                                m_theHnd->handleEvent(EventHandler::NORMAL,  qBary);

                                m_camereTimer = new QTimer(this);
                                connect(m_camereTimer, SIGNAL(timeout()), this, SLOT(stopTimer()));
                                m_camereTimer->setSingleShot(true);
                                m_camereTimer->start(CAMERA_TIMEOUT);
                            } else {
                                if (m_camStatus->getstatus() == CAM_ACTIVE)
                                {
                                    qDebug() << "Deactivate the cam ... (Another CAM has been activated)";
                                    m_camStatus->setStatus(CAM_INACTIVE);
                                    QString qStr;
                                    qStr = m_camStatus->getId() + ":" + QString::number(m_camStatus->getstatus());
                                    QByteArray qBary;
                                    qBary.append(qStr);
                                    m_theHnd->handleEvent(EventHandler::NORMAL,  qBary);
                                    return (false); // per evitare di avere un messaggio di ACK ...
                                }
                            }
                        }
                    } else {
                        qDebug() << "The Message is'nt for me";
                    }
                    break;
                }
                case 9:
                {
                    qDebug() << "Deactivate the cam ... ";
                    m_camStatus->setStatus(CAM_INACTIVE);
                    QString qStr;
                    qStr = m_camStatus->getId() + ":" + QString::number(m_camStatus->getstatus());
                    QByteArray qBary;
                    qBary.append(qStr);
                    m_theHnd->handleEvent(EventHandler::NORMAL,  qBary);
                    break;
                }
                case 10:
                {
                    if (m_camStatus->getstatus() == CAM_ACTIVE)
                    {
                        qDebug() << "Webserver asked for an image ...";

                        PlantMessage rspACKmsg;
                        QByteArray htmlMsg = CreateHTMLResp(theErr);
                        if (theErr.isError())
                            return false;
                        rspACKmsg.setValue(PlantMessage::HTTP, htmlMsg);
                        rspACKmsg.setSourceAddr(QPair<QString, QString>(PlantMessage::INTERNAL, PlantMessage::DEVICE), true);
                        rspACKmsg.setDestAddr(QPair<QString, QString>(theMsg.getSourceAddr().first, theMsg.getSourceAddr().second), true);
                        rspACKmsg.setType(theMsg.getType());
                        emit ReleaseMessage(rspACKmsg, theErr);

                        // Devo mandare l'immagine ....
                    }
                    return (false); // per evitare di avere un messaggio di ACK ...
                    break;
                }
                    break;
                default:
                    break;
                }
            return true;
        }
    }
    return (false);
}

QByteArray Btcam_dev::CreateHTMLResp(SysError &theErr) {

    QByteArray HTLMResponse;

    QDir dir(m_camStatus->getDir());

    if (m_camStatus->getDir().isEmpty() || !dir.exists())
    {
        theErr = SysError(SysError::CAM_IMAGE_DIR_NOT_FOUND, "CAM_IMAGE_DIR_NOT_FOUND, Camera: " + getID());
        return HTLMResponse;
    }

    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    dir.setSorting(QDir::Size | QDir::Reversed);
    QStringList filters;
    filters << "*.png";
    dir.setNameFilters(filters);
    QFileInfoList list = dir.entryInfoList();

    if (list.size() < 1)
    {
        theErr = SysError(SysError::CAM_NO_IMAGES_IN_DIR, "CAM_NO_IMAGES_IN_DIR, Camera: " + getID());
        return HTLMResponse;
    }

    //    for (int i = 0; i < list.size(); ++i) {
    //        QFileInfo fileInfo = list.at(i);
    //        qDebug() << qPrintable(QString("%1 %2").arg(fileInfo.size(), 10).arg(fileInfo.fileName()));
    //    }

    m_imageCounter++;
    QString imageSelected = list.at(m_imageCounter % list.size()).fileName();
    QString filename = m_camStatus->getDir() + QDir::separator() + imageSelected;

    qDebug() << "Loading image " << filename;
    QByteArray block;
    QFile inFile( filename );
    if( ! inFile.open( QIODevice::ReadOnly ) )
    {
        qDebug() << "HostThread: Cant find file " << filename;
        block.append("HostThread: Cant find file ");
    } else {
        block =  inFile.readAll();
    }

//    QByteArray HTLMResponse;

    //  Prepare the answer header
    HTLMResponse.append("HTTP/1.0 200 OK\r\n");
    HTLMResponse.append("Server: grabtofile\r\n");
    HTLMResponse.append("Connection: close\r\n");

    //  Evalutes the content length
    QString sizeLine = "Content-length: " + QString::number(block.size()) + "\r\n";
    HTLMResponse.append(sizeLine);
    HTLMResponse.append("Content-Type: image/jpeg\r\n");

    //  Send an empty line as required by HTTP protocol to detach the content
    HTLMResponse.append("\r\n");

    //  Send the content
    HTLMResponse.append(block);

    //  Send an empty line as required by HTTP to terminate  the answer
    HTLMResponse.append("\r\n");

    return HTLMResponse;
}


QDomDocument Btcam_dev::save(SysError &theErr) {

    QDomDocument xml;
    CAM_XmlSerializer::serialize(m_deviceID, xml, m_camStatus, theErr);

    return(xml);

}

bool Btcam_dev::setMode(QString /*mode*/) {
    return(false);
}

void Btcam_dev::stopTimer() {
    qDebug()<< "StopTimer";
    m_camStatus->setStatus(CAM_INACTIVE);
    QString qStr;
    qStr = m_camStatus->getId() + ":" + QString::number(m_camStatus->getstatus());
    QByteArray qBary;
    qBary.append(qStr);
    m_theHnd->handleEvent(EventHandler::NORMAL,  qBary);
}
