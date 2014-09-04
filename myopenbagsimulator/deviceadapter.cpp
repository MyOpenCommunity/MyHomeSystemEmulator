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

#include "deviceadapter.h"

#include <devicemodel.h>
#include <devicewidget.h>
#include <QDebug>
#include <plantmgr.h>
#include <setupwidget.h>
#include <syserror.h>
#include <QByteArray>
#include <plantdevice.h>
#include <QMap>
#include <QString>
#include <graphdialog.h>


DeviceAdapter::DeviceAdapter(QString plantName, QObject *parent) :
    QObject(parent)
{
    _deviceCounter = 0;
    _plantName = plantName;
}

bool DeviceAdapter::addDevice(DeviceModel* dModel)
{
    if (dModel->getDId() < 0)
    {
        // NUOVO DEVICE CREATO SENZA ID DEVE ESSERE AGGIUNTO AL DIZIONARIO...
        bool foundId = false;
        do
        {
            _deviceCounter++;
            if (!_mapDeviceModel.contains(_deviceCounter))
            {
                dModel->setDId(_deviceCounter);
                foundId = true;
            }

        }while(foundId != true);
    }

    if (_mapDeviceModel.contains(dModel->getDId()))
    {
        qDebug() << "ERROR : Inserito device con lo stesso ID = " << dModel->getDId();
        _mapDeviceModel.remove(dModel->getDId());
    }

    _mapDeviceModel.insert(dModel->getDId(),dModel);
    qDebug() << "Added device ID : " << dModel->getDId() << " pos : " << dModel->getCol() << "," << dModel->getRow();
    return true;
}

bool DeviceAdapter::delDevice(int idDev)
{
    if (_mapDeviceModel.contains(idDev))
    {
        PlantMgr &theMgr = PlantMgr::getInstance();
        QSharedPointer<Plant> plant = theMgr.getPlant(_plantName);
        if (!plant->removeDevice(_mapDeviceModel[idDev]->GetPlantDev())) {
            plant->removeGateway(_mapDeviceModel[idDev]->GetPlantDev());
        }
        if (_mapDeviceModel.contains(idDev)) {
            qDebug() << "Remove " << _mapDeviceModel.remove(idDev) << " devices";
        }
    }
    qDebug() << "Removed device ID : " << idDev;
    return true;
}

bool DeviceAdapter::setupDevice(int idDev)
{
    if (_mapDeviceModel.contains(idDev))
    {
        QString qStr="4#";  // CODICE CARICAMENT XML
        QByteArray qBary;
        SysError sysErr;
        QByteArray theResult;
        qBary.append(qStr);

        //        _mapDeviceModel[idDev]->_plantDev->executeCmd(CmdExecutor::HIGH, qBary, theResult, sysErr );
        QSharedPointer<Plant> plant = PlantMgr::getInstance().getPlant(_mapDeviceModel[idDev]->GetPlantName());

        switch (_mapDeviceModel[idDev]->GetDeviceType()) {
        case 0:
            plant->getGateway(_mapDeviceModel[idDev]->GetPlantDev())->executeCmd(CmdExecutor::HIGH, qBary, theResult, sysErr );
            break;
        case 1:
            plant->getDevice(_mapDeviceModel[idDev]->GetPlantDev())->executeCmd(CmdExecutor::HIGH, qBary, theResult, sysErr );
            break;
        default:
            qDebug() << "Value not handled ... ";
        }

        QDomDocument doc("mydocument");

        if (!doc.setContent(theResult))
        {
            qDebug() << "ERRORE : Il file xml non è corretto ....";
            return false;
        }

        qDebug() << doc.toString();
        QDomElement docElem = doc.documentElement();
                        SysError theErr;
                        QString newConf;
        if(doc.toString().contains("BtF520_dev")) {
            GraphDialog grdDialog(docElem);
            int resDialog = grdDialog.exec();
            qDebug() << "RESULT DAL QDIALOG " << resDialog;
            if (resDialog==QDialog::Accepted) {
                newConf = grdDialog.newConfParam(theErr);
            } else {
                return true;
            }
        } else {
            Dialog dialog(docElem);
            int resDialog = dialog.exec();
            qDebug() << "RESULT DAL QDIALOG " << resDialog;
            if (resDialog==QDialog::Accepted) {
                newConf = dialog.newConfParam(theErr);
            } else {
                return true;
            }
        }


        if (theErr.isError())
        {
            QMessageBox::information(0, QString("Information"), theErr.toString(), QMessageBox::Ok);
            return false;
        }


        qDebug() << "RETURN DAL QDIALOG " << newConf;
        QString msg = "5#";
        msg.append(newConf);
        QByteArray theResultResp;
        SysError sysErrResp;

        QByteArray qBaryResp;
        qBaryResp.append(msg);

        //            _mapDeviceModel[idDev]->_plantDev->executeCmd(CmdExecutor::HIGH, qBary, theResult, sysErr );
        QSharedPointer<Plant> plant2 = PlantMgr::getInstance().getPlant(_mapDeviceModel[idDev]->GetPlantName());
        switch (_mapDeviceModel[idDev]->GetDeviceType()) {
        case 0:
            plant2->getGateway(_mapDeviceModel[idDev]->GetPlantDev())->executeCmd(CmdExecutor::HIGH, qBaryResp, theResultResp, sysErrResp );
            break;
        case 1:
            plant2->getDevice(_mapDeviceModel[idDev]->GetPlantDev())->executeCmd(CmdExecutor::HIGH, qBaryResp, theResultResp, sysErrResp );
            break;
        default:
            qDebug() << "Value not handled ... ";
        }

        qDebug()<< (sysErrResp.isError() ? "true" : "false");
        if(sysErrResp.isError()) {
            QMessageBox::information(0, QString("Error"), sysErrResp.toString(), QMessageBox::Ok);
        }

    }
    return true;
}

bool DeviceAdapter::setupDeviceConfiguration(int idDev, QString xmlConf)
{
    if (_mapDeviceModel.contains(idDev))
    {
        qDebug() << "CONF";
        qDebug() << xmlConf;
        QString msg = "5#";
        msg.append(xmlConf);
        QByteArray theResult;
        SysError sysErr;

        QByteArray qBary;
        qBary.append(msg);

        //        _mapDeviceModel[idDev]->_plantDev->executeCmd(CmdExecutor::HIGH, qBary, theResult, sysErr );
        QSharedPointer<Plant> plant = PlantMgr::getInstance().getPlant(_mapDeviceModel[idDev]->GetPlantName());
        switch (_mapDeviceModel[idDev]->GetDeviceType()) {
        case 0:
            plant->getGateway(_mapDeviceModel[idDev]->GetPlantDev())->executeCmd(CmdExecutor::HIGH, qBary, theResult, sysErr );
            break;
        case 1:
            plant->getDevice(_mapDeviceModel[idDev]->GetPlantDev())->executeCmd(CmdExecutor::HIGH, qBary, theResult, sysErr );
            break;
        default:
            qDebug() << "Type not handled ... ";
        }
        return(true);
    }
    return(false);
}

DeviceModel* DeviceAdapter::getDevice(int did)
{
    return _mapDeviceModel.value(did);
}

//
QWidget* DeviceAdapter::getView(int did, QWidget* theView, QWidget* parent)
{
    if (theView)
        return theView;
    else
    {
        if (_mapDeviceModel.contains(did) )
        {
            DeviceWidget* dw = new DeviceWidget(_mapDeviceModel.value(did),parent);
            return (dw);
        }
    }

    return 0;
}
