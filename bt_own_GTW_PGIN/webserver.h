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
