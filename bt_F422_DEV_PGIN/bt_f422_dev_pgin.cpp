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

#include "bt_f422_dev_pgin.h"
#include <QtPlugin>
#include <btf422_dev.h>


Bt_F422_DEV_PGIN::Bt_F422_DEV_PGIN()
{
}

QSharedPointer<PlantDevice>     Bt_F422_DEV_PGIN::createDevice(const QString& /*theKind*/, SysError& /*theErr*/)    {

    QSharedPointer<PlantDevice> deviceRef = QSharedPointer<PlantDevice>(new BtF422_dev());
    return(deviceRef);
}

const QString                    Bt_F422_DEV_PGIN::getFactoryID(void)    {

    QString rtnValue = BtF422_dev::className();
    return(rtnValue.toStdString().c_str());
}
