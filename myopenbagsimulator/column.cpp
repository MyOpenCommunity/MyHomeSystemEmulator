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

#include <QPainter>
#include <column.h>
#include <histogram.h>
#include <math.h>
#include <QDebug>

Column::Column(Histogram *sourceNode, Histogram *destNode)
{
    setAcceptedMouseButtons(0);
    sourceR = sourceNode;
    destR = destNode;
    sourceR->addColonna(this);
    destR->addColonna(this);
    adjust();
    m_brushColor = Qt::red;
}

void Column::setColor(QBrush color)
{
    m_brushColor = color;
}

void Column::adjust()
{
    QLineF line(mapFromItem(sourceR, 0, 0), mapFromItem(destR, 0, 0));
    prepareGeometryChange();
    sourcePoint     = line.p1();
    destPoint       = line.p2();
}

QRectF Column::boundingRect() const
{
    if (!sourceR || !destR)
        return QRectF();

    qreal penWidth  = 1;
    qreal extra     = penWidth / 3.0;

    return QRectF(sourcePoint, QSizeF(destPoint.x() - sourcePoint.x(),
                                      destPoint.y() - sourcePoint.y()))
            .normalized()
            .adjusted(-extra, -extra, extra, extra);
}

void Column::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (!sourceR || !destR)
        return;

    int colonna_height = destPoint.ry() - sourcePoint.ry();
//    qDebug() << " " << destPoint.ry() << " - " << sourcePoint.ry() << " " << colonna_height;
//    qDebug() << "Colonna height = " << colonna_height << " hei " << destPoint.ry();
//    qDebug() << "Dest   POINT  = " << destPoint.ry() << " XXX " << destPoint.rx();
//    qDebug() << "Source POINT  = " << sourcePoint.ry() << " XXX " << sourcePoint.rx();



    if (destPoint.ry() < 0)
    {
        painter->setPen(Qt::NoPen);
        painter->setBrush(m_brushColor);
        painter->drawRect(sourcePoint.rx() - sourceR->getWidth() / 2, sourcePoint.ry(), sourceR->getWidth(), colonna_height);
    }

    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::black);
    painter->drawRect(sourcePoint.rx() - sourceR->getWidth() / 2, sourcePoint.ry(), sourceR->getWidth(), -1);
}

int Column::getValue()
{
    int colonna_height = destPoint.ry() - sourcePoint.ry();
    return colonna_height;
}

