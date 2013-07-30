#-------------------------------------------------
#
# Project created by QtCreator 2012-07-30T14:21:57
#
#-------------------------------------------------

QT       += network xml

QT       -= gui

TARGET = bt_scs_BUS_PGIN
TEMPLATE = lib
CONFIG += plugin
INCLUDEPATH = ../libplant
DESTDIR = ../myhome/plugins
VERSION = 1.0.1
TARGET_EXT = .dll

DEFINES += BT_SCS_BUS_PGIN_LIBRARY

SOURCES += bt_scs_bus_pgin.cpp \
    btscsbus.cpp \
    busxmlserializer.cpp

HEADERS += bt_scs_bus_pgin.h\
        bt_scs_BUS_PGIN_global.h \
    btscsbus.h \
    busxmlserializer.h

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../BIN_libplant/release/ -llibplant
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../BIN_libplant/debug/ -llibplant
else:symbian: LIBS += -llibplant
else:unix: LIBS += -L$$PWD/../BIN_libplant -llibplant

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE7E88883
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = bt_scs_BUS_PGIN.dll
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


