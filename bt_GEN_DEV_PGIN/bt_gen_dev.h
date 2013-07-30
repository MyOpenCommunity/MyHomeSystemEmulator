#ifndef GEN_DEV_H
#define GEN_DEV_H

#include <plantdevice.h>
#include <QString>
#include <QMultiMap>
#include <eventhandler.h>
#include <QSharedPointer>
#include <scenario.h>
#include <QRunnable>

class SysError;

class BtGEN_dev : public PlantDevice
{
private:


public:

    static QString const className() { return "BtGen_dev"; }

    BtGEN_dev();
    ~BtGEN_dev();

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

     void run();

private:

    QString                                     m_deviceID;
    EventHandler*                               m_theHnd;
    QString                                     m_xmlSrcFile;
    // It contains scenarios managed
    QMap<QString, QSharedPointer<Scenario> >    m_scenariosMap;

public slots:
        void              ReceiveMessage(PlantMessage &theMsg, SysError& theErr);
        void              ReceiveResponce(QObject * msg);

};

#endif // GEN_DEV_H
