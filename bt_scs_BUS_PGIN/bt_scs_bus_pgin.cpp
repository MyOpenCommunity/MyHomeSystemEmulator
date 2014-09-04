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

#include <bt_scs_bus_pgin.h>
#include <btscsbus.h>
#include <QtPlugin>

Bt_scs_BUS_PGIN::Bt_scs_BUS_PGIN()
{
    ;
}


QSharedPointer<PlantBus>    Bt_scs_BUS_PGIN::createBus(const QString& /*theKind*/, SysError& /*theErr*/)  {
     QSharedPointer<PlantBus> plantBusRef = QSharedPointer<PlantBus>(new Btscsbus());

     return(plantBusRef);
}

const QString Bt_scs_BUS_PGIN::getFactoryID(void) {
    return(Btscsbus::className().toStdString().c_str());
}
