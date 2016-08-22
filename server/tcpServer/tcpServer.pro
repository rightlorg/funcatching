#-------------------------------------------------
#
# Project created by QtCreator 2011-11-06T15:33:11
#
#-------------------------------------------------

QT       += core gui
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tcpServer
TEMPLATE = app


SOURCES += main.cpp\
        server.cpp \
    client.cpp \
    map.cpp

HEADERS  += server.h \
    client.h \
    map.h

FORMS    += server.ui \
    client.ui
