#include "btf411_4_dev.h"
#include <QDebug>

static const QString RESOURCE_XML = "F411_4.xml";

BtF411_4_dev::BtF411_4_dev()
{

    addLed(QSharedPointer<LedStatus>(new LedStatus("L1")));
    addLed(QSharedPointer<LedStatus>(new LedStatus("L2")));
    addLed(QSharedPointer<LedStatus>(new LedStatus("L3")));
    addLed(QSharedPointer<LedStatus>(new LedStatus("L4")));
    setMode(LedStatus::NO_PUL);
    setResoucesFileName(RESOURCE_XML);

}

BtF411_4_dev::~BtF411_4_dev() {
    qDebug() << getDeviceID() << " ~btf411_1_dev ";
}
