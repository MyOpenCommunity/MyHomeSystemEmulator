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

#include "bt_scs_busconn_pgin.h"
#include <QtPlugin>
#include <btscsbusconn.h>


Bt_scs_BUSCONN_PGIN::Bt_scs_BUSCONN_PGIN()
{
}

QSharedPointer<PlantBusConn>     Bt_scs_BUSCONN_PGIN::createBusConn(const QString& /*theKind*/, SysError& /*theErr*/)    {

    QSharedPointer<PlantBusConn> deviceRef = QSharedPointer<PlantBusConn>(new BtScsBusConn());
    return(deviceRef);
}

QString Bt_scs_BUSCONN_PGIN::getFactoryID(void)    {

    QString rtnValue = BtScsBusConn::className();
    return(rtnValue.toStdString().c_str());
}

