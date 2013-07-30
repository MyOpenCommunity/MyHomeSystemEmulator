#include <btowngtw.h>
#include <openmsg.h>
#include <syserror.h>
#include <gwtxmlserializer.h>
#include <qmyserver.h>
#include <websecureserver.h>

static const QString RESOURCE_FOLDER = ":/files/resources/";
static const QString RESOURCE_XML = "F454.xml";
static const QString HTTP_BLACK_IMAGE = "blackCam.png";
static const QString TIMER_ID = "timerId";
static const int TIMEOUT = 500;
static const int MAX_TIMER_ID = 1000000;
static const QString TYPE = "TYPE";
static const QString FRAME_TYPE = "FRAME_TYPE";

Btowngtw::Btowngtw()    {
    m_gtwStatus = QSharedPointer<GwtStatus>(new GwtStatus("C1"));
    m_tcpSckSrv = QSharedPointer<TcpServer>(new TcpServer());
    connect(m_tcpSckSrv.data(), SIGNAL(sendCMD(QString, QString)), this, SLOT(receiveCMD(QString, QString)));
    m_webSckSrv = QSharedPointer<WebServer>(new WebServer(m_gtwStatus));
    connect(m_webSckSrv.data(), SIGNAL(sendCMD(QString, QString)), this, SLOT(receiveCMD(QString, QString)));

    m_webSecSckSrv = QSharedPointer<WebSecureServer>(new WebSecureServer(m_gtwStatus));
    connect(m_webSecSckSrv.data(), SIGNAL(sendCMD(QString, QString)), this, SLOT(receiveCMD(QString, QString)));
}

Btowngtw::~Btowngtw()   {
    qDebug() << "~Btowngtw ";
}

void Btowngtw::init(SysError& theErr) {
    m_tcpSckSrv->init(theErr);
    if (theErr.isError())
        return;
    m_webSckSrv->init(theErr);
    if (theErr.isError())
        return;
    m_webSecSckSrv->init(theErr);
    if (theErr.isError())
        return;
}

void    Btowngtw::setID(const QString& theID)  {
    m_gtwID = theID;
}

const QString&  Btowngtw::getID(void)  {
    return(m_gtwID);
}

void    Btowngtw::HandleMessage(const PlantMessage& /*theSrt*/, SysError& /*theErr*/)    {

}

unsigned int Btowngtw::executeCmd(CMD_PRIO /*thePrio*/, const QByteArray& theCmd, QByteArray& theResult, SysError& theErr)  {

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
    case 4: // setup parameter request
    {
        QDomDocument xml;
        GWTXmlSerializer::serialize(m_gtwID, xml, m_gtwStatus, theErr);
        theResult = xml.toByteArray();
    }
        break;
    case 5:
    {
        QDomDocument xml;
        xml.setContent(list2[1]);
        GWTXmlSerializer::deserialize(xml, m_gtwStatus, theErr);
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

bool    Btowngtw::cancelCmd(unsigned int /*theCmdId*/) {
    return(false);
}

void    Btowngtw::receiveCMD(QString value, QString key) {
    qDebug() << m_gtwID << " Receive command from Gateway: " << value;
    PlantMessage myMsg;
    SysError theSysError;
    if(OpenMsg::parse(value, myMsg)) {

        QTimer *timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(stopTimer()));
        timer->setSingleShot(true);
        int msgID = qrand() % ((MAX_TIMER_ID + 1) - 0) + 0;
        m_timeoutMap.insert(msgID, key);
        timer->setProperty(TIMER_ID.toStdString().c_str(), msgID);
        timer->setProperty(TYPE.toStdString().c_str(), sender()->property(TYPE.toStdString().c_str()).toString());
        timer->setProperty(FRAME_TYPE.toStdString().c_str(), OpenMsg::getFrameType(value));
        qDebug() << " timer getProperty: " << sender()->property(TYPE.toStdString().c_str()).toString() << " " << timer->property(TYPE.toStdString().c_str()).toString();
        timer->start(TIMEOUT);

        qDebug() << m_gtwID << " Parsing ok, emit sendCommand Signal";
        myMsg.setSourceAddr(QPair<QString, QString>(PlantMessage::EXTERNAL, key), true);
        myMsg.setDestAddr(QPair<QString, QString>(PlantMessage::INTERNAL, PlantMessage::DEVICE), true);
        myMsg.setType(sender()->property(TYPE.toStdString().c_str()).toString());
        myMsg.setId(msgID);
        emit ReleaseMessage(myMsg, theSysError);
    }
    else {
        qDebug() << m_gtwID << " Receive command from Gateway is WRONG FORMAT";
    }
}

void    Btowngtw::receiveHttpCMD(QString value, QString /*key*/) {
    qDebug() << m_gtwID << " Receive command from Gateway: " << value;
    PlantMessage myMsg;
    SysError theSysError;


}

void  Btowngtw::ReceiveMessage(PlantMessage& theMsg, SysError& /*theErr*/)
{    
    qDebug() << m_gtwID << " GWT: Receive Message from Bus" ;
    QPair<QString, QString>  destAddr = theMsg.getDestAddr();
    if ((destAddr.first.compare(PlantMessage::INTERNAL) == 0 && destAddr.second.compare(PlantMessage::GATEWAY) == 0) ||
            destAddr.first.compare(PlantMessage::EXTERNAL) == 0) {
        qDebug() << m_gtwID << " GWT: Send response" ;
        if(theMsg.getType() == PlantMessage::FEED_BACK && m_timeoutMap.contains(m_timeoutMap.key(destAddr.second))) {
            qDebug() << m_gtwID << " GWT: Send FEED_BACK" ;
            m_tcpSckSrv->write(theMsg.getValue(PlantMessage::OPEN), destAddr.second);

            if (m_timeoutMap.contains(m_timeoutMap.key(destAddr.second))) {
                qDebug() << m_gtwID << " GWT: remove " << destAddr.second << " timeout" ;
                qDebug() << m_gtwID << " GWT: remove Key" << m_timeoutMap.key(destAddr.second) << " timeout" ;
                m_timeoutMap.remove(m_timeoutMap.key(destAddr.second));
            }
        } else if (theMsg.getType() == PlantMessage::GEN_MESSAGE) {
            qDebug() << m_gtwID << " GWT: Send GEN_MESSAGE: " << destAddr.second.compare(PlantMessage::GATEWAY) ;
            if(destAddr.first.compare(PlantMessage::INTERNAL) == 0 && destAddr.second.compare(PlantMessage::GATEWAY) == 0) {
                m_tcpSckSrv->write(theMsg.getValue(PlantMessage::OPEN));
            } else {
                m_tcpSckSrv->write(theMsg.getValue(PlantMessage::OPEN), destAddr.second);
                if (m_timeoutMap.contains(m_timeoutMap.key(destAddr.second))) {
                    qDebug() << m_gtwID << " GWT: remove " << destAddr.second << " timeout" ;
                    qDebug() << m_gtwID << " GWT: remove Key" << m_timeoutMap.key(destAddr.second) << " timeout" ;
                    m_timeoutMap.remove(m_timeoutMap.key(destAddr.second));
                }
            }
        } else if (theMsg.getType() == PlantMessage::EVENT) {
            QString openMsg;
            OpenMsg::parse(theMsg, openMsg);
            if(destAddr.first.compare(PlantMessage::INTERNAL) == 0 && destAddr.second.compare(PlantMessage::GATEWAY) == 0) {
                m_tcpSckSrv->write(openMsg);
            } else {
                m_tcpSckSrv->write(openMsg, destAddr.second);
            }
        } else if(theMsg.getType() == PlantMessage::HTTP_BACK) {
            qDebug() << "######################## HTTP SEND ....";
            m_webSckSrv->write(theMsg.getValueByteArray(PlantMessage::HTTP));
        } else if(theMsg.getType() == PlantMessage::HTTPS_BACK) {
            qDebug() << "######################## HTTPS SEND ....";
            m_webSecSckSrv->write(theMsg.getValueByteArray(PlantMessage::HTTP));
        } else if ((theMsg.getType() == PlantMessage::COMMAND) && !m_msgSent.contains(theMsg.getId())){
            m_msgSent.append(theMsg.getId());
            QString openMsg;
            OpenMsg::parse(theMsg, openMsg);
            m_tcpSckSrv->write(openMsg);
        } else if(m_msgSent.contains(theMsg.getId())) {
            qDebug() << " GWT: message " << theMsg.getId() << " already sent" ;
        }
    } else {
        qDebug()<< m_gtwID << " Discart Message, it is not for me";
    }

}

void Btowngtw::registerEventHnd(EventHandler* theHnd, SysError& /*theErr*/)
{
    m_theHnd = theHnd;
}

void Btowngtw::unRegisterEventHnd(EventHandler* /*theHnd*/)
{

}

bool Btowngtw::setApl(const QString& /*theAddress*/, bool /*theVal*/)
{
    return false;
}

QList<bool> Btowngtw::getApl(const QString& /*theAddress*/)
{
    QList<bool> res;
    return res;
}

bool Btowngtw::execCommand(PlantMessage &/*theMsg*/)
{
    return false;
}

QDomDocument Btowngtw::save(SysError &theErr) {

    QDomDocument xml;

    GWTXmlSerializer::serialize(m_gtwID, xml, m_gtwStatus, theErr);

    return(xml);
}

void Btowngtw::stopTimer() {
    int msgId = sender()->property(TIMER_ID.toStdString().c_str()).toInt();
    QString type = sender()->property(TYPE.toStdString().c_str()).toString();
    QString frame_type = sender()->property(FRAME_TYPE.toStdString().c_str()).toString();
    qDebug() << "Btowngtw  stopTimer... Timer id: " << msgId << " Socket Type: " << type;
    bool rtn = m_msgSent.removeOne(msgId);
    qDebug() << "Remove m_msgSent element: " << msgId << " " << rtn << " remain: " << m_msgSent.length();

    if(frame_type.compare(PlantMessage::STATUS) == 0) {
        if (m_timeoutMap.contains(msgId)) {
            m_tcpSckSrv->write(TcpServer::OPEN_ACK, m_timeoutMap[msgId]);
            qDebug() << m_gtwID << " GWT: remove Key" << msgId ;
            m_timeoutMap.remove(msgId);
        }
    } else {
        if (m_timeoutMap.contains(msgId)) {
            if (type.compare(PlantMessage::HTTP) == 0) {
                qDebug() << "Gtw: send HTTP_NACK";
                m_webSckSrv->write(CreateHTMLResp());
                m_timeoutMap.remove(msgId);
            } else if (type.compare(PlantMessage::HTTPS) == 0) {
                qDebug() << "Gtw: send HTTP_NACK";
                m_webSecSckSrv->write(CreateHTMLResp());
                m_timeoutMap.remove(msgId);
            } else {
                m_tcpSckSrv->write(TcpServer::OPEN_NACK, m_timeoutMap[msgId]);
                qDebug() << m_gtwID << " GWT: remove Key" << msgId ;
                m_timeoutMap.remove(msgId);
            }
        }
    }
}

QByteArray Btowngtw::CreateHTMLResp() {

    QString filename = RESOURCE_FOLDER + HTTP_BLACK_IMAGE;

    qDebug() << "LOAD IMAGE " << filename;
    QByteArray block;
    QFile inFile( filename );
    if( ! inFile.open( QIODevice::ReadOnly ) )
    {
        qDebug() << "HostThread: Cant find file " << filename;
        block.append("HostThread: Cant find file ");
    } else {
        block =  inFile.readAll();
    }

    QByteArray HTLMResponse;

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
