#-------------------------------------------------
#
# Project created by QtCreator 2012-10-29T10:24:10
#
#-------------------------------------------------

QT       += network xml

QT       -= gui

TARGET = bt_F422_DEV_PGIN
TEMPLATE = lib
CONFIG += plugin
INCLUDEPATH = ../libplant
DESTDIR = ../myhome/plugins
VERSION = 1.0.5
TARGET_EXT = .dll

DEFINES += BT_F422_DEV_PGIN_LIBRARY

SOURCES += bt_f422_dev_pgin.cpp \
    btf422_dev.cpp \
    f422xmlserializer.cpp \
    f422status.cpp

HEADERS += bt_f422_dev_pgin.h\
        bt_F422_DEV_PGIN_global.h \
    btf422_dev.h \
    f422xmlserializer.h \
    f422status.h \
        plantmessage.h \

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xEDA1D5E5
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = bt_F422_DEV_PGIN.dll
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

OTHER_FILES += \
    resources/F422.xml
