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

#ifndef F520XMLSERIALIZER_H
#define F520XMLSERIALIZER_H

#include <QDomDocument>
#include <syserror.h>
#include <f520status.h>
//#include <f520resource.h>
#include <QSharedPointer>

class F520xmlserializer
{
public:
    static void serialize(QString deviceID, F520Status *f520, QDomDocument &xml, bool saveEnergy, SysError &sysErr);

    static void deserialize(QDomDocument xml, F520Status &f520, SysError &sysErr);

    static QString query(F520Status *f520, QString day, QString month, SysError &sysErr);

    static QStringList query(F520Status *f520, QString month, SysError &sysErr);

    static QString avarageQuery(F520Status *f520, QString month, SysError &sysErr);

    static QString lastYearQuery(F520Status *f520, QString month, SysError &sysErr);

private:

    static int getDayOfMonth(int month);

};

#endif // F520XMLSERIALIZER_H
