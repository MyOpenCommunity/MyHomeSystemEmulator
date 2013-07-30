#include "btf411_1_dev.h"
#include <QDebug>

static const QString RESOURCE_XML = "F411_1.xml";

BtF411_1_dev::BtF411_1_dev()
{
    addLed(QSharedPointer<LedStatus>(new LedStatus("L1")));
    setMode(LedStatus::NO_PUL);
    setResoucesFileName(RESOURCE_XML);
}

BtF411_1_dev::~BtF411_1_dev() {
    qDebug() << getDeviceID() << " ~btf411_1_dev ";
}
