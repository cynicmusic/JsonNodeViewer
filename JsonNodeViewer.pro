#-------------------------------------------------
#
# Project created by QtCreator 2018-08-21T19:11:48
#
#-------------------------------------------------

QT       += core gui
CONFIG   += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = JsonNodeViewer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    jsonlistmodel.cpp \
    jsonitem.cpp

HEADERS  += mainwindow.h \
    jsonlistmodel.h \
    jsonitem.h

FORMS    += mainwindow.ui

DISTFILES += \
    json.txt
