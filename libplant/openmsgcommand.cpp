#include "openmsgcommand.h"
#include <plantmessage.h>

OpenMsgCommand::OpenMsgCommand()
{
}


bool OpenMsgCommand::parse(QStringList msg,  PlantMessage &rtnValue)
{

    if(msg.isEmpty())
        return (false);
    if(msg.length() < 2)
        return (false);

    rtnValue.setValue(PlantMessage::WHO, msg[0]);
    rtnValue.setValue(PlantMessage::WHAT, msg[1]);
    if (msg.length() > 2)
        rtnValue.setValue(PlantMessage::WHERE, msg[2]);

    return (true);
}

bool OpenMsgCommand::parse(PlantMessage msg, QString &rtnValue) {

        rtnValue.append("*");
        rtnValue.append(msg.getValue(PlantMessage::WHO));
        rtnValue.append("*");
        rtnValue.append(msg.getValue(PlantMessage::WHAT));
        rtnValue.append("*");
        rtnValue.append(msg.getValue(PlantMessage::WHERE));
        rtnValue.append("##");

    return true;
}
