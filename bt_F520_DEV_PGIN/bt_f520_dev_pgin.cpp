#include "bt_f520_dev_pgin.h"
#include <QtPlugin>
#include <plantdevice.h>
#include <btf520_dev.h>

Bt_F520_DEV_PGIN::Bt_F520_DEV_PGIN()
{
}

QSharedPointer<PlantDevice>     Bt_F520_DEV_PGIN::createDevice(const QString& /*theKind*/, SysError& /*theErr*/)    {

    QSharedPointer<PlantDevice> deviceRef = QSharedPointer<PlantDevice>(new BtF520_dev());
    return(deviceRef);
}

const QString                    Bt_F520_DEV_PGIN::getFactoryID(void)    {

    QString rtnValue = BtF520_dev::className();
    return(rtnValue.toStdString().c_str());
}
