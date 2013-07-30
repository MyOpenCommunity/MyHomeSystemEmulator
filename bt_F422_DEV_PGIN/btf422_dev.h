#ifndef BTF422_DEV_H
#define BTF422_DEV_H

#include <plantdevice.h>
#include <f422status.h>
#include <QSharedPointer>


class BtF422_dev : public PlantDevice
{
public:
    static QString const className() { return "Btf422_dev"; }

    BtF422_dev();
    ~BtF422_dev();

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
    QSharedPointer<F422Status>  m_f422Status;

    QString                     m_localBus;
    QString                     m_oldLocalBus;

public slots:
        void                ReceiveMessage(PlantMessage &theMsg, SysError& theErr);
        void                ReceiveMsgLocalBUS( PlantMessage &theMsg, SysError &theErr);
};

#endif // BTF422_DEV_H
