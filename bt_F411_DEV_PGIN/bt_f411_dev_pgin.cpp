#include <bt_f411_dev_pgin.h>
#include <btf411_1_dev.h>
#include <btf411_2_dev.h>
#include <btf411_4_dev.h>
#include <QtPlugin>



Bt_F411_DEV_PGIN::Bt_F411_DEV_PGIN()
{
}

QSharedPointer<PlantDevice>     Bt_F411_DEV_PGIN::createDevice(const QString& theKind, SysError& /*theErr*/)    {

    QSharedPointer<PlantDevice> deviceRef;
    if (theKind.compare(BtF411_1_dev::className()) == 0) {
        deviceRef = QSharedPointer<PlantDevice>(new BtF411_1_dev());
    } else if (theKind.compare(BtF411_2_dev::className()) == 0) {
        deviceRef = QSharedPointer<PlantDevice>(new BtF411_2_dev());
    } else if (theKind.compare(BtF411_4_dev::className()) == 0) {
        deviceRef = QSharedPointer<PlantDevice>(new BtF411_4_dev());
    }

    return(deviceRef);
}

const QString                    Bt_F411_DEV_PGIN::getFactoryID(void)    {

    QString rtnValue = BtF411_1_dev::className() + "|" + BtF411_2_dev::className() + "|" + BtF411_4_dev::className();
    return(rtnValue.toStdString().c_str());
}
