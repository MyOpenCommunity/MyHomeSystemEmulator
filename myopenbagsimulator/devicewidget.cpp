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
