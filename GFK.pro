#-------------------------------------------------
#
# Project created by QtCreator 2016-11-25T15:13:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GFK
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    canvas.cpp \
    shapeslist.cpp

HEADERS  += mainwindow.h \
    canvas.h \
    shapeslist.h

FORMS    += mainwindow.ui