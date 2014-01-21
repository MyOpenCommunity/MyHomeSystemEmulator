#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QtWidgets/QGraphicsView>
#include <QString>
#include <QList>

class Column;

class GraphWidget : public QGraphicsView
{
    Q_OBJECT

public:
    GraphWidget(QList<int> values, QString title, int day, int month, int maxSize, QWidget *parent = 0);
    ~GraphWidget();
    void        setLabels(QString title, QString xLabel, QString yLabel);
    void        plotList(QList<int> values, int maxValue = 0, int minValue = 0);
    void        plotTwoLists(QList<int> firstList, QList<int> secondList, int maxValue = 0, int minValue = 0);
    void        plotListLists(QList< QList<int> > lists, int maxValue = 0, int minValue = 0);
    QString getRes();
    void        setTitle(QString title) { m_title = title; }
    QString     getTitle() { return m_title; }
    QList<int>  getValues();
    void        setDefault(bool isDefault) { m_isDefault = isDefault; }
    bool        getDefault() { return m_isDefault; }

    void        setDay(int day) { m_day = day; }
    int         getDay() { return m_day; }
    void        setMonth(int month) { m_month = month; }
    int         getMonth() { return m_month; }

private:
    int     getPositionY(int valu, int maxValu);
    int     getPositionX(int numCol);
    void    drawAxisX(QPainter *painter);
    void    drawAxisY(QPainter *painter);
    void    drawSubAxisX(QPainter *painter);
    void    drawSubAxisY(QPainter *painter);
    void    drawLabelsX(QPainter *painter);
    void    drawLabelsY(QPainter *painter);
    void    drawTitleX(QPainter *painter);
    void    drawTitleY(QPainter *painter);
    int     getElementValue(int item);

public slots:


protected:
    void    drawBackground(QPainter *painter, const QRectF &rect);

private:
    QGraphicsScene  *scene;
    int             m_col;
    int             m_numeroSerie;
    QString         m_x_title;
    QString         m_y_title;
    QString         m_title;
    QList<QBrush>   m_colorList;
    int             m_day;
    int             m_month;

    bool            m_isDefault;
    int             m_tabIndex;
};

#endif
