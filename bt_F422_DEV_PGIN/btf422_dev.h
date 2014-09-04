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

#ifndef BTF422_DEV_H
#define BTF422_DEV_H

#include <plantdevice.h>
#include <f422status.h>
#include <QSharedPointer>


class BtF422_dev : public PlantDevice
{
public:
    static QString const className() { return "Btf422_dev"; }

    BtF422_dev();
    ~BtF422_dev();

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


    QDomDocument                save(SysError &theErr);

private:

    QString                     m_deviceID;
    EventHandler*               m_theHnd;
    QSharedPointer<F422Status>  m_f422Status;

    QString                     m_localBus;
    QString                     m_oldLocalBus;

public slots:
        void                ReceiveMessage(PlantMessage &theMsg, SysError& theErr);
        void                ReceiveMsgLocalBUS( PlantMessage &theMsg, SysError &theErr);
};

#endif // BTF422_DEV_H
