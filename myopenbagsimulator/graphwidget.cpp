#include "graphwidget.h"
#include "histogram.h"
#include <QtGui>
#include <column.h>

#include <math.h>
#include <QMap>
#include <QtGlobal>

static int SIZE_X       = 700;
static int SIZE_Y       = 400;
static int X_SEGMENT    = 10;
static int X_MAX_VALUE  = 0;

GraphWidget::GraphWidget(QList<int> values, QString title, int day, int month, int maxSize, QWidget *parent)
    : QGraphicsView(parent)
{
    m_title = title;
    m_day = day;
    m_month = month;
    scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setSceneRect(0, -SIZE_Y, SIZE_X, SIZE_Y);
    setScene(scene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    //    scale(qreal(0.8), qreal(0.8));
    setMinimumSize(SIZE_X *1.2, SIZE_Y*1.2);
    //    setMaximumSize(SIZE_X *1.2, SIZE_Y*1.2);
    //    setWindowTitle(m_title);

    X_MAX_VALUE = maxSize == 0 ? 2000 : maxSize;
    m_x_title = "";
    m_y_title = "";
    m_numeroSerie = 1;
    m_colorList.append(Qt::red);
    m_colorList.append(Qt::blue);
    m_colorList.append(Qt::green);
    m_colorList.append(Qt::yellow);
    m_colorList.append(Qt::cyan);

    QList< QList<int> > xxxx;
    xxxx.append(values);
    //    xxxx.append(valueFirstList);
    //    xxxx.append(valueSecondList);
    //    xxxx.append(valueThirdList);
    //    xxxx.append(valueSecondList);
    //    xxxx.append(valueThirdList);
    plotListLists(xxxx, X_MAX_VALUE, 0);
}

GraphWidget::~GraphWidget() {
    qDebug() << "GraphWidget::~GraphWidget: " << getTitle();
}

void GraphWidget::setLabels(QString title, QString xLabel, QString yLabel)
{
    m_title     = title;
    setWindowTitle(m_title);
    m_x_title   = xLabel;
    m_y_title   = yLabel;
}

void GraphWidget::plotList(QList<int> values, int maxValue, int minValue)
{
    int delta_X = getPositionX(values.size());
    m_col = values.length();
    int larghezzaCol = delta_X * 3 / 4;
    for(int i = 0; i < m_col; i++)
    {
        int x_pos = i*delta_X + delta_X / 2;
        Histogram *rectDOWN = new Histogram(larghezzaCol, this);
        Histogram *rectUP= new Histogram(larghezzaCol, this);
        scene->addItem(rectDOWN);
        scene->addItem(rectUP);
        rectDOWN->block(true);
        rectDOWN->setPos(x_pos , 0);
        Column *col = new Column(rectDOWN, rectUP);
        col->setColor(m_colorList[0]);
        scene->addItem(col);
        rectUP->setPos(x_pos, -getPositionY(values[i], maxValue));
        rectUP->setMaxExcursion(0, SIZE_Y);
    }
}

void GraphWidget::plotListLists(QList< QList<int> > lists, int maxValue, int minValue)
{

    m_numeroSerie = lists.size();
    m_col = lists[0].length();
    qDebug() << "Numero di serie da plottare : " << m_numeroSerie;
    int delta_X = getPositionX(lists[0].size());
    int margins_x = delta_X / 8 / m_numeroSerie;
    int space_x = delta_X - margins_x;
    int larghezzaCol = space_x / m_numeroSerie;

    for (int j = 0; j < m_numeroSerie;j ++)
    {
        int offset = margins_x / 2 + j * larghezzaCol + larghezzaCol / 2;
        for(int i = 0; i < m_col; i++)
        {
            int x_pos = i*delta_X;
            Histogram *rectDOWN = new Histogram(larghezzaCol, this);
            Histogram *rectUP= new Histogram(larghezzaCol, this);
            scene->addItem(rectDOWN);
            scene->addItem(rectUP);
            rectDOWN->block(true);
            rectDOWN->setPos(x_pos + offset, 0);
            Column *col = new Column(rectDOWN, rectUP);
            col->setColor(m_colorList[j]);
            scene->addItem(col);
            rectUP->setPos(x_pos + offset, -getPositionY(lists[j][i], maxValue));
            rectUP->setMaxExcursion(0, SIZE_Y);
        }
    }
}

int GraphWidget::getPositionY(int valu, int maxValu)
{
    int res = 0;
    res = valu * SIZE_Y / maxValu;
    return res;
}

int GraphWidget::getPositionX(int numCol)
{
    int res = 0;
    qDebug() << "Valori da plottare : " << numCol;
    res = SIZE_X/numCol;

    return res;
}

void GraphWidget::drawBackground(QPainter *painter, const QRectF &rect)
{
    drawLabelsY(painter);
    drawLabelsX(painter);
    drawSubAxisX(painter);
    drawSubAxisY(painter);
    drawAxisX(painter);
    //    drawAxisY(painter);
    drawTitleY(painter);
    drawTitleX(painter);

}

void GraphWidget::drawAxisX(QPainter *painter)
{
    QPen myPenAxis(Qt::black, 2, Qt::SolidLine);
    painter->setPen(myPenAxis);
    painter->drawLine(0 , 0 ,SIZE_X, 0);
}

void GraphWidget::drawAxisY(QPainter *painter)
{
    QPen myPenAxis(Qt::black, 2, Qt::SolidLine);
    painter->setPen(myPenAxis);
    painter->drawLine(0 , 0 ,0, -SIZE_Y);
}

void GraphWidget::drawSubAxisX(QPainter *painter)
{
    QPen myPen(Qt::darkGray, 1, Qt::DotLine);
    painter->setPen(myPen);
    for (int i = 1; i <= X_SEGMENT ; i++)
        painter->drawLine(0, -(SIZE_Y / X_SEGMENT) * i ,SIZE_X, -(SIZE_Y / X_SEGMENT) * i);
}

void GraphWidget::drawSubAxisY(QPainter *painter)
{
    QPen myPen(Qt::darkGray, 1, Qt::DotLine);
    painter->setPen(myPen);
    for (int i = 1; i < m_col ; i++)  {
        painter->drawLine((SIZE_X / m_col ) * i, 0 ,(SIZE_X / m_col ) * i, -SIZE_Y);
//        QString text = QString::number(X_MAX_VALUE / m_col * (i-1));
//        int tmp = ((SIZE_X / m_col) * (i) - (SIZE_X / m_col) * (i-1));
//        painter->drawText( ((SIZE_X / m_col) * (i-1)), 10,tmp,40, Qt::TextDontClip | Qt::AlignHCenter, QString::number(i-1));
    }
}

void GraphWidget::drawLabelsX(QPainter *painter)
{
    QPen myPen(Qt::darkGray, 1, Qt::SolidLine);
    painter->setPen(myPen);
    for (int i = 0; i < m_col ; i++)
    {
        QString text = QString::number(X_MAX_VALUE / m_col * i);
        int tmp = ((SIZE_X / m_col) * (i) - (SIZE_X / m_col) * (i-1));
        painter->drawText( ((SIZE_X / m_col) * (i)), 10,tmp,40, Qt::TextDontClip | Qt::AlignHCenter, QString::number(i+1));
    }
}

void GraphWidget::drawLabelsY(QPainter *painter)
{
    QPen myPen(Qt::darkGray, 1, Qt::SolidLine);
    painter->setPen(myPen);
    for (int i = 1; i <= X_SEGMENT ; i++)
    {
        QString text = QString::number(X_MAX_VALUE / X_SEGMENT * i);
        painter->drawText(-40, -(SIZE_Y / X_SEGMENT) * i, 30,40, Qt::TextDontClip | Qt::AlignRight, text);
    }
}

void GraphWidget::drawTitleX(QPainter *painter)
{
    QPen myPen(Qt::darkGray, 1, Qt::SolidLine);
    painter->setPen(myPen);
    painter->drawText(SIZE_Y, -100, 10, SIZE_Y, Qt::TextDontClip | Qt::AlignCenter, m_y_title);
}

void GraphWidget::drawTitleY(QPainter *painter)
{
    QPen myPen(Qt::darkGray, 1, Qt::SolidLine);
    painter->setPen(myPen);
    painter->rotate(-90);
    painter->drawText(SIZE_Y / 2, -(50+SIZE_Y/4), 10, SIZE_Y / 2, Qt::TextDontClip | Qt::AlignCenter, m_y_title);
}

QString GraphWidget::getRes()
{
    QString res;
    QList<QGraphicsItem *> list = scene->items();
    int i = 0;
    foreach( QGraphicsItem * item, list )
    {
        if( Column * node = qgraphicsitem_cast<Column *>( item ) ) // the QGraphicsItem is a Node
        {
            //qDebug() << "Valore " << i++ << " -> " << node->getValue();
            res.append(QString::number(getElementValue(node->getValue())) + '.');
        }
    }
    return res.remove(res.count() - 1, 1);
}

QList<int> GraphWidget::getValues() {
    QList<int> res;
    QList<QGraphicsItem *> list = scene->items();
    int i = 0;
    foreach( QGraphicsItem * item, list )
    {
        if( Column * node = qgraphicsitem_cast<Column *>( item ) ) // the QGraphicsItem is a Node
        {
            //qDebug() << "Valore " << i++ << " -> " << node->getValue();
            res.append(getElementValue(node->getValue()));
        }
    }
    return res;
}

int GraphWidget::getElementValue(int item) {
    return ((X_MAX_VALUE * qAbs(item)) / SIZE_Y);
}
