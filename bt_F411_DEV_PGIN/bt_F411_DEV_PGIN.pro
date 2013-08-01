#-------------------------------------------------
#
# Project created by QtCreator 2012-07-30T14:24:41
#
#-------------------------------------------------
QT       += network xml

QT       -= gui

TARGET = bt_F411_DEV_PGIN
TEMPLATE = lib
CONFIG += plugin
INCLUDEPATH = ../libplant
DESTDIR = ../myhome/plugins
VERSION = 1.3.1
TARGET_EXT = .dll

DEFINES += BT_F411_DEV_PGIN_LIBRARY

SOURCES += bt_f411_dev_pgin.cpp \
    btf411dev.cpp \
    ledstatus.cpp \
    f411xmlserializer.cpp \
    btf411_1_dev.cpp \
    btf411_2_dev.cpp \
    btf411_4_dev.cpp

HEADERS += bt_f411_dev_pgin.h\
        bt_F411_DEV_PGIN_global.h \
        btf411dev.h \
        plantmessage.h \
        ledstatus.h \
        f411xmlserializer.h \
    btf411_1_dev.h \
    btf411_2_dev.h \
    btf411_4_dev.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE0136F48
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = bt_F411_DEV_PGIN.dll
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

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../BUILD_REL/libplant/release/ -llibplant
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../BUILD_DEB/libplant/debug/ -llibplant
else:unix:CONFIG(release, debug|release) LIBS += -L$$PWD/../BUILD_REL/libplant -llibplant
else:unix:CONFIG(debug, debug|release) LIBS += -L$$PWD/../BUILD_DEB/libplant -llibplant

RESOURCES += \
    bt_F411_DEV_PGIN.qrc
