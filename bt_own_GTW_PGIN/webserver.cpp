#include <webserver.h>
#include <QString>
#include <QFile>
#include <plantmessage.h>

int WebServer::MAXCONNECTION = 10;
int WebServer::WEB_SERVER_PORT = 8000;

static QString REQ_IMAGE = "*6*A*0000##";
static QString HTTP_OK = "";
static QString HTTP_SERVER = "";
static QString HTTP_CONNECTION = "Connection: close";
static QString HTTP_CONTENT_LENGHT = "";
static QString HTTP_CONTENT_TYPE = "";
static QString USER = "telecamera.php";
static QString PASSWORD = "?CAM_PASSWD=";
static QString TYPE = "TYPE";


WebServer::WebServer() {
    qDebug() << "Create a Server HTTP";
    m_tcpSckSrv = new QTcpServer(this);
}

WebServer::WebServer(QSharedPointer<GwtStatus> gtwStatus) {
    qDebug() << "Create a Server HTTP";
    m_gtwStatus = gtwStatus;
    m_tcpSckSrv = new QTcpServer(this);
}

WebServer::~WebServer() {
    if (m_tcpSckSrv->isListening())
    {
        qDebug() << "Close HTTP Server TCP";
        m_tcpSckSrv->close();
    }
}

void WebServer::init(SysError &sysErr){
    qDebug() << "HTTP Server initialized";

    m_channelType = Unknown_Channel;
    this->setProperty(TYPE.toStdString().c_str(), PlantMessage::HTTP);
    qDebug() << "Property type: " << this->property(TYPE.toStdString().c_str()).toString();

    m_tcpSckSrv->setMaxPendingConnections(MAXCONNECTION);
    if (!m_tcpSckSrv->listen(QHostAddress::Any, WEB_SERVER_PORT)) {
        qDebug()<< "Unable to start the server: " << m_tcpSckSrv->errorString();
        QString msgErr = "Unable to start the server: " + m_tcpSckSrv->errorString() + " (" + QString::number(WEB_SERVER_PORT) + ")";
        sysErr = SysError(SysError::HTTP_ERROR, msgErr);
        return;
    }
    QObject::connect(m_tcpSckSrv, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
    qDebug() << "Server PORT: " << m_tcpSckSrv->serverPort() << " | Server Address: " << m_tcpSckSrv->serverAddress();

}

void WebServer::acceptConnection()
{
    qDebug()<< "acceptHandler, have new pending connecctions: ";
    QTcpSocket *client = m_tcpSckSrv->nextPendingConnection();
    m_clients.insert(QString::number(client->socketDescriptor()), client);

    connect(client, SIGNAL(readyRead()), this, SLOT(startRead()));
    connect(client, SIGNAL(disconnected()), this, SLOT(clientDisconnected()));

}

void WebServer::startRead() {
    m_client = dynamic_cast<QTcpSocket *>(sender());
    qDebug() << "HTTP: Receive message from client: " << m_client->socketDescriptor();


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

void WebServer::clientDisconnected(){
    QTcpSocket *client = dynamic_cast<QTcpSocket *>(sender());
    qDebug() << "Remove Client: " << client->peerAddress() << "on Port: " << client->peerPort();
    m_clients.remove(m_clients.key(client));
}

void WebServer::cmdRead(){
    QTcpSocket *client = dynamic_cast<QTcpSocket *>(sender());
    qDebug() << "Receive command from Client" << client->socketDescriptor();
    char buffer[1024] = {0};
    client->read(buffer, client->bytesAvailable());
    qDebug() << "READ: " << buffer;
    emit sendCMD(QString(buffer), QString::number(client->socketDescriptor()));
    //client->write(OPEN_ACK.toStdString().c_str());
}

bool WebServer::write(const QByteArray value){

    qDebug()<<"----------- HTTP ---------------------";
    QString pippo(value);
    qDebug()<<pippo;
    qDebug()<<"--------------------------------------";

    m_client->write(value);
    return (true);
}
