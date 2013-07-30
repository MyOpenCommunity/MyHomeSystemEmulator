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
