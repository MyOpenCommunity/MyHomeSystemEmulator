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
