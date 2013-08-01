#-------------------------------------------------
#
# Project created by QtCreator 2012-06-27T21:53:29
#
#-------------------------------------------------

QT       += network xml

QT       -= gui

TARGET = bt_PLANT_PGIN
TEMPLATE = lib
CONFIG += plugin
VERSION = 1.0.4
TARGET_EXT = .dll

INCLUDEPATH = ../libplant

DESTDIR = ../myhome/plugins

DEFINES += BT_PLANT_PGIN_LIBRARY

SOURCES += bt_plant_pgin.cpp \
    btplant.cpp

HEADERS += bt_plant_pgin.h\
        bt_PLANT_PGIN_global.h \
    btplant.h


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../BUILD_REL/libplant/release/ -llibplant
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../BUILD_DEB/libplant/debug/ -llibplant
else:unix:CONFIG(release, debug|release) LIBS += -L$$PWD/../BUILD_REL/libplant -llibplant
else:unix:CONFIG(debug, debug|release) LIBS += -L$$PWD/../BUILD_DEB/libplant -llibplant

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE7A34866
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = bt_PLANT_PGIN.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
