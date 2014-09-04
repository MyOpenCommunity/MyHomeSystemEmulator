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

#ifndef PLANTNODE_H
#define PLANTNODE_H

#include "libplant_global.h"
#include <QSharedPointer>
#include <QMap>
#include <QObject>
#include <cmdexecutor.h>
#include <eventsource.h>
#include <QDomDocument>
#include <QMetaType>
#include <syserror.h>


class QByteArray;
//class SysError;
class PlantMessage;

class  LIBPLANTSHARED_EXPORT PlantNode: public QObject, public CmdExecutor, public EventSource
{
    Q_OBJECT
public:
    PlantNode();
    ~PlantNode();

    virtual void                        setID(const QString& theID)=0;

    virtual const QString&              getID(void)=0;

    virtual void                        init(SysError& theErr){ theErr = SysError(); }

    virtual void                        HandleMessage(const PlantMessage& theMsg, SysError& theErr)=0;

    virtual QDomDocument                save(SysError &theErr){ theErr = SysError(); return(QDomDocument()); }

    void                                setSuperior(const QSharedPointer<PlantNode>& theNode);

    void                                attachChild(const QSharedPointer<PlantNode>& theNodeNode);

    void                                detachChild(QSharedPointer<PlantNode>  theNode);

    const QSharedPointer<PlantNode>&    getSuperior(void);

    void                                setBusName(const QString& busName);

    const QString&                      getBusName();

    virtual void                        UpdateTime(int /*month*/, int /*day*/, int /*hour*/ , int /*minute*/) {}

signals:
    void                                ReleaseMessage(PlantMessage& theMsg, SysError& theErr);


public slots:
    virtual void                        ReceiveMessage(PlantMessage& /*theMsg*/, SysError& /*theErr*/){}
    virtual void                        ReceiveResponce(QObject */*msg*/){}
    virtual void                        ReceiveMsgLocalBUS( PlantMessage &/*theMsg*/, SysError &/*theErr*/) {}
    virtual void                        stopTimer() {}


private:
    QMap<QString, QSharedPointer<PlantNode> >    m_childList;
    QSharedPointer<PlantNode>                    m_superior;
    QString                                      m_busName;
};



#endif // PLANTNODE_H
