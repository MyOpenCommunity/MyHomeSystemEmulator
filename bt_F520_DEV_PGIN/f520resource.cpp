#include "f520resource.h"

const int F520resource::MAX_WHERE = 255;
const int F520resource::MIN_WHERE = 1;

F520resource::F520resource(QObject *parent) :
    QObject(parent)
{
}

F520resource::~F520resource() {

}

void F520resource::setXmlPath(QString path) {
    m_xmlPath = path;
}

QString F520resource::getXmlPath() {
    return m_xmlPath;
}

QString F520resource::getWhere() {
    return m_where;
}

void F520resource::setWhere(QString where) {
    m_where = where;
}

QString F520resource::getMaxNrgValue() {
    return m_maxNrgValue;
}

void F520resource::setMaxNrgValue(QString value) {
    m_maxNrgValue = value;
}
