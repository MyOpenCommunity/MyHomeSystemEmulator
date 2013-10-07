#include "gwtxmlserializer.h"
#include <btowngtw.h>

static const QString DEV_TYPE = "gateway";
static const QString ID = "id";
static const QString TYPE = "type";
static const QString PWD = "password";
static const QString HTTP_PORT = "http_port";
static const QString HTTPS_PORT = "https_port";
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

    QDomElement xmlAttributeHTTP = xml.createElement(ATTRIBUTE);
    xmlAttributeHTTP.setAttribute(NAME, HTTP_PORT);
    xmlAttributeHTTP.setAttribute(VALUE, gwt->httpPort());
    gateway.appendChild(xmlAttributeHTTP);

    QDomElement xmlAttributeHTTPS = xml.createElement(ATTRIBUTE);
    xmlAttributeHTTPS.setAttribute(NAME, HTTPS_PORT);
    xmlAttributeHTTPS.setAttribute(VALUE, gwt->httpsPort());
    gateway.appendChild(xmlAttributeHTTPS);


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
        } else if (element.attributes().namedItem(NAME).nodeValue().compare(HTTP_PORT) == 0) {
            qDebug() << "Element Value " << element.attributes().namedItem(VALUE).nodeValue();
            if (!gwt->setHttpPort(element.attributes().namedItem(VALUE).nodeValue().toInt())) {
                sysErr = SysError(SysError::GWT_HTTP_PORT_ERROR, "GWT_HTTP_PORT_ERROR, Slot: " + gwt->getId());
            }
        } else if (element.attributes().namedItem(NAME).nodeValue().compare(HTTPS_PORT) == 0) {
            qDebug() << "Element Value " << element.attributes().namedItem(VALUE).nodeValue();
            if (!gwt->setHttpsPort(element.attributes().namedItem(VALUE).nodeValue().toInt())) {
                sysErr = SysError(SysError::GWT_HTTPS_PORT_ERROR, "GWT_HTTPS_PORT_ERROR, Slot: " + gwt->getId());
            }
        }

    }

    return (true);
}
