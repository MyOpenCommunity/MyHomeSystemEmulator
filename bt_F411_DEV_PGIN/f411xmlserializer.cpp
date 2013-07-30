#include "f411xmlserializer.h"
#include <qdebug.h>
#include <btf411dev.h>
#include <plantmgr.h>

static const QString DEVICE = "device";
static const QString XMLSLOT = "slot";
static const QString ID = "id";
static const QString NAME = "name";
static const QString VALUE = "value";
static const QString ATTRIBUTE = "attribute";
static const QString DEVATTRIBUTE = "devattribute";
static const QString AMBIENTE = "a";
static const QString PUNTOLUCE = "pl";
static const QString GROUP = "group";
static const QString TYPE = "type";
static const QString MAX = "max";
static const QString MIN = "min";
static const QString MODE = "mode";
static const QString VALUES = "values";
static const QString COMBO = "comboBox";
static const QString BUS = "bus";

void F411xmlserializer::serialize(QString deviceID, QMap <QString, QSharedPointer<LedStatus> > leds, QString type, QString mode, QString currBUS, QDomDocument &xml, SysError &sysErr) {

    QDomElement device = xml.createElement(DEVICE);
    device.setAttribute(ID, deviceID);
    device.setAttribute(TYPE, type);
    xml.appendChild(device);
    QDomElement xmlAttributeMode = xml.createElement(DEVATTRIBUTE);
    xmlAttributeMode.setAttribute(NAME, MODE);
    xmlAttributeMode.setAttribute(VALUE, mode);
    xmlAttributeMode.setAttribute(TYPE, COMBO);
    xmlAttributeMode.setAttribute(VALUES, BtF411_dev::getModeTypeValues());
    device.appendChild(xmlAttributeMode);

    QDomElement xmlAttributeBus = xml.createElement(DEVATTRIBUTE);
    xmlAttributeBus.setAttribute(NAME, BUS);
    xmlAttributeBus.setAttribute(VALUE, currBUS);
    xmlAttributeBus.setAttribute(TYPE, COMBO);
    xmlAttributeBus.setAttribute(VALUES, PlantMgr::getInstance().getBuses());
    device.appendChild(xmlAttributeBus);

    foreach(QSharedPointer<LedStatus> led, leds)
    {
        QDomElement xmlSlot = xml.createElement(XMLSLOT);
        xmlSlot.setAttribute(ID, led->getId());
        device.appendChild(xmlSlot);

        QDomElement xmlAttributeA = xml.createElement(ATTRIBUTE);
        xmlAttributeA.setAttribute(NAME, AMBIENTE);
        xmlAttributeA.setAttribute(VALUE, led->getA());
        xmlAttributeA.setAttribute(MAX, LedStatus::getMaxA());
        xmlAttributeA.setAttribute(MIN, LedStatus::getMinA());
        xmlSlot.appendChild(xmlAttributeA);

        QDomElement xmlAttributePL = xml.createElement(ATTRIBUTE);
        xmlAttributePL.setAttribute(NAME, PUNTOLUCE);
        xmlAttributePL.setAttribute(VALUE, led->getPl());
        xmlAttributePL.setAttribute(MAX, LedStatus::getMaxPL());
        xmlAttributePL.setAttribute(MIN, LedStatus::getMinPL());
        xmlSlot.appendChild(xmlAttributePL);

        QDomElement xmlAttributeGRP = xml.createElement(ATTRIBUTE);
        xmlAttributeGRP.setAttribute(NAME, GROUP);
        xmlAttributeGRP.setAttribute(VALUE, led->getGroup());
        xmlAttributeGRP.setAttribute(MAX, LedStatus::getMaxGRP());
        xmlAttributeGRP.setAttribute(MIN, LedStatus::getMinGRP());
        xmlSlot.appendChild(xmlAttributeGRP);
    }
    qDebug()<<xml.toString();

    sysErr = SysError();
}

void F411xmlserializer::deserialize(QDomDocument xml, QMap <QString, QSharedPointer<LedStatus> > &leds, QString &mode, QString &currBUS, SysError &sysErr) {

    qDebug() << "DESERIALIZE";
    //qDebug() << xml.toString();

    QDomElement myXml = xml.documentElement();

    QDomNodeList deviceAttributes = myXml.elementsByTagName( DEVATTRIBUTE );
    for(int index = 0; index < deviceAttributes.count() ; index++) {

        QDomElement element = deviceAttributes.item(index).toElement();
        if (element.attributes().namedItem(NAME).nodeValue().compare(MODE) == 0) {
            mode = element.attributes().namedItem(VALUE).nodeValue();
        } else if (element.attributes().namedItem(NAME).nodeValue().compare(BUS) == 0) {
            currBUS = element.attributes().namedItem(VALUE).nodeValue();
        }
    }

    QDomNodeList nodes = myXml.elementsByTagName( XMLSLOT );

    if(nodes.isEmpty()) {
        sysErr = SysError(SysError::F411_NO_PARAMETER_SET, "F411_NO_PARAMETER_SET");
        return;
    }

    for (unsigned index = 0; index < nodes.length(); index++) {
        QDomElement element = nodes.item(index).toElement();
        QString ledId = element.attribute(ID);
        if (!leds.contains(ledId)){
            sysErr = SysError(SysError::F411_LED_NOT_FOUND, "F411_LED_NOT_FOUND: " +ledId);
            return;
        }

        sysErr = SysError();

        QDomNodeList attributeNodes = element.elementsByTagName(ATTRIBUTE);
        for(unsigned indey = 0; indey < attributeNodes.length(); indey++) {
            QDomElement element = attributeNodes.at(indey).toElement();
            qDebug() << "Element Name: " << element.attributes().namedItem(NAME).nodeValue();
            if (element.attributes().namedItem(NAME).nodeValue().compare(AMBIENTE) == 0) {
                qDebug() << "Element Value: " << element.attributes().namedItem(VALUE).nodeValue();
                if (!leds[ledId]->setA(element.attributes().namedItem(VALUE).nodeValue())) {
                    sysErr = SysError(SysError::F411_SET_A_ERROR, "F411_SET_A_ERROR, Slot: " + ledId);
                }
            } else if (element.attributes().namedItem(NAME).nodeValue().compare(PUNTOLUCE) == 0) {
                qDebug() << "Element Value " << element.attributes().namedItem(VALUE).nodeValue();
                if (!leds[ledId]->setPl(element.attributes().namedItem(VALUE).nodeValue())) {
                    sysErr = SysError(SysError::F411_SET_PL_ERROR, "F411_SET_PL_ERROR, Slot: " + ledId);
                }
            } else if (element.attributes().namedItem(NAME).nodeValue().compare(GROUP) == 0) {
                qDebug() << "Element Value " << element.attributes().namedItem(VALUE).nodeValue();
                if (!leds[ledId]->setGroup(element.attributes().namedItem(VALUE).nodeValue())) {
                    sysErr = SysError(SysError::F411_SET_GRP_ERROR, "F411_SET_GRP_ERROR, Slot: " + ledId);
                }
            }
        }

        if (!sysErr.isError()){
            leds[ledId]->setConfigured(true);
        }
    }
    sysErr = SysError();
}
