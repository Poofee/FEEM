#-------------------------------------------------
#
# Project created by QtCreator 2018-01-22T09:08:53
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = feem
TEMPLATE = app


SOURCES += \
    feem/main.cpp \
    feem/mainwindow.cpp \
    feem/pf_actionhandler.cpp \
    feem/pf_actiongroupmanager.cpp \
    feem/pf_widgetfactory.cpp \
    feem/pf_actionfactory.cpp \
    feem/pf_centralwidget.cpp \
    feem/pf_graphicview.cpp \
    feem/pf_mdisubwindow.cpp \
    feem/pf_grid.cpp \
    feem/pf_eventhandler.cpp

HEADERS  += \
    feem/mainwindow.h \
    feem/pf_actionhandler.h \
    feem/pf_actiongroupmanager.h \
    feem/pf_widgetfactory.h \
    feem/pf_actionfactory.h \
    feem/pf_centralwidget.h \
    feem/pf_graphicview.h \
    feem/pf_mdisubwindow.h \
    feem/pf_grid.h \
    feem/pf_eventhandler.h

RESOURCES += \
    feem/res/main.qrc
