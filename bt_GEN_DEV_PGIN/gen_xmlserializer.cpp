#include "gen_xmlserializer.h"
#include <qdebug.h>
#include <bt_gen_dev.h>
#include <scenario.h>
#include <message.h>
#include <QSharedPointer>
#include <plantmgr.h>
#include <QList>


static const QString DEVICE = "device";
static const QString ID = "id";
static const QString NAME = "name";
static const QString VALUE = "value";
static const QString ATTRIBUTE = "attribute";
static const QString DEVATTRIBUTE = "devattribute";
static const QString TYPE = "type";
static const QString SRCFILE = "xmlfile";
static const QString FL = "file";
static const QString EXTFILE = "extension";
static const QString XML_FILE = ".xml";

static const QString SCENARIO = "Scenario";
static const QString TRIGGER = "Trigger";
static const QString ACTIONS = "Actions";
static const QString ACTION = "Action";
static const QString MESSAGE = "Message";
static const QString CHANNEL = "chan";
static const QString XMLSLOT = "slot";
static const QString DESCRIPTION = "descr";
static const QString START_FROM = "from";
static const QString REPEAT = "repeat";
static const QString DELAY = "delay";

static const QString BUS = "bus";
static const QString VALUES = "values";
static const QString COMBO = "comboBox";

void GEN_xmlserializer::serialize(QString deviceID, QString currBUS, QString sourceFile, QDomDocument &xml, SysError &sysErr)
{
    qDebug() << "SERIALIZE";
    QDomElement device = xml.createElement(DEVICE);
    device.setAttribute(ID, deviceID);
    device.setAttribute(TYPE, BtGEN_dev::className());
    xml.appendChild(device);

    QDomElement xmlAttributeDIR = xml.createElement(DEVATTRIBUTE);
    xmlAttributeDIR.setAttribute(NAME, SRCFILE);
    xmlAttributeDIR.setAttribute(VALUE, sourceFile);
    xmlAttributeDIR.setAttribute(TYPE, FL);
    xmlAttributeDIR.setAttribute(EXTFILE, XML_FILE);
    device.appendChild(xmlAttributeDIR);

    QDomElement xmlAttributeBus = xml.createElement(DEVATTRIBUTE);
    xmlAttributeBus.setAttribute(NAME, BUS);
    xmlAttributeBus.setAttribute(VALUE, currBUS);
    xmlAttributeBus.setAttribute(TYPE, COMBO);
    xmlAttributeBus.setAttribute(VALUES, PlantMgr::getInstance().getBuses());
    device.appendChild(xmlAttributeBus);

    qDebug() << "SERIALIZE";


    sysErr = SysError();
}

void GEN_xmlserializer::deserialize(QDomDocument xml, QString &sourceFile, QString &currBUS, SysError &sysErr)
{
    qDebug() << "DESERIALIZE";
    qDebug() << xml.toString();

    QDomElement myXml = xml.documentElement();

    QDomNodeList deviceAttributes = myXml.elementsByTagName( DEVATTRIBUTE );
    for(int index = 0; index < deviceAttributes.count() ; index++) {

        QDomElement element = deviceAttributes.item(index).toElement();
        if (element.attributes().namedItem(NAME).nodeValue().compare(SRCFILE) == 0) {
            sourceFile = element.attributes().namedItem(VALUE).nodeValue();
        } else if (element.attributes().namedItem(NAME).nodeValue().compare(BUS) == 0) {
            currBUS = element.attributes().namedItem(VALUE).nodeValue();
        }
    }

    sysErr = SysError();
}

void GEN_xmlserializer::loadScenarios(QDomDocument xml, QMap<QString, QSharedPointer<Scenario> > &scenariosMap, SysError &sysErr) {
    qDebug() << "Load xml";
    qDebug() << xml.toString();
    qDebug() << "loadScenarios";
    QDomElement myXml = xml.documentElement();
    QDomNodeList dom_scenarios = myXml.elementsByTagName(SCENARIO);
    for(int index = 0; index < dom_scenarios.count(); index++) {

        QSharedPointer<Scenario> scenario = QSharedPointer<Scenario>(new Scenario());
        QDomElement dom_scenario = dom_scenarios.at(index).toElement();
        scenario->setName(dom_scenario.attributes().namedItem(DESCRIPTION).nodeValue());

        QDomElement dom_triggerMsg = dom_scenario.elementsByTagName(TRIGGER).at(0).toElement();
        QDomElement dom_message = dom_triggerMsg.elementsByTagName(MESSAGE).at(0).toElement();
        Message trigger_msg;
        trigger_msg.setChannel(dom_message.attributes().namedItem(CHANNEL).nodeValue());
        trigger_msg.setDelay(dom_message.attributes().namedItem(DELAY).nodeValue().toInt());
        trigger_msg.setMsgValue(dom_message.attributes().namedItem(VALUE).nodeValue());
        trigger_msg.setDescription(dom_message.attributes().namedItem(DESCRIPTION).nodeValue());
        scenario->setTrigMsg(trigger_msg);

        QDomNodeList dom_actions = dom_scenario.elementsByTagName(ACTION);
        for(int indey = 0; indey < dom_actions.count(); indey++) {

            QSharedPointer<Action> action = QSharedPointer<Action>(new Action());
            QDomElement dom_action = dom_actions.at(indey).toElement();
            action->setStartFrom(dom_action.attributes().namedItem(START_FROM).nodeValue().toInt());
            action->setRepeat(dom_action.attributes().namedItem(REPEAT).nodeValue().toInt());

            QDomNodeList dom_ActMessages = dom_action.elementsByTagName(MESSAGE);
            for(int indez = 0; indez < dom_ActMessages.count(); indez++) {

                QDomElement dom_ActMessage = dom_ActMessages.at(indez).toElement();

                QSharedPointer<Message> msg = QSharedPointer<Message>(new Message());
                msg->setChannel(dom_ActMessage.attributes().namedItem(CHANNEL).nodeValue());
                msg->setDelay(dom_ActMessage.attributes().namedItem(DELAY).nodeValue().toInt());
                msg->setMsgValue(dom_ActMessage.attributes().namedItem(VALUE).nodeValue());
                msg->setDescription(dom_ActMessage.attributes().namedItem(DESCRIPTION).nodeValue());
                action->addMessage(msg);


            }
            scenario->addAction(action);
        }

        scenariosMap.insert(trigger_msg.getMsgValue(), scenario);

    }

}
