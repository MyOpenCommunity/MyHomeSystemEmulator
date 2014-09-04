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

#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QString>
#include <gwtstatus.h>
#include <QSharedPointer>
#include <syserror.h>

class WebServer : public QObject
{
    Q_OBJECT
public:
    WebServer();
    ~WebServer();

    void init(QSharedPointer<GwtStatus> gtwStatus, SysError &sysErr);

    enum ChannelType {
        CMD_Channel = 0,
        MONITOR_Channel =1,
        Unknown_Channel = -1
    };


private:
    static int MAXCONNECTION;
    static int WEB_SERVER_PORT;

    QTcpServer *m_tcpSckSrv;
    QMap<QString, QTcpSocket *> m_clients;
    ChannelType m_channelType;
    QTcpSocket *m_client;
    QSharedPointer<GwtStatus> m_gtwStatus;
    int m_port;


public slots:
    void acceptConnection();
    void startRead();
    void clientDisconnected();
    void cmdRead();
    bool write(const QByteArray value);

signals:
    void sendCMD(QString value, QString key);

};

#endif // WEBSERVER_H
