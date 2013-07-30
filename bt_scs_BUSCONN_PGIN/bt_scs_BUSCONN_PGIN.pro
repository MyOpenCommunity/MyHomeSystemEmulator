#-------------------------------------------------
#
# Project created by QtCreator 2012-11-05T09:59:19
#
#-------------------------------------------------

QT       += network xml

QT       -= gui

TARGET = bt_scs_BUSCONN_PGIN
TEMPLATE = lib
CONFIG += plugin
INCLUDEPATH = ../libplant
DESTDIR = ../myhome/plugins
VERSION = 1.0.1
TARGET_EXT = .dll

DEFINES += BT_SCS_BUSCONN_PGIN_LIBRARY

SOURCES += bt_scs_busconn_pgin.cpp \
    btscsbusconn.cpp \
    busconnxml.cpp

HEADERS += bt_scs_busconn_pgin.h\
        bt_scs_BUSCONN_PGIN_global.h \
    btscsbusconn.h \
    busconnxml.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE7BA2F4B
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = bt_scs_BUSCONN_PGIN.dll
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

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../BIN_libplant/release/ -llibplant
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../BIN_libplant/debug/ -llibplant
else:symbian: LIBS += -llibplant
else:unix: LIBS += -L$$PWD/../BIN_libplant/ -llibplant
