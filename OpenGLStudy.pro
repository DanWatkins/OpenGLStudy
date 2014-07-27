#-------------------------------------------------
#
# Project created by QtCreator 2014-07-26T19:37:39
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Terrain3DTest
TEMPLATE = app

CONFIG += mobility
MOBILITY = 

HEADERS += \
    Src/MainWindow.h \
    Src/OpenGLWindow.h

SOURCES += \
    Src/Main.cpp \
    Src/MainWindow.cpp \
    Src/OpenGLWindow.cpp
