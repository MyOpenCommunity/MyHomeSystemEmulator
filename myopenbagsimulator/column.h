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
