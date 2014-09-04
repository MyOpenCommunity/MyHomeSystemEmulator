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

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>

#include <histogram.h>
#include "graphwidget.h"
#include <QDebug>
#include <column.h>

Histogram::Histogram(int size, GraphWidget *graphWidget)
    : graph(graphWidget)
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);
    m_width = size;
    m_height = 200;
    m_max_val = 100;
    m_min_val = 100;
    block(false);
}

void Histogram::addColonna(Column *edge)
{
    edgeList << edge;
    edge->adjust();
}

void Histogram::setMaxExcursion(int min, int max)
{
    m_max_val = max;
    m_min_val = min;
    m_height  = (max - min) * 5;
}

void Histogram::block(bool blockState)
{
    m_isBlocked = blockState;
    if (blockState)
    {
        m_height = 0;
    }
}

QList<Column *> Histogram::edges() const
{
    return edgeList;
}

bool Histogram::advance()
{
    if (newPos == pos())
        return false;

    setPos(newPos);
    return true;
}

QRectF Histogram::boundingRect() const
{
    return QRectF( -m_width / 2, -m_height / 2, m_width, m_height);
}

QPainterPath Histogram::shape() const
{
    QPainterPath path;
    path.addRect( -m_width / 2, -m_height / 2, m_width, m_height);
    return path;
}

Qt::GlobalColor Histogram::getColor()
{
    if (m_isBlocked)
        return Qt::black;
    return Qt::transparent;
}

void Histogram::paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/, QWidget *)
{
    painter->setPen(Qt::NoPen);
    painter->setBrush(this->getColor());
    painter->drawRect( -m_width / 2, -m_height / 2, m_width, m_height);
}

QVariant Histogram::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case ItemPositionHasChanged:
        foreach (Column *edge, edgeList) {
            edge->adjust();
//            qDebug() << "Colonna Valore: " << edge->getValue();
        }
        break;
    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}

void Histogram::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    mouseMoveEvent(event);
    QGraphicsItem::mousePressEvent(event);
}

void Histogram::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    if (m_isBlocked)
        return;
    if (-this->pos().ry() - event->pos().ry() <= m_max_val && -this->pos().ry() - event->pos().ry() >= m_min_val)
        QGraphicsItem::moveBy(0, event->pos().ry());
}

void Histogram::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}
