#-------------------------------------------------
#
# Project created by QtCreator 2015-05-07T21:47:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = labyrinth2015
TEMPLATE = app
QMAKE_CXXFLAGS += -std=c++11

SOURCES += ../src/gui/main.cpp\
        ../src/gui/mainwindow.cpp \
	../src/gui/game.cpp

HEADERS  += ../src/gui/mainwindow.h \
	../src/gui/game.h

FORMS    += mainwindow.ui \
	game.ui

RESOURCES += \
    resources.qrc

DESTDIR = ../bin
OBJECTS_DIR = ../obj/gui
