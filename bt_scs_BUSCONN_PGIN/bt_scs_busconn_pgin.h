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

#ifndef BT_SCS_BUSCONN_PGIN_H
#define BT_SCS_BUSCONN_PGIN_H

#include "bt_scs_BUSCONN_PGIN_global.h"
#include <busconnfactory.h>
#include <plantbus.h>

class Bt_scs_BUSCONN_PGIN: public QObject, public BusConnFactory {

public:

    Q_OBJECT
    Q_INTERFACES(BusConnFactory)
    Q_PLUGIN_METADATA(IID "it.bticino.myopen.BusConnFactoryInterface/1.0")
public:
    Bt_scs_BUSCONN_PGIN();
    ~Bt_scs_BUSCONN_PGIN(){}

    QSharedPointer<PlantBusConn> createBusConn(const QString& theKind, SysError& theErr);
    QString                getFactoryID(void);


};

#endif // BT_SCS_BUSCONN_PGIN_H
