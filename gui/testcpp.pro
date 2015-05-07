#-------------------------------------------------
#
# Project created by QtCreator 2015-05-05T17:55:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = labyrinth2015
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        lobby.cpp

HEADERS  += mainwindow.h \
        lobby.h

FORMS    += mainwindow.ui \
        lobby.ui

DESTDIR = ../bin
OBJECTS_DIR = ../obj/gui
