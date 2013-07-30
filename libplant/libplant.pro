#-------------------------------------------------
#
# Project created by QtCreator 2012-05-06T21:21:14
#
#-------------------------------------------------

QT       += network xml

QT       -= gui

TARGET = libplant
TEMPLATE = lib
VERSION = 2.1.3
TARGET_EXT = .dll

DEFINES += LIBPLANT_LIBRARY

SOURCES += plantmgr.cpp \
    jsonhandler.cpp \
    plantmessage.cpp \
    plantnode.cpp \
    openmsgdiagnostics.cpp \
    openmsgcommand.cpp \
    openmsg.cpp

HEADERS += plantmgr.h\
        libplant_global.h \
    cmdexecutor.h \
    plant.h \
    jsonhandler.h \
    eventsource.h \
    eventhandler.h \
    plantbus.h \
    plantdevice.h \
    plantbusconn.h \
    plantgateway.h \
    syserror.h \
    plantfactory.h \
    busfactory.h \
    busconnfactory.h \
    devicefactory.h \
    gatewayfactory.h \
    plantnode.h \
    plantmessage.h \
    openmsgdiagnostics.h \
    openmsgcommand.h \
    openmsg.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xEB77A421
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = libplant.dll
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
