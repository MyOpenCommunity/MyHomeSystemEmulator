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

#ifndef BTSCSBUSCONN_H
#define BTSCSBUSCONN_H

#include <plantbusconn.h>


class BtScsBusConn : public PlantBusConn
{
public:
    static QString const className() { return "BtScsBusConn"; }

    ~BtScsBusConn();
    BtScsBusConn();

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

    void            connectBus(const QString& /*theFatherBusID*/,  const QString& /*theChildBusID*/, SysError& /*theErr*/) {}
    void            disconnectBus(void) {}

public slots:
    void                                            ReceiveMessage(PlantMessage &theMsg, SysError& theErr);

private:
    QString m_busID;
};

#endif // BTSCSBUSCONN_H
