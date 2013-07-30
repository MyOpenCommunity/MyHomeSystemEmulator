#ifndef BTSCSBUSCONN_H
#define BTSCSBUSCONN_H

#include <plantbusconn.h>


class BtScsBusConn : public PlantBusConn
{
public:
    static QString const className() { return "BtScsBusConn"; }

    ~BtScsBusConn();
    BtScsBusConn();

    void                                            setID(const QString& theID);
    const QString&                                  getID(void);

    void                                            HandleMessage(const PlantMessage& theSrt, SysError& theErr);

    unsigned int                                    executeCmd(CMD_PRIO thePrio, const QByteArray& theCmd, QByteArray& theResult, SysError& theErr);

    bool                                            cancelCmd(unsigned int theCmdId);

    void                                            init(SysError& theErr);

    void                                            registerEventHnd(EventHandler* theHnd, SysError& theErr);

    void                                            unRegisterEventHnd(EventHandler* theHnd);

    bool                                            setApl(const QString& theAddress, bool theVal);

    QList<bool>                                     getApl(const QString& theAddress);

    bool                                            execCommand(PlantMessage &theMsg);

    QDomDocument                                    save(SysError &theErr);

    void            connectBus(const QString& /*theFatherBusID*/,  const QString& /*theChildBusID*/, SysError& /*theErr*/) {}
    void            disconnectBus(void) {}

public slots:
    void                                            ReceiveMessage(PlantMessage &theMsg, SysError& theErr);

private:
    QString m_busID;
};

#endif // BTSCSBUSCONN_H
