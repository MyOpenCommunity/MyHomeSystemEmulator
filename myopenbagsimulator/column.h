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

#ifndef COLONNA_H
#define COLONNA_H

#include <QGraphicsItem>

class Histogram;

class Column : public QGraphicsItem
{
public:
    Column(Histogram *sourceNode, Histogram *destNode);
    int         getValue();
    void        adjust();
    void        setColor(QBrush color);

    enum { Type = UserType + 2 };
    int type() const { return Type; }

protected:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    Histogram *sourceR, *destR;
    QBrush m_brushColor;
    QPointF sourcePoint;
    QPointF destPoint;
};


#endif // COLONNA_H
