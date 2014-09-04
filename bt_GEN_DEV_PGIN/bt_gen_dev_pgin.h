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

#ifndef BT_GEN_DEV_PGIN_H
#define BT_GEN_DEV_PGIN_H

#include <bt_gen_DEV_PGIN_global.h>
#include <devicefactory.h>

// Genric devices plugin
class Bt_GEN_DEV_PGIN : public QObject, public DeviceFactory{

    Q_OBJECT
    Q_INTERFACES(DeviceFactory)
    Q_PLUGIN_METADATA(IID "it.bticino.myopen.DeviceFactoryInterface/1.0")

public:
    Bt_GEN_DEV_PGIN();
    ~Bt_GEN_DEV_PGIN(){}

    QSharedPointer<PlantDevice>         createDevice(const QString& theKind, SysError& theErr);
    const QString                       getFactoryID(void);

};

#endif // BT_GEN_DEV_PGIN_H
