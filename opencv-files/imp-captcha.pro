#-------------------------------------------------
#
# Project created by QtCreator 2015-07-12T18:13:09
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = imp-captcha
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp

include(opencv.pri)

DISTFILES += \
    opencv.pri
