#include "gwtxmlserializer.h"
#include <btowngtw.h>

static const QString DEV_TYPE = "gateway";
static const QString ID = "id";
static const QString TYPE = "type";
static const QString PWD = "password";
static const QString NAME = "name";
static const QString VALUE = "value";
static const QString ATTRIBUTE = "devattribute";

bool GWTXmlSerializer::serialize(QString deviceID, QDomDocument &xml, QSharedPointer<GwtStatus> gwt, SysError &/*sysErr*/) {

    QDomElement gateway = xml.createElement(DEV_TYPE);
    xml.appendChild(gateway);

    gateway.setAttribute(ID, deviceID);
    gateway.setAttribute(TYPE, Btowngtw::className());

    QDomElement xmlAttributePWD = xml.createElement(ATTRIBUTE);
    xmlAttributePWD.setAttribute(NAME, PWD);
    xmlAttributePWD.setAttribute(VALUE, gwt->getPwd());
    gateway.appendChild(xmlAttributePWD);

    return(true);
}

bool GWTXmlSerializer::deserialize(QDomDocument xml, QSharedPointer<GwtStatus> &gwt, SysError &sysErr) {

    qDebug() << "DESERIALIZE";
    QDomElement myXml = xml.documentElement();

    sysErr = SysError();

    QDomNodeList attributeNodes = myXml.elementsByTagName(ATTRIBUTE);
    for(unsigned indey = 0; indey < attributeNodes.length(); indey++) {
        QDomElement element = attributeNodes.at(indey).toElement();
        qDebug() << "Element Name: " << element.attributes().namedItem(NAME).nodeValue();
        if (element.attributes().namedItem(NAME).nodeValue().compare(PWD) == 0) {
            qDebug() << "Element Value " << element.attributes().namedItem(VALUE).nodeValue();
            if (!gwt->setPwd(element.attributes().namedItem(VALUE).nodeValue())) {
                sysErr = SysError(SysError::GWT_PASSWORD_ERROR, "GWT_PASSWORD_ERROR, Slot: " + gwt->getId());
            }
        }
    }

    return (true);
}
