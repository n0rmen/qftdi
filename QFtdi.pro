#-------------------------------------------------
#
# Project created by QtCreator 2015-05-05T10:19:56
#
#-------------------------------------------------

QT       -= gui

TARGET = QFtdi
TEMPLATE = lib
CONFIG += staticlib

SOURCES += qftdi.cpp

HEADERS += qftdi.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}

unix|win32: LIBS += -L$$PWD/../lib/ -lftd2xx

INCLUDEPATH += $$PWD/../lib
DEPENDPATH += $$PWD/../lib

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../lib/ftd2xx.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$PWD/../lib/libftd2xx.a
