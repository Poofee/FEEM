#-------------------------------------------------
#
# Project created by QtCreator 2018-01-22T09:08:53
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = feem
TEMPLATE = app

INCLUDEPATH += \
    feem \
    feem/entity \
    feem/action \

RESOURCES += \
    feem/res/main.qrc

HEADERS += \
    feem/mainwindow.h \
    feem/pf.h \
    feem/pf_centralwidget.h \
    feem/pf_eventhandler.h \
    feem/pf_graphicview.h \
    feem/pf_mdisubwindow.h \
    feem/pf_widgetfactory.h \
    feem/action/pf_actiondrawcircle.h \
    feem/action/pf_actionfactory.h \
    feem/action/pf_actiongroupmanager.h \
    feem/action/pf_actionhandler.h \
    feem/action/pf_actioninterface.h \
    feem/action/pf_actionpreviewinterface.h \
    feem/entity/pf_atomicentity.h \
    feem/entity/pf_circle.h \
    feem/entity/pf_entity.h \
    feem/entity/pf_grid.h \
    feem/entity/pf_vector.h \
    feem/action/pf_snapper.h \
    feem/entity/pf_entitycontainer.h \
    feem/entity/pf_document.h \
    feem/entity/pf_preview.h \
    feem/entity/pf_point.h \
    feem/action/pf_actiondrawpoint.h \
    feem/action/pf_actiondrawline.h \
    feem/entity/pf_line.h

SOURCES += \
    feem/main.cpp \
    feem/mainwindow.cpp \
    feem/pf_centralwidget.cpp \
    feem/pf_eventhandler.cpp \
    feem/pf_graphicview.cpp \
    feem/pf_mdisubwindow.cpp \
    feem/pf_widgetfactory.cpp \
    feem/action/pf_actiondrawcircle.cpp \
    feem/action/pf_actionfactory.cpp \
    feem/action/pf_actiongroupmanager.cpp \
    feem/action/pf_actionhandler.cpp \
    feem/action/pf_actioninterface.cpp \
    feem/action/pf_actionpreviewinterface.cpp \
    feem/entity/pf_atomicentity.cpp \
    feem/entity/pf_circle.cpp \
    feem/entity/pf_entity.cpp \
    feem/entity/pf_grid.cpp \
    feem/entity/pf_vector.cpp \
    feem/action/pf_snapper.cpp \
    feem/entity/pf_entitycontainer.cpp \
    feem/entity/pf_document.cpp \
    feem/entity/pf_preview.cpp \
    feem/entity/pf_point.cpp \
    feem/action/pf_actiondrawpoint.cpp \
    feem/action/pf_actiondrawline.cpp \
    feem/entity/pf_line.cpp
