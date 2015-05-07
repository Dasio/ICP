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
        ../src/gui/mainwindow.cpp

HEADERS  += ../src/gui/mainwindow.h

FORMS    += mainwindow.ui

DESTDIR = ../bin
OBJECTS_DIR = ../obj/gui
