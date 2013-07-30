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
