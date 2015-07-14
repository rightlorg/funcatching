#-------------------------------------------------
#
# Project created by QtCreator 2015-06-22T18:13:31
#
#-------------------------------------------------

TEMPLATE = app
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = funcatching
DEPENDPATH += .
INCLUDEPATH += .

HEADERS += game.h \
	mainwindow.h \
	mapeditor.h \
	readypage.h \
	settings.h \
	player.h \
	preparing.h

FORMS += mainwindow.ui \
	mapeditor.ui \
	readypage.ui \
	settings.ui

SOURCES += game.cpp \
	main.cpp \
	mainwindow.cpp \
	mapeditor.cpp \
	readypage.cpp \
	settings.cpp \
	player.cpp \
	preparing.cpp

RESOURCES += funcatching.qrc
