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

