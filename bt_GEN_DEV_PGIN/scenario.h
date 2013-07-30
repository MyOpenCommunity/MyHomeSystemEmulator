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
