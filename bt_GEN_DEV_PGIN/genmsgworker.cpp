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

#include "genmsgworker.h"

#include <action.h>
#include <message.h>

GenMsgWorker::GenMsgWorker()
{
}

GenMsgWorker::GenMsgWorker(EventHandler* theHnd, QSharedPointer<Scenario> scenario, PlantMessage theMsg)
{
    m_theHnd = theHnd;
    m_scenario = scenario;
    m_theMsg = theMsg;

    qDebug() << "GenMsgWorker senario:" << scenario->getName();
}

void GenMsgWorker::run() {

    SysError theErr;
    foreach (QSharedPointer<Action> action, m_scenario->getActionList()){
        qDebug() << "GenMsgWorker - Loop for " << action->getRepeat() << " times";
        qDebug() << "GenMsgWorker - Start from " << action->getStartFrom();
        for (int index = action->getStartFrom(); index <= action->getRepeat(); index++) {
            foreach (QSharedPointer<Message> message, action->getMessageList()) {
                switch(message->getChannel())
                {
                case Message::CMD:
                {
                    qDebug() << "GenMsgWorker - Have command frame to send: " <<  message->getMsgValue() << ", " << message->getDescription();
                    PlantMessage rspMessage;
                    rspMessage.setValue(PlantMessage::OPEN, message->getMsgValue(index));
                    rspMessage.setSourceAddr(QPair<QString, QString>(PlantMessage::INTERNAL, PlantMessage::DEVICE), true);
                    rspMessage.setDestAddr(QPair<QString, QString>(m_theMsg.getSourceAddr().first, m_theMsg.getSourceAddr().second), true);
                    rspMessage.setType(PlantMessage::GEN_MESSAGE);
                    PlantResponse *rsp = new PlantResponse();
                    rsp->setRpsMessage(rspMessage);
                    rsp->setRspErr(theErr);
                    emit ReleaseResponse(rsp);
                    break;
                }
                case Message::MONITOR:
                {
                    qDebug() << "GenMsgWorker - Have Monitor frame to send: " <<  message->getMsgValue() << ", " << message->getDescription();
                    PlantMessage rspMessage;
                    rspMessage.setId(m_theMsg.getId());
                    rspMessage.setValue(PlantMessage::OPEN, message->getMsgValue(index));
                    rspMessage.setDestAddr(QPair<QString, QString>(PlantMessage::INTERNAL, PlantMessage::GATEWAY));
                    rspMessage.setSourceAddr(QPair<QString, QString>(PlantMessage::INTERNAL, PlantMessage::DEVICE), true);
                    rspMessage.setType(PlantMessage::GEN_MESSAGE);
                    PlantResponse *rsp = new PlantResponse();
                    rsp->setRpsMessage(rspMessage);
                    rsp->setRspErr(theErr);
                    emit ReleaseResponse(rsp);
                    break;
                }
                case Message::GUI:
                {
                    qDebug() << "GenMsgWorker - Have gui frame to send: " <<  message->getMsgValue() << ", " << message->getDescription();
                    QString qStr = message->getMsgValue(index);

                    QByteArray qBary;
                    qBary.append(qStr);

                    m_theHnd->handleEvent(EventHandler::NORMAL,  qBary);
                    break;
                }
                default:
                {
                    qDebug() << "Have not valid channel frame to send";
                    break;
                }
                }
                qDebug() << "message have a delay: " <<  message->getDelay();
                // some OS specific stuff
                // mingw (3.4.2) sleep on windows is called _sleep and uses microseconds
                #ifdef Q_OS_WIN32
                //_sleep( message->getDelay());
                unsigned long dwMilliseconds = message->getDelay();
                sleep(dwMilliseconds);
                #else
                msleep( message->getDelay());
                #endif
            }
        }
        qDebug() << "GenMsgWorker - End Loop";
    }
     qDebug() << "GenMsgWorker - END Run";
}
