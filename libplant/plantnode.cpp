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

#include <plantnode.h>
#include <QDebug>

PlantNode::PlantNode():m_busName("")
{

}

PlantNode::~PlantNode() {
    qDebug() << "~PlantNode()";
}

void PlantNode::setSuperior(const QSharedPointer<PlantNode>& theNode) {
    m_superior = theNode;
}

void PlantNode::attachChild(const QSharedPointer<PlantNode>& theNode) {
    m_childList.insert(theNode->getID(), theNode);
}

void PlantNode::detachChild(QSharedPointer<PlantNode> theNode)  {
    QSharedPointer<PlantNode> nullRef;

    m_childList.remove(theNode->getID());
    theNode->setSuperior(nullRef);

}

void PlantNode::setBusName(const QString& busName){
    m_busName = busName;
}

const QString& PlantNode::getBusName(){
    return (m_busName);
}

const QSharedPointer<PlantNode>&    PlantNode::getSuperior(void)    {
    return(m_superior);
}
