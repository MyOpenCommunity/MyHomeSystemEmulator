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
