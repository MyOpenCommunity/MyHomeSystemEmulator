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


Q_EXPORT_PLUGIN2(Bt_F422_DEV_PGIN, Bt_F422_DEV_PGIN)
