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
    days.appendChild(defaultDay);

    QDomElement defaultMounth = xml.createElement(MONTH);
    defaultMounth.setAttribute(NAME, DEFAULT_MONTH);
    defaultMounth.setAttribute(WATT, DEFAULT_MOUNT_VALUE);
    defaultMounth.setAttribute(MONTH_VALUE, "0");
    months.appendChild(defaultMounth);

    inFile->write(xml.toString().toStdString().c_str());

    inFile->close();

    return true;
}

void F520xmlserializer::serialize(QString deviceID, QSharedPointer<F520Status> f520, QDomDocument &xml, bool saveEnergy, SysError &sysErr) {
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
        qDebug() << (!f520->getXmlPath().compare("")) << " " << QFile::exists(f520->getXmlPath());
        if (f520->getXmlPath().compare("") ) {
            f520->setXmlPath(XML_SRC + deviceID + "_" + FILE_NAME);
        }
        if (!QFile::exists(f520->getXmlPath())) {
            inFile = QSharedPointer<QFile>( new QFile(f520->getXmlPath()));
            if( ! inFile->open( QIODevice::ReadOnly ) )
            {
                qDebug() << f520->getXmlPath() << " Not exist";
                if (!createNewXml(inFile))
                    return;
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
            return;
        }
        inFile->close();
        qDebug() << "F520xmlserializer::serialize";
        qDebug() << f520Xml.toString();
        device.appendChild(f520Xml.documentElement());

    }

    xml.appendChild(device);
}

void F520xmlserializer::deserialize(QDomDocument xml, QSharedPointer<F520Status> &f520, SysError &sysErr) {

    qDebug() << "F520xmlserializer -> deserialize";


    QDomNodeList attributeNodes = xml.elementsByTagName( ATTRIBUTE );

    for(unsigned indey = 0; indey < attributeNodes.length(); indey++) {
        QDomElement element = attributeNodes.at(indey).toElement();
        qDebug() << "Element Name: " << element.attributes().namedItem(NAME).nodeValue();
        if (element.attributes().namedItem(NAME).nodeValue().compare(WHERE) == 0) {
            qDebug() << "Element Value: " << element.attributes().namedItem(VALUE).nodeValue();
            f520->setWhere(element.attributes().namedItem(VALUE).nodeValue());
        } else if (element.attributes().namedItem(NAME).nodeValue().compare(SRC) == 0) {
            f520->setXmlPath(element.attributes().namedItem(VALUE).nodeValue());
        }
    }

    QDomNodeList carico = xml.elementsByTagName( CARICO );

    if (carico.count() < 1) {
        return;
    }

    QDomDocument document;

    QDomNode tmp = document.importNode(carico.item(0), true);
    document.appendChild(tmp);

    qDebug() << document.toString();

    QFile inFile( f520->getXmlPath() );
    inFile.open( QIODevice::ReadWrite );
    inFile.resize(0);

    inFile.write(document.toString().toStdString().c_str());

    inFile.close();
}

QString F520xmlserializer::query(QSharedPointer<F520Status> f520, QString day, QString month, SysError &sysErr) {
    QXmlQuery query;
    QString res;
    QFile xml(f520->getXmlPath());
    if ( ! xml.exists()) {
        xml.setFileName(f520->getXmlPath());
        if ( ! xml.exists()) {
            sysErr = SysError();
            return "";
        }
    }

    if (!xml.open(QIODevice::ReadOnly | QIODevice::Text)) {
        sysErr = SysError();
        return "";
    }

    qDebug() << "F520xmlserializer::query: " << CARICO + "/" + DAYS + "/" + DAY + "[@" + DAY_VALUE + "='" + day + "'][@" + MONTH_VALUE + "='" + month + "']";

    query.setFocus(&xml);
    query.setQuery(CARICO + "/" + DAYS + "/" + DAY + "[@" + DAY_VALUE + "='" + day + "'][@" + MONTH_VALUE + "='" + month + "']");
    if ( ! query.isValid()) {
        qDebug() << "F520xmlserializer::query query not valid";
        sysErr = SysError();
        return "";
    }

    query.evaluateTo(&res);

    if (!(res.length() > 1)) {
        qDebug() << "F520xmlserializer::query: " << CARICO + "/" + DAYS + "/" + DAY + "[@" + NAME + "='" + DEFAULT_DAY + "']";
        query.setQuery(CARICO + "/" + DAYS + "/" + DAY + "[@" + NAME + "='" + DEFAULT_DAY + "']");
        if (!query.isValid()) {
            qDebug() << "F520xmlserializer::query query not valid";
            sysErr = SysError();
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

QString F520xmlserializer::query(QSharedPointer<F520Status> f520, QString month, SysError &sysErr) {
    QXmlQuery query;
    QString res;
    QFile xml(f520->getXmlPath());
    if ( ! xml.exists()) {
        xml.setFileName(f520->getXmlPath());
        if ( ! xml.exists()) {
            sysErr = SysError();
            return "";
        }
    }

    if (!xml.open(QIODevice::ReadOnly | QIODevice::Text)) {
        sysErr = SysError();
        return "";
    }

    qDebug() << "F520xmlserializer::query: " << CARICO + "/" + MONTHS + "/" + MONTH + "[@" + MONTH_VALUE + "='" + month + "']";

    query.setFocus(&xml);
    query.setQuery(CARICO + "/" + MONTHS + "/" + MONTH + "[@" + MONTH_VALUE + "='" + month + "']");
    if ( ! query.isValid()) {
        qDebug() << "F520xmlserializer::query query not valid";
        sysErr = SysError();
        return "";
    }

    query.evaluateTo(&res);

    if (!(res.length() > 1)) {
        qDebug() << "F520xmlserializer::query: " << CARICO + "/" + MONTHS + "/" + MONTH + "[@" + NAME + "='" + DEFAULT_MONTH + "']";
        query.setQuery(CARICO + "/" + MONTHS + "/" + MONTH + "[@" + NAME + "='" + DEFAULT_MONTH + "']");
        if (!query.isValid()) {
            qDebug() << "F520xmlserializer::query query not valid";
            sysErr = SysError();
            return "";
        }
        query.evaluateTo(&res);
    }

    xml.close();

    qDebug() << "F520xmlserializer::query " << res;

    QDomDocument watt;
    watt.setContent("" + res + "");
    QDomNodeList entryNodes = watt.elementsByTagName(MONTH);

    QString rtn;
    for (int i = 0; i < entryNodes.count(); i++) {
        QDomElement node = entryNodes.at(i).toElement();
        rtn = node.attribute(WATT);
        break;
    }
    qDebug() << "F520xmlserializer::query " << rtn;
    return rtn;
}

QString F520xmlserializer::avarageQuery(QSharedPointer<F520Status> f520, QString month, SysError &sysErr) {
    QXmlQuery query;
    QString res;
    QString defaultRes;
    QFile xml(f520->getXmlPath());
    if ( ! xml.exists()) {
        xml.setFileName(f520->getXmlPath());
        if ( ! xml.exists()) {
            sysErr = SysError();
            return "";
        }
    }

    if (!xml.open(QIODevice::ReadOnly | QIODevice::Text)) {
        sysErr = SysError();
        return "";
    }

    qDebug() << "F520xmlserializer::query: " << CARICO + "/" + DAYS + "/" + DAY + "[@" + MONTH_VALUE + "='" + month + "']";

    query.setFocus(&xml);
    query.setQuery(CARICO + "/" + DAYS + "/" + DAY + "[@" + MONTH_VALUE + "='" + month + "']");
    if ( ! query.isValid()) {
        qDebug() << "F520xmlserializer::query query not valid";
        sysErr = SysError();
        return "";
    }

    query.evaluateTo(&res);

    qDebug() << "F520xmlserializer::query: " << CARICO + "/" + DAYS + "/" + DAY + "[@" + NAME + "='" + DEFAULT_DAY + "']";
    query.setQuery(CARICO + "/" + DAYS + "/" + DAY + "[@" + NAME + "='" + DEFAULT_DAY + "']");
    if (!query.isValid()) {
        qDebug() << "F520xmlserializer::query query not valid";
        sysErr = SysError();
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
            sysErr = SysError();
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
