#ifndef BTPLANT_H
#define BTPLANT_H

#include <QObject>
#include <plant.h>

class PlantMessage;

class BtPlant : public Plant
{
public:

    static QString const className() { return "BtPlant"; }

    BtPlant();

    ~BtPlant();

    void                                            setID(const QString& theID);
    const QString&                                  getID(void);

    void                                            addBus(const QSharedPointer<PlantBus>& theBus);
    void                                            removeBus(const QString& theID);
            QSharedPointer<PlantBus>                getBus(const QString& theID);
            QList< QSharedPointer<PlantBus> >       getBusList(void);

    void                                            addDevice(const QSharedPointer<PlantDevice>& theDevice);
    bool removeDevice(const QString& theID);
            QSharedPointer<PlantDevice>             getDevice(const QString& theID);
            QList< QSharedPointer<PlantDevice> >    getDeviceList(void);

    void                                            addBusConn(const QSharedPointer<PlantBusConn>& theBusConn);
    bool removeBusConn(const QString& theID);
            QSharedPointer<PlantBusConn>            getBusConn(const QString& theID);
            QList< QSharedPointer<PlantBusConn> >   getBusConnList(void);

    void addGateway(const QSharedPointer<PlantGateway>& theGateway, SysError &theErr);
    bool removeGateway(const QString& theID);
            QSharedPointer<PlantGateway>            getGateway(const QString& theID);
            QList< QSharedPointer<PlantGateway> >   getGatewayList(void);

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
    QString m_plantID;
    QMap<QString, QSharedPointer<PlantBus> >        m_plantBusList;
    QMap<QString, QSharedPointer<PlantDevice> >     m_plantDeviceList;
    QMap<QString, QSharedPointer<PlantBusConn> >    m_plantBusConnList;
    QMap<QString, QSharedPointer<PlantGateway> >    m_plantGatewayList;


};

#endif // BTPLANT_H
