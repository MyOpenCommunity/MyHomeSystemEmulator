#include "f520status.h"

const int F520Status::MAX_WHERE = 255;
const int F520Status::MIN_WHERE = 1;

F520Status::F520Status()
{
}

F520Status::~F520Status() {

}

void F520Status::setXmlPath(QString path) {
    m_xmlPath = path;
}

QString F520Status::getXmlPath() {
    return m_xmlPath;
}

QString F520Status::getWhere() {
    return m_where;
}

void F520Status::setWhere(QString where) {
    m_where = where;
}
