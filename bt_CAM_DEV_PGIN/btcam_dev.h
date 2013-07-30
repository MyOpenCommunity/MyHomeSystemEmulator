#ifndef BTCAM_DEV_H
#define BTCAM_DEV_H

#include <plantdevice.h>
#include <QString>
#include <QMultiMap>
#include <eventhandler.h>
#include <camerastatus.h>
#include <QSharedPointer>
#include <QTimer>

class SysError;

class Btcam_dev : public PlantDevice
{
public:

    static QString const className() { return "Btcam_dev"; }

    Btcam_dev();
    ~Btcam_dev();

    void                setID(const QString& theID);

    const QString&      getID(void);

    void                HandleMessage(const PlantMessage& theSrt, SysError& theErr);

    void                setSuperior(const QSharedPointer<PlantNode>& theSuperior);

    void                attachChild(const QSharedPointer<PlantNode>& theMsgHndObjRef);

    unsigned int        executeCmd(CMD_PRIO thePrio, const QByteArray& theCmd, QByteArray& theResult, SysError& theErr);

    bool                cancelCmd(unsigned int theCmdId);

    void                registerEventHnd(EventHandler* theHnd, SysError& theErr);

    void                unRegisterEventHnd(EventHandler* theHnd);

    bool                execCommand(PlantMessage &theMsg, SysError &theErr);

    QDomDocument        save(SysError &theErr);

    bool                setMode(QString mode);

public slots:
    void            ReceiveMessage(PlantMessage &theMsg, SysError& theErr);
    void            stopTimer();

private:
    QByteArray      CreateHTMLResp(SysError& theErr);


private:
    //  It define a pair container where each pair
    //  identify an address, value pair
    QString                         m_deviceID;
    EventHandler*                   m_theHnd;
    QSharedPointer<CameraStatus>    m_camStatus;
    bool                            m_active;
    QTimer *                        m_camereTimer;
    int                             m_imageCounter;
};

#endif // BTCAM_DEV_H
