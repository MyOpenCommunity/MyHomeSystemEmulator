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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <dragdinwidget.h>
#include <syserror.h>
#include <QDomElement>
#include <QScrollArea>
#include <QLabel>
#include <QTimer>
#include <simulationtime.h>
#include <QPushButton>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

protected:
    void closeEvent(QCloseEvent *event);

    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void saveAsPlantFile(QString fileName);
    bool saveAsPlantFilePopup();
    void init();
    void setupActions();
    void setupMenus();      // Sostituita da createMenus
    void setupWidgets();
    void openPlantFile(QString fileName);
    void closePlant(QString plantName);
    void createBus(QString idGw, QString busName="");
    void createGateway(QString idGw, QString busId, QString gtwName="", QString pos="", QString conf="");
    void createDevice(QString idDev, QString busId, QString devName="", QString pos="", QString conf="");
    void loadPlant(QDomElement el);
    void loadBus(QDomElement el);
    void loadDevices(QDomElement el, QString busId);
    bool saveOldPlant();
    void setPlantName(QString plantName);
    QString getMonth(int month);


public slots:
    void createNewPlant();
    void createNewPlant(QString plantName, int rows = 3, int col = 30);
    void connectToServer();
    void openPlantFile();
    void savePlantFile();
    void saveAsPlantFile();
    void createNewGateway();
    void createNewDevice();
    void deleteDevice();
    void addNewBarraDinOnPlant();
    void addNewSlotOnBarraDinOnPlant();
    void about();
    void setDefaultPlant();
    void updateStatusBar(int month, int day, int hour, int minute);

private slots:
    void onPlusClicked();
    void onMinusClicked();

private:
    Ui::MainWindow *ui;
    DeviceAdapter* _da;
    DragDinWidget *_ddw;
    QScrollArea* m_techScroll;

    QMenu *createNewGatewayMenu;
    QMenu *createNewDeviceMenu;
    QMenu *fileMenu;
    QMenu *plantMenu;
    QMenu *optionMenu;
    QMenu *helpMenu;

    QMenu *m_simulation;
    QMenu *m_energySimulator;

    QAction *createAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
//    QAction *connAct;
    QList<QAction *> createNewGateways;
    QList<QAction *> createNewDevices;
    QAction *exitAct;
    QAction *printAct;
    QAction *clearScreenAct;
    QAction *addNewBarraDinAct;
    QAction *addNewSlotOnBarraDinAct;
    QAction *aboutAct;
    QAction *defaultPlant;

    QAction *m_startTimer;
    QAction *m_stopTimer;
    QAction *m_pauseTimer;

    QMap<QString, QString> m_deviceDisponibili;
    QMap<QString, QString> m_gatewayDisponibili;

    SysError theErr;

    QString m_plantName;
    QString m_plantFileName;
    QString m_busName;
    int     m_rows;
    int     m_col;
    QLabel *m_msg;
    QLabel *m_fwr;
    QPushButton *m_plus;
    QPushButton *m_minus;
    QList<QString>  m_speed;
    SimulationTime *m_sbTimer;
};

#endif // MAINWINDOW_H
