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

#ifndef F411XMLSERIALIZER_H
#define F411XMLSERIALIZER_H

#include <QDomDocument>
#include <QMap>
#include <ledstatus.h>
#include <syserror.h>
#include <QSharedPointer>

class F411xmlserializer
{
public:
    static void serialize(QString deviceID, QMap<QString, QSharedPointer<LedStatus> > leds, QString type, QString mode, QString currBUS, QDomDocument &xml, SysError &sysErr);

    static void deserialize(QDomDocument xml, QMap<QString, QSharedPointer<LedStatus> > &leds, QString &mode, QString &currBUS, SysError &sysErr);

};

#endif // F411XMLSERIALIZER_H
