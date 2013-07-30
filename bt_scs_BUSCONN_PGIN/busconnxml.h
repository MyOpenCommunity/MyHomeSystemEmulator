#ifndef BUSCONNXML_H
#define BUSCONNXML_H

#include <QDomDocument>
#include <QString>

class BusConnXml
{
public:

    static bool serialize(QString deviceID, QDomDocument &xml);

    static bool deserialize(QDomDocument xml);
};

#endif // BUSCONNXML_H
