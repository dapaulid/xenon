#-------------------------------------------------
#
# Project created by QtCreator 2018-11-25T17:01:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = xenon
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    logentryitemdelegate.cpp \
    highlighter.cpp \
    logfile.cpp \
    logfileparser.cpp \
    logfilemodel.cpp \
    timestampparser.cpp \
    histogram.cpp \
    timingstat.cpp

HEADERS  += mainwindow.h \
    logentryitemdelegate.h \
    highlighter.h \
    logfile.h \
    logfileparser.h \
    logfilemodel.h \
    timestampparser.h \
    histogram.h \
    timingstat.h

FORMS    += mainwindow.ui
