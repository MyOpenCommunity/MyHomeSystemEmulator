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
