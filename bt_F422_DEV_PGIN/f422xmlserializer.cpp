/* This file is part of My Home System Emulator
 *
 * My Home System Emulator is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.

 * My Home System Emulator is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with My Home System Emulator.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "f422xmlserializer.h"
#include <QDebug>
#include <btf422_dev.h>
#include <f422status.h>

static const QString DEVICE = "device";
static const QString ID = "id";
static const QString NAME = "name";
static const QString VALUE = "value";
static const QString ATTRIBUTE = "devattribute";
static const QString I3 = "I3";
static const QString I4 = "I4";
static const QString TYPE = "type";
static const QString MAX = "max";
static const QString MIN = "min";

void F422xmlserializer::serialize(QString deviceID, QDomDocument &xml, QSharedPointer<F422Status> f422Status, SysError &/*sysErr*/) {

    qDebug() << "serialize";

    QDomElement device = xml.createElement(DEVICE);
    device.setAttribute(ID, deviceID);
    device.setAttribute(TYPE, BtF422_dev::className());
    xml.appendChild(device);

    QDomElement xmlAttributeI3 = xml.createElement(ATTRIBUTE);
    xmlAttributeI3.setAttribute(NAME, I3);
    xmlAttributeI3.setAttribute(VALUE, f422Status->getI3());
    xmlAttributeI3.setAttribute(MAX, F422Status::getMaxI3());
    xmlAttributeI3.setAttribute(MIN, F422Status::getMinI3());
    device.appendChild(xmlAttributeI3);

    QDomElement xmlAttributeI4 = xml.createElement(ATTRIBUTE);
    xmlAttributeI4.setAttribute(NAME, I4);
    xmlAttributeI4.setAttribute(VALUE, f422Status->getI4());
    xmlAttributeI4.setAttribute(MAX, F422Status::getMaxI4());
    xmlAttributeI4.setAttribute(MIN, F422Status::getMinI4());
    device.appendChild(xmlAttributeI4);
}

void F422xmlserializer::deserialize(QDomDocument xml, QSharedPointer<F422Status> &f422Status, SysError &sysErr) {

    qDebug() << "DESERIALIZE";
    QDomElement myXml = xml.documentElement();
    QDomNodeList deviceAttributes = myXml.elementsByTagName( DEVICE );
    sysErr = SysError();

    QDomNodeList attributeNodes = myXml.elementsByTagName(ATTRIBUTE);
    for(unsigned indey = 0; indey < attributeNodes.length(); indey++) {
        QDomElement element = attributeNodes.at(indey).toElement();
        qDebug() << "Element Name: " << element.attributes().namedItem(NAME).nodeValue();
        if (element.attributes().namedItem(NAME).nodeValue().compare(I3) == 0) {
            qDebug() << "Element Value: " << element.attributes().namedItem(VALUE).nodeValue();
            if (!f422Status->setI3(element.attributes().namedItem(VALUE).nodeValue())) {
                sysErr = SysError(SysError::F422_SET_I3_ERROR, "F422_SET_I3_ERROR");
            }
        } else if (element.attributes().namedItem(NAME).nodeValue().compare(I4) == 0) {
            qDebug() << "Element Value: " << element.attributes().namedItem(VALUE).nodeValue();
            if (!f422Status->setI4(element.attributes().namedItem(VALUE).nodeValue())) {
                sysErr = SysError(SysError::F422_SET_I4_ERROR, "F422_SET_I4_ERROR");
            }
        }
    }

    if (!sysErr.isError()){
        f422Status->setConfigured(true);
    } else {
        f422Status->setConfigured(false);
    }
}
