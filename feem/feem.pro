#-------------------------------------------------
#
# Project created by QtCreator 2018-01-22T09:08:53
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT +=  printsupport gui-private

TARGET = feem
TEMPLATE = app


include($$PWD/../qtribbon/ribbonsample/qtitanribbon.pri)



DESTDIR = $$PWD/../bin

!debug_and_release|build_pass {
    CONFIG(debug, debug|release) {
        TARGET = $$member(TARGET, 0)d
    }
}

INCLUDEPATH += \
    . \
    ./entity \
    ./action \

RESOURCES += \
    ./res/main.qrc

RC_FILE = ./res/icon.rc

HEADERS += \
    ./mainwindow.h \
    ./pf.h \
    ./pf_centralwidget.h \
    ./pf_eventhandler.h \
    ./pf_graphicview.h \
    ./pf_mdisubwindow.h \
    ./pf_widgetfactory.h \
    ./action/pf_actiondrawcircle.h \
    ./action/pf_actiondrawpoint.h \
    ./action/pf_actiondrawline.h \
    ./action/pf_actionfactory.h \
    ./action/pf_actiongroupmanager.h \
    ./action/pf_actionhandler.h \
    ./action/pf_actioninterface.h \
    ./action/pf_actionpreviewinterface.h \
    ./entity/pf_atomicentity.h \
    ./entity/pf_circle.h \
    ./entity/pf_entity.h \
    ./entity/pf_grid.h \
    ./entity/pf_vector.h \
    ./action/pf_snapper.h \
    ./entity/pf_entitycontainer.h \
    ./entity/pf_document.h \
    ./entity/pf_preview.h \
    ./entity/pf_point.h \
    ./entity/pf_line.h

SOURCES += \
    ./main.cpp \
    ./mainwindow.cpp \
    ./pf_centralwidget.cpp \
    ./pf_eventhandler.cpp \
    ./pf_graphicview.cpp \
    ./pf_mdisubwindow.cpp \
    ./pf_widgetfactory.cpp \
    ./action/pf_actiondrawcircle.cpp \
    ./action/pf_actiondrawpoint.cpp \
    ./action/pf_actiondrawline.cpp \
    ./action/pf_actionfactory.cpp \
    ./action/pf_actiongroupmanager.cpp \
    ./action/pf_actionhandler.cpp \
    ./action/pf_actioninterface.cpp \
    ./action/pf_actionpreviewinterface.cpp \
    ./entity/pf_atomicentity.cpp \
    ./entity/pf_circle.cpp \
    ./entity/pf_entity.cpp \
    ./entity/pf_grid.cpp \
    ./entity/pf_vector.cpp \
    ./action/pf_snapper.cpp \
    ./entity/pf_entitycontainer.cpp \
    ./entity/pf_document.cpp \
    ./entity/pf_preview.cpp \
    ./entity/pf_point.cpp \
    ./entity/pf_line.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$_PRO_FILE_PWD_/../bin/ -lqtdock
else:win32:CONFIG(debug, debug|release): LIBS += -L$$_PRO_FILE_PWD_/../bin/ -lqtdockd

INCLUDEPATH += $$PWD/../qtdock
DEPENDPATH += $$PWD/../qtdock

include($$PWD/../qtribbon/ribbonsample/shared/aboutdialog.pri)
include($$PWD/../qtribbon/ribbonsample/shared/ribbonwindow.pri)

DISTFILES += \
    res/icon.rc
