#-------------------------------------------------
#
# Project created by QtCreator 2012-07-30T16:17:08
#
#-------------------------------------------------
QT       += network xml

QT       -= gui

TARGET = bt_own_GTW_PGIN
TEMPLATE = lib
CONFIG += plugin
INCLUDEPATH = ../libplant
DESTDIR = ../myhome/plugins
VERSION = 2.2.0
TARGET_EXT = .dll

DEFINES += BT_OWN_GTW_PGIN_LIBRARY

SOURCES += bt_own_gtw_pgin.cpp \
    btowngtw.cpp \
    tcpserver.cpp \
    gwtxmlserializer.cpp \
    webserver.cpp \
    qmyserver.cpp \
    websecureserver.cpp \
    gwtstatus.cpp \

HEADERS += bt_own_gtw_pgin.h\
        bt_own_GTW_PGIN_global.h \
    btowngtw.h \
    tcpserver.h \
    openmsgcommand.h \
    plantmessage.h \
    gwtxmlserializer.h \
    webserver.h \
    qmyserver.h \
    websecureserver.h \
    gwtstatus.h \
    openmsgdiagnostics.h \
    openmsg.h

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../BUILD_REL/libplant/release/ -llibplant
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../BUILD_DEB/libplant/debug/ -llibplant
else:unix:CONFIG(release, debug|release) LIBS += -L$$PWD/../BUILD_REL/libplant -llibplant
else:unix:CONFIG(debug, debug|release) LIBS += -L$$PWD/../BUILD_DEB/libplant -llibplant

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

OTHER_FILES += \
    resources/F454.xml

RESOURCES += \
    bt_own_GTW_PGIN.qrc

