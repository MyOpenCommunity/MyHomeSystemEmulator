#include "btf411_2_dev.h"
#include <QDebug>

static const QString RESOURCE_XML = "F411_2.xml";

BtF411_2_dev::BtF411_2_dev()
{

    addLed(QSharedPointer<LedStatus>(new LedStatus("L1")));
    addLed(QSharedPointer<LedStatus>(new LedStatus("L2")));
    setMode(LedStatus::NO_PUL);
    setResoucesFileName(RESOURCE_XML);

}

BtF411_2_dev::~BtF411_2_dev() {
    qDebug() << getDeviceID() << " ~btf411_1_dev ";
}
