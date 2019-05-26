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

DEFINES += _CRT_SECURE_NO_WARNINGS

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
    ./core \

RESOURCES += \
    ./res/main.qrc

RC_FILE = ./res/icon.rc

HEADERS += \    
    ./util/pf.h \
    ./CAD/action/pf_eventhandler.h \
    ./core/pf_widgetfactory.h \
    ./CAD/action/pf_actiondrawcircle.h \
    ./CAD/action/pf_actiondrawpoint.h \
    ./CAD/action/pf_actiondrawline.h \
    ./CAD/action/pf_actiondrawrectangle.h \
    ./CAD/entity/pf_polyline.h \
    ./core/pf_actionfactory.h \
    ./core/pf_actiongroupmanager.h \
    ./core/pf_actionhandler.h \
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
    ./core/mainwindow.h \
    ./CAD/pf_graphicview.h \
    project/viewitem.h \
    project/navigationtreeview.h \
    project/treemodel.h \
    actionmanager/actioncontainer.h \
    actionmanager/actioncontainer_p.h \
    actionmanager/actionmanager.h \
    actionmanager/actionmanager_p.h \
    actionmanager/command.h \
    actionmanager/command_p.h \
    util/idocument.h \
    util/filename.h \
    util/id.h \
    util/context.h \
    actionmanager/proxyaction.h \
    util/icore.h \
    ouptput/outputwindow.h \
    ouptput/ioutputpane.h \
    ouptput/messageoutputwindow.h \
    ouptput/messagemanager.h \
    material/pf_materiallibrary.h \
    ouptput/outputpanemanager.h \
    CAD/pf_setting.h \
    CAD/pf_flag.h \
    project/pf_modelwidget.h \
    CAD/pf_cadwidget.h \
    ui/pf_icons.h \
    util/pf_variabledict.h \
    project/pf_nodetreebuilder.h \
    project/pf_sessionmanager.h \
    CAD/action/pf_actionselectsingle.h \
    project/inavigationwidgetfactory.h \
    core/coreapp.h \
    project/projectexplorerconstants.h \
    material/pf_materialtreemodel.h \
    material/pf_magmaterialdialog.h \
    fem/solver/magnetodynamics2d.h \
    fem/solver/types.h

SOURCES += \
    ./CAD/action/pf_eventhandler.cpp \
    ./core/pf_widgetfactory.cpp \
    ./CAD/action/pf_actiondrawcircle.cpp \
    ./CAD/action/pf_actiondrawpoint.cpp \
    ./CAD/action/pf_actiondrawline.cpp \
    ./CAD/action/pf_actiondrawrectangle.cpp \
    ./CAD/entity/pf_polyline.cpp \
    ./core/pf_actionfactory.cpp \
    ./core/pf_actiongroupmanager.cpp \
    ./core/pf_actionhandler.cpp \
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
    ./core/mainwindow.cpp \
    ./CAD/pf_graphicview.cpp \
    project/viewitem.cpp \
    project/navigationtreeview.cpp \
    project/treemodel.cpp \
    actionmanager/actioncontainer.cpp \
    actionmanager/actionmanager.cpp \
    actionmanager/command.cpp \
    util/idocument.cpp \
    util/filename.cpp \
    util/id.cpp \
    util/context.cpp \
    actionmanager/proxyaction.cpp \
    util/icore.cpp \
    ouptput/outputwindow.cpp \
    ouptput/ioutputpane.cpp \
    ouptput/messageoutputwindow.cpp \
    ouptput/messagemanager.cpp \
    material/pf_materiallibrary.cpp \
    ouptput/outputpanemanager.cpp \
    CAD/pf_flag.cpp \
    project/pf_modelwidget.cpp \
    CAD/pf_cadwidget.cpp \
    ui/pf_icons.cpp \
    util/pf_variabledict.cpp \
    project/pf_nodetreebuilder.cpp \
    project/pf_sessionmanager.cpp \
    CAD/action/pf_actionselectsingle.cpp \
    project/inavigationwidgetfactory.cpp \
    core/coreapp.cpp \
    material/pf_materialtreemodel.cpp \
    material/pf_magmaterialdialog.cpp \
    fem/solver/magnetodynamics2d.cpp \
    fem/solver/types.cpp


win32:CONFIG(release, debug|release): LIBS += -L$$_PRO_FILE_PWD_/../bin/ -lqtdock
else:win32:CONFIG(debug, debug|release): LIBS += -L$$_PRO_FILE_PWD_/../bin/ -lqtdockd

INCLUDEPATH += $$PWD/../qtdock
DEPENDPATH += $$PWD/../qtdock

include($$PWD/../qtribbon/ribbonsample/shared/aboutdialog.pri)
include($$PWD/../qtribbon/ribbonsample/shared/ribbonwindow.pri)

DISTFILES += \
    res/icon.rc
