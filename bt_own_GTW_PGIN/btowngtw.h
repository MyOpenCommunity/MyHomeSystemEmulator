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
