#ifndef BTOWNGTW_H
#define BTOWNGTW_H

#include <plantgateway.h>
#include <QSocketNotifier>
#include <QList>
#include <tcpserver.h>
#include <webserver.h>
#include <plantmessage.h>
#include <plantnode.h>
#include <QDomDocument>
#include <qmyserver.h>
#include <websecureserver.h>
#include <gwtstatus.h>
#include <QSharedPointer>

class Btowngtw :  public PlantGateway
{
    Q_OBJECT
public:

    static QString const className() { return "Btowngtw"; }

    Btowngtw();
    ~Btowngtw();

    void                                            setID(const QString& theID);
    const QString&                                  getID(void);

    void                                            HandleMessage(const PlantMessage& theSrt, SysError& theErr);

    unsigned int                                    executeCmd(CMD_PRIO thePrio, const QByteArray& theCmd, QByteArray& theResult, SysError& theErr);

    bool                                            cancelCmd(unsigned int theCmdId);

    void                                            init(SysError& theErr);

    void                                            registerEventHnd(EventHandler* theHnd, SysError& theErr);

    void                                            unRegisterEventHnd(EventHandler* theHnd);

    bool                                            setApl(const QString& theAddress, bool theVal);

    QList<bool>                                     getApl(const QString& theAddress);

    bool                                            execCommand(PlantMessage &theMsg);

    QDomDocument                                    save(SysError &theErr);

private:

    QByteArray                                      CreateHTMLResp();

private:
    QString                         m_gtwID;
    EventHandler*                   m_theHnd;
    QSharedPointer<TcpServer>       m_tcpSckSrv;
    QSharedPointer<WebServer>       m_webSckSrv;
    QSharedPointer<GwtStatus>       m_gtwStatus;
    QMap<int, QString>              m_timeoutMap;
    QList<int>                      m_msgSent;
    QSharedPointer<WebSecureServer> m_webSecSckSrv;

public slots:
    void                                             receiveCMD(QString value, QString key);
    void                                             receiveHttpCMD(QString value, QString key);
    void                                             ReceiveMessage(PlantMessage &theMsg, SysError& theErr);
    void                                             stopTimer();

};

#endif // BTOWNGTW_H
