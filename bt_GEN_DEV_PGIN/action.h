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

#ifndef ACTION_H
#define ACTION_H

#include <QList>
#include <QSharedPointer>
#include <message.h>

// Class to manage configuration xml
class Action
{
public:
    Action();

    QList<QSharedPointer<Message> >     getMessageList();

    void                                addMessage(QSharedPointer<Message> message);

    void                                setRepeat(int repeat);

    int                                 getRepeat();

    void                                setStartFrom(int startFrom);

    int                                 getStartFrom();

private:
    // Messages list
    QList<QSharedPointer<Message> > m_messageList;
    // number of times to repeat messages list
    int                             m_repeat;
    // volue of starting index in repeater list
    int                             m_startFrom;
};

inline Action::Action() {
    m_repeat = 0;
    m_startFrom = 0;
}

inline QList<QSharedPointer<Message> > Action::getMessageList() {
    return(m_messageList);
}

inline void Action::addMessage(QSharedPointer<Message> message) {
    m_messageList << message;
}

inline void Action::setRepeat(int repeat) {
    m_repeat = repeat;
}

inline int Action::getRepeat() {
    return(m_repeat);
}

inline void Action::setStartFrom(int startFrom) {
    m_startFrom = startFrom;
}

inline int Action::getStartFrom() {
    return(m_startFrom);
}


#endif // ACTION_H
