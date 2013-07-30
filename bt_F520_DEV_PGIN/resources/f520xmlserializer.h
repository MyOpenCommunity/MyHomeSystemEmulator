#ifndef F520XMLSERIALIZER_H
#define F520XMLSERIALIZER_H

#include <QDomDocument>
#include <syserror.h>
#include <f520status.h>
#include <QSharedPointer>

class F520xmlserializer
{
public:
    static void serialize(QString deviceID, QSharedPointer<F520Status> f520, QDomDocument &xml, bool saveEnergy, SysError &sysErr);

    static void deserialize(QDomDocument xml, QSharedPointer<F520Status> &f520, SysError &sysErr);

    static QString query(QSharedPointer<F520Status> f520, QString day, QString month, SysError &sysErr);

    static QString query(QSharedPointer<F520Status> f520, QString month, SysError &sysErr);

    static QString avarageQuery(QSharedPointer<F520Status> f520, QString month, SysError &sysErr);

private:

    static int getDayOfMonth(int month);

};

#endif // F520XMLSERIALIZER_H
