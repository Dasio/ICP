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

LIBS += -lboost_serialization -static-libstdc++

SOURCES += ../src/gui/main.cpp\
        ../src/gui/mainwindow.cpp \
	../src/gui/game.cpp \
    ../src/game/Game.cpp \
    ../src/game/MazeBoard.cpp \
    ../src/game/Stone.cpp

HEADERS  += ../src/gui/mainwindow.h \
	../src/gui/game.h \
    ../src/game/Game.h \
    ../src/game/MazeBoard.h \
    ../src/game/Stone.h

FORMS    += ../gui/mainwindow.ui \
	../gui/game.ui

RESOURCES += \
    ../gui/resources.qrc

DESTDIR = ../bin
OBJECTS_DIR = ../obj/gui
