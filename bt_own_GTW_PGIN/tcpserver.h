#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QString>
#include <syserror.h>

class TcpServer : public QObject
{
    Q_OBJECT
public:
    TcpServer();
    ~TcpServer();

    void init(SysError &sysErr);

    enum ChannelType {
        CMD_Channel = 0,
        MONITOR_Channel =1,
        SUPERVISOR_Channel = 2,
        REQUEST_Channel = 3,
        Unknown_Channel = -1
    };

private:
    static int MAXCONNECTION;
    static int OPEN_PORT;
    static QString OPEN_MONITOR;
    static QString OPEN_SUPERVISOR;
    static QString OPEN_CMD;
    static QString OPEN_REQUEST;

    QTcpServer *m_tcpSckSrv;
    QMap<QString, QTcpSocket *> m_clients;
    QMap<int, ChannelType> m_clientsType;
    ChannelType m_channelType;
    QString m_msg;

public:
    static QString OPEN_ACK;
    static QString OPEN_NACK;

public slots:
    void acceptConnection();
    void startRead();
    void clientDisconnected();
    void cmdRead();
    bool write(const QString value);
    bool write(const QString value, QString key);

signals:
    void sendCMD(QString value, QString key);
    
};

#endif // TCPSERVER_H
