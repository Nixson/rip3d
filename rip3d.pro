#-------------------------------------------------
#
# Project created by QtCreator 2016-01-30T12:21:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = rip3d
TEMPLATE = app


SOURCES += main.cpp\
        rip3d.cpp \
    plotgl.cpp \
    scobject.cpp \
    work.cpp

HEADERS  += rip3d.h \
    plotgl.h \
    cloudtypelist.h \
    scobject.h \
    work.h

FORMS    += rip3d.ui