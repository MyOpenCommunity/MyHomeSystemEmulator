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

#include "devicewidget.h"

#include <devicemodel.h>
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>

DeviceWidget::DeviceWidget(DeviceModel* dm, QWidget *parent) :
    QWidget(parent), _dm(dm)
{
    QObject::connect(_dm, SIGNAL(valueChanged()),
                     this, SLOT(dataModelChanged()));

    initImage();

    resize(_dm->getSlotWidth()*_dm->getWidth(),_dm->getSlotHeight());
    move(_dm->getSlotWidth() * _dm->getRow(), _dm->getSlotHeight() * _dm->getCol());
}

void DeviceWidget::paintEvent(QPaintEvent *)
{
    buildImage();
}

void DeviceWidget::dataModelChanged()
{
    resize(_dm->getSlotWidth()*_dm->getWidth(),_dm->getSlotHeight());
    move(_dm->getSlotWidth() * _dm->getRow(), _dm->getSlotHeight() * _dm->getCol());
}

void DeviceWidget::initImage()
{
    qDebug() << "Size IMAGE " << _dm->GetBackgroundImage().width() << " " << _dm->GetBackgroundImage().height();
    this->size() = _dm->GetBackgroundImage().size();
}

void DeviceWidget::buildImage()
{
    QPalette p = palette();
    p.setBrush(QPalette::Background,  _dm->createGui());
    setPalette(p);
    setAutoFillBackground(true);
}

void DeviceWidget::mousePressEvent ( QMouseEvent * event )
{
    _dm->MouseClicked(event);
    event->ignore();  // ignoro l'evento in modo tale da propagarlo al livello superiore...
}
