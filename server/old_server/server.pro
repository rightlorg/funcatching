#-------------------------------------------------
#
# Project created by QtCreator 2015-07-23T10:34:25
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = server
TEMPLATE = app


SOURCES += main.cpp\
        server_start.cpp \
    server.cpp \
    clientthread.cpp \
    storing_player.cpp

HEADERS  += server_start.h \
    server.h \
    clientthread.h \
    storing_player.h

FORMS    += server_start.ui
