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
