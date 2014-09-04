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

#include <bt_own_gtw_pgin.h>
#include <btowngtw.h>
#include <QtPlugin>

Bt_own_GTW_PGIN::Bt_own_GTW_PGIN()
{
}


QSharedPointer<PlantGateway>    Bt_own_GTW_PGIN::createGateway(const QString& /*theKind*/, SysError& /*theErr*/)    {

    QSharedPointer<PlantGateway> theGtwRef = QSharedPointer<PlantGateway>(new Btowngtw());
    return(theGtwRef);
}

const QString                    Bt_own_GTW_PGIN::getFactoryID(void) {
     return(Btowngtw::className().toStdString().c_str());;
}
