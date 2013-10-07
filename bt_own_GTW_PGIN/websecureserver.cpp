#include "websecureserver.h"
#include <QString>
#include <QFile>
#include <qmyserver.h>
#include <QSslSocket>
#include <plantmessage.h>
#include <syserror.h>

int WebSecureServer::MAXCONNECTION = 10;

static QString REQ_IMAGE = "*6*A*0000##";
static QString HTTP_OK = "";
static QString HTTP_SERVER = "";
static QString HTTP_CONNECTION = "Connection: close";
static QString HTTP_CONTENT_LENGHT = "";
static QString HTTP_CONTENT_TYPE = "";
static QString TYPE = "TYPE";
static QString USER = "telecamera.php";
static QString PASSWORD = "?CAM_PASSWD=";

WebSecureServer::WebSecureServer()
{
    qDebug() << "Create a Server HTTPS";
    m_channelType = Unknown_Channel;
    m_tcpSckSrv = new QMyServer();
    m_port = -1;
}

WebSecureServer::~WebSecureServer() {
    qDebug() << "Close HTTPS Server TCP";
    if (m_tcpSckSrv->isListening())
        m_tcpSckSrv->close();
}

void WebSecureServer::init(QSharedPointer<GwtStatus> gtwStatus, SysError &sysErr){
    qDebug() << "HTTPS Server initialized";
    m_gtwStatus = gtwStatus;
    m_port = gtwStatus->httpsPort();

    if (m_port == -1) {
        return;
    }

    m_tcpSckSrv->setMaxPendingConnections(MAXCONNECTION);
    if (m_tcpSckSrv->isListening()) {
        qDebug() << "Server is already connected on TCP port: " << m_tcpSckSrv->serverPort();
        qDebug() << "Close current connection";
        m_tcpSckSrv->close();
    }

    this->setProperty(TYPE.toStdString().c_str(), PlantMessage::HTTPS);

    m_tcpSckSrv->setMaxPendingConnections(MAXCONNECTION);
    if (!m_tcpSckSrv->listen(QHostAddress::Any, m_port)) {
        qDebug()<< "Unable to start the server: " << m_tcpSckSrv->errorString();
        QString msgErr = "Unable to start the server: " + m_tcpSckSrv->errorString() + " (" + QString::number(m_port) + ")";
        sysErr = SysError(SysError::HTTPS_ERROR, msgErr);
        return;
    }
    QObject::connect(m_tcpSckSrv, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
    qDebug() << "Server PORT: " << m_tcpSckSrv->serverPort() << " | Server Address: " << m_tcpSckSrv->serverAddress();
}

void WebSecureServer::acceptConnection()
{
    //    qDebug()<< "acceptHandler, have new pending connecctions: ";
    QSslSocket *client = m_tcpSckSrv->socket;
    m_clients.insert(QString::number(client->socketDescriptor()), client);

    connect(client, SIGNAL(readyRead()), this, SLOT(startRead()));
    connect(client, SIGNAL(disconnected()), this, SLOT(clientDisconnected()));
}

void WebSecureServer::startRead() {
    m_client = dynamic_cast<QSslSocket *>(sender());
    qDebug() << "HTTPS: Receive message from client: " << m_client->socketDescriptor();


    char buffer[2048] = {0};
    m_client->read(buffer, m_client->bytesAvailable());
    qDebug() << "READ: " << buffer;
    QString buf = QString(buffer);
    QString user = buf.mid(buf.indexOf(USER), USER.length());
    QString pwd;
    if(buf.contains(PASSWORD))
        pwd = buf.mid(buf.indexOf(PASSWORD) + 12, buf.indexOf(" HTTP") - (buf.indexOf(PASSWORD) + 12));

    qDebug() << "user: " << user;
    qDebug() << "pwd: " << pwd;
    qDebug() << "isEmpty" << m_gtwStatus->getPwd().isEmpty() << " " << m_gtwStatus->getPwd();
    if(user.compare(USER) == 0 && (m_gtwStatus->getPwd().isEmpty() || m_gtwStatus->getPwd().compare(pwd) == 0))
        emit sendCMD(REQ_IMAGE, QString::number(m_client->socketDescriptor()));
    else
        qDebug() << "Sender Error, address not found: " << buffer;
}

void WebSecureServer::clientDisconnected(){
    QSslSocket *client = dynamic_cast<QSslSocket *>(sender());
    qDebug() << "Remove Client: " << client->peerAddress() << "on Port: " << client->peerPort();
    m_clients.remove(m_clients.key(client));
}

void WebSecureServer::cmdRead(){
    QTcpSocket *client = dynamic_cast<QTcpSocket *>(sender());
    qDebug() << "Receive command from Client" << client->socketDescriptor();
    char buffer[1024] = {0};
    client->read(buffer, client->bytesAvailable());
    qDebug() << "READ: " << buffer;
    emit sendCMD(QString(buffer), QString::number(client->socketDescriptor()));
    //client->write(OPEN_ACK.toStdString().c_str());
}

bool WebSecureServer::write(const QByteArray value){

    qDebug()<<"----------- HTTPS --------------------";
    QString pippo(value);
    qDebug()<<pippo;
    qDebug()<<"--------------------------------------";

    m_client->write(value);
    return (true);
}
