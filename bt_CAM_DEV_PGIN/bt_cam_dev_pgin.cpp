#include "bt_cam_dev_pgin.h"
#include <QtPlugin>
#include <QDebug>
#include <btcam_dev.h>


Bt_CAM_DEV_PGIN::Bt_CAM_DEV_PGIN()
{
}

QSharedPointer<PlantDevice>     Bt_CAM_DEV_PGIN::createDevice(const QString& /*theKind*/, SysError& /*theErr*/)    {

    QSharedPointer<PlantDevice> deviceRef = QSharedPointer<PlantDevice>(new Btcam_dev());
    return(deviceRef);
}

const QString                    Bt_CAM_DEV_PGIN::getFactoryID(void)    {

    return(Btcam_dev::className().toStdString().c_str());
}
