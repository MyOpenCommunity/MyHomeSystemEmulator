#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <dragdinwidget.h>
#include <deviceadapter.h>
#include <devicemodel.h>

#include <QMessageBox>
#include <QMenu>
#include <QMenuBar>
#include <QPushButton>

#include <QDebug>
#include <QFileDialog>
#include <QtWidgets>
#include <QTime>


// PROVA
#include <plantmgr.h>
#include <syserror.h>
#include <plant.h>
#include <plantdevice.h>

#include <QtCore/QCoreApplication>
#include <QSharedPointer>
#include <QDebug>

static QString MAINWINDOW_TITLE = "My Home Virtual Development Kit";
static QString RESOURCES_FOLDER = ":/xml/resources/";
static QString DEFAULT_PLANT    = "default.plnt";

static QString NAME = "name";
static QString ID   = "id";
static QString TYPE = "type";
static QString BUS  = "bus";
static QString POS  = "pos";
static QString ROWS = "rows";
static QString COLS = "cols";
static QString GTW  = "gateway";
static QString DEV  = "device";

static int SLOT_X   = 50;
static int SLOT_Y   = 285;
static int INITIAL_SIZE_X = 800;
static int INITIAL_SIZE_Y = 600;
static int BARRA_DIN_LIMIT = 10;
static int BLOCK_DIN_LIMIT = 50;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    m_sbTimer = new SimulationTime(this);
    connect(m_sbTimer, SIGNAL(timeUpdate(int,int,int,int)), this, SLOT(updateStatusBar(int,int,int,int)));

    ui->setupUi(this);

    setWindowTitle(MAINWINDOW_TITLE);

    setupWidgets();
    setupActions();
    setupMenus();

    resize(INITIAL_SIZE_X, INITIAL_SIZE_Y);

    setDefaultPlant();


    // Status Bar Init
    m_speed << "x1" << "x2" << "x6" << "x12" << "x30" << "x60" << "x120" << "x240";
    QString message;
    message = QString::number(1) + " - " + getMonth(1) + " (h" + QString("%1").arg(0, 2, 10, QChar('0')).toUpper() + ")";
    m_msg = new QLabel(message, this);
    m_fwr = new QLabel(m_speed[0], this);
    m_fwr->setContentsMargins(10, 0 , 10, 0);
    ui->statusBar->addPermanentWidget(m_msg);
    ui->statusBar->addPermanentWidget(m_fwr);
    m_plus = new QPushButton("+", this);
    connect(m_plus, SIGNAL(clicked()), this, SLOT(onPlusClicked()));
    m_minus = new QPushButton("-", this);
    connect(m_minus, SIGNAL(clicked()), this, SLOT(onMinusClicked()));


    ui->statusBar->addPermanentWidget(m_plus);
    ui->statusBar->addPermanentWidget(m_minus);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setPlantName(QString plantName)
{
    m_plantName = plantName;
    PlantMgr::getInstance().setCurrPlantName(m_plantName);
    QString ttl = MAINWINDOW_TITLE;
    if (!m_plantName.isEmpty())
        ttl.append(" - " + m_plantName);
    setWindowTitle(ttl);
}

void MainWindow::setupActions()
{
    // FILE

    createAct = new QAction(tr("&New..."), this);
    createAct->setShortcut(tr("Ctrl+N"));
    connect(createAct, SIGNAL(triggered()), this, SLOT(createNewPlant()));

    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcut(tr("Ctrl+O"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(openPlantFile()));

    saveAct = new QAction(tr("&Save"), this);
    saveAct->setShortcut(tr("Ctrl+S"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(savePlantFile()));

    saveAsAct = new QAction(tr("&Save As..."), this);
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAsPlantFile()));
    /*
    connAct = new QAction(tr("&Connect to server..."), this);
    connAct->setShortcut(tr("Ctrl+S"));
    connect(connAct, SIGNAL(triggered()), this, SLOT(connectToServer()));
*/
    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    // OPTIONS

    clearScreenAct = new QAction(tr("&Clear Screen"), this);
    connect(clearScreenAct, SIGNAL(triggered()), this, SLOT(deleteDevice()));

    addNewBarraDinAct = new QAction(tr("&Add new DIN bar"), this);
    connect(addNewBarraDinAct, SIGNAL(triggered()), this, SLOT(addNewBarraDinOnPlant()));

    addNewSlotOnBarraDinAct = new QAction(tr("&Add new Slot on DIN bar"), this);
    connect(addNewSlotOnBarraDinAct, SIGNAL(triggered()), this, SLOT(addNewSlotOnBarraDinOnPlant()));


    // PLANT

    if (m_gatewayDisponibili.size() > 0)
    {
        foreach (QString format, m_gatewayDisponibili.keys()) {
            QString text = tr("%1...").arg(QString(format).toUpper());
            QAction *action = new QAction(text, this);
            action->setData(m_gatewayDisponibili[format]);
            connect(action, SIGNAL(triggered()), this, SLOT(createNewGateway()));
            createNewGateways.append(action);
        }
    }

    if (m_deviceDisponibili.size() > 0)
    {
        foreach (QString format, m_deviceDisponibili.keys()) {
            QString text = tr("%1...").arg(QString(format).toUpper());
            QAction *action = new QAction(text, this);
            action->setData(m_deviceDisponibili[format]);
            connect(action, SIGNAL(triggered()), this, SLOT(createNewDevice()));
            createNewDevices.append(action);
        }
    }

    defaultPlant = new QAction(tr("Default Plant"), this);
    defaultPlant->setShortcut(tr("Ctrl+D"));
    connect(defaultPlant, SIGNAL(triggered()), this, SLOT(setDefaultPlant()));


    // HELP

    aboutAct = new QAction(tr("&About"), this);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    m_startTimer = new QAction(tr("&Start Simulation"), this);
    connect(m_startTimer, SIGNAL(triggered()), m_sbTimer, SLOT(startTimer()));
    m_stopTimer = new QAction(tr("&Stop Simulation"), this);
    connect(m_stopTimer, SIGNAL(triggered()), m_sbTimer, SLOT(stopTimer()));
    m_pauseTimer = new QAction(tr("&Pause Simulation"), this);
    connect(m_pauseTimer, SIGNAL(triggered()), m_sbTimer, SLOT(pauseTimer()));
}

void MainWindow::setupMenus()
{
    fileMenu = new QMenu(tr("&File"), this);
    fileMenu->addAction(createAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    saveAct->setEnabled(false);
    fileMenu->addAction(saveAsAct);
    saveAsAct->setEnabled(false);
    //fileMenu->addAction(connAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);


    createNewGatewayMenu = new QMenu(tr("&Create new Gateway"), this);
    foreach (QAction *action, createNewGateways)
        createNewGatewayMenu->addAction(action);

    createNewDeviceMenu = new QMenu(tr("&Create new Device"), this);
    foreach (QAction *action, createNewDevices)
        createNewDeviceMenu->addAction(action);

    plantMenu = new QMenu(tr("&Plant"), this);
    plantMenu->addMenu(createNewGatewayMenu);
    plantMenu->addMenu(createNewDeviceMenu);
    plantMenu->addAction(defaultPlant);
    createNewDeviceMenu->setEnabled(false);
    createNewGatewayMenu->setEnabled(false);


    optionMenu = new QMenu(tr("&Options"), this);
    optionMenu->addAction(clearScreenAct);
    optionMenu->addAction(addNewBarraDinAct);
    optionMenu->addAction(addNewSlotOnBarraDinAct);
    optionMenu->setEnabled(false);

    helpMenu = new QMenu(tr("&Help"), this);
    helpMenu->addAction(aboutAct);

    m_simulation = new QMenu(tr("&Simulation"));
    m_energySimulator = new QMenu(tr("&Energy Simulation"));
    m_simulation->addMenu(m_energySimulator);
    m_energySimulator->addAction(m_startTimer);
    m_energySimulator->addAction(m_stopTimer);
    m_energySimulator->addAction(m_pauseTimer);

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(plantMenu);
    menuBar()->addMenu(m_simulation);
    menuBar()->addMenu(optionMenu);
    menuBar()->addMenu(helpMenu);
}

void MainWindow::setupWidgets()
{
    //  Create the Plant Manager and load all the available plugins
    PlantMgr &theMgr = PlantMgr::getInstance();
    theMgr.setPluginPath("../myhome/plugins");
    theMgr.loadPlugin();

    QString key;
    QMap<QString, QString> gatewayMap = theMgr.getGatewayType();
    foreach( key, gatewayMap.keys() )
    {
        qDebug() << "Gateway FOUND " << key;
        m_gatewayDisponibili.insert(key , key );
    }
    QMap<QString, QString> deviceMap = theMgr.getDeviceType();
    foreach( key, deviceMap.keys() )
    {
        qDebug() << "Device FOUND " << key;
        m_deviceDisponibili.insert(key , key );
    }
}

void MainWindow::about()
{
    QString vdkVersion = "VDK Version: ";
    vdkVersion.append(VERSION);
    QMessageBox::about(this, tr("About VDK"),tr(vdkVersion.toStdString().c_str()));
}

void MainWindow::connectToServer()
{
    QMessageBox msgBox;
    qDebug() << "Connect to Server .... ";
    msgBox.setText("connectToServer.");
    msgBox.exec();
}

void MainWindow::createNewPlant()
{
    if (!saveOldPlant())
        return;

    bool ok;
    QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"), tr("Plant Name:"), QLineEdit::Normal, "", &ok);
    if (ok && !text.isEmpty())
    {
        qDebug() << "Nuovo progetto " << text;
        int r = QInputDialog::getInt(this, tr("QInputDialog::getInteger()"), tr("Rows:"), 2, 1, BARRA_DIN_LIMIT, 1, &ok);
        if (ok)
        {
            int c = QInputDialog::getInt(this, tr("QInputDialog::getInteger()"), tr("Columns:"), 30, 1, BLOCK_DIN_LIMIT, 2, &ok);
            if (ok)
            {
                createNewPlant(text , r, c);

                //  Create a bus
                createBus(PlantMgr::getInstance().getBusType().values()[0]);

                // Set visible actions
                createNewGatewayMenu->setEnabled(true);
                createNewDeviceMenu->setEnabled(true);
                optionMenu->setEnabled(true);
                saveAct->setEnabled(false);
                saveAsAct->setEnabled(true);
            }
        }
    }
}

bool MainWindow::saveOldPlant()
{
    if (m_plantName.isEmpty())
        return true;

    QMessageBox msgBox;
    msgBox.setText("The plant has been modified.");
    msgBox.setInformativeText("Do you want to save your changes?");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    int ret = msgBox.exec();

    switch (ret) {
    case QMessageBox::Save:
        if (!saveAsPlantFilePopup())
            return false;
        break;
    case QMessageBox::Discard:
        break;
    case QMessageBox::Cancel:
        return false;
    default:
        return false;
    }

    closePlant(m_plantName);
    return true;
}

void MainWindow::createNewPlant(QString plantName, int rows, int col)
{
    //  Create a plant
    PlantMgr &theMgr = PlantMgr::getInstance();
    setPlantName(plantName);
    QSharedPointer<Plant> thePlant = theMgr.addPlant(theMgr.getPlantType().values()[0], m_plantName, theErr);
    if (thePlant.isNull())
    {
        QMessageBox::information(0, QString("Information"), QString("The plant name is already in use!"), QMessageBox::Ok);
        return;
    }
    qDebug()<<"This is the PLANT : "<< thePlant->getID();

    // Creazione del widget di drag
    _ddw = new DragDinWidget();
    _ddw->setNumberOfRows(rows);
    _ddw->setNumberOfCol(col);
    m_rows = rows;
    m_col = col;

    _da = new DeviceAdapter(m_plantName);
    _ddw->setAdapter(_da);

    //scrollview so all items fit in window
    m_techScroll = new QScrollArea();
    m_techScroll->setMinimumHeight(SLOT_Y + 4);
    m_techScroll->setMaximumSize(SLOT_X*m_col + 25, SLOT_Y*m_rows + 20);
    m_techScroll->setBackgroundRole(QPalette::Window);
    m_techScroll->setFrameShadow(QFrame::Plain);
    m_techScroll->setFrameShape(QFrame::NoFrame);
    m_techScroll->setFrameStyle(QFrame::Panel | QFrame::Raised);
    m_techScroll->setWidgetResizable(true);
    _ddw->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    m_techScroll->setWidget(_ddw);
    _ddw->setScrollElement(m_techScroll);
    setCentralWidget(m_techScroll);
}

void MainWindow::savePlantFile()
{
    qDebug() << "Save to " << m_plantFileName;
    saveAsPlantFile(m_plantFileName);
}

void MainWindow::saveAsPlantFile()
{
    saveAsPlantFilePopup();
}

bool MainWindow::saveAsPlantFilePopup()
{
    QFileDialog::Options options;
    options |= QFileDialog::DontUseNativeDialog;
    QString selectedFilter;
    QString namefile = m_plantName + ".plnt";
    QString fileName = QFileDialog::getSaveFileName(this, tr("QFileDialog::getSaveFileName()"), namefile, tr("All Files (*)"), &selectedFilter, options);
    if (!fileName.isEmpty()) {
        qDebug() << "Save to " << fileName;
        saveAsPlantFile(fileName);
        m_plantFileName = fileName;
        saveAct->setEnabled(true);
        return true;
    }
    return false;
}


void MainWindow::createNewGateway()
{
    QAction *action = qobject_cast<QAction *>(sender());
    QString tipoDaCreare = action->data().toString();
    qDebug() << "Need to create a gateway : " << tipoDaCreare;
    createGateway(tipoDaCreare, m_busName);
}

void MainWindow::saveAsPlantFile(QString fileName) {

    SysError theErr;
    QFile xmlPlant(fileName);
    if (!xmlPlant.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "saveAsPlantFile: " << xmlPlant.isWritable();
        return;
    }

    QString saveRes(PlantMgr::getInstance().savePlant(m_plantName, theErr));
    if(!theErr.isError()) {
        QDomDocument saveResDom;
        saveResDom.setContent(saveRes);

        QMap<QString, QString> deviceOnPlant;
        deviceOnPlant = _ddw->getDevicesOnBoards();
        saveResDom.documentElement().setAttribute(ROWS,  QString::number(m_rows));
        saveResDom.documentElement().setAttribute(COLS,  QString::number(m_col));
        QDomNodeList nodeList = saveResDom.elementsByTagName(BUS); //.at(0).toElement().childNodes();
        for(int index = 0; index < nodeList.count(); index++){
            QDomNodeList busList = nodeList.at(index).toElement().childNodes();
            if (busList.count() > 0)
            {
                for(int iDx = 0;iDx < busList.count(); iDx++)
                {
                    QString sens_id(busList.at(iDx).attributes().namedItem(ID).nodeValue());
                    if (deviceOnPlant.contains(sens_id))
                    {
                        busList.at(iDx).toElement().setAttribute(POS, deviceOnPlant[sens_id]);
                        qDebug() << "ID: " << sens_id << ":" << deviceOnPlant[sens_id];
                    }
                }
            }

        }

        xmlPlant.write(saveResDom.toString().toStdString().c_str());
    } else {
        QMessageBox::information(0, QString("Error"), theErr.toString(), QMessageBox::Ok);
    }
    xmlPlant.close();
}

void MainWindow::createNewDevice()
{
    QAction *action = qobject_cast<QAction *>(sender());
    QString tipoDaCreare = action->data().toString();
    qDebug() << "Need to create a device : " << tipoDaCreare;
    createDevice(tipoDaCreare, m_busName);
}

void MainWindow::openPlantFile(QString fileName)
{
    QFile inFile( fileName );
    if( ! inFile.open( QIODevice::ReadOnly ) )
    {
        qDebug() << "HostThread: Cant find file " << fileName;
        return;
    }

    QByteArray block;

    block =  inFile.readAll();

    QDomDocument doc("mydocument");

    if (!doc.setContent(block))
    {
        QMessageBox::information(0, QString("Error"), theErr.toString(), QMessageBox::Ok);
        return;
    }

    QDomElement docElem = doc.documentElement();
    loadPlant(docElem);

    // Set visible actions
    createNewGatewayMenu->setEnabled(true);
    createNewDeviceMenu->setEnabled(true);
    optionMenu->setEnabled(true);
    saveAct->setEnabled(true);
    saveAsAct->setEnabled(true);
    m_plantFileName = fileName;

    inFile.close();
}


void MainWindow::closePlant(QString plantName)
{
    qDebug() << "Close plant " << plantName;
    PlantMgr &theMgr = PlantMgr::getInstance();
    // Delete first all the devices
    _ddw->clearPlant();
    // Delete plant from plantManager
    theMgr.deletePlant(plantName);
    // Reset plant name on proj
    setPlantName("");
}

void MainWindow::loadPlant(QDomElement el)
{
    qDebug() << "ELEMENT" << el.tagName();
    qDebug() << "ELEMENT ATTRIBUTE ID" << el.attribute(ID);
    QString plantName = el.attribute(ID);
    qDebug() << "ELEMENT ATTRIBUTE ROWS" << el.attribute(ROWS);
    int plantRows = el.attribute(ROWS).toInt();
    if (el.hasAttribute(COLS))
    {
        qDebug() << "ELEMENT ATTRIBUTE COLS" << el.attribute(COLS);
        int plantCols = el.attribute(COLS).toInt();
        createNewPlant(plantName , plantRows, plantCols);
    } else {
        createNewPlant(plantName , plantRows);
    }

    QDomNodeList nodeList = el.toElement().elementsByTagName(BUS);
    if (nodeList.count() > 0)
    {
        for(int iDx = 0;iDx < nodeList.count(); iDx++)
        {
            qDebug() << "BUS FOUND" << nodeList.at(iDx).toElement().tagName();
            loadBus(nodeList.at(iDx).toElement());
        }
    }
}

void MainWindow::loadBus(QDomElement el)
{
    qDebug() << "ELEMENT: " << el.tagName();
    qDebug() << "ELEMENT ATTRIBUTE ID: " << el.attribute(ID);
    qDebug() << "ELEMENT ATTRIBUTE TYPE: " << el.attribute(TYPE);
    QString busName = el.attribute(ID);
    QString busType = el.attribute(TYPE);
    //PlantMgr &theMgr = PlantMgr::getInstance();
    PlantMgr::getInstance();
    createBus(busType, busName);

    QDomNodeList nodeList = el.toElement().childNodes();
    if (nodeList.count() > 0)
    {
        qDebug() << "NodeListCount " << nodeList.count();
        for(int iDx = 0;iDx < nodeList.count(); iDx++)
        {
            qDebug() << "DEVICE FOUND" << nodeList.at(iDx).toElement().attribute(ID) ;
            loadDevices(nodeList.at(iDx).cloneNode().toElement(), busName);
        }
    }
}

void MainWindow::loadDevices(QDomElement el, QString busId)
{
    qDebug() << "ELEMENT" << el.tagName();
    qDebug() << "ELEMENT ATTRIBUTE ID" << el.attribute(ID);
    QString devName = el.attribute(ID);
    qDebug() << "ELEMENT ATTRIBUTE TYPE" << el.attribute(TYPE);
    QString devType = el.attribute(TYPE);
    qDebug() << "ELEMENT BUS " << busId;
    qDebug() << "ELEMENT ATTRIBUTE POS" << el.attribute(POS);
    QString devPos = el.attribute(POS);

    QDomDocument configuration("");
    configuration.appendChild(el);

    if (el.tagName().compare(DEV) == 0)
        createDevice(devType, busId, devName, devPos, configuration.toString());
    if (el.tagName().compare(GTW) == 0)
        createGateway(devType, busId, devName, devPos, configuration.toString());
}


void MainWindow::createBus(QString idBus, QString busName)
{
    // Recupero l'istanza di plant manager
    PlantMgr &theMgr = PlantMgr::getInstance();

    // Recupero il plant
    QSharedPointer<Plant> plant = theMgr.getPlant(m_plantName);
    qDebug() << "Recuperato  il plant .... " << m_plantName;

    SysError theErr;
    // try to find BUS in main bus list
    QSharedPointer<PlantBus> theBus = theMgr.createBus((idBus), theErr);
    if(theErr.isError()) {
        // don't find bus in main bus list, try to find its in secondary bus list
        //theErr = SysError();
        //theBus = theMgr.createBusConn(idBus, theErr);
        //plant->addBusConn(qSharedPointerCast<PlantBusConn>(theBus));
        //if (theErr.isError()) {
        //    qDebug() << "Error, No BUS find";
        return;
        //}
    }

    if (busName.isEmpty())
    {
        // no bus name, it is e new plant, add main bus
        theBus->setID("Main_Bus");
    } else {
        theBus->setID(busName);
    }
    qDebug()<<"This is Bus " << theBus->getID();
    plant->addBus(theBus);
    m_busName = theBus->getID();


}

void MainWindow::createGateway(QString idGw, QString busId, QString gtwName, QString pos, QString conf)
{
    // Recupero il plant
    QSharedPointer<Plant> plant = PlantMgr::getInstance().getPlant(m_plantName);
    qDebug() << "Recuperato  il plant .... " << m_plantName;

    SysError theErr;
    QSharedPointer<PlantGateway> theGateWay = PlantMgr::getInstance().createGateway(idGw, theErr);
    if (theErr.isError())
    {
        QMessageBox::information(0, QString("Information"), theErr.toString(), QMessageBox::Ok);
        return;
    }
    theGateWay->setBusName(busId);
    if (gtwName.isEmpty())
        theGateWay->setID("GateWay_" + QString::number(qrand() % (10000 + 1)));
    else
        theGateWay->setID(gtwName);
    qDebug()<<"This is the GateWay : "<< theGateWay->getID();

    plant->addGateway(theGateWay, theErr);
    if (theErr.isError())
    {
        QMessageBox::information(0, QString("Information"), theErr.toString(), QMessageBox::Ok);
        return;
    }

    DeviceModel* dm = new DeviceModel(theGateWay->getID(), m_plantName, 0);
    if (!pos.isEmpty())
        dm->setPanelPosition(pos.split(",").at(1).toInt(),pos.split(",").at(0).toInt());

    _da->addDevice(dm);
    _ddw->updateAdapterModel(dm);
    if (dm->getCol() == -1 || dm->getRow() == -1)
    {
        qDebug() << "Impianto troppo piccolo";
        _da->delDevice(dm->getDId());
        QMessageBox::information(0, QString("Information"), "Plant too small ...", QMessageBox::Ok);
        plant->removeGateway(theGateWay->getID());
        return;
    }
    if (!conf.isEmpty())
    {
        _da->setupDeviceConfiguration(dm->getDId(), conf);
    }

    qDebug()<<"Initialize the GateWay : "<< theGateWay->getID();
    theGateWay->init(theErr);
    if (theErr.isError())
    {
        QMessageBox::information(0, QString("Information"), theErr.toString(), QMessageBox::Ok);
        if ((theErr.errCode() != SysError::HTTP_ERROR) && (theErr.errCode() != SysError::HTTPS_ERROR)) {
            qDebug()<<"Error I can not create GateWay : "<< theGateWay->getID();
            plant->removeGateway(theGateWay->getID());
            return;
        }
        else {
            qDebug()<<"Error, but I can create GateWay : "<< theGateWay->getID();
            theErr = SysError();
        }
    }
}

void MainWindow::createDevice(QString idDev, QString busId, QString devName, QString pos, QString conf)
{
    // Recupero il plant
    QSharedPointer<Plant> plant = PlantMgr::getInstance().getPlant(m_plantName);
    qDebug() << "Recuperato  il plant .... " << m_plantName;

    SysError theErr;
    QSharedPointer<PlantDevice> theDevice= PlantMgr::getInstance().createDevice(idDev, theErr);
    theDevice->setBusName(busId);
    if (devName.isEmpty())
        theDevice->setID("Device_" + QString::number(qrand() % (10000 + 1)));
    else
        theDevice->setID(devName);
    qDebug()<<"This is the Device : "<< theDevice->getID();
    plant->addDevice(theDevice);
    qDebug()<<"Device addedd to plant ...";

    DeviceModel* dm = new DeviceModel(theDevice->getID(), m_plantName, 1);

    qDebug() << "POS:" << pos;
    if (!pos.isEmpty())
    {
        qDebug() << "POS:" << pos;
        dm->setPanelPosition(pos.split(",").at(1).toInt(),pos.split(",").at(0).toInt());
    }
    _da->addDevice(dm);
    _ddw->updateAdapterModel(dm);
    if (dm->getCol() == -1 || dm->getRow() == -1)
    {
        qDebug() << "Impianto troppo piccolo";
        _da->delDevice(dm->getDId());
        QMessageBox::information(0, QString("Information"), "Plant too small ...", QMessageBox::Ok);
        return;
    }

    if (!conf.isEmpty())
    {
        _da->setupDeviceConfiguration(dm->getDId(), conf);
    }
}

void MainWindow::deleteDevice()
{
    qDebug() << "Clean plant from all devices ... ";
    _ddw->clearPlant();
}

void MainWindow::addNewBarraDinOnPlant()
{
    if (m_rows >= BARRA_DIN_LIMIT)
    {
        QString msgBoxString = "Limit for BarraDIN reached (" + QString::number(BARRA_DIN_LIMIT) + ").";
        QMessageBox::about(this, tr("Add new BarraDIN"),msgBoxString);
        return;
    }
    qDebug() << "Added new barra din (" << m_rows << " -> " << m_rows+1 << ")";
    m_rows = m_rows + 1;
    m_techScroll->setMaximumHeight(SLOT_Y*m_rows + 20);
    _ddw->setNumberOfRows(m_rows);
}

void MainWindow::addNewSlotOnBarraDinOnPlant()
{
    if (m_col >= BLOCK_DIN_LIMIT)
    {
        QString msgBoxString = "Limit for blocks on a single BarraDIN reached (" + QString::number(BLOCK_DIN_LIMIT) + ").";
        QMessageBox::about(this, tr("Add new BarraDIN"),msgBoxString);
        return;
    }
    qDebug() << "Added new slot (" << m_col << " -> " << m_col+1 << ")";
    m_col = m_col + 1;
    m_techScroll->setMaximumWidth(SLOT_X*m_col + 4);
    _ddw->setNumberOfCol(m_col);
}

void MainWindow::openPlantFile()
{
    if (!saveOldPlant())
        return;

    QString fileName = QFileDialog::getOpenFileName(this, tr("QFileDialog::getOpenFileName()"), QDir::currentPath());
    if (!fileName.isEmpty()) {
        qDebug() << "Open to " << fileName;
        openPlantFile(fileName);
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (saveOldPlant()) {
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::setDefaultPlant() {
    if (!saveOldPlant())
        return;

    QString fileName = RESOURCES_FOLDER + DEFAULT_PLANT;
    if (!fileName.isEmpty()) {
        qDebug() << "Open to " << fileName;
        openPlantFile(fileName);
        saveAct->setEnabled(false);
    }
}

void MainWindow::updateStatusBar(int month, int day, int hour, int minute) {
    qDebug() << "An hour is ended" ;
    QString message;
    message = QString::number(day) + " - " + getMonth(month) + " (h" + QString("%1").arg(hour, 2, 10, QChar('0')).toUpper() + ")";
//    message = QString::number(day) + " - " + getMonth(month) + " (h" + QString::number(hour) + ")";
    m_msg->setText(message);

    QSharedPointer<Plant> plant = PlantMgr::getInstance().getPlant(PlantMgr::getInstance().getCurrPlantName());
    QList< QSharedPointer<PlantDevice> > devices = plant->getDeviceList();
    foreach (QSharedPointer<PlantDevice> device, devices) {
        device->UpdateTime(month, day, hour, minute);
    }
}

QString MainWindow::getMonth(int month) {

    switch (month) {
    case 1:
        return "January";
    case 2:
        return "February";
    case 3:
        return "March";
    case 4:
        return "April";
    case 5:
        return "May";
    case 6:
        return "June";
    case 7:
        return "July";
    case 8:
        return "August";
    case 9:
        return "September";
    case 10:
        return "October";
    case 11:
        return "November";
    case 12:
        return "December";
    default:
        return "";
    }
}

void MainWindow::onPlusClicked() {
    if (m_speed.length()-1 > m_speed.indexOf(m_fwr->text())) {
        m_fwr->setText(m_speed[m_speed.indexOf(m_fwr->text()) + 1]);
        m_sbTimer->setSpeed(m_fwr->text().replace('x',"").toInt());
    }
}

void MainWindow::onMinusClicked() {
    if (m_speed.indexOf(m_fwr->text()) > 0) {
        m_fwr->setText(m_speed[m_speed.indexOf(m_fwr->text()) - 1]);
        m_sbTimer->setSpeed(m_fwr->text().replace('x',"").toInt());
    }
}
