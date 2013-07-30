#include "tcpserver.h"
#include <QString>
#include <plantmessage.h>
#include <syserror.h>
#include <QStringList>

int TcpServer::MAXCONNECTION = 10;
int TcpServer::OPEN_PORT = 20000;
QString TcpServer::OPEN_ACK = "*#*1##";
QString TcpServer::OPEN_NACK = "*#*0##";
QString TcpServer::OPEN_MONITOR = "*99*1##";
QString TcpServer::OPEN_REQUEST = "*99*9##";
QString TcpServer::OPEN_SUPERVISOR = "*99*10##";
QString TcpServer::OPEN_CMD = "*99*0##";
static QString TYPE = "TYPE";


TcpServer::TcpServer()
{
    qDebug() << "Create a Server TCP";
    m_channelType = Unknown_Channel;
    m_tcpSckSrv = new QTcpServer(this);
}

TcpServer::~TcpServer() {
    if (m_tcpSckSrv->isListening())
    {
        qDebug() << "Close Server TCP";
        m_tcpSckSrv->close();
    }
}

void TcpServer::init(SysError &sysErr){
    qDebug() << "TcpServer initialize";
    this->setProperty(TYPE.toStdString().c_str(), PlantMessage::TCP);
    m_tcpSckSrv->setMaxPendingConnections(MAXCONNECTION);
    if (!m_tcpSckSrv->listen(QHostAddress::Any, OPEN_PORT)) {
        qDebug()<< "Unable to start the server: " << m_tcpSckSrv->errorString();
        QString msgErr = "Unable to start the server: " + m_tcpSckSrv->errorString() + " (" + QString::number(OPEN_PORT) + ")";
        sysErr = SysError(SysError::TCP_ERROR, msgErr);
        return;
    }
    QObject::connect(m_tcpSckSrv, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
    qDebug() << "Server PORT: " << m_tcpSckSrv->serverPort() << " | Server Address: " << m_tcpSckSrv->serverAddress();

}

void TcpServer::acceptConnection()
{
    qDebug()<< "acceptHandler, have new pending connecctions: ";
    QTcpSocket *client = m_tcpSckSrv->nextPendingConnection();
    m_clients.insert(QString::number(client->socketDescriptor()), client);

    connect(client, SIGNAL(readyRead()), this, SLOT(startRead()));
    connect(client, SIGNAL(disconnected()), this, SLOT(clientDisconnected()));

    qDebug() << "Send open ACK: " << OPEN_ACK;
    client->write(OPEN_ACK.toStdString().c_str());
}

void TcpServer::startRead() {
    QTcpSocket *client = dynamic_cast<QTcpSocket *>(sender());
    qDebug() << "startRead:: Receive message from client: " << client->socketDescriptor();

    char buffer[1024] = {0};
    qDebug() << "Byte to be read: " << client->bytesAvailable();
    int byteRead = client->bytesAvailable();
    client->read(buffer, client->bytesAvailable());
    qDebug() << "READ: " << buffer;

    for (int index = 0; index < byteRead; index++) {
        m_msg = m_msg.append(buffer[index]);
        qDebug() << "Messege read: " << m_msg;
        if(m_msg.contains("##")) {
            qDebug() << "msg: " << m_msg ;
            qDebug() << "Check channel connection";
            QString msgReceive = m_msg;
            if(msgReceive.compare(OPEN_CMD) == 0) {
                m_clientsType.insert(client->socketDescriptor(), CMD_Channel);
                m_channelType = CMD_Channel;
                qDebug() << "Client want open a CMD Channel";
            } else if (msgReceive.compare(OPEN_MONITOR) == 0){
                m_clientsType.insert(client->socketDescriptor(), MONITOR_Channel);
                m_channelType = MONITOR_Channel;
                qDebug() << "Client want open a MONITOR Channel";
            } else if (msgReceive.compare(OPEN_SUPERVISOR) == 0){
                m_clientsType.insert(client->socketDescriptor(), SUPERVISOR_Channel);
                m_channelType = SUPERVISOR_Channel;
                qDebug() << "Client want open a SUPERVISOR Channel";
            } else if (msgReceive.compare(OPEN_REQUEST) == 0){
                m_clientsType.insert(client->socketDescriptor(), REQUEST_Channel);
                m_channelType = REQUEST_Channel;
                qDebug() << "Client want open a REQUEST Channel";
            } else {
                qDebug() << "READ: " << m_msg;
                emit sendCMD(m_msg, QString::number(client->socketDescriptor()));
                m_msg.clear();
                return;
            }
            qDebug() << "Send open ACK: " << OPEN_ACK;
            client->write(OPEN_ACK.toStdString().c_str());
            m_msg.clear();
        }
    }
}

void TcpServer::clientDisconnected(){
    QTcpSocket *client = dynamic_cast<QTcpSocket *>(sender());
    qDebug() << "Remove Client: " << client->peerAddress() << "on Port: " << client->peerPort();
    m_clientsType.remove(client->socketDescriptor());
    m_clients.remove(m_clients.key(client));
}

void TcpServer::cmdRead(){
    QTcpSocket *client = dynamic_cast<QTcpSocket *>(sender());
    qDebug() << "cmdRead:: Receive command from Client" << client->socketDescriptor();
    char buffer[1024] = {0};
    client->read(buffer, client->bytesAvailable());
    qDebug() << "READ buffer : " << buffer;
    QString messages(buffer);
    QStringList msgSplitted = messages.split("##", QString::SkipEmptyParts);

     qDebug() << "msgSplitted length : " << msgSplitted.length();

    foreach (QString msg, msgSplitted) {
        msg.append("##");
        if(!(msg.compare(OPEN_CMD) == 0 && msg.compare(OPEN_MONITOR) == 0)) {
            qDebug() << "READ: " << msg;
            emit sendCMD(msg, QString::number(client->socketDescriptor()));
            //client->write(OPEN_ACK.toStdString().c_str());
        }
    }
}

bool TcpServer::write(const QString value){

    foreach (QTcpSocket *client, m_clients) {
        if ((m_clientsType[client->socketDescriptor()] == MONITOR_Channel))
        {
            qDebug() << "Send Message To client: " << client->socketDescriptor() << " - " << value.toStdString().c_str();
            client->write(value.toStdString().c_str());
        }
    }

    return (true);
}

bool TcpServer::write(const QString value, QString key){

    qDebug() << "Send Message To client: " << key << " - " << value.toStdString().c_str();
    if (m_clients.contains(key))
        m_clients[key]->write(value.toStdString().c_str());
    else {
        qDebug() << "Client: " << key << " not found";
    }

    return (true);
}

