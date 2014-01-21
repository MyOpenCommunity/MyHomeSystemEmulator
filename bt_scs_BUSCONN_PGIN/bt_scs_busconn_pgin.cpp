#include "bt_scs_busconn_pgin.h"
#include <QtPlugin>
#include <btscsbusconn.h>


Bt_scs_BUSCONN_PGIN::Bt_scs_BUSCONN_PGIN()
{
}

QSharedPointer<PlantBusConn>     Bt_scs_BUSCONN_PGIN::createBusConn(const QString& /*theKind*/, SysError& /*theErr*/)    {

    QSharedPointer<PlantBusConn> deviceRef = QSharedPointer<PlantBusConn>(new BtScsBusConn());
    return(deviceRef);
}

QString Bt_scs_BUSCONN_PGIN::getFactoryID(void)    {

    QString rtnValue = BtScsBusConn::className();
    return(rtnValue.toStdString().c_str());
}

