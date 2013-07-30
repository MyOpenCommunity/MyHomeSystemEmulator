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
