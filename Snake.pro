#-------------------------------------------------
#
# Project created by QtCreator 2016-04-01T11:26:38
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Snake
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    sprite.cpp

HEADERS  += widget.h \
    sprite.h

FORMS    += widget.ui
