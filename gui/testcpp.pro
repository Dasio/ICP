#-------------------------------------------------
#
# Project created by QtCreator 2015-05-05T17:55:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = labyrinth2015
TEMPLATE = app
QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp\
        mainwindow.cpp \
        lobby.cpp \
        ../src/client/Client.cpp \
        ../src/common/Packet.cpp \

HEADERS  += mainwindow.h \
        lobby.h \
        ../src/client/Client.h \
        ../src/common/Packet.h \
        ../src/common/OpCode.h \
        ../src/common/Packet.h

FORMS    += mainwindow.ui \
        lobby.ui

LIBS += -lpthread -lboost_system

DESTDIR = ../bin
OBJECTS_DIR = ../obj/gui
