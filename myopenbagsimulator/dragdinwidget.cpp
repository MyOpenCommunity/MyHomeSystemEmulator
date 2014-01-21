#include "dragdinwidget.h"
#include <QtWidgets>
#include <QMap>
#include <devicewidget.h>
#include <deviceadapter.h>
#include <plantmgr.h>
#include <syserror.h>
#include <plant.h>
#include <plantdevice.h>
#include <QtCore/QCoreApplication>
#include <QSharedPointer>
#include <QDebug>
#include <setupwidget.h>


static int SLOT_WIDTH   = 50;
static int SLOT_HEIGTH  = 285;
static int SLIDE_MARGIN_X = 70;
static int SLIDE_MARGIN_Y = 100;
static int SIZE_MIN_X = 800;
static int SIZE_MIN_Y = 600;



DragDinWidget::DragDinWidget(QWidget *parent) :
    QWidget(parent), _pda(0)
{
    init();
    setMinimumSize(SIZE_MIN_X, SIZE_MIN_Y);
    setAutoFillBackground(true);
    setSlotSize(QSize(SLOT_WIDTH, SLOT_HEIGTH));
    setAcceptDrops(true);
}

void DragDinWidget::setNumberOfRows(int rows)
{
    _row = rows;
    setPlantSize(_col, _row);
}

void DragDinWidget::setNumberOfCol(int cols)
{
    _col = cols;
    setPlantSize(_col, _row);
}

void DragDinWidget::setPlantSize(int col, int row)
{
    int h = (_slot_Y * row) + 2;
    int w = (_slot_X * col) + 2;
    setMinimumSize(w,h);
}

void DragDinWidget::setScrollElement(QScrollArea *scrEl)
{
    scrollElement = scrEl;
}

QMap<QString, QString> DragDinWidget::getDevicesOnBoards()
{

    QMap<QString, QString> res;
    int id;

    foreach(id, _idWidgetIdDevice.keys())
    {
        if (_idrect.contains(id))
        {
            QString pos;
            int x = _idrect[id].left() / SLOT_WIDTH;
            int y = _idrect[id].top() / SLOT_HEIGTH;
            qDebug() <<"XY : " << x << " " << y;
            pos = QString::number(x) + "," + QString::number(y);
            res.insert(_idWidgetIdDevice[id], pos);
            qDebug() <<"Valore inserito : " << _idWidgetIdDevice[id] << " " << pos;
        }
    }
    return res;
}

void DragDinWidget::init()
{
    _col = 0;
    _row = 0;

    _selectedDId = -1;
}

void DragDinWidget::drawBackground()
{
    //Using QPalette you can set background image as follows.
    QPalette p = palette();

    //Load image to QPixmap, Give full path of image
    QPixmap pixmap1(":/image/resources/BarraDin.png");

    QSize size(_slot_X*2, _slot_Y);  // Immagine Din pensata per 2 slot
    QPixmap pixmap(pixmap1.scaled(size));

    p.setBrush(QPalette::Background,  pixmap);
    setPalette(p);
}

void DragDinWidget::drawGrid(QPaintEvent */*pe*/)
{
    QPainter *paint = new QPainter (this);
    paint->setPen(Qt::white);
    //paint->begin (this);
    for (int r = 0; r < _row+1 ; ++r)
    {
        paint->drawLine(0,r*_slot_Y,this->width(),(r)*_slot_Y);
    }

    for(int c = 0; c < _col+1 ; ++c)
    {
        paint->drawLine(c*_slot_X,0,(c)*_slot_X,this->height());
    }
    paint->end();

}

void DragDinWidget::drawSelectCellToDrop(QPaintEvent */*pe*/)
{
    QPainter painter;
    painter.begin(this);
    if (highlightedRect.isValid() && _selectedDId > 0)
    {
        painter.setBrush(QColor("#ffcccc"));
        //painter.setPen(Qt::NoPen);
        painter.setOpacity(0.6);
        painter.drawRect(QRect(highlightedRect.topLeft().x(), highlightedRect.topLeft().y(), _mwidget[_selectedDId]->width()/_slot_X * _slot_X, _slot_Y).adjusted(0, 0, -1, -1));
    }

    painter.end();
}

void DragDinWidget::setSlotSize(const QSize &s)
{
    _slot_X = s.width();
    _slot_Y = s.height();

    _col = width() / _slot_X;
    _row = height() / _slot_Y;
}

void DragDinWidget::resizeEvent(QResizeEvent *)
{
    _col = width() / _slot_X;
    _row = height() / _slot_Y;
    //    qDebug()<< "RESIZE CHIAMATA "<< _row << " "<< _col;
}

void DragDinWidget::setAdapter(DeviceAdapter* da)
{
    _pda = da;
    //onAdapterChanged();
}

void DragDinWidget::updateAdapterModel(DeviceModel* dm)
{
    if (dm->getCol() < 0 && dm->getRow() < 0)
    {
        // Se le coordinate non sono state settate ricerco la prima posizione libera all'interno del piano di lavoro ....
        QPoint pos = getFirstFreeSlot(dm->GetBackgroundImage().size());
        qDebug() << "The device has not a position -> NEW position : " << pos.rx() << ":" << pos.ry();
        dm->setPanelPosition(pos.ry(), pos.rx());
        qDebug() << " slot info " << dm->getSlotWidth() << " " << dm->getSlotHeight() << " " << dm->getWidth() << " " << dm->getHeigth();
    } else {
        qDebug() << "The device has already a position : " << dm->getCol() << ":" << dm->getRow();
    }

    if (dm->getCol() < 0 && dm->getRow() < 0)
    {
        qDebug() << "Plant is full ... No place for the new device ... ";
        return;
    }

    dm->setPanelSize(_slot_X,_slot_Y);

    if (_idrect.contains(dm->getDId()))
        _idrect.remove(dm->getDId());

    // Calcolo la posizione in cui andrÃ  il device e ne aggiungo il riferimento nel dict
    _idrect.insert(dm->getDId(), getTheoricallyFilledRect(dm->getCol(), dm->getRow(), QSize(dm->getWidth()*_slot_X, dm->getHeigth()*_slot_Y)));


    // Devo muovere il device nella posizione identificata
    QWidget* pw = 0;
    pw = _pda->getView(dm->getDId(),pw, this);
    pw->show();
    pw->move(getTheoricallyFilledRect(dm->getCol(), dm->getRow(), QSize(dm->getWidth()*_slot_X, dm->getHeigth()*_slot_Y)).topLeft());

    _mwidget.insert(dm->getDId(),pw);

    _idWidgetIdDevice.insert(dm->getDId(), dm->GetPlantDev() );
    qDebug() << "Added : " << dm->getDId() << " " << dm->GetPlantDev();
}

int DragDinWidget::getTargetCell(int x, int y)
{
    int col = x/_slot_X;
    int row = y/_slot_Y;
    return col + (row * _col);
}

int DragDinWidget::pointOffset(QPoint pointA, QPoint pointB)
{
    int dist = 0;
    int x = pointB.x() - pointA.x();
    int y = pointB.y() - pointA.y();
    int d = pow(x,2) + pow(y,2);
    dist = sqrt(d);
    return dist;
}

void DragDinWidget::mousePressEvent(QMouseEvent *ev)
{
    _selectedDId = -1;
    int key;
    foreach( key, _idrect.keys() )
    {
        if (isPointInRect(ev->pos(), _idrect[key]))
        {
            _selectedDId = key;
            startingRect = _idrect[key];
            _rectRelativeClickPos = ev->pos() - startingRect.topLeft();
            startingMovePosition = ev->pos();
            if (targetSquare(ev->pos() - QPoint((_rectRelativeClickPos.rx()/_slot_X)*_slot_X, 0), _mwidget[_selectedDId]->size(), highlightedRect))
            {
                qDebug() << "Device selected : " << _selectedDId;
                ev->accept();
            }
            return;
        }
    }
}

void DragDinWidget::mouseMoveEvent(QMouseEvent *ev)
{
    if (_selectedDId > 0)
    {
        if (distanceBetweenPoints(startingMovePosition, ev->pos()) > 15 || isMoving)
        {
            isMoving = true;
            _mwidget[_selectedDId]->move(ev->pos() - _rectRelativeClickPos);
            moveScrollBars(ev->pos());
            if (targetSquare(ev->pos() - QPoint((_rectRelativeClickPos.rx()/_slot_X)*_slot_X, 0), _mwidget[_selectedDId]->size(), highlightedRect))
            {
                update();
            }
        }
    }
}

void DragDinWidget::moveScrollBars(QPoint pos)
{
    moveScrollBarsHorizontaly(pos);
    moveScrollBarsVerticaly(pos);
}

void DragDinWidget::moveScrollBarsHorizontaly(QPoint pos)
{
    int moveRight = pos.x() - this->visibleRegion().boundingRect().width() - this->visibleRegion().boundingRect().left() + SLIDE_MARGIN_X;
    if (moveRight > 0)
    {
        this->scrollElement->horizontalScrollBar()->setValue(pos.x() + SLIDE_MARGIN_X - this->visibleRegion().boundingRect().width());
        return;
    }
    int moveLeft = this->visibleRegion().boundingRect().left() + SLIDE_MARGIN_X - pos.x();
    if (moveLeft > 0 )
        this->scrollElement->horizontalScrollBar()->setValue(pos.x() - SLIDE_MARGIN_X);
}

void DragDinWidget::moveScrollBarsVerticaly(QPoint pos)
{
    int moveDown = pos.y() - this->visibleRegion().boundingRect().height() - this->visibleRegion().boundingRect().top() + SLIDE_MARGIN_Y;
    if (moveDown > 0)
    {
        this->scrollElement->verticalScrollBar()->setValue(pos.y() + SLIDE_MARGIN_Y - this->visibleRegion().boundingRect().height());
        return;
    }
    int moveUp = this->visibleRegion().boundingRect().top() + SLIDE_MARGIN_Y - pos.y();
    if (moveUp > 0 )
        this->scrollElement->verticalScrollBar()->setValue(pos.y() - SLIDE_MARGIN_Y);
}

void DragDinWidget::mouseReleaseEvent(QMouseEvent *ev)
{
    if (_selectedDId > 0)
    {
        if (startingRect.topLeft() != highlightedRect.topLeft())
        {
            if (isRectValid(highlightedRect, _selectedDId))
            {
                _mwidget[_selectedDId]->move(highlightedRect.topLeft());
                _idrect.remove(_selectedDId);
                _idrect.insert(_selectedDId, highlightedRect);

            } else {
                _mwidget[_selectedDId]->move(startingRect.topLeft());
            }
        } else
        {
            //qDebug() << "Rettangolo di partenza e di destinazione al rilascio sono gli stessi ... (" << _selectedDId << ")";
            _mwidget[_selectedDId]->move(highlightedRect.topLeft());

            if(ev->button()==Qt::RightButton)
            {
                onRightClick(ev->pos());
                ev->accept();
            }
        }
    }
    isMoving = false;
    _selectedDId = -1;
    update();
}

void DragDinWidget::onRightClick(const QPoint &pt)
{
    QPoint global = this->mapToGlobal(pt);
    QMenu pPopup(this);
    QAction *pAction1 = new QAction("Configure",this);
    QAction *pAction2 = new QAction("Destroy",this);
    pPopup.addAction(pAction1);
    pPopup.addAction(pAction2);
    QAction *pItem = pPopup.exec(global);
    if(pItem==pAction1)
    {
        qDebug("Config ...");
        _pda->setupDevice(_selectedDId);
    }
    else if(pItem==pAction2)
    {
        qDebug() << "Destroy " << QString::number(_selectedDId) << " ...";
        deleteDevice(_selectedDId);
    }
}

void DragDinWidget::clearPlant()
{
    int key;
    foreach(key, _mwidget.keys())
    {
        deleteDevice(key);
    }
}

void DragDinWidget::deleteDevice(int devId)
{
    _pda->delDevice(devId);

    _mwidget[devId]->close();
    _mwidget.remove(devId);
    _idrect.remove(devId);
    _idWidgetIdDevice.remove(devId);
}

void DragDinWidget::wheelEvent(QWheelEvent *ev)
{
    if (false)
    {
        int numDegrees = ev->delta() / 8;
        int numSteps = numDegrees / 15;
        qDebug() << "CONTATORE WHEEL = " << numSteps;
        if (numSteps > 0)
            increaseSlotSize(1);
        else
            decreaseSlotSize(1);
        update();
    }

    ev->accept();
}

void DragDinWidget::updateSlotSize(int old_x, int old_y, int new_x, int new_y)
{
    qDebug() << "updateSlotSize = " << old_x << " " << old_y << "  " << new_x << " " << new_y ;
    int key;
    foreach( key, _idrect.keys() )
    {
        _idrect[key] = QRect(_idrect[key].topLeft().rx()/old_x*new_x, _idrect[key].topLeft().ry()/old_y*new_y, _idrect[key].width()*1000/old_x*new_x/1000, _idrect[key].height()*1000/old_y*new_y/1000);
        _mwidget[key]->move(_idrect[key].topLeft());
        _mwidget[key]->resize(_idrect[key].size());
    }
}

void DragDinWidget::increaseSlotSize(int step)
{
    qDebug() << "increaseSlotSize = " << step;

    int delta_X = SLOT_WIDTH * 10 / 100;
    int delta_Y = SLOT_HEIGTH * 10 / 100;
    int old_x = _slot_X;
    int old_y = _slot_Y;
    if (50 >= _slot_X+delta_X) // 50 valore di max size...
    {
        setSlotSize(QSize(_slot_X+delta_X, _slot_Y+delta_Y));
        updateSlotSize(old_x, old_y, _slot_X, _slot_Y);
    }
}

void DragDinWidget::decreaseSlotSize(int step)
{
    qDebug() << "decreaseSlotSize = " << step;
    int delta_X = SLOT_WIDTH * 10 / 100;
    int delta_Y = SLOT_HEIGTH * 10 / 100;
    if (_slot_X > delta_X)
    {
        int old_x = _slot_X;
        int old_y = _slot_Y;
        setSlotSize(QSize(_slot_X-delta_X, _slot_Y-delta_Y));
        updateSlotSize(old_x, old_y, _slot_X, _slot_Y);
    }
}

void DragDinWidget::paintEvent(QPaintEvent *pe)
{
    drawBackground();
    //    drawGrid(pe);
    drawSelectCellToDrop(pe);
}

bool DragDinWidget::isPointInRect(QPoint p, QRect r)
{
    if (r.left()<=p.rx() && p.rx() <= r.right())
    {
        if (r.top() <= p.ry() && p.ry() <= r.bottom())
        {
            return true;
        }
    }
    return false;
}

bool DragDinWidget::isRectInRect(QRect p, QRect r)
{
    if (!isPointInRect(p.topLeft(), r) && !isPointInRect(p.topRight(), r))
    {
        if (!isPointInRect(p.bottomLeft(), r) && !isPointInRect(p.bottomRight(), r))
        {
            // In caso di device + largo ....
            if (p.left() < r.left() && p.right() > r.right() && p.top() == r.top())
                return true;
            return false;
        }
    }
    return true;
}


QRect DragDinWidget::getTheoricallyFilledRect(int slot, int row, QSize imageSize)
{
    int numSlotToUse = imageSize.width()/_slot_X;
    qDebug() << "getTheoricallyFilledRect slot: " << slot << " row: " << row << " numSlotToUse: " << numSlotToUse;
    return QRect(slot*_slot_X, row*_slot_Y, _slot_X*numSlotToUse, _slot_Y);
}

QRect DragDinWidget::getTheoricallyFilledRect(QPoint p, QSize imageSize)
{
    QPoint slotRowPoint = getBlockPoint(p);
    return getTheoricallyFilledRect(slotRowPoint.x(), slotRowPoint.y(), imageSize);
}

bool DragDinWidget::targetSquare(const QPoint &position, QSize imageSize, QRect& tgtSquare)
{
    int numSlotToUse = imageSize.width()/_slot_X;
    QRect dstRect = QRect(position.x()/_slot_X * _slot_X, position.y()/_slot_Y * _slot_Y, numSlotToUse*_slot_X, _slot_Y);
    if (!isRectValid(dstRect, _selectedDId))
        return false;
    tgtSquare = dstRect;
    return true;
}

QPoint DragDinWidget::getBlockPoint(QPoint p)
{
    int x = p.x()/_slot_X;
    int y = p.y()/_slot_Y;
    qDebug() << "SLOT ID " << x << "," << y;
    return QPoint(x, y);
}

bool DragDinWidget::isRectValid(QRect Rect, int id)
{
    int key;
    foreach( key, _idrect.keys() )
    {
        if (key != id)
        {
            if (!isRectInRect(Rect, _idrect[key])) {
                //qDebug() << "VALID";
            }
            else
                return false;
        }
    }

    // Controllo che sia all'interno del pannello
    if (Rect.top() < 0)
        return false;
    if (Rect.bottom()> this->height())
        return false;
    if (Rect.left()<0)
        return false;
    if (Rect.right()>this->width())
        return false;

    return true;
}

int DragDinWidget::distanceBetweenPoints(QPoint a, QPoint b)
{
    int distanza = 0;
    int x = b.x() - a.x();
    int y = b.y() - a.y();
    int d = pow(x,2) + pow(y,2);
    distanza = sqrt(d);
    return distanza;
}


QPoint DragDinWidget::getFirstFreeSlot(QSize imageSize)
{
    int numRow  = this->height()/_slot_Y;
    int numSlot = this->width()/_slot_X;

    for (int row = 0; row < numRow; row++)
    {
        for (int slot = 0; slot < numSlot; slot++)
        {
            if (isRectValid(getTheoricallyFilledRect(slot, row, imageSize), -1))
            {
                QPoint pos(slot,row);
                return pos;
            }
        }
    }
    QPoint pos(-1,-1);
    return pos;
}
