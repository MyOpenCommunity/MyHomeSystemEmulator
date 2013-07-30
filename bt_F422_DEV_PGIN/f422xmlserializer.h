#ifndef F422XMLSERIALIZER_H
#define F422XMLSERIALIZER_H

#include <QString>
#include <QDomDocument>
#include <syserror.h>
#include <QSharedPointer>
#include <f422status.h>

class F422xmlserializer
{
public:
    static void serialize(QString deviceID, QDomDocument &xml, QSharedPointer<F422Status> f422Status, SysError &sysErr);

    static void deserialize(QDomDocument xml, QSharedPointer<F422Status> &f422Status, SysError &sysErr);

};

#endif // F422XMLSERIALIZER_H
