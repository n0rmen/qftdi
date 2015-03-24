#-------------------------------------------------
#
# Project created by QtCreator 2015-03-23T10:17:00
#
#-------------------------------------------------

QT       -= gui

TARGET = usb_ftdi
TEMPLATE = lib
CONFIG += staticlib

SOURCES += usb_ftdi.cpp

HEADERS += usb_ftdi.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}

