#-------------------------------------------------
#
# Project created by QtCreator 2015-05-17T01:33:28
#
#-------------------------------------------------

QT       += core gui network testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = subtitles_qt
TEMPLATE = app
CONFIG += c++11


SOURCES += main.cpp\
        mainwindow.cpp \
    translator.cpp \
    fileloader.cpp \
    myatomicinteger.cpp

HEADERS  += mainwindow.h \
    translator.h \
    fileloader.h \
    myatomicinteger.h

FORMS    += mainwindow.ui
