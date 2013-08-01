#-------------------------------------------------
#
# Project created by QtCreator 2012-09-12T12:03:40
#
#-------------------------------------------------

QT       += network xml

QT       -= gui

TARGET = bt_GEN_DEV_PGIN
TEMPLATE = lib
CONFIG += plugin
INCLUDEPATH = ../libplant
DESTDIR = ../myhome/plugins
VERSION = 1.2.4
TARGET_EXT = .dll

DEFINES += BT_GEN_DEV_PGIN_LIBRARY

QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter

SOURCES += \
    bt_gen_dev.cpp \
    bt_gen_dev_pgin.cpp \
    gen_xmlserializer.cpp \
    genmsgworker.cpp

HEADERS +=\
        plantmessage.h\
    bt_gen_dev.h \
    bt_gen_DEV_PGIN_global.h \
    bt_gen_dev_pgin.h \
    gen_xmlserializer.h \
    message.h \
    action.h \
    scenario.h \
    genmsgworker.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE19AC106
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = bt_GEN_DEV_PGIN.dll
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

OTHER_FILES += \
    resources/GEN.xml

RESOURCES += \
    bt_GEN_DEV_PGIN.qrc
