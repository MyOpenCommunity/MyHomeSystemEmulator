#include "devicemodel.h"
#include <syserror.h>
#include <QByteArray>
#include <QDebug>
#include <QPainter>
#include <QtXml>
#include <plantmgr.h>
#include <plant.h>
#include <QMessageBox>


ImageView::ImageView(QString imgName, QPoint pos, QSize siz, QImage pix)
{
    imageName = imgName;
    position = pos;
    size = siz;
    img = pix;
}

DeviceModel::DeviceModel(QString plantDev, QString plantName, int deviceType, int did,  QObject */*parent*/)
{
    m_plantDev = plantDev;
    m_plantName = plantName;
    m_deviceType = deviceType;

    loadGui();

    SysError error;
//    _plantDev->registerEventHnd(this, error);


    PlantMgr &theMgr = PlantMgr::getInstance();
    QSharedPointer<Plant> plant = theMgr.getPlant(m_plantName);
    switch (m_deviceType) {
    case 0:
        plant->getGateway(m_plantDev)->registerEventHnd(this, error);
        break;
    case 1:
        plant->getDevice(m_plantDev)->registerEventHnd(this, error);
        break;
    default:
        qDebug() << "Value not handled ... ";
    }

    _did = did;

    _row = -1;
    _col = -1;
}

void DeviceModel::setPanelPosition(int row, int col)
{
    _row = row;
    _col = col;
}

void DeviceModel::setPanelSize(int width,int height)
{
    _slotwidth = width;
    _slotheight = height;
    emit valueChanged();
}

void DeviceModel::loadGui()
{
    SysError sysErr;
    QByteArray theResult;

    QString qStr="2#";  // CODICE CARICAMENT XML
    QByteArray qBary;
    qBary.append(qStr);

//    _plantDev->executeCmd(_plantDev->HIGH, qBary, theResult, sysErr );
    QSharedPointer<Plant> plant = PlantMgr::getInstance().getPlant(m_plantName);
    switch (m_deviceType) {
    case 0:
        plant->getGateway(m_plantDev)->executeCmd(CmdExecutor::HIGH, qBary, theResult, sysErr );
        break;
    case 1:
        plant->getDevice(m_plantDev)->executeCmd(CmdExecutor::HIGH, qBary, theResult, sysErr );
        break;
    default:
        qDebug() << "Value not handled ... ";
    }

    QDomDocument doc("mydocument");

    if (!doc.setContent(theResult))
    {
        qDebug() << "ERRORE : Il file xml non è corretto ....";
        return;
    }

    QDomElement docElem = doc.documentElement();
    loadBackgroundImage(docElem);   // LOAD BACKGROUND
    loadSensibleAreas(docElem);     // LOAD SENSIBLE AREAS
    loadFeedbackAreas(docElem);     // LOAD LEDS
}

void DeviceModel::loadBackgroundImage(QDomElement el)
{
    QDomNodeList nodeList = el.elementsByTagName("Background");
    if (nodeList.count() == 1)
    {
        QString image(nodeList.at(0).attributes().namedItem("image").nodeValue());
        m_backgroundImage = loadImage(image);
        QString size(nodeList.at(0).attributes().namedItem("size").nodeValue());
        _width = size.toInt();
        _heigth = 1;
    } else {
        qDebug() << "ERROR : Background image error ....";
    }
}

void DeviceModel::loadSensibleAreas(QDomElement el)
{
    QDomNodeList nodeList = el.elementsByTagName("SensibleAreas").at(0).toElement().elementsByTagName("SensibleArea");
    if (nodeList.count() > 0)
    {
        for(int iDx = 0;iDx < nodeList.count(); iDx++)
        {
            QString sens_id(nodeList.at(iDx).attributes().namedItem("id").nodeValue());
            QString sens_pos(nodeList.at(iDx).attributes().namedItem("pos").nodeValue());
            QString sens_size(nodeList.at(iDx).attributes().namedItem("size").nodeValue());

            qDebug() << "Sensible : ID " << sens_id << " POS: " << sens_pos <<" SIZE:" << sens_size;

            QStringList cord_pos =  sens_pos.split(',');
            QString x_pos = cord_pos[0];
            QString y_pos = cord_pos[1];
            QStringList cord_size =  sens_size.split(',');
            QString x_siz = cord_size[0];
            QString y_siz = cord_size[1];

            _sensibleAreas.insert(sens_id, ImageView("", QPoint(x_pos.toInt(), y_pos.toInt()), QSize(x_siz.toInt(),y_siz.toInt()), loadImage("")));
        }
    }
}

void DeviceModel::loadFeedbackAreas(QDomElement el)
{
    QDomNodeList nodeList = el.elementsByTagName("FeedbackAreas").at(0).toElement().elementsByTagName("FeedbackArea");
    if (nodeList.count() > 0)
    {
        _feedbackAreas.clear();
        _feedbackStatus.clear();

        for(int iDx = 0;iDx < nodeList.count(); iDx++)
        {
            QString feed_id(nodeList.at(iDx).attributes().namedItem("id").nodeValue());
            QString feed_pos(nodeList.at(iDx).attributes().namedItem("pos").nodeValue());
            QString feed_size(nodeList.at(iDx).attributes().namedItem("size").nodeValue());
            QString feed_init(nodeList.at(iDx).attributes().namedItem("initValue").nodeValue());


            qDebug() << "Feedback : ID: " << feed_id << " POS: " << feed_pos <<" SIZE: " << feed_size << " INITVALUE: " << feed_init;

            QStringList cord_pos =  feed_pos.split(',');
            QString x_pos = cord_pos[0];
            QString y_pos = cord_pos[1];
            QStringList cord_size =  feed_pos.split(',');
            QString x_siz = cord_size[0];
            QString y_siz = cord_size[1];

            QMap<QString, ImageView> btnDict;
            QDomNodeList imageList = nodeList.at(iDx).toElement().elementsByTagName("FeedbackImage");
            if (imageList.count() > 0)
            {
                for(int iDimage = 0;iDimage < imageList.count(); iDimage++)
                {
                    QString feed_dict_key(imageList.at(iDimage).attributes().namedItem("key").nodeValue());
                    QString feed_dict_image(imageList.at(iDimage).attributes().namedItem("image").nodeValue());

                    qDebug() << "Feedback dict : ID " << feed_id << " KEY: " << feed_dict_key << " POS: " << feed_pos << " SIZE: " << feed_size << " IAMGE: " << feed_dict_image;
                    btnDict.insert(feed_dict_key, ImageView(feed_dict_image,  QPoint(x_pos.toInt(), y_pos.toInt()), QSize(x_siz.toInt(),y_siz.toInt()), loadImage(feed_dict_image)));

                }
                _feedbackAreas.insert(feed_id, btnDict);
                _feedbackStatus.insert(feed_id, feed_init);
            }
        }
    }
}

QImage DeviceModel::loadImage(QString imgName)
{
    QImage imgNull;
    if (imgName.isEmpty())
        return imgNull;

    if (!m_plantDev.isNull())
    {
        SysError errr;
        QByteArray theResult;

        QString qStr="1#" + imgName;
        QByteArray qBary;
        qBary.append(qStr);

//                _plantDev->executeCmd(_plantDev->HIGH, qBary, theResult, errr );
        QSharedPointer<Plant> plant = PlantMgr::getInstance().getPlant(m_plantName);
        switch (m_deviceType) {
        case 0:
            plant->getGateway(m_plantDev)->executeCmd(CmdExecutor::HIGH, qBary, theResult, errr );
            break;
        case 1:
            plant->getDevice(m_plantDev)->executeCmd(CmdExecutor::HIGH, qBary, theResult, errr );
            break;
        default:
            qDebug() << "Value not handled ... ";
        }




        QImage img;
        img.loadFromData(theResult);
        qDebug() << "OK nel caricamento dell'immagine.... ";
        return img;
    }

    return imgNull;
}

// BUILD QIMAGE DEL WIDGET
QImage DeviceModel::createGui()
{
    QImage resultImage;
    resultImage = QImage(m_backgroundImage.size(), QImage::Format_ARGB32_Premultiplied);
    QPainter::CompositionMode mode = QPainter::CompositionMode_DestinationOver;

    QPainter painter(&resultImage);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(resultImage.rect(), Qt::transparent);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawImage(0, 0, m_backgroundImage);
    painter.setCompositionMode(mode);

    foreach(QString key, _feedbackAreas.keys() )
    {
        QMap<QString, QString >::iterator theIter = _feedbackStatus.find(key);
        if(theIter != _feedbackStatus.end())    {
            QString retVal = theIter.value();   // IMMAGINE DA USARE
            //qDebug() << "Valore nel dizionario " << retVal;


            QMap<QString, QMap<QString, ImageView> >::iterator theIterDict = _feedbackAreas.find(key);
            if(theIterDict != _feedbackAreas.end())
            {
                QMap<QString, ImageView> retValDict = theIterDict.value();


                QMap<QString, ImageView>::iterator theIterDictVal = retValDict.find(retVal);
                if(theIterDictVal != retValDict.end())
                {
                    QImage imma = ((ImageView)theIterDictVal.value()).img;
                    QPoint poin = ((ImageView)theIterDictVal.value()).position;
                    painter.drawImage(poin, imma);
                }
            }
            if(retVal.isNull())
                qDebug()<<"No element found";
        }
    }

    painter.end();

    return resultImage;
}

void DeviceModel::MouseClicked(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        QMapIterator<QString, ImageView> i(_sensibleAreas);
        while (i.hasNext()) {
            i.next();
            ImageView t = (ImageView)i.value();
            if (areaPressed(t.position, t.size, event->pos()))
            {
                SysError theErr;
                QByteArray theResult;

                QString qStr="3#" + i.key();
                QByteArray qBary;
                qBary.append(qStr);

//                _plantDev->executeCmd(_plantDev->HIGH, qBary, theResult, theErr );
                QSharedPointer<Plant> plant = PlantMgr::getInstance().getPlant(m_plantName);
                switch (m_deviceType) {
                case 0:
                    plant->getGateway(m_plantDev)->executeCmd(CmdExecutor::HIGH, qBary, theResult, theErr );
                    break;
                case 1:
                    plant->getDevice(m_plantDev)->executeCmd(CmdExecutor::HIGH, qBary, theResult, theErr );
                    break;
                default:
                    qDebug() << "Value not handled ... ";
                }
                if (theErr.isError())
                {
                    QMessageBox::information(0, QString("Error"), theErr.toString(), QMessageBox::Ok);
                }
            }
        }
    }
}


void DeviceModel::mousePressEvent ( QMouseEvent * event )
{
    event->ignore();  // ignoro l'evento in modo tale da propagarlo al livello superiore...
}

bool DeviceModel::areaPressed(QPoint topleft, QSize siz, QPoint pressedPoint)
{
    if ((pressedPoint.rx() >= topleft.rx()) && pressedPoint.rx()<= (topleft.rx()+siz.width()))
    {
        if (pressedPoint.ry() >= topleft.ry() && pressedPoint.ry() <= topleft.ry()+siz.height())
        {
            return true;
        }
    }
    return false;
}


// GESTICO GLI EVENTI PROVENIENTI DAL DEVICE....
void DeviceModel::handleEvent(CMD_PRIO /*thePrio*/, const QByteArray& theEvent)
{
    QString myString(theEvent);
    qDebug() << "Chiamata la HANDLE EVENT " << myString;
    if (myString.isEmpty())
        return;

    // CAMBIO LO STATO DEL VISIBLE LED
    QStringList param = myString.split(":");
    _feedbackStatus.insert(param[0], param[1]);
}
