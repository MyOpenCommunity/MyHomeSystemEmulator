#ifndef BUSXMLSERIALIZER_H
#define BUSXMLSERIALIZER_H

#include <QDomDocument>
#include <QString>

class BusXmlSerializer
{
public:

    static bool serialize(QString deviceID, QDomDocument &xml);

    static bool deserialize(QDomDocument xml);

};

#endif // BUSXMLSERIALIZER_H
