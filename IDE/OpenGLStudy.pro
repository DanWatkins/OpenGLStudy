#-------------------------------------------------
#
# Project created by QtCreator 2014-07-26T19:37:39
#
#-------------------------------------------------

QT	   += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OpenGLStudy
TEMPLATE = app

CONFIG += mobility
MOBILITY = 


DESTDIR = ../Bin/


OTHER_FILES += \
    ../Bin/Shaders/std_fragment.glsl \
    ../Bin/Shaders/std_vertex.glsl

HEADERS += \
    ../src/Main.h \
    ../src/MainWindow.h \
    ../src/Object.h \
	../src/openglwindow.h \
	../src/KTX.h

SOURCES += \
    ../src/Main.cpp \
    ../src/MainWindow.cpp \
    ../src/Object.cpp \
	../src/openglwindow.cpp \
	../src/KTX.cpp
