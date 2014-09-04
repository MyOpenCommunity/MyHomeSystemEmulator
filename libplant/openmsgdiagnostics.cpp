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

#include "openmsgdiagnostics.h"

OpenMsgDiagnostics::OpenMsgDiagnostics()
{
}

bool OpenMsgDiagnostics::parse(QStringList msg,  PlantMessage &rtnValue)
{


    if(msg.isEmpty()) {
        // Error message frame is empty
        qDebug() << "Error message frame is empty";
        return (false);
    }
    if(msg.length() < 1) {
        // Error Message contain to few elements
        qDebug() << "Error Message contain to few elements";
        return (false);
    }

    if(msg.length() > 1) {
        // The message contain two or more elements, frame can be a status or dimension frame
        qDebug() << "Status or dimension frame" ;
        rtnValue.setValue(PlantMessage::WHO, msg[0]);
        rtnValue.setValue(PlantMessage::WHERE, msg[1]);
    }
    if(msg.length() > 2) {
        // The message contain more then two element, frame can be a dimension frame
        qDebug() << "Dimension frame" ;
        rtnValue.setValue(PlantMessage::DIMENSION, msg[2]);
        for (int index = 1; index + 2 < msg.length(); index++) {
            rtnValue.setValue(PlantMessage::DIM_VALUE + index, msg[index+2]);
        }
    }
    return (true);

}

bool OpenMsgDiagnostics::parse(PlantMessage msg, QString &rtnValue) {

    if (!(msg.getValues().contains(PlantMessage::WHO)) && !(msg.getValues().contains(PlantMessage::WHERE))) {
        // Plant Message not well formed there are not WHO and WHERE tags
        qDebug() <<  "Plant Message not well formed";
        return(false);
    }

    rtnValue.append("*");
    rtnValue.append(msg.getValue(PlantMessage::WHO));
    rtnValue.append("*");
    rtnValue.append(msg.getValue(PlantMessage::WHERE));

    if (msg.getValues().contains(PlantMessage::DIMENSION)){
        // Plant Message contain tag dimension
        qDebug() <<  "Plant Message contain tag dimension";
        rtnValue.append("*");
        rtnValue.append(msg.getValue(PlantMessage::DIMENSION));
        int index = 1;
        while (msg.getValues().contains(PlantMessage::DIM_VALUE + index)) {
            // Plant Message contain some dimensions value
            qDebug() <<  "Plant Message contain dimension value number: " + index;
            rtnValue.append("*");
            rtnValue.append(msg.getValue(PlantMessage::DIM_VALUE + index));
            index++;
        }
    }


    rtnValue.append("##");


    qDebug() << "Diagnostic frame TODO";
    return true;
}
