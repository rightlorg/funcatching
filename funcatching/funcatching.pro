#-------------------------------------------------
#
# Project created by QtCreator 2015-06-22T18:13:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = funcatching
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    setting.cpp \
    readypage.cpp \
    mapeditor.cpp

HEADERS  += mainwindow.h \
    settings.h \
    readypage.h \
    mapeditor.h

FORMS    += mainwindow.ui \
    settings.ui \
    readypage.ui \
    mapeditor.ui
