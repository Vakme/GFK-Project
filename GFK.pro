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
    shapeslist.cpp \
    element.cpp \
    panel.cpp \
    comparator.cpp

HEADERS  += mainwindow.h \
    canvas.h \
    shapeslist.h \
    element.h \
    utils.h \
    panel.h \
    comparator.h

FORMS    += mainwindow.ui

#Jeśli nie działa, usunąć CONFIG += c++14
CONFIG += std=c++14
CONFIG += c++14
