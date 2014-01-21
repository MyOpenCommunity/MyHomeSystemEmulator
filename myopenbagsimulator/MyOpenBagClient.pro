#-------------------------------------------------
#
# Project created by QtCreator 2012-06-12T14:54:05
#
#-------------------------------------------------

QT       += core xml widgets

TARGET = MyOpenSimulator
TEMPLATE = app
VERSION = 1.3.3

DEFINES += VERSION=\\\"$$VERSION\\\"

SOURCES += main.cpp\
        mainwindow.cpp \
    dragdinwidget.cpp \
    devicewidget.cpp \
    deviceadapter.cpp \
    devicemodel.cpp \
    setupwidget.cpp \
    graphwidget.cpp \
    graphdialog.cpp \
    histogram.cpp \
    column.cpp \
    settingdialog.cpp \
    simulationtime.cpp

HEADERS  += mainwindow.h \
    dragdinwidget.h \
    devicewidget.h \
    deviceadapter.h \
    devicemodel.h \
    setupwidget.h \
    graphwidget.h \
    graphdialog.h \
    histogram.h \
    column.h \
    settingdialog.h \
    simulationtime.h

FORMS    += mainwindow.ui

RESOURCES += \
    resource.qrc



INCLUDEPATH = ../libplant

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../BUILD_REL/libplant/release/ -llibplant
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../BUILD_DEB/libplant/debug/ -llibplant
else:unix:CONFIG(release, debug|release) LIBS += -L$$PWD/../BUILD_REL/libplant -llibplant
else:unix:CONFIG(debug, debug|release) LIBS += -L$$PWD/../BUILD_DEB/libplant -llibplant
