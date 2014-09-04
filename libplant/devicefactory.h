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

#ifndef DEVICEFACTORY_H
#define DEVICEFACTORY_H

#include <QSharedPointer>
#include <plantdevice.h>

class DeviceFactory
{
public:
    virtual ~DeviceFactory(){}

    virtual QSharedPointer<PlantDevice>     createDevice(const QString& theKind, SysError& theErr)=0;
    virtual const QString                     getFactoryID(void)=0;
};

Q_DECLARE_INTERFACE(DeviceFactory, "it.bticino.myopen.DeviceFactoryInterface/1.0")

#endif // DEVICEFACTORY_H
