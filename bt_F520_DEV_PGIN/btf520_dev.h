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

#ifndef BTF520_DEV_H
#define BTF520_DEV_H

#include <plantdevice.h>
#include <f520status.h>
//#include <f520resource.h>
#include <QObject>
#include <QTimer>

class BtF520_dev : public PlantDevice
{
public:
    static QString const className() { return "BtF520_dev"; }

    BtF520_dev();
    ~BtF520_dev();

    void                        setID(const QString& theID);

    const QString&              getID(void);

    void                        HandleMessage(const PlantMessage& theMsg, SysError& theErr);

    void                        setSuperior(const QSharedPointer<PlantNode>& theSuperior);

    void                        attachChild(const QSharedPointer<PlantNode>& theMsgHndObjRef);

    unsigned int                executeCmd(CMD_PRIO thePrio, const QByteArray& theCmd, QByteArray& theResult, SysError& theErr);

    bool                        cancelCmd(unsigned int theCmdId);

    void                        registerEventHnd(EventHandler* theHnd, SysError& theErr);

    void                        unRegisterEventHnd(EventHandler* theHnd);

    bool                        execCommand(PlantMessage &theMsg);

    void                        UpdateTime(int month, int day, int hour , int minute);


    QDomDocument                save(SysError &theErr);

private:

    QString                     m_deviceID;
    EventHandler*               m_theHnd;

    QString                     m_localBus;
    QString                     m_oldLocalBus;

    F520Status                  *m_f520Status;

    QList<int>                  m_totalizers;
    int                         m_allTimeTot;
    QMap<int, QString>          m_lastReset;
    int                         m_currentMonth;
    int                         m_currentDay;
    int                         m_currentHour;
    int                         m_currentMinute;

    QTimer                      *m_activePwrTimer;
    bool                        m_actPwrisToSend;
    int                         m_actPwrTimer;
    int                         m_timePassed;
    QString                     m_lastNrgSent;

private:

    void commandFrame(PlantMessage &theMsg);
    void requestFrame(PlantMessage &theMsg);
    void sendACK(PlantMessage &theMsg);
    void getDailyTotalizer(QStringList whatValue, PlantMessage &rspMessage, SysError &theErr);
    void activePwrTimeout();

public slots:
        void                ReceiveMessage(PlantMessage &theMsg, SysError& theErr);
};

#endif // BTF520_DEV_H
