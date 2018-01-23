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

	QMenu* edit_menu = new QMenu(tr("&Edit"), menu_bar);
	edit_menu->setObjectName("Edit");
	edit_menu->addAction(a_map["Undo"]);
	edit_menu->addAction(a_map["Redo"]);
	edit_menu->addSeparator();
	edit_menu->addAction(a_map["Cut"]);
	edit_menu->addAction(a_map["Copy"]);
	edit_menu->addAction(a_map["Paste"]);
	edit_menu->addAction(a_map["Delete"]);

	QMenu* operation_menu = new QMenu(tr("&Operation"), menu_bar);
	operation_menu->setObjectName("operation");
	operation_menu->addAction(a_map["SelectSingle"]);
	operation_menu->addAction(a_map["SelectAll"]);
	operation_menu->addSeparator();
	operation_menu->addAction(a_map["DrawPoint"]);
	operation_menu->addAction(a_map["DrawLine"]);
	operation_menu->addAction(a_map["DrawArc"]);

	QMenu* view_menu = new QMenu(tr("&View"), menu_bar);
	view_menu->setObjectName("view");
	view_menu->addAction(a_map["ZoomIn"]);
	view_menu->addAction(a_map["ZoomOut"]);
	view_menu->addAction(a_map["ZoomAuto"]);

	QMenu* grid_menu = new QMenu(tr("&Grid"), menu_bar);
	grid_menu->setObjectName("Grid");
	grid_menu->addAction(a_map["ShowGrid"]);
	grid_menu->addAction(a_map["SetGrid"]);	

	QMenu* mesh_menu = new QMenu(tr("&Mesh"), menu_bar);
	mesh_menu->setObjectName("mesh");
	mesh_menu->addAction(a_map["DoMesh"]);
	mesh_menu->addAction(a_map["ShowMesh"]);
	mesh_menu->addAction(a_map["DelMesh"]);

	QMenu* solve_menu = new QMenu(tr("&Solve"), menu_bar);
	solve_menu->setObjectName("solve");
	solve_menu->addAction(a_map["Solve"]);
	solve_menu->addAction(a_map["ShowResult"]);
	solve_menu->addAction(a_map["SolveSetting"]);

	QMenu* help_menu = new QMenu(tr("&Help"), menu_bar);
	help_menu->setObjectName("help");
	help_menu->addAction(a_map["ShowAbout"]);

    menu_bar->addMenu(file_menu);
	menu_bar->addMenu(edit_menu);
	menu_bar->addMenu(operation_menu);
	menu_bar->addMenu(view_menu);
	menu_bar->addMenu(grid_menu);
	menu_bar->addMenu(mesh_menu);
	menu_bar->addMenu(solve_menu);
	menu_bar->addMenu(help_menu);
}
