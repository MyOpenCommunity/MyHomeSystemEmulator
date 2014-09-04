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

#ifndef GEN_DEV_H
#define GEN_DEV_H

#include <plantdevice.h>
#include <QString>
#include <QMultiMap>
#include <eventhandler.h>
#include <QSharedPointer>
#include <scenario.h>
#include <QRunnable>

class SysError;

class BtGEN_dev : public PlantDevice
{
private:


public:

    static QString const className() { return "BtGen_dev"; }

    BtGEN_dev();
    ~BtGEN_dev();

    void                setID(const QString& theID);

    const QString&      getID(void);

    void                HandleMessage(const PlantMessage& theSrt, SysError& theErr);

    void                setSuperior(const QSharedPointer<PlantNode>& theSuperior);

    void                attachChild(const QSharedPointer<PlantNode>& theMsgHndObjRef);

    unsigned int        executeCmd(CMD_PRIO thePrio, const QByteArray& theCmd, QByteArray& theResult, SysError& theErr);

    bool                cancelCmd(unsigned int theCmdId);

    void                registerEventHnd(EventHandler* theHnd, SysError& theErr);

    void                unRegisterEventHnd(EventHandler* theHnd);

    bool                execCommand(PlantMessage &theMsg, SysError &theErr);

    QDomDocument        save(SysError &theErr);

     void run();

private:

    QString                                     m_deviceID;
    EventHandler*                               m_theHnd;
    QString                                     m_xmlSrcFile;
    // It contains scenarios managed
    QMap<QString, QSharedPointer<Scenario> >    m_scenariosMap;

public slots:
        void              ReceiveMessage(PlantMessage &theMsg, SysError& theErr);
        void              ReceiveResponce(QObject * msg);

};

#endif // GEN_DEV_H
