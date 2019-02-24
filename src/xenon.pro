#-------------------------------------------------
#
# Project created by QtCreator 2018-11-25T17:01:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = log-analyzer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    growingfilemodel.cpp \
    logentryitemdelegate.cpp \
    highlighter.cpp \
    logfile.cpp \
    logfileparser.cpp \
    logfilemodel.cpp \
    timestampparser.cpp

HEADERS  += mainwindow.h \
    growingfilemodel.h \
    logentryitemdelegate.h \
    highlighter.h \
    logfile.h \
    logfileparser.h \
    logfilemodel.h \
    timestampparser.h

FORMS    += mainwindow.ui
