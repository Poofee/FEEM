#-------------------------------------------------
#
# Project created by QtCreator 2018-01-22T09:08:53
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

QT +=  printsupport gui-private

TARGET = feem
TEMPLATE = app


include($$PWD/../qtribbon/ribbonsample/qtitanribbon.pri)

#ignore warning C4819
QMAKE_CXXFLAGS += /wd"4819"

DESTDIR = $$PWD/../bin

!debug_and_release|build_pass {
    CONFIG(debug, debug|release) {
        TARGET = $$member(TARGET, 0)d
    }
}

INCLUDEPATH += \
    . \
    ./CAD \
    ./CAD/entity \
    ./CAD/action \
    ./util \
    ./material \
    ./project \
    ./fem \

RESOURCES += \
    ./res/main.qrc

RC_FILE = ./res/icon.rc

HEADERS += \    
    ./util/pf.h \
    ./CAD/pf_centralwidget.h \
    ./pf_eventhandler.h \
    ./pf_widgetfactory.h \
    ./CAD/action/pf_actiondrawcircle.h \
    ./CAD/action/pf_actiondrawpoint.h \
    ./CAD/action/pf_actiondrawline.h \
    ./CAD/action/pf_actiondrawrectangle.h \
    ./CAD/entity/pf_polyline.h \
    ./pf_actionfactory.h \
    ./pf_actiongroupmanager.h \
    ./pf_actionhandler.h \
    ./CAD/action/pf_actioninterface.h \
    ./CAD/action/pf_actionpreviewinterface.h \
    ./CAD/entity/pf_atomicentity.h \
    ./CAD/entity/pf_circle.h \
    ./CAD/entity/pf_entity.h \
    ./CAD/entity/pf_grid.h \
    ./CAD/entity/pf_vector.h \
    ./CAD/action/pf_snapper.h \
    ./CAD/entity/pf_entitycontainer.h \
    ./CAD/entity/pf_document.h \
    ./CAD/entity/pf_preview.h \
    ./CAD/entity/pf_point.h \
    ./CAD/entity/pf_line.h \
    ./CAD/pf_plot.h \
    project/pf_project.h \
    project/pf_projecttree.h \
    project/pf_node.h \
    project/pf_projectexplorer.h \
    project/pf_projectwelcomepage.h \
    project/pf_projecttreewidget.h \
    project/pf_projectmanager.h \
    material/pf_material.h \
    material/pf_materialmanager.h \
    project/pf_projectmodel.h \
    util/constants.h \
    ./mainwindow.h \
    ./CAD/pf_graphicview.h \
    ./CAD/pf_graphicwindow.h \
    project/viewitem.h \
    project/navigationtreeview.h

SOURCES += \    
    ./CAD/pf_centralwidget.cpp \
    ./pf_eventhandler.cpp \
    ./pf_widgetfactory.cpp \
    ./CAD/action/pf_actiondrawcircle.cpp \
    ./CAD/action/pf_actiondrawpoint.cpp \
    ./CAD/action/pf_actiondrawline.cpp \
    ./CAD/action/pf_actiondrawrectangle.cpp \
    ./CAD/entity/pf_polyline.cpp \
    ./pf_actionfactory.cpp \
    ./pf_actiongroupmanager.cpp \
    ./pf_actionhandler.cpp \
    ./CAD/action/pf_actioninterface.cpp \
    ./CAD/action/pf_actionpreviewinterface.cpp \
    ./CAD/entity/pf_atomicentity.cpp \
    ./CAD/entity/pf_circle.cpp \
    ./CAD/entity/pf_entity.cpp \
    ./CAD/entity/pf_grid.cpp \
    ./CAD/entity/pf_vector.cpp \
    ./CAD/action/pf_snapper.cpp \
    ./CAD/entity/pf_entitycontainer.cpp \
    ./CAD/entity/pf_document.cpp \
    ./CAD/entity/pf_preview.cpp \
    ./CAD/entity/pf_point.cpp \
    ./CAD/entity/pf_line.cpp \
    ./CAD/pf_plot.cpp \
    project/pf_project.cpp \
    project/pf_projecttree.cpp \
    project/pf_node.cpp \
    project/pf_projectexplorer.cpp \
    project/pf_projectwelcomepage.cpp \
    project/pf_projecttreewidget.cpp \
    project/pf_projectmanager.cpp \
    material/pf_material.cpp \
    material/pf_materialmanager.cpp \
    project/pf_projectmodel.cpp \
    ./main.cpp \
    ./mainwindow.cpp \
    ./CAD/pf_graphicview.cpp \
    ./CAD/pf_graphicwindow.cpp \
    project/viewitem.cpp \
    project/navigationtreeview.cpp


win32:CONFIG(release, debug|release): LIBS += -L$$_PRO_FILE_PWD_/../bin/ -lqtdock
else:win32:CONFIG(debug, debug|release): LIBS += -L$$_PRO_FILE_PWD_/../bin/ -lqtdockd

INCLUDEPATH += $$PWD/../qtdock
DEPENDPATH += $$PWD/../qtdock

include($$PWD/../qtribbon/ribbonsample/shared/aboutdialog.pri)
include($$PWD/../qtribbon/ribbonsample/shared/ribbonwindow.pri)

DISTFILES += \
    res/icon.rc
