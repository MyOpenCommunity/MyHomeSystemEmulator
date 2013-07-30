#include "busxmlserializer.h"
#include <btscsbus.h>

static const QString DEV_TYPE = "bus";
static const QString ID = "id";
static const QString TYPE = "type";


bool BusXmlSerializer::serialize(QString deviceID, QDomDocument &xml) {

    QDomElement bus = xml.createElement(DEV_TYPE);
    xml.appendChild(bus);

    bus.setAttribute(ID, deviceID);
    bus.setAttribute(TYPE, Btscsbus::className());

    return(true);
}

bool BusXmlSerializer::deserialize(QDomDocument /*xml*/) {
    return(true);
}
