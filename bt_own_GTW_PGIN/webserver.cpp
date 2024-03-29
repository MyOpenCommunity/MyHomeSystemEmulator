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

#include <webserver.h>
#include <QString>
#include <QFile>
#include <plantmessage.h>

int WebServer::MAXCONNECTION = 10;
//int WebServer::WEB_SERVER_PORT = 10000;

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
    m_port = -1;
}

WebServer::~WebServer() {
    if (m_tcpSckSrv->isListening())
    {
        qDebug() << "Close HTTP Server TCP";
        m_tcpSckSrv->close();
    }
}

void WebServer::init(QSharedPointer<GwtStatus> gtwStatus, SysError &sysErr){
    qDebug() << "HTTP Server initialized";

    m_gtwStatus = gtwStatus;
    m_port = gtwStatus->httpPort();

    if (m_port == -1) {
        return;
    }

    m_tcpSckSrv->setMaxPendingConnections(MAXCONNECTION);
    if (m_tcpSckSrv->isListening()) {
        qDebug() << "Server is already connected on TCP port: " << m_tcpSckSrv->serverPort();
        qDebug() << "Close current connection";
        m_tcpSckSrv->close();
    }

    m_channelType = Unknown_Channel;
    this->setProperty(TYPE.toStdString().c_str(), PlantMessage::HTTP);
    qDebug() << "Property type: " << this->property(TYPE.toStdString().c_str()).toString();

    if (!m_tcpSckSrv->listen(QHostAddress::Any, m_port)) {
        qDebug()<< "Unable to start the server: " << m_tcpSckSrv->errorString();
        QString msgErr = "Unable to start the server: " + m_tcpSckSrv->errorString() + " (" + QString::number(m_port) + ")";
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
    if (client != NULL) {

        m_clients.insert(QString::number(client->socketDescriptor()), client);

        connect(client, SIGNAL(readyRead()), this, SLOT(startRead()));
        connect(client, SIGNAL(disconnected()), this, SLOT(clientDisconnected()));
    } else {
        qDebug()<< "acceptHandler, have no new pending connecctions: ";
    }

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
