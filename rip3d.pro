#-------------------------------------------------
#
# Project created by QtCreator 2016-01-30T12:21:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = rip3d
TEMPLATE = app

SOURCES += main.cpp\
        rip3d.cpp \
    plotgl.cpp \
    scobject.cpp \
    work.cpp \
    ripmath.cpp \
    formsettings.cpp \
    rcontrol.cpp \
    mathworker.cpp \
    plotter.cpp \
    measuresettings.cpp \
    qcustomplot.cpp

HEADERS  += rip3d.h \
    plotgl.h \
    cloudtypelist.h \
    scobject.h \
    work.h \
    ripmath.h \
    formsettings.h \
    rcontrol.h \
    mathworker.h \
    plotter.h \
    measuresettings.h \
    qcustomplot.h


FORMS    += rip3d.ui \
    formsettings.ui \
    plotter.ui \
    measuresettings.ui

