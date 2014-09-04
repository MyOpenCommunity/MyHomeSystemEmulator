/* This file is part of My Home System Emulator
 *
 * My Home System Emulator is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.

 * My Home System Emulator is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with My Home System Emulator.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "f520status.h"

const int F520Status::MAX_WHERE = 255;
const int F520Status::MIN_WHERE = 1;

F520Status::F520Status(QObject *parent) :
    QObject(parent)
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

QString F520Status::getMaxNrgValue() {
    return m_maxNrgValue;
}

void F520Status::setMaxNrgValue(QString value) {
    m_maxNrgValue = value;
}
