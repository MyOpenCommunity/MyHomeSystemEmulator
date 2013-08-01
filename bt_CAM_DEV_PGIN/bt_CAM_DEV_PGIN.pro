#-------------------------------------------------
#
# Project created by QtCreator 2012-09-27T01:47:43
#
#-------------------------------------------------

QT       += network xml

QT       -= gui

TARGET = bt_CAM_DEV_PGIN
TEMPLATE = lib
CONFIG += plugin
INCLUDEPATH = ../libplant
DESTDIR = ../myhome/plugins
VERSION = 1.0.1
TARGET_EXT = .dll

DEFINES += BT_CAM_DEV_PGIN_LIBRARY

SOURCES += bt_cam_dev_pgin.cpp \
    btcam_dev.cpp \
    cam_xmlserializer.cpp \
    camerastatus.cpp

HEADERS += bt_cam_dev_pgin.h\
        bt_CAM_DEV_PGIN_global.h \
    btcam_dev.h \
    cam_xmlserializer.h \
    camerastatus.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../BUILD_REL/libplant/release/ -llibplant
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../BUILD_DEB/libplant/debug/ -llibplant
else:unix:CONFIG(release, debug|release) LIBS += -L$$PWD/../BUILD_REL/libplant -llibplant
else:unix:CONFIG(debug, debug|release) LIBS += -L$$PWD/../BUILD_DEB/libplant -llibplant


OTHER_FILES += \
    resources/CAM.xml

RESOURCES += \
    bt_CAM_DEV_PGIN.qrc
