#ifndef BTF520_DEV_H
#define BTF520_DEV_H

#include <plantdevice.h>
#include <QSharedPointer>
#include <f520status.h>
#include <QObject>

class BtF520_dev : public PlantDevice
{
public:
    static QString const className() { return "BtF520_dev"; }

    BtF520_dev();
    ~BtF520_dev();

    void                        setID(const QString& theID);

    const QString&              getID(void);

    void                        HandleMessage(const PlantMessage& theMsg, SysError& theErr);

    void                        setSuperior(const QSharedPointer<PlantNode>& theSuperior);

    void                        attachChild(const QSharedPointer<PlantNode>& theMsgHndObjRef);

    unsigned int                executeCmd(CMD_PRIO thePrio, const QByteArray& theCmd, QByteArray& theResult, SysError& theErr);

    bool                        cancelCmd(unsigned int theCmdId);

    void                        registerEventHnd(EventHandler* theHnd, SysError& theErr);

    void                        unRegisterEventHnd(EventHandler* theHnd);

    bool                        execCommand(PlantMessage &theMsg);


    QDomDocument                save(SysError &theErr);

private:

    QString                     m_deviceID;
    EventHandler*               m_theHnd;

    QString                     m_localBus;
    QString                     m_oldLocalBus;

    QSharedPointer<F520Status>  m_f520Status;

private:

    void commandFrame(PlantMessage &theMsg);
    void requestFrame(PlantMessage &theMsg);
    void sendACK(PlantMessage &theMsg);

public slots:
        void                ReceiveMessage(PlantMessage &theMsg, SysError& theErr);
};

#endif // BTF520_DEV_H
