#include <QtPlugin>
#include <QDebug>
#include <bt_gen_dev_pgin.h>
#include <bt_gen_dev.h>




Bt_GEN_DEV_PGIN::Bt_GEN_DEV_PGIN()
{
}

QSharedPointer<PlantDevice>     Bt_GEN_DEV_PGIN::createDevice(const QString& theKind, SysError& theErr)    {

    QSharedPointer<PlantDevice> deviceRef = QSharedPointer<PlantDevice>(new BtGEN_dev());
    return(deviceRef);
}

const QString                    Bt_GEN_DEV_PGIN::getFactoryID(void)    {

    return(BtGEN_dev::className().toStdString().c_str());
}
