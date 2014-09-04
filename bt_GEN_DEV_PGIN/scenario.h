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

#ifndef SCENARIO_H
#define SCENARIO_H

#include <QString>
#include <message.h>
#include <QList>
#include <action.h>

class Scenario
{
public:
    Scenario();

    void                    setName(QString name);

    QString                 getName();

    void                    setTrigMsg(Message msg);

    Message                 getTrigMsg();

    void                    addAction(QSharedPointer<Action> action);

    QList<QSharedPointer<Action> > getActionList();

private:

    QString                                 m_name;
    Message                                 m_trigMsg;
    QList<QSharedPointer<Action> >          m_actionList;

};

inline Scenario::Scenario() {

}

inline void Scenario::setName(QString name) {
    m_name = name;
}

inline QString Scenario::getName() {
    return(m_name);
}

inline void Scenario::setTrigMsg(Message msg) {
    m_trigMsg = msg;
}

inline Message Scenario::getTrigMsg() {
    return(m_trigMsg);
}

inline void Scenario::addAction(QSharedPointer<Action> action) {
    m_actionList << action;
}

inline QList<QSharedPointer<Action> >  Scenario::getActionList() {
    return(m_actionList);
}

#endif // SCENARIO_H
