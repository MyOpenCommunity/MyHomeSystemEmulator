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

#ifndef GENMSGWORKER_H
#define GENMSGWORKER_H

#include <eventhandler.h>
#include <QSharedPointer>
#include <scenario.h>
#include <plantmessage.h>
#include <syserror.h>
#include <QObject>
#include <QThread>


class GenMsgWorker : public QThread
{
    Q_OBJECT
public:
    GenMsgWorker();
    GenMsgWorker(EventHandler* theHnd, QSharedPointer<Scenario> scenario, PlantMessage theMsg);

private:

    EventHandler*                               m_theHnd;
    QSharedPointer<Scenario>                    m_scenario;
    PlantMessage                                m_theMsg;


signals:
    void ReleaseResponse(QObject * theMsg);

public:
    void run();

};

class PlantResponse : public QObject
{
    Q_OBJECT
public:
    PlantResponse();

    PlantMessage                    getRspMessage();

    SysError                        getCurrentErr();

    void                            setRpsMessage(PlantMessage rspMessage);

    void                            setRspErr(SysError rspErr);

private:
    PlantMessage                                m_rspMessage;
    SysError                                    m_rspTheErr;

};

inline PlantMessage PlantResponse::getRspMessage() {
    return(m_rspMessage);
}

inline SysError PlantResponse::getCurrentErr() {
    return(m_rspTheErr);
}

inline void PlantResponse::setRpsMessage(PlantMessage rspMessage){
    m_rspMessage = rspMessage;
}

inline void PlantResponse::setRspErr(SysError rspErr){
    m_rspTheErr = rspErr;
}

inline PlantResponse::PlantResponse() {

}

#endif // GENMSGWORKER_H
