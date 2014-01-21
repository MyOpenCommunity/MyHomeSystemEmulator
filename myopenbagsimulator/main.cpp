#include "mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <plantmgr.h>
#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    if(argc > 1)
    {
        PlantMgr::getInstance().activeLog(argv[1]);
    } else {
        PlantMgr::getInstance().activeLog("");
    }
    MainWindow w;
    w.show();

    return a.exec();
}
