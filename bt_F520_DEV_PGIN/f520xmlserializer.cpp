#include "f520xmlserializer.h"
#include <btf520_dev.h>
#include <QFile>
#include <QDebug>
#include <QXmlQuery>
#include <QStringList>

static const QString DEVICE = "device";
static const QString ID = "id";
static const QString TYPE = "type";
static const QString XML_SRC = "../myhome/Resources/";
static const QString FILE_NAME = "energy_configuration.xml";
static const QString CARICO = "carico";
static const QString DEFAULT_DAY = "default_day";
static const QString DEFAULT_MONTH = "default_month";
static const QString DAYS = "days";
static const QString MONTHS = "months";
static const QString LAST_YEAR_DAYS = "last_year_days";
static const QString LAST_YEAR_MONTHS = "last_year_months";
static const QString DAY = "day";
static const QString MONTH_VALUE = "month_value";
static const QString DAY_VALUE = "day_value";
static const QString MONTH = "month";
static const QString WATT = "watt";
static const QString NAME = "name";
static const QString F520_DEFAULT = "F520_First";
static const QString WHERE = "where";
static const QString MAX = "max";
static const QString MIN = "min";
static const QString ATTRIBUTE = "devattribute";
static const QString VALUE = "value";
static const QString SRC = "src";
static const QString MAX_ENERGY_VALUE = "max_nrg";
static const QString IS_DEFAUT = "is_default";


static const QString DEFAULT_DAY_VALUE = "0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0";
static const QString DEFAULT_MOUNT_VALUE = "0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0";

bool createNewXml(QSharedPointer<QFile> inFile) {
    if (!inFile->open( QIODevice::ReadWrite )) {
        qDebug() << "Can't create file " << inFile->fileName();
        return false;
    }
    inFile->resize(0);

    QDomDocument xml;
    QDomElement carico = xml.createElement(CARICO);
    carico.setAttribute(NAME, F520_DEFAULT);
    xml.appendChild(carico);

    QDomElement days = xml.createElement(DAYS);
    QDomElement months = xml.createElement(MONTHS);
    carico.appendChild(days);
    carico.appendChild(months);

    QDomElement defaultDay = xml.createElement(DAY);
    defaultDay.setAttribute(NAME, DEFAULT_DAY);
    defaultDay.setAttribute(WATT, DEFAULT_DAY_VALUE);
    defaultDay.setAttribute(DAY_VALUE, "0");
    defaultDay.setAttribute(MONTH_VALUE, "0");
    defaultDay.setAttribute(IS_DEFAUT, "1");
    days.appendChild(defaultDay);

    QDomElement defaultMounth = xml.createElement(MONTH);
    defaultMounth.setAttribute(NAME, DEFAULT_MONTH);
    defaultMounth.setAttribute(WATT, DEFAULT_MOUNT_VALUE);
    defaultMounth.setAttribute(MONTH_VALUE, "0");
    defaultMounth.setAttribute(IS_DEFAUT, "1");
    months.appendChild(defaultMounth);

    QDomElement lastYearDays = xml.createElement(LAST_YEAR_DAYS);
    QDomElement lastYearMonths = xml.createElement(LAST_YEAR_MONTHS);
    carico.appendChild(lastYearDays);
    carico.appendChild(lastYearMonths);

    defaultDay = xml.createElement(DAY);
    defaultDay.setAttribute(NAME, DEFAULT_DAY);
    defaultDay.setAttribute(WATT, DEFAULT_DAY_VALUE);
    defaultDay.setAttribute(DAY_VALUE, "0");
    defaultDay.setAttribute(MONTH_VALUE, "0");
    defaultDay.setAttribute(IS_DEFAUT, "1");
    lastYearDays.appendChild(defaultDay);

    defaultMounth = xml.createElement(MONTH);
    defaultMounth.setAttribute(NAME, DEFAULT_MONTH);
    defaultMounth.setAttribute(WATT, DEFAULT_MOUNT_VALUE);
    defaultMounth.setAttribute(MONTH_VALUE, "0");
    defaultMounth.setAttribute(IS_DEFAUT, "1");
    lastYearMonths.appendChild(defaultMounth);

    inFile->write(xml.toString().toStdString().c_str());

    inFile->close();

    return true;
}

void F520xmlserializer::serialize(QString deviceID, F520Status *f520, QDomDocument &xml, bool saveEnergy, SysError &sysErr) {
    QDomElement device = xml.createElement(DEVICE);
    device.setAttribute(ID, deviceID);
    device.setAttribute(TYPE, BtF520_dev::className());

    QDomElement xmlWhere = xml.createElement(ATTRIBUTE);
    xmlWhere.setAttribute(NAME, WHERE);
    xmlWhere.setAttribute(VALUE, f520->getWhere());
    xmlWhere.setAttribute(MAX, F520Status::getMaxWhere());
    xmlWhere.setAttribute(MIN, F520Status::getMinWhere());
    device.appendChild(xmlWhere);

    QDomElement xmlSrc = xml.createElement(ATTRIBUTE);
    xmlSrc.setAttribute(NAME, SRC);
    xmlSrc.setAttribute(VALUE, f520->getXmlPath());
    device.appendChild(xmlSrc);

    if (saveEnergy) {
        QSharedPointer<QFile> inFile;
        qDebug() << "-" << f520->getXmlPath() << "-" << (!f520->getXmlPath().compare("")) << " " << QFile::exists(f520->getXmlPath());
        if (!f520->getXmlPath().compare("") ) {
            f520->setXmlPath(XML_SRC + deviceID + "_" + FILE_NAME);
        }
        if (!QFile::exists(f520->getXmlPath())) {
            inFile = QSharedPointer<QFile>( new QFile(f520->getXmlPath()));
            if( ! inFile->open( QIODevice::ReadOnly ) )
            {
                qDebug() << f520->getXmlPath() << " Not exist";
                if (!createNewXml(inFile)) {
                    sysErr = SysError(SysError::F520_CAN_NOT_OPEN_FILE, "F520 can not open or create xml resource file");
                    return;
                }
            }
        } else {
            inFile = QSharedPointer<QFile>( new QFile(f520->getXmlPath()));
        }
        xmlSrc.setAttribute(VALUE, f520->getXmlPath());
        inFile->open(QIODevice::ReadOnly);
        QDomDocument f520Xml;
        if (!f520Xml.setContent(inFile->readAll()))
        {
            inFile->close();
            sysErr = SysError(SysError::F520_XML_PARSER_ERROR, "F520 Resource file is incorrect");
            return;
        }
        inFile->close();
        qDebug() << "F520xmlserializer::serialize";
        qDebug() << f520Xml.toString();
        device.appendChild(f520Xml.documentElement());

    }

    QDomElement xmlMaxnrg = xml.createElement(ATTRIBUTE);
    xmlMaxnrg.setAttribute(NAME, MAX_ENERGY_VALUE);
    xmlMaxnrg.setAttribute(VALUE, f520->getMaxNrgValue());
    device.appendChild(xmlMaxnrg);


    xml.appendChild(device);
}

void F520xmlserializer::deserialize(QDomDocument xml, F520Status &f520, SysError &sysErr) {

    qDebug() << "F520xmlserializer -> deserialize";


    QDomNodeList attributeNodes = xml.elementsByTagName( ATTRIBUTE );

    for(unsigned indey = 0; indey < attributeNodes.length(); indey++) {
        QDomElement element = attributeNodes.at(indey).toElement();
        qDebug() << "Element Name: " << element.attributes().namedItem(NAME).nodeValue();
        if (element.attributes().namedItem(NAME).nodeValue().compare(WHERE) == 0) {
            qDebug() << "Element Value: " << element.attributes().namedItem(VALUE).nodeValue();
            f520.setWhere(element.attributes().namedItem(VALUE).nodeValue());
        } else if (element.attributes().namedItem(NAME).nodeValue().compare(SRC) == 0) {
            f520.setXmlPath(element.attributes().namedItem(VALUE).nodeValue());
        } else if (element.attributes().namedItem(NAME).nodeValue().compare(MAX_ENERGY_VALUE) == 0) {
            f520.setMaxNrgValue(element.attributes().namedItem(VALUE).nodeValue());
        }
    }

    QDomNodeList carico = xml.elementsByTagName( CARICO );

    if (carico.count() < 1) {
        sysErr = SysError(SysError::F520_RESOUCES_NOT_FOUND, "F520 Can not find resources");
        return;
    }

    QDomDocument document;

    QDomNode tmp = document.importNode(carico.item(0), true);
    document.appendChild(tmp);

    qDebug() << document.toString();

    QFile inFile( f520.getXmlPath() );
    inFile.open( QIODevice::ReadWrite );
    inFile.resize(0);

    inFile.write(document.toString().toStdString().c_str());

    inFile.close();
}

QString F520xmlserializer::query(F520Status *f520, QString day, QString month, SysError &sysErr) {
    QXmlQuery query;
    QString res;
    QFile xml(f520->getXmlPath());
    if ( ! xml.exists()) {
        xml.setFileName(f520->getXmlPath());
        if ( ! xml.exists()) {
            sysErr = SysError(SysError::F520_CAN_NOT_OPEN_FILE, "F520 Rasource file not exist");
            return "";
        }
    }

    if (!xml.open(QIODevice::ReadOnly | QIODevice::Text)) {
        sysErr = SysError(SysError::F520_RESOUCES_NOT_EXIST, "F520 can not open xml resource file");
        return "";
    }

    QString queryStr = CARICO + "/" + DAYS + "/" + DAY + "[@" + DAY_VALUE + "='" + day + "'][@" + MONTH_VALUE + "='" + month + "']";
    qDebug() << "F520xmlserializer::query: " << queryStr;

    query.setFocus(&xml);
    query.setQuery(queryStr);
    if ( ! query.isValid()) {
        qDebug() << "F520xmlserializer::query query not valid";
        sysErr = SysError(SysError::F520_QUERY_ERROR, "F520 Query is invalid: " + queryStr);
        return "";
    }

    query.evaluateTo(&res);

    if (!(res.length() > 1)) {
        queryStr = CARICO + "/" + DAYS + "/" + DAY + "[@" + NAME + "='" + DEFAULT_DAY + "']";
        qDebug() << "F520xmlserializer::query: " << queryStr;
        query.setQuery(queryStr);
        if (!query.isValid()) {
            qDebug() << "F520xmlserializer::query query not valid";
            sysErr = SysError(SysError::F520_QUERY_ERROR, "F520 Query is invalid: " + queryStr);
            return "";
        }
        query.evaluateTo(&res);
    }

    xml.close();

    qDebug() << "F520xmlserializer::query " << res;

    QDomDocument watt;
    watt.setContent("" + res + "");
    QDomNodeList entryNodes = watt.elementsByTagName(DAY);

    QString rtn;
    for (int i = 0; i < entryNodes.count(); i++) {
        QDomElement node = entryNodes.at(i).toElement();
        rtn = node.attribute(WATT);
        break;
    }
    qDebug() << "F520xmlserializer::query " << rtn;
    return rtn;
}

QStringList F520xmlserializer::query(F520Status *f520, QString month, SysError &sysErr) {
    QXmlQuery query;
    QString res;
    QFile xml(f520->getXmlPath());
    if ( ! xml.exists()) {
        xml.setFileName(f520->getXmlPath());
        if ( ! xml.exists()) {
            sysErr = SysError(SysError::F520_CAN_NOT_OPEN_FILE, "F520 Rasource file not exist");
            return QStringList();
        }
    }

    if (!xml.open(QIODevice::ReadOnly | QIODevice::Text)) {
        sysErr = SysError(SysError::F520_RESOUCES_NOT_EXIST, "F520 can not open xml resource file");
        return QStringList();
    }

    QString queryStr = CARICO + "/" + MONTHS + "/" + MONTH + "[@" + MONTH_VALUE + "='" + month + "']";
    qDebug() << "F520xmlserializer::query: " << queryStr;

    query.setFocus(&xml);
    query.setQuery(queryStr);
    if ( ! query.isValid()) {
        qDebug() << "F520xmlserializer::query query not valid";
        sysErr = SysError(SysError::F520_QUERY_ERROR, "F520 Query is invalid: " + queryStr);
        return QStringList();
    }

    query.evaluateTo(&res);

    if (!(res.length() > 1)) {
        queryStr = CARICO + "/" + MONTHS + "/" + MONTH + "[@" + NAME + "='" + DEFAULT_MONTH + "']";
        qDebug() << "F520xmlserializer::query: " << queryStr;
        query.setQuery(queryStr);
        if (!query.isValid()) {
            qDebug() << "F520xmlserializer::query query not valid";
            sysErr = SysError(SysError::F520_QUERY_ERROR, "F520 Query is invalid: " + queryStr);
            return QStringList();
        }
        query.evaluateTo(&res);
    }

    xml.close();

    qDebug() << "F520xmlserializer::query " << res;

    QDomDocument watt;
    watt.setContent("" + res + "");
    QDomNodeList entryNodes = watt.elementsByTagName(MONTH);

    QStringList rtn;
    for (int i = 0; i < entryNodes.count(); i++) {
        QDomElement node = entryNodes.at(i).toElement();
        rtn = node.attribute(WATT).split('.');
        break;
    }
    //qDebug() << "F520xmlserializer::query " << rtn;

    for(int index = rtn.count(); index < 31; index++) {
        rtn.append("0");
    }

    return rtn;
}

QString F520xmlserializer::avarageQuery(F520Status *f520, QString month, SysError &sysErr) {
    QXmlQuery query;
    QString res;
    QString defaultRes;
    QFile xml(f520->getXmlPath());
    if ( ! xml.exists()) {
        xml.setFileName(f520->getXmlPath());
        if ( ! xml.exists()) {
            sysErr = SysError(SysError::F520_CAN_NOT_OPEN_FILE, "F520 Rasource file not exist");
            return "";
        }
    }

    if (!xml.open(QIODevice::ReadOnly | QIODevice::Text)) {
        sysErr = SysError(SysError::F520_RESOUCES_NOT_EXIST, "F520 can not open xml resource file");
        return "";
    }

    QString queryStr = CARICO + "/" + DAYS + "/" + DAY + "[@" + MONTH_VALUE + "='" + month + "']";
    qDebug() << "F520xmlserializer::query: " << queryStr;

    query.setFocus(&xml);
    query.setQuery(queryStr);
    if ( ! query.isValid()) {
        qDebug() << "F520xmlserializer::query query not valid";
        sysErr = SysError(SysError::F520_QUERY_ERROR, "F520 Query is invalid: " + queryStr);
        return "";
    }

    query.evaluateTo(&res);

    queryStr = CARICO + "/" + DAYS + "/" + DAY + "[@" + NAME + "='" + DEFAULT_DAY + "']";
    qDebug() << "F520xmlserializer::query: " << queryStr;
    query.setQuery(queryStr);
    if (!query.isValid()) {
        qDebug() << "F520xmlserializer::query query not valid";
        sysErr = SysError(SysError::F520_QUERY_ERROR, "F520 Query is invalid: " + queryStr);
        return "";
    }
    query.evaluateTo(&defaultRes);

    qDebug() << "F520xmlserializer::query " << res;

    xml.close();

    QDomDocument avarageWatt;
    res.prepend("<" +DAYS +">\n");
    res.append("<\\" + DAYS + ">");
    avarageWatt.setContent("" + res + "");
    QDomNodeList entryNodes = avarageWatt.elementsByTagName(DAY);

    QDomDocument defaultWatt;
    defaultWatt.setContent("" + defaultRes + "");
    QDomNodeList entryDefaultNodes = defaultWatt.elementsByTagName(DAY);


    QList<float> avarage;

    qDebug() << "F520xmlserializer::query avarageWatt: " << defaultWatt.toString();
    int i;
    for (i = 0; i < getDayOfMonth(month.toInt()); i++) {
        QDomElement node;
        if(i < entryNodes.count())
            node = entryNodes.at(i).toElement();
        else
            node = entryDefaultNodes.at(0).toElement();

        QStringList tmpList = node.attribute(WATT).split('.');
        if (tmpList.length() != 24) {
            sysErr = SysError(SysError::F520_RESOURCE_ERROR, "F520 Resouce Error");
            return "";
        }
        for (int index = 0; index < tmpList.length(); index++) {
            if (avarage.length() <= index) {
                avarage.append(tmpList.at(index).toFloat()  / getDayOfMonth(month.toInt()));
            } else {
                avarage[index] += (tmpList.at(index).toFloat() / getDayOfMonth(month.toInt()));
            }
        }
    }

    QString rtn;
    foreach (int element, avarage) {
        rtn += QString::number(element) + '.';
    }

    return rtn.remove(rtn.length()-1, 1);
}

QString F520xmlserializer::lastYearQuery(F520Status *f520, QString month, SysError &sysErr) {

    QXmlQuery query;
    QString res;
    QString lastYearRes;
    QFile xml(f520->getXmlPath());
    if ( ! xml.exists()) {
        xml.setFileName(f520->getXmlPath());
        if ( ! xml.exists()) {
            sysErr = SysError(SysError::F520_CAN_NOT_OPEN_FILE, "F520 Rasource file not exist");
            return "";
        }
    }

    if (!xml.open(QIODevice::ReadOnly | QIODevice::Text)) {
        sysErr = SysError(SysError::F520_RESOUCES_NOT_EXIST, "F520 can not open xml resource file");
        return "";
    }

    QString queryStr = CARICO + "/" + MONTHS + "/" + MONTH + "[@" + MONTH_VALUE + "='" + month + "']";
    qDebug() << "F520xmlserializer::query: " << queryStr;

    query.setFocus(&xml);
    query.setQuery(queryStr);
    if ( ! query.isValid()) {
        qDebug() << "F520xmlserializer::query query not valid";
        sysErr = SysError(SysError::F520_QUERY_ERROR, "F520 Query is invalid: " + queryStr);
        return "";
    }

    query.evaluateTo(&res);

    if (!(res.length() > 1)) {
        queryStr = CARICO + "/" + MONTHS + "/" + MONTH + "[@" + NAME + "='" + DEFAULT_MONTH + "']";
        qDebug() << "F520xmlserializer::query: " << queryStr;
        query.setQuery(queryStr);
        if (!query.isValid()) {
            qDebug() << "F520xmlserializer::query query not valid";
            sysErr = SysError(SysError::F520_QUERY_ERROR, "F520 Query is invalid: " + queryStr);
            return "";
        }
        query.evaluateTo(&res);
    }

    queryStr = CARICO + "/" + LAST_YEAR_MONTHS + "/" + MONTH + "[@" + MONTH_VALUE + "='" + month + "']";
    qDebug() << "F520xmlserializer::query: " << queryStr;

    query.setQuery(queryStr);
    if ( ! query.isValid()) {
        qDebug() << "F520xmlserializer::query query not valid";
        sysErr = SysError(SysError::F520_QUERY_ERROR, "F520 Query is invalid: " + queryStr);
        return "";
    }

    query.evaluateTo(&lastYearRes);

    if (!(lastYearRes.length() > 1)) {
        queryStr = CARICO + "/" + LAST_YEAR_MONTHS + "/" + MONTH + "[@" + NAME + "='" + DEFAULT_MONTH + "']";
        qDebug() << "F520xmlserializer::query: " << queryStr;
        query.setQuery(queryStr);
        if (!query.isValid()) {
            qDebug() << "F520xmlserializer::query query not valid";
            sysErr = SysError(SysError::F520_QUERY_ERROR, "F520 Query is invalid: " + queryStr);
            return "";
        }
        query.evaluateTo(&lastYearRes);
    }

    xml.close();

    qDebug() << "F520xmlserializer::query " << res;
    qDebug() << "F520xmlserializer::query " << lastYearRes;

    QDomDocument watt;
    watt.setContent("" + res + "");
    QDomNodeList entryNodes = watt.elementsByTagName(MONTH);
    QDomDocument lastYearWatt;
    lastYearWatt.setContent("" + lastYearRes + "");
    QDomNodeList lastYearEntryNodes = lastYearWatt.elementsByTagName(MONTH);

    QString rtn;
    QList<int> diff;
    for (int i = 0; i < entryNodes.count(); i++) {
        QDomElement lastYearNode = lastYearEntryNodes.at(i).toElement();
        QDomElement node = entryNodes.at(i).toElement();
        QStringList tmpList = node.attribute(WATT).split('.');
        QStringList lastTmpList = lastYearNode.attribute(WATT).split('.');
        for (int index = 0; index < tmpList.length(); index++) {
            diff.append(tmpList.at(index).toInt()  - lastTmpList.at(index).toInt());
        }
        break;
    }

    foreach (int element, diff) {
        rtn += QString::number(element) + '.';
    }

    qDebug() << "F520xmlserializer::query " << rtn;
    return rtn.remove(rtn.length()-1, 1);
}

int F520xmlserializer::getDayOfMonth(int month) {
    switch (month) {
    case 11:
    case 4:
    case 6:
    case 9:
    {
        return 30;
    }
        break;
    case 2:
    {
        return 28;
    }
        break;

    default:
        return 31;
        break;
    }
}
