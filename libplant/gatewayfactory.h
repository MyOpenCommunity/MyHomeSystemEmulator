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

#ifndef GATEWAYFACTORY_H
#define GATEWAYFACTORY_H

#include <QtPlugin>
#include <plantgateway.h>
#include <QSharedPointer>

class GatewayFactory
{
public:
    virtual ~GatewayFactory(){}

    virtual QSharedPointer<PlantGateway>    createGateway(const QString& theKind, SysError& theErr)=0;
    virtual const QString                     getFactoryID(void)=0;
};

Q_DECLARE_INTERFACE(GatewayFactory, "it.bticino.myopen.GatewayFactoryInterface/1.0")

#endif // GATEWAYFACTORY_H
