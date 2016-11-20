#-------------------------------------------------
#
# Project created by QtCreator 2016-11-20T23:42:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SharePaste
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    clipboard.cpp \
    controller.cpp

HEADERS  += mainwindow.h \
    clipboard.h \
    controller.h

FORMS    += mainwindow.ui
