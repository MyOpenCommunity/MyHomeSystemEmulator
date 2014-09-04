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

#ifndef GEN_XMLSERIALIZER_H
#define GEN_XMLSERIALIZER_H

#include <QDomDocument>
#include <QMap>
#include <syserror.h>
#include <QSharedPointer>
#include <scenario.h>
#include <QString>

class GEN_xmlserializer
{
public:

    static void serialize(QString deviceID, QString currBUS, QString sourceFile, QDomDocument &xml, SysError &sysErr);

    static void deserialize(QDomDocument xml, QString &sourceFile, QString &currBUS, SysError &sysErr);

    static void loadScenarios(QDomDocument xml, QMap<QString, QSharedPointer<Scenario> > &scenariosMap, SysError &sysErr);

    static const QString getBuses();

};

#endif // GEN_XMLSERIALIZER_H
