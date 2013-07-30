#ifndef F411XMLSERIALIZER_H
#define F411XMLSERIALIZER_H

#include <QDomDocument>
#include <QMap>
#include <ledstatus.h>
#include <syserror.h>
#include <QSharedPointer>

class F411xmlserializer
{
public:
    static void serialize(QString deviceID, QMap<QString, QSharedPointer<LedStatus> > leds, QString type, QString mode, QString currBUS, QDomDocument &xml, SysError &sysErr);

    static void deserialize(QDomDocument xml, QMap<QString, QSharedPointer<LedStatus> > &leds, QString &mode, QString &currBUS, SysError &sysErr);

};

#endif // F411XMLSERIALIZER_H
