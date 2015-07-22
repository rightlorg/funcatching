#-------------------------------------------------
#
# Project created by QtCreator 2015-07-21T16:06:21
#
#-------------------------------------------------

QT       += core network
QT       -= gui
TARGET = server
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app
SOURCES += main.cpp \
    clientsocket.cpp

HEADERS  += \
    clientsocket.h

FORMS    +=
