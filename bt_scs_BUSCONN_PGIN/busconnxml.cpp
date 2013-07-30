#include "busconnxml.h"
#include <btscsbusconn.h>

static const QString DEV_TYPE = "bus";
static const QString ID = "id";
static const QString TYPE = "type";


bool BusConnXml::serialize(QString deviceID, QDomDocument &xml) {

    QDomElement bus = xml.createElement(DEV_TYPE);
    xml.appendChild(bus);

    bus.setAttribute(ID, deviceID);
    bus.setAttribute(TYPE, BtScsBusConn::className());

    return(true);
}

bool BusConnXml::deserialize(QDomDocument /*xml*/) {
    return(true);
}
