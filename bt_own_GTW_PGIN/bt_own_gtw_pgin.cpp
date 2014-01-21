#include <bt_own_gtw_pgin.h>
#include <btowngtw.h>
#include <QtPlugin>

Bt_own_GTW_PGIN::Bt_own_GTW_PGIN()
{
}


QSharedPointer<PlantGateway>    Bt_own_GTW_PGIN::createGateway(const QString& /*theKind*/, SysError& /*theErr*/)    {

    QSharedPointer<PlantGateway> theGtwRef = QSharedPointer<PlantGateway>(new Btowngtw());
    return(theGtwRef);
}

const QString                    Bt_own_GTW_PGIN::getFactoryID(void) {
     return(Btowngtw::className().toStdString().c_str());;
}
