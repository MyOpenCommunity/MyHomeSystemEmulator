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

#include "cam_xmlserializer.h"
#include "btcam_dev.h"
#include <QDebug>
#include <QDir>

static const QString DEVICE = "device";
static const QString ID = "id";
static const QString NAME = "name";
static const QString VALUE = "value";
static const QString ATTRIBUTE = "devattribute";
static const QString IDCAM = "p";
static const QString PUNTOLUCE = "pl";
static const QString GROUP = "group";
static const QString TYPE = "type";
static const QString MAX = "max";
static const QString MIN = "min";
static const QString FOLDER = "folder";
static const QString DIR = "dir";

void CAM_XmlSerializer::serialize(QString deviceID, QDomDocument &xml, QSharedPointer<CameraStatus> camera, SysError &sysErr) {
    qDebug() << "serialize";

    QDomElement device = xml.createElement(DEVICE);
    device.setAttribute(ID, deviceID);
    device.setAttribute(TYPE, Btcam_dev::className());
    xml.appendChild(device);

    QDomElement xmlAttributeA = xml.createElement(ATTRIBUTE);
    xmlAttributeA.setAttribute(NAME, IDCAM);
    xmlAttributeA.setAttribute(VALUE, camera->getaddrCam());
    xmlAttributeA.setAttribute(MAX, CameraStatus::getMaxA());
    xmlAttributeA.setAttribute(MIN, CameraStatus::getMinA());
    device.appendChild(xmlAttributeA);

    QDomElement xmlAttributeDIR = xml.createElement(ATTRIBUTE);
    xmlAttributeDIR.setAttribute(NAME, DIR);
    xmlAttributeDIR.setAttribute(VALUE, camera->getDir());
    xmlAttributeDIR.setAttribute(TYPE, FOLDER);
    device.appendChild(xmlAttributeDIR);

    qDebug() << xml.toString();

    sysErr = SysError();

}

void CAM_XmlSerializer::deserialize(QDomDocument xml, QSharedPointer<CameraStatus> &camera, SysError &sysErr) {

    qDebug() << "DESERIALIZE";
    QDomElement myXml = xml.documentElement();

    QDomNodeList deviceAttributes = myXml.elementsByTagName( DEVICE );

    sysErr = SysError();

    QDomNodeList attributeNodes = myXml.elementsByTagName(ATTRIBUTE);
    for(unsigned indey = 0; indey < attributeNodes.length(); indey++) {
        QDomElement element = attributeNodes.at(indey).toElement();
        qDebug() << "Element Name: " << element.attributes().namedItem(NAME).nodeValue();
        if (element.attributes().namedItem(NAME).nodeValue().compare(IDCAM) == 0) {
            qDebug() << "Element Value: " << element.attributes().namedItem(VALUE).nodeValue();
            if (!camera->setAddrCam(element.attributes().namedItem(VALUE).nodeValue())) {
                sysErr = SysError(SysError::CAM_SET_A_ERROR, "CAM_SET_A_ERROR, Slot: " + camera->getId());
            }
        } else if (element.attributes().namedItem(NAME).nodeValue().compare(DIR) == 0) {
            qDebug() << "Element Value " << element.attributes().namedItem(VALUE).nodeValue();
            QDir dir(element.attributes().namedItem(VALUE).nodeValue());
            if (!dir.exists())
                sysErr = SysError(SysError::CAM_IMAGE_DIR_NOT_FOUND, "CAM_IMAGE_DIR_NOT_FOUND, Slot: " + camera->getId());
            if (!camera->setDir(element.attributes().namedItem(VALUE).nodeValue())) {
                sysErr = SysError(SysError::CAM_SET_GRP_ERROR, "CAM_SET_DIR_ERROR, Slot: " + camera->getId());
            }
        }
    }

    if (!sysErr.isError()){
        camera->setConfigured(true);
    } else {
        camera->setConfigured(false);
    }
}
