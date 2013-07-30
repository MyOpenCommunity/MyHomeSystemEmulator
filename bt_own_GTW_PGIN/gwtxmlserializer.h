#ifndef GWTXMLSERIALIZER_H
#define GWTXMLSERIALIZER_H

#include <QDomDocument>
#include <QString>
#include <gwtstatus.h>
#include <QSharedPointer>
#include <syserror.h>

class GWTXmlSerializer
{
public:

    static bool serialize(QString deviceID, QDomDocument &xml, QSharedPointer<GwtStatus> gwt, SysError &sysErr);

    static bool deserialize(QDomDocument xml, QSharedPointer<GwtStatus> &gwt, SysError &sysErr);

};

#endif // GWTXMLSERIALIZER_H
