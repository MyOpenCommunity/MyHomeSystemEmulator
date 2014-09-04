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

#ifndef DEVICEWIDGET_H
#define DEVICEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QMap>

class DeviceModel;




class DeviceWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DeviceWidget(DeviceModel* dm, QWidget *parent = 0);
    
protected:
    void mousePressEvent ( QMouseEvent * event ) ;                              // OVERRIDE DEL SEGNALE DI MOUSE CLICK E CALCOLO DI QUALE ZONA SENSIBILE È STATA PREMUTA CON CONSEGUENTE SEGNALE CLICKED

signals:
    void clicked(const QString &); // SEGNALE CHIAMATO SE HO CLICCATO UNA ZONA SENSIBILE
    
public slots:
    void dataModelChanged();

private:
    DeviceModel* _dm;

    void paintEvent(QPaintEvent *);

    void buildImage(); //PROVA
    void initImage();
    QImage backImage;
    QImage resultImage;

};

#endif // DEVICEWIDGET_H
