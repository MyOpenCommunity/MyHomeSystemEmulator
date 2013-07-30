#ifndef DEVICEMODEL_H
#define DEVICEMODEL_H

#include <QObject>
#include <plantdevice.h>
#include <QImage>
#include <QPixmap>
#include <QMap>
#include <QMouseEvent>
#include <eventhandler.h>
#include <QDomElement>
#include <QDebug>

class ImageView
{
public:
    ImageView(QString imgName, QPoint pos, QSize siz, QImage pix);
    QString imageName;
    QPoint position;
    QSize size;
    QImage img;
};

class DeviceModel : public QObject, public EventHandler
{
    Q_OBJECT
public:
//    explicit DeviceModel(int did, int width = 1, QObject *parent = 0);
//    explicit DeviceModel(QSharedPointer<PlantNode> plantDev, int did = -1,  QObject *parent = 0);
    explicit DeviceModel(QString plantDev, QString plantName, int deviceType, int did = -1,  QObject *parent = 0);

    ~DeviceModel() {qDebug() << "~DeviceModel()"; }

    // get device data
    inline int getDId() { return _did; }

    // get device position data
    inline int getRow() { return _row; }
    inline int getCol() { return _col; }

    // get width
    inline int getWidth() { return _width; }

    // get height
    inline int getHeigth() { return _heigth; }

    // get slot size
    inline int getSlotWidth() { return _slotwidth; }
    inline int getSlotHeight() { return _slotheight; }

    inline void setDId(int did) { _did = did; }
    inline void setRow(int row) { _row = row; }
    inline void setCol(int col) { _col = col; }

    void setPanelPosition(int row, int col);
    void setPanelSize(int width,int height);



    QImage      createGui(); // COSTRUISCE LA QIMAGE DA PASSARE AL WIDGET COMPRENSIVO DEI LED
    void        MouseClicked(QMouseEvent * event);

    void        handleEvent(CMD_PRIO thePrio, const QByteArray& theEvent);

    QImage      GetBackgroundImage() {return m_backgroundImage; }
    QString     GetPlantDev() {return m_plantDev; }
    QString     GetPlantName() {return m_plantName; }
    int         GetDeviceType() {return m_deviceType; }
    void        setUpDevice();


protected:
    void mousePressEvent ( QMouseEvent * event ) ;


public slots:


signals:
     void valueChanged();

private:
    //the device id
    int _did;
    // the position
    int _row;
    int _col;
    // the width
    int _width;
    // the heigth
    int _heigth;

    // the slot size
    int _slotwidth;
    int _slotheight;


    void loadGui();
    QImage loadImage(QString imgName);
    void loadSensibleAreas(QDomElement el);
    void loadFeedbackAreas(QDomElement el);
    void loadBackgroundImage(QDomElement el);

    QMap<QString, ImageView> _sensibleAreas;
    QMap<QString, QMap<QString, ImageView> > _feedbackAreas;
    QMap<QString, QString > _feedbackStatus;

    QImage      m_backgroundImage;
    QString     m_plantDev;
    QString     m_plantName;
    int         m_deviceType;

    bool areaPressed(QPoint topleft, QSize siz, QPoint pressedPoint);

};

#endif // DEVICEMODEL_H
