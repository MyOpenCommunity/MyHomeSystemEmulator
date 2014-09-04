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

#ifndef DRAGDINWIDGET_H
#define DRAGDINWIDGET_H

#include <QWidget>
#include <QMap>
#include <QScrollArea>

class DeviceAdapter;
class DeviceModel;

class DragDinWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DragDinWidget(QWidget *parent = 0);

    void    setAdapter(DeviceAdapter* da);
    void    updateAdapterModel(DeviceModel* dm);
    void    setNumberOfRows(int rows);
    void    setNumberOfCol(int rows);
    void    clearPlant();
    QMap<QString, QString> getDevicesOnBoards();
    void    setScrollElement(QScrollArea *scrEl);

private:
    void    init();
    void    drawBackground();
    void    setSlotSize(const QSize &s);
    int     getTargetCell(int x, int y);
    int     pointOffset(QPoint pointA, QPoint pointB);
    void    setPlantSize(int col, int row);
    bool    targetSquare(const QPoint &position, QSize imageSize, QRect& tgtSquare);

    QRect   getTheoricallyFilledRect(int slot, int row, QSize imageSize);
    QRect   getTheoricallyFilledRect(QPoint p, QSize imageSize);
    QPoint  getBlockPoint(QPoint p);
    QPoint  getFirstFreeSlot(QSize imageSize);

    bool    isRectValid(QRect Rect, int id);
    bool    isPointInRect(QPoint p, QRect r);
    bool    isRectInRect(QRect p, QRect r);
    int     distanceBetweenPoints(QPoint a, QPoint b);

    void    deleteDevice(int devId);
    void    onRightClick(const QPoint &pt);

    // MOVE AREA
    void    moveScrollBars(QPoint pos);
    void    moveScrollBarsHorizontaly(QPoint pos);
    void    moveScrollBarsVerticaly(QPoint pos);

    // ZOOM  TO BE TESTED....
    void    updateSlotSize(int old_x, int old_y, int new_x, int new_y);
    void    increaseSlotSize(int step);
    void    decreaseSlotSize(int step);

protected:
    void onAdapterChanged();
    void resizeEvent(QResizeEvent *);

    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void wheelEvent(QWheelEvent *ev);
    void paintEvent(QPaintEvent *pe);
    void drawGrid(QPaintEvent *pe);
    void drawSelectCellToDrop(QPaintEvent *pe);


signals:
    
public slots:

private:
    DeviceAdapter*      _pda;
    QMap<int, QWidget*> _mwidget;
    QMap<int, QRect>    _idrect;  //_mrect
    QMap<int, QString>  _idWidgetIdDevice;

    // Dimensione slot di drag & drop (corrisponde al modulo DIN)
    int     _col;
    int     _row;

    int     _slot_X;
    int     _slot_Y;

    int     _selectedDId;
    int     _cellSelected;
    int     _cellSelectedWidth;

    QPoint _mousePressPoint;

    QRect   highlightedRect;
    QRect   startingRect;
    QPoint  startingMovePosition;
    QPoint  _rectRelativeClickPos;
    bool    isMoving;

    QScrollArea* scrollElement;


};

#endif // DRAGDINWIDGET_H
