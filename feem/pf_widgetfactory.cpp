#include "pf_widgetfactory.h"
#include "mainwindow.h"
#include "pf_actionhandler.h"

#include <QMenu>
#include <QMenuBar>
#include <QActionGroup>
#include <QToolBar>

PF_WidgetFactory::PF_WidgetFactory(MainWindow* mainwin,
                                   QMap<QString, QAction*>& action_map,
                                   PF_ActionGroupManager* agm)
    : QObject(nullptr)
    , mainwindow(mainwin)
    , a_map(action_map)
    , ag_manager(agm)
{

}

void PF_WidgetFactory::createStandardToolbars(PF_ActionHandler* a_handler){
    QSizePolicy toolBarPolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QToolBar* file_toolbar = new QToolBar(mainwindow);
    file_toolbar->setSizePolicy(toolBarPolicy);
    file_toolbar->addAction(a_map["FileOpen"]);
    file_toolbar->setObjectName("file_toolbar");
    file_toolbar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    mainwindow->addToolBar(Qt::TopToolBarArea, file_toolbar);
}

void PF_WidgetFactory::createMenus(QMenuBar* menu_bar){
    QMenu* submenu;

    file_menu = new QMenu(tr("&File"),menu_bar);
    file_menu->setObjectName("File");
    file_menu->addAction(a_map["FileNew"]);
    file_menu->addAction(a_map["FileOpen"]);
    file_menu->addAction(a_map["FileSave"]);
    file_menu->addAction(a_map["FileSaveAs"]);
    file_menu->addSeparator();
    file_menu->addAction(a_map["ImportDXF"]);
    file_menu->addAction(a_map["ExportDXF"]);
    file_menu->addSeparator();
    file_menu->addAction(a_map["Print"]);
    file_menu->addAction(a_map["PrintView"]);
    file_menu->addSeparator();
    file_menu->addAction(a_map["FileQuit"]);

    menu_bar->addMenu(file_menu);
}
