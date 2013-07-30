#ifndef BTF411DEV_H
#define BTF411DEV_H


#include <plantdevice.h>
#include <QString>
#include <QMultiMap>
#include <eventhandler.h>
#include <ledstatus.h>
#include <QSharedPointer>


class BtF411_dev : public PlantDevice
{
private:

    bool findLed(QString address, QSharedPointer<LedStatus> &led);

public:

    static QString const className() { return "Btf411_dev"; }
    static QString getModeTypeValues();

    BtF411_dev();
    ~BtF411_dev();

    void                setID(const QString& theID);

    const QString&      getID(void);

    void                HandleMessage(const PlantMessage& theSrt, SysError& theErr);

    void                setSuperior(const QSharedPointer<PlantNode>& theSuperior);

    void                attachChild(const QSharedPointer<PlantNode>& theMsgHndObjRef);

    unsigned int        executeCmd(CMD_PRIO thePrio, const QByteArray& theCmd, QByteArray& theResult, SysError& theErr);

    bool                cancelCmd(unsigned int theCmdId);

    void                registerEventHnd(EventHandler* theHnd, SysError& theErr);

    void                unRegisterEventHnd(EventHandler* theHnd);

    bool                execCommand(PlantMessage &theMsg);

    QDomDocument        save(SysError &theErr);

private:

    bool                setApl(QSharedPointer<LedStatus> led, bool theVal);

    QList<bool>         getApl(const QString& theAddress);

    void                sendACK(PlantMessage &theMsg);

protected:

    void                        setResoucesFileName(QString fileName);

    inline QString              getResourceFileName() { return (m_ResourcesFileName); }

    inline void                 setDeviceID(QString deviceID) { m_deviceID = deviceID; }

    inline QString              getDeviceID() { return(m_deviceID); }

    inline void                 setLedAddress(QMap <QString, QSharedPointer<LedStatus> > led) { m_ledAddress = led; }

    inline void                 addLed(QSharedPointer<LedStatus> led) { m_ledAddress.insert(led->getId(), led); }

    inline void                 setMode(QString mode) { m_mode = mode; }

    inline QString              getMode() { return(m_mode); }

    virtual QString             getClassName() { return(QString()); }

private:
    //  It define a pair container where each pair
    //  identify an address, value pair
    QString                                         m_deviceID;
    EventHandler*                                   m_theHnd;
    QMap <QString, QSharedPointer<LedStatus> >      m_ledAddress;
    QString                                         m_mode;
    QString                                         m_ResourcesFileName;

public slots:
        void              ReceiveMessage(PlantMessage &theMsg, SysError& theErr);
};

#endif // BTF411DEV_H
