#include <plantmessage.h>
#include <QStack>
#include <QMap>

const QString PlantMessage::LIGHT = "1";

const QString PlantMessage::WHO = "WHO";
const QString PlantMessage::WHERE = "WHERE";
const QString PlantMessage::WHAT = "WHAT";
const QString PlantMessage::DIMENSION = "DIMENSION";
const QString PlantMessage::DIM_VALUE = "DIM_VALUE";

const QString PlantMessage::INTERNAL = "INTERNAL";
const QString PlantMessage::EXTERNAL = "EXTERNAL";
const QString PlantMessage::DEVICE = "DEVICE";
const QString PlantMessage::GATEWAY = "GWT";
const QString PlantMessage::OPEN = "OPEN";
const QString PlantMessage::HTTP = "HTTP";
const QString PlantMessage::HTTPS = "HTTPS";
const QString PlantMessage::TCP = "TCP";
const QString PlantMessage::CMD = "CMD";
const QString PlantMessage::STATUS =  "STATUS";

PlantMessage::PlantMessage(CMD_PRIO thePrio)
{
    m_prio=thePrio;
    m_type=UNKNOW;
}

QPair<QString, QString> PlantMessage::getDestAddr(bool pop)  {
    if(m_destAddr.isEmpty())
        return(qMakePair(QString("NA"), QString("NA")));

    if(!pop)
        return(m_destAddr.top());
    else
        return(m_destAddr.pop());

}

QPair<QString, QString> PlantMessage::getSourceAddr(bool pop) {
    if(m_sourceAddr.isEmpty())
        return(qMakePair(QString("NA"), QString("NA")));

    if(!pop)
        return(m_sourceAddr.top());
    else
        return(m_sourceAddr.pop());

}

void PlantMessage::setDestAddr(const QPair<QString, QString>& theAddr, bool push) {
    if(!push)
        m_destAddr.clear();

    m_destAddr.push(theAddr);
}

void PlantMessage::setSourceAddr(const QPair<QString, QString>& theAddr, bool push)   {
    if(!push)
        m_sourceAddr.clear();

    m_sourceAddr.push(theAddr);
}

void PlantMessage::setValue(const QString& theKey, const QString& theVal)   {

    QByteArray val;
    val.append(theVal);

    QMap<QString, QByteArray>::iterator theIter = m_values.find(theKey);
    if(theIter != m_values.end())    {

        theIter.value() = val;
    }
    else {
        m_values.insert(theKey, val);
    }
}

void PlantMessage::setValue(const QString& theKey, const QByteArray& theVal)   {

    QMap<QString, QByteArray>::iterator theIter = m_values.find(theKey);
    if(theIter != m_values.end())    {
        theIter.value() = theVal;
    }
    else {
        m_values.insert(theKey, theVal);
    }
}

void PlantMessage::setValues(const QMap<QString, QByteArray>& theValues)   {
    m_values = theValues;
}

const QMap<QString, QByteArray>& PlantMessage::getValues(void) {
    return(m_values);
}

QString PlantMessage::getValue(const QString &theKey) {
    QString retVal;

    QMap<QString, QByteArray>::const_iterator theIter = m_values.find(theKey);
    if(theIter != m_values.end())    {
        retVal = QString(theIter.value());
    }

    return(retVal);
}

QByteArray PlantMessage::getValueByteArray(const QString &theKey) {
    QByteArray retVal;

    QMap<QString, QByteArray>::const_iterator theIter = m_values.find(theKey);
    if(theIter != m_values.end())    {
        retVal = theIter.value();
    }

    return(retVal);
}

PlantMessage::CMD_PRIO PlantMessage::getPrio(void)    {
    return(m_prio);
}

PlantMessage::CMD_TYPE PlantMessage::getType(void) {
    return(m_type);
}

void PlantMessage::setType(PlantMessage::CMD_TYPE type) {
    m_type = type;
}

void PlantMessage::setType(QString type)
{
    if (type.compare(PlantMessage::HTTPS) == 0)
        m_type = PlantMessage::HTTPS_BACK;
    else if (type.compare(PlantMessage::HTTP) == 0)
        m_type = PlantMessage::HTTP_BACK;
    else
        m_type = PlantMessage::UNKNOW;
}

void PlantMessage::setId(int id) {
    m_id = id;
}

int PlantMessage::getId() {
    return(m_id);
}
