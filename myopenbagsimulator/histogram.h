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

#ifndef RETTANGOLO_H
#define RETTANGOLO_H

#include <QGraphicsItem>
#include <QList>

class Column;
class GraphWidget;
class QGraphicsSceneMouseEvent;

class Histogram : public QGraphicsItem
{
public:
    Histogram(int size, GraphWidget *graphWidget);
    void block(bool blockState);
    void setMaxExcursion(int min, int max);

    void addColonna(Column *edge);
    QList<Column *> edges() const;

    enum { Type = UserType + 1 };
    int type() const { return Type; }

    bool advance();

    QRectF boundingRect() const;
    QPainterPath shape() const;

    int getWidth(){return m_width;}

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    Qt::GlobalColor getColor();

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

private:
    QList<Column *> edgeList;
    QPointF newPos;
    GraphWidget *graph;
    bool m_isBlocked;

    int m_height;
    int m_width;
    int m_max_val;
    int m_min_val;


};


#endif // RETTANGOLO_H
