#-------------------------------------------------
#
# Project created by QtCreator 2015-07-21T16:06:21
#
#-------------------------------------------------

TEMPLATE = app
TARGET =
DEPENDPATH += .
INCLUDEPATH += .
QT           += network opengl
#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


SOURCES += main.cpp \
    server.cpp \
    clientsocket.cpp

HEADERS  += \
    server.h \
    clientsocket.h

FORMS    += \
    server.ui
