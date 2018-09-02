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

SOURCES += main.cpp


SOURCES += mainwindow.cpp \
    jsonlistmodel.cpp \
    jsonitem.cpp \
    jsonfileio.cpp \
    jsonrepresentationbuilder.cpp

HEADERS  += mainwindow.h \
    jsonlistmodel.h \
    jsonitem.h \
    jsonfileio.h \
    jsonrepresentationbuilder.h \

FORMS    += mainwindow.ui

DISTFILES += \
    json.txt


## UNIT TESTS ==========================
test {
    message(Test build)

    QT += testlib
    TARGET = JsonNodeViewerTests

    SOURCES -= main.cpp

    SOURCES += test/unittests.cpp \
               test/main.cpp

    HEADERS += test/unittests.h

} else {
    message(Normal build)
}
