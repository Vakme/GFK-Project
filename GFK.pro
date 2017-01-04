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
    puzzlewidget.cpp \
    pieceslist.cpp

HEADERS  += mainwindow.h \
    canvas.h \
    puzzlewidget.h \
    pieceslist.h

FORMS    += mainwindow.ui
