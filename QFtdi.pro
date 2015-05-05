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
