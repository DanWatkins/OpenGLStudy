#-------------------------------------------------
#
# Project created by QtCreator 2014-07-26T19:37:39
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OpenGLStudy
TEMPLATE = app

CONFIG += mobility
MOBILITY = 

HEADERS += \
    ../src/MainWindow.h \
    ../src/OpenGLWindow.h

SOURCES += \
    ../src/Main.cpp \
    ../src/MainWindow.cpp \
    ../src/OpenGLWindow.cpp

DESTDIR = ../Bin/