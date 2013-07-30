#include "openmsg.h"
#include <openmsgcommand.h>
#include <openmsgdiagnostics.h>
#include <QDebug>

OpenMsg::OpenMsg()
{
}

bool OpenMsg::parse(QString msg, PlantMessage &rtnValue)
{
    qDebug() <<"OpenMsg Receiv e msg: " << msg;
    rtnValue.setValue(PlantMessage::OPEN, msg);
    QStringList msgSplitted = msg.remove("##").split("*", QString::SkipEmptyParts);
    //msgSplitted.removeFirst();

     qDebug() << "First message convert in integer:" << msgSplitted.first().toInt();

    if (msgSplitted.first().toInt() != 0) {
        qDebug() << "it is a command frame";
        return (OpenMsgCommand::parse(msgSplitted, rtnValue));
    }
    if (msgSplitted.first().at(0) == '#') {
        qDebug() << "it is a diagnostics frame";
        return (OpenMsgDiagnostics::parse(msgSplitted, rtnValue));
    }
    return(false);
}

bool OpenMsg::parse(PlantMessage msg, QString &rtnValue) {

    if (msg.getValues().contains(PlantMessage::WHO)) {
        if (msg.getValue(PlantMessage::WHO).toInt() != 0)
            return OpenMsgCommand::parse(msg, rtnValue);
        if (msg.getValue(PlantMessage::WHO).at(0) == '#')
            return OpenMsgDiagnostics::parse(msg, rtnValue);
    }
    return false;
}

QString OpenMsg::getFrameType(QString msg) {
    qDebug() <<"OpenMsg getFrameType: " << msg;
    QStringList msgSplitted = msg.remove("##").split("*", QString::SkipEmptyParts);

    if (msgSplitted.first().toInt() != 0) {
        qDebug() << "it is a command frame";
        return (PlantMessage::CMD);
    }
    if (msgSplitted.first().at(0) == '#') {
        qDebug() << "it is a diagnostics frame";
        return (PlantMessage::STATUS);
    }
    return "";
}
