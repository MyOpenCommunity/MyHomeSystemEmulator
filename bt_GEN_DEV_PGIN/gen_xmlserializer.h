#ifndef GEN_XMLSERIALIZER_H
#define GEN_XMLSERIALIZER_H

#include <QDomDocument>
#include <QMap>
#include <syserror.h>
#include <QSharedPointer>
#include <scenario.h>
#include <QString>

class GEN_xmlserializer
{
public:

    static void serialize(QString deviceID, QString currBUS, QString sourceFile, QDomDocument &xml, SysError &sysErr);

    static void deserialize(QDomDocument xml, QString &sourceFile, QString &currBUS, SysError &sysErr);

    static void loadScenarios(QDomDocument xml, QMap<QString, QSharedPointer<Scenario> > &scenariosMap, SysError &sysErr);

    static const QString getBuses();

};

#endif // GEN_XMLSERIALIZER_H
