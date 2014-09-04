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

#include "plantmgr.h"
#include <QDir>
#include <QLibrary>
#include <QPluginLoader>
#include <syserror.h>
#include <QDebug>
#include <QDomDocument>

#include <QtGlobal>
#include <QCoreApplication>

static const QString LOG_FILE_NAME = "MyOpenBag.log";
static const QString DEBUG = "DEBUG";
static const QString LOG = "LOG";

static QFile m_logFile;
static QTextStream *m_logOutput;

QString PlantMgr::logIsActive = "";


//typedef void (*QtMessageHandler)(QtMsgType, const QMessageLogContext &, const QString &);
void myMessageOutput(QtMsgType type, const QMessageLogContext &msgContext, const QString &msg)
{

    if(PlantMgr::logIsActive.compare(LOG) == 0) {
        switch (type) {
        case QtDebugMsg:
            *m_logOutput << "Debug: " << msg << endl;
            break;
        case QtWarningMsg:
            *m_logOutput << "Warning: " << msg << endl;
            break;
        case QtCriticalMsg:
            *m_logOutput << "Critical: " << msg << endl;
            break;
        case QtFatalMsg:
            *m_logOutput << "Fatal: " << msg << endl;
            break;
        }
    }
}

void PlantMgr::activeLog(QString active) {

    logIsActive = active;
    if(active.compare(DEBUG) != 0)
        qInstallMessageHandler(myMessageOutput);
        //qInstallMessageHandler(myMessageOutput);
//        qInstallMsgHandler(myMessageOutput);

    if (logIsActive.compare(LOG) == 0) {
        m_logFile.setFileName(LOG_FILE_NAME);
        if(QFile::exists(LOG_FILE_NAME))
            QFile::remove(LOG_FILE_NAME);
        m_logFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
        m_logOutput = new QTextStream(&m_logFile);
    }

}

PlantMgr::PlantMgr():PLANT_EXT("_PLANT_PGIN"), GW_EXT("_GTW_PGIN"), DEV_EXT("_DEV_PGIN"), BUS_EXT("_BUS_PGIN"), BUSC_EXT("_BUSCONN_PGIN")
{

}

PlantMgr::~PlantMgr() {
    if (logIsActive.compare(LOG) == 0)
        m_logFile.close();
}

PlantMgr&                       PlantMgr::getInstance() {
    static PlantMgr thePlant;

    return(thePlant);
}

QSharedPointer<Plant>                         PlantMgr::addPlant(const QString& theKind, const QString& theID, SysError& theErr){
    if(getPlant(theID).isNull())   {
        QSharedPointer<Plant> thePlantRef = createPlant(theKind, theErr);
        thePlantRef->setID(theID);
        m_plantList.insert(theID, thePlantRef);
        return(thePlantRef);
    }
    else    {
        qWarning()<<"Error attempt to add a plant with an already present ID: "<< theID;
        return(QSharedPointer<Plant>());
    }

}

QSharedPointer<Plant>    PlantMgr::getPlant(const QString& theId)   {
    QSharedPointer<Plant> retVal;

    QMap<QString, QSharedPointer<Plant> >::iterator theIter = m_plantList.find(theId);
    if(theIter != m_plantList.end())    {
        retVal = theIter.value();
    }

    return(retVal);
}

QList<QSharedPointer<Plant> >             PlantMgr::getPlantList(void)  {
    return(m_plantList.values());
}

void                            PlantMgr::deletePlant(const QString& theId) {
    if( !(getPlant(theId).isNull()) )
        m_plantList.remove(theId);
}

QSharedPointer<Plant>           PlantMgr::createPlant(const QString& theKind, SysError& theErr)    {
    QSharedPointer<Plant> retVal;
    QMap<QString, PlantFactory *>::iterator cTor = m_plantFactList.find(theKind);
    if(cTor != m_plantFactList.end())   {
        PlantFactory * theFact = cTor.value();

        if(theFact != 0)
            retVal = theFact->createPlant(theKind, theErr);

        if(!retVal.isNull() && !theErr.isError())
            retVal->init(theErr);

        return(retVal);
    }
    else    {
        theErr = SysError(1001, "Error Building the required Plant object" + theKind);
        return(retVal);
    }
}

QByteArray  PlantMgr::savePlant(const QString& platId, SysError &theErr) {

    return(m_plantList[platId]->save(theErr).toByteArray());

}

QSharedPointer<PlantDevice>     PlantMgr::createDevice(const QString& theKind, SysError& theErr)   {
    QSharedPointer<PlantDevice>  retVal;

    QMap<QString, DeviceFactory *>::iterator cTor = m_devFactList.find(theKind);
    if(cTor != m_devFactList.end())   {
        DeviceFactory * theFact = cTor.value();
        if(theFact != 0)
            retVal=theFact->createDevice(theKind, theErr);

        if(!retVal.isNull() && !theErr.isError())
            retVal->init(theErr);

        return(retVal);
    }
    else    {
        theErr = SysError(1002, "Error Building the required PlantDevice object" + theKind);
        return(retVal);
    }
}

QSharedPointer<PlantBus>        PlantMgr::createBus(const QString& theKind, SysError& theErr)  {

    QSharedPointer<PlantBus>    retVal;
    QMap<QString, BusFactory *>::iterator cTor = m_busFactList.find(theKind);
    if(cTor != m_busFactList.end())   {
        BusFactory * theFact = cTor.value();
        if(theFact != 0)
            retVal = theFact->createBus(theKind, theErr);

//        if(!retVal.isNull() && !theErr.isError())
//            retVal->init(theErr);

        return(retVal);
    }
    else    {
        theErr = SysError(1003, "Error Building the required PlantBus object" + theKind);
        return(retVal);
    }
}

QSharedPointer<PlantBusConn>    PlantMgr::createBusConn(const QString& theKind, SysError& theErr)  {
    QSharedPointer<PlantBusConn>  retVal;
    QMap<QString, BusConnFactory *>::iterator cTor = m_busConnFactList.find(theKind);
    if(cTor != m_busConnFactList.end())   {
        BusConnFactory * theFact = cTor.value();

        if(theFact != 0)
            retVal = theFact->createBusConn(theKind, theErr);

        if(!retVal.isNull() && !theErr.isError())
            retVal->init(theErr);

        return(retVal);
    }
    else    {
        theErr = SysError(1004, "Error Building the required PlantBus object" + theKind);
        return(retVal);
    }
}

QSharedPointer<PlantGateway>    PlantMgr::createGateway(const QString& theKind, SysError& theErr)  {
    QSharedPointer<PlantGateway> retVal;

    QMap<QString, GatewayFactory *>::iterator cTor = m_gatewayFactList.find(theKind);

    if(cTor != m_gatewayFactList.end())   {
        GatewayFactory * theFact = cTor.value();
        if(theFact != 0)
            retVal = theFact->createGateway(theKind, theErr);
//        if(!retVal.isNull() && !theErr.isError())
//            retVal->init(theErr);

        return(retVal);
    }
    else    {
        theErr = SysError(1004, "Error Building the required PlantBus object" + theKind);
        return(retVal);
    }

}

QMap<QString, QString>    PlantMgr::getGatewayType()
{
    QMap<QString, QString> res;
    QString key;
    foreach( key, m_gatewayFactList.keys() )
        res.insert(key , key );

    return res;
}

QMap<QString, QString>    PlantMgr::getDeviceType()
{
    QMap<QString, QString> res;
    QString key;
    foreach( key, m_devFactList.keys() )
        res.insert(key , key );

    return res;
}

QMap<QString, QString>    PlantMgr::getBusType()
{
    QMap<QString, QString> res;
    QString key;
    foreach( key, m_busFactList.keys() )
        res.insert(key , key );

    return res;
}

QMap<QString, QString>    PlantMgr::getBusConnType()
{
    QMap<QString, QString> res;
    QString key;
    foreach( key, m_busConnFactList.keys() )
        res.insert(key , key );

    return res;
}

QMap<QString, QString>    PlantMgr::getPlantType()
{
    QMap<QString, QString> res;
    QString key;
    foreach( key, m_plantFactList.keys() )
        res.insert(key , key );

    return res;
}

void    PlantMgr::setPluginPath(const QString& thePath) {
    m_pluginPath = thePath;
}

void     PlantMgr::addPluginFactory(const QString& libName, PlantMgr::PGIN_TYPE theType)   {

    QObject* thePlugInRef=0;
    QString theID="";

    QPluginLoader loader(libName);

    thePlugInRef=loader.instance();

    qDebug()<< thePlugInRef->metaObject()->className();

    if(thePlugInRef != 0)   {

        switch(theType) {
        case    PLANT:
        {
            PlantFactory* theFact = qobject_cast<PlantFactory*>(thePlugInRef);
            if(theFact != 0)    {
                theID = theFact->getFactoryID();
                if(m_plantFactList.contains(theID)>0)   {
                    qWarning()<<"Error a plugin with the same ID has been already loaded: "<<libName;
                }
                else    {
                    m_plantFactList.insert(theID, theFact);
                    qDebug()<<"Loaded the plugin library: "<<libName;
                }
            }
            else {
                qWarning()<<"Error loading factory for plugin: ";
            }
            break;
        }
        case    DEVICE:
        {
            DeviceFactory* theFact = qobject_cast<DeviceFactory*>(thePlugInRef);
            if(theFact != 0)    {
                QString deviceIDs(theFact->getFactoryID());
                QStringList devicesLstIDs = deviceIDs.split('|', QString::SkipEmptyParts);
                foreach (QString deviceID, devicesLstIDs) {
                    QString theID(deviceID);
                    if(m_devFactList.contains(theID)>0)   {
                        qWarning()<<"Error a plugin with the same ID has been already loaded: "<<libName;
                    }
                    else    {
                        m_devFactList.insert(theID, theFact);
                        qDebug()<<"Loaded the plugin library: "<<libName;
                    }
                }
            }
            else {
                qWarning()<<"Error loading factory for plugin: "<<libName;
            }
            break;
        }
        case    BUS:
        {
            BusFactory* theFact = qobject_cast<BusFactory*>(thePlugInRef);
            if(theFact != 0)    {
                QString theID(theFact->getFactoryID());
                if(m_busFactList.contains(theID)>0)   {
                    qWarning()<<"Error a plugin with the same ID has been already loaded: "<<libName;
                }
                else    {
                    m_busFactList.insert(theID, theFact);
                    qDebug()<<"Loaded the plugin library: "<<libName;
                }
            }
            else {
                qWarning()<<"Error loading factory for plugin: "<<libName;
            }
            break;
        }
        case    BUSCONN:
        {
            BusConnFactory* theFact = qobject_cast<BusConnFactory*>(thePlugInRef);
            if(theFact != 0)    {
                QString theID(theFact->getFactoryID());
                if(m_busConnFactList.contains(theID)>0)   {
                    qWarning()<<"Error a plugin with the same ID has been already loaded: "<<libName;
                }
                else    {
                    m_busConnFactList.insert(theID, theFact);
                    qDebug()<<"Loaded the plugin library: "<<libName;
                }
            }
            else {
                qWarning()<<"Error loading factory for plugin: "<<libName;
            }
            break;
        }
        case    GATEWAY:
        {
            GatewayFactory* theFact = qobject_cast<GatewayFactory*>(thePlugInRef);
            if(theFact != 0)    {
                QString theID(theFact->getFactoryID());
                if(m_gatewayFactList.contains(theID)>0)   {
                    qWarning()<<"Error a plugin with the same ID has been already loaded: "<<libName;
                }
                else    {
                    m_gatewayFactList.insert(theID, theFact);
                    qDebug()<<"Loaded the plugin library: "<<libName;
                }
            }
            else {
                qWarning()<<"Error loading factory for plugin: "<<libName;
            }
            break;
        }
        default:
            qWarning()<<"Error unhadled plugin type";
            break;
        }

    }
    else
    {
        qDebug()<<"Unable to create Instance Loader for:"<<libName;
        qDebug()<<"Error Detail: "<< loader.errorString();
    }
}

void    PlantMgr::loadPlugin(void)  {
    QDir pginDir(m_pluginPath);

    qDebug() << "My Path: " << pginDir.currentPath();

    QString theAbsPathName="";
    foreach(QString fileName, pginDir.entryList(QDir::Files))   {
        theAbsPathName = pginDir.absoluteFilePath(fileName);
        if(QLibrary::isLibrary(theAbsPathName))  {
            if(fileName.contains(PLANT_EXT, Qt::CaseInsensitive))   {
                addPluginFactory(theAbsPathName, PLANT);
            }
            else if(fileName.contains(GW_EXT, Qt::CaseInsensitive))   {
                addPluginFactory(theAbsPathName, GATEWAY);
            }
            else if(fileName.contains(DEV_EXT, Qt::CaseInsensitive))    {
                addPluginFactory(theAbsPathName, DEVICE);
            }
            else if(fileName.contains(BUS_EXT, Qt::CaseInsensitive))    {
                addPluginFactory(theAbsPathName, BUS);
            }
            else if(fileName.contains(BUSC_EXT, Qt::CaseInsensitive))    {
                addPluginFactory(theAbsPathName, BUSCONN);
            }
            else    {
                qWarning()<<"Unmanaged Plugin found: "<<theAbsPathName;
            }
        }
    }
}


const QString PlantMgr::getBuses() {

    qDebug() << "getBuses";
    QString rtnValue;

    QList<QSharedPointer<PlantBus> > mainBUSs = this->getPlant(this->getCurrPlantName())->getBusList();
    foreach (QSharedPointer<PlantBus> mainBus, mainBUSs) {
        qDebug() << "main bus: " << mainBus->getID();
        rtnValue = rtnValue + "," + mainBus->getID();
    }

    QList<QSharedPointer<PlantBusConn> > secondaryBUSs = this->getPlant(this->getCurrPlantName())->getBusConnList();
    foreach (QSharedPointer<PlantBusConn> bus, secondaryBUSs) {
        qDebug() << "bus: " << bus->getID();
        rtnValue = rtnValue + "," + bus->getID();
    }

    qDebug() << "rtnValue: " << rtnValue;

    return rtnValue;
}






unsigned int    PlantMgr::executeCmd(CMD_PRIO /*thePrio*/, const QByteArray& /*theCmd*/, QByteArray& /*theResult*/, SysError& /*theErr*/)   {

    return(0);
}

bool            PlantMgr::cancelCmd(unsigned int /*theCmdId*/)  {
    return(false);
}
