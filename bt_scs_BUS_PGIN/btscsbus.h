#ifndef BTSCSBUS_H
#define BTSCSBUS_H


#include <plantbus.h>

class PlantMessage;
class Btscsbus : public PlantBus
{

public:

    static QString const className() { return "Btscsbus"; }

    ~Btscsbus();
    Btscsbus();
    
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

private:
    QString m_busID;
    
public slots:
    void                                            ReceiveMessage(PlantMessage &theMsg, SysError& theErr);


};

#endif // BTSCSBUS_H
