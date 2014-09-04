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

#ifndef DEVICEADAPTER_H
#define DEVICEADAPTER_H

#include <QObject>
#include <QMap>

#include <devicemodel.h>

class DeviceAdapter : public QObject
{
    Q_OBJECT
public:
    explicit DeviceAdapter(QString plantName, QObject *parent = 0);
    
    bool addDevice(DeviceModel* dModel);
    bool delDevice(int idDev);
    bool setupDevice(int idDev);
    bool setupDeviceConfiguration(int idDev, QString xmlConf);
    DeviceModel* getDevice(int pos) ;

    //inline int getDeviceId(int did) { return _mapDeviceModel.values(did); };
    QWidget* getView(int did, QWidget* theView, QWidget* parent);

    QMap<int, DeviceModel*>::const_iterator begin() { return _mapDeviceModel.constBegin(); }
    QMap<int, DeviceModel*>::const_iterator end() { return _mapDeviceModel.constEnd(); }

signals:
    
public slots:
    
private:
    QMap<int,DeviceModel*> _mapDeviceModel;
    int _deviceCounter;
    QString _plantName;
};

#endif // DEVICEADAPTER_H

