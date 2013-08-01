#-------------------------------------------------
#
# Project created by QtCreator 2013-05-29T15:11:26
#
#-------------------------------------------------

QT       += network xml xmlpatterns

QT       -= gui

TARGET = bt_F520_DEV_PGIN
TEMPLATE = lib
CONFIG += plugin
INCLUDEPATH = ../libplant
DESTDIR = ../myhome/plugins
VERSION = 1.0.2
TARGET_EXT = .dll

DEFINES += BT_F520_DEV_PGIN_LIBRARY

SOURCES += bt_f520_dev_pgin.cpp \
    btf520_dev.cpp \
    f520xmlserializer.cpp \
    f520resource.cpp \
    f520status.cpp

HEADERS += bt_f520_dev_pgin.h\
        bt_F520_DEV_PGIN_global.h \
    btf520_dev.h \
    f520xmlserializer.h \
    f520resource.h \
    f520status.h

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../BUILD_REL/libplant/release/ -llibplant
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../BUILD_DEB/libplant/debug/ -llibplant
else:unix:CONFIG(release, debug|release) LIBS += -L$$PWD/../BUILD_REL/libplant -llibplant
else:unix:CONFIG(debug, debug|release) LIBS += -L$$PWD/../BUILD_DEB/libplant -llibplant

RESOURCES += \
    bt_F520_DEV_PGIN.qrc
