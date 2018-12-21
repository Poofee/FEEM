#include "pf_widgetfactory.h"
#include "mainwindow.h"
#include "pf_actionhandler.h"
#include <QApplication>
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
    operation_menu->addAction(a_map["DrawCircle"]);

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

void PF_WidgetFactory::createMenuFile()
{
    QIcon iconLogo;
    iconLogo.addPixmap(QPixmap(":/res/qtitanlogo32x32.png"));
    if(QAction* actionFile = mainwindow->ribbonBar()->addSystemButton(iconLogo, tr("&File")))
    {

    }
}

void PF_WidgetFactory::createQuickAccessBar()
{

}

void PF_WidgetFactory::createRibbon()
{
    if (Qtitan::RibbonPage* pageHome = mainwindow->ribbonBar()->addPage(tr("&Home")))
    {
        createGroupProject(pageHome);
        createGroupLayout(pageHome);
        createGroupHelp(pageHome);
    }

    if (Qtitan::RibbonPage* pageDefinitions = mainwindow->ribbonBar()->addPage(tr("&Definitions")))
    {
        createGroupVariable(pageDefinitions);
    }

    if (Qtitan::RibbonPage* pageGeometry = mainwindow->ribbonBar()->addPage(tr("&Geometry")))
    {
        createGroupImExportGeometry(pageGeometry);
        createGroupBuildGeometry(pageGeometry);
        createGroupDrawSetting(pageGeometry);
        createGroupDraw(pageGeometry);
        createGroupDrawOperation(pageGeometry);
    }

    if (Qtitan::RibbonPage* pageMaterials = mainwindow->ribbonBar()->addPage(tr("&Materials")))
    {
        createGroupMaterial(pageMaterials);
    }

    if (Qtitan::RibbonPage* pageMesh = mainwindow->ribbonBar()->addPage(tr("&Mesh")))
    {
        createGroupBuildMesh(pageMesh);
        createGroupGenerator(pageMesh);
        createGroupImExportMesh(pageMesh);
        createGroupStatics(pageMesh);
        createGroupClearMesh(pageMesh);
    }

    if (Qtitan::RibbonPage* pageSolve = mainwindow->ribbonBar()->addPage(tr("&Solve")))
    {
        createGroupSolve(pageSolve);
        createGroupSolverSetting(pageSolve);
        createGroupClearSolution(pageSolve);
    }

    if (Qtitan::RibbonPage* pageResults = mainwindow->ribbonBar()->addPage(tr("&Results")))
    {
        createGroupPlot(pageResults);
    }
}

void PF_WidgetFactory::createStatusBar()
{

}

void PF_WidgetFactory::createGroupProject(RibbonPage *page)
{
    if(Qtitan::RibbonGroup* groupProject = page->addGroup(QIcon(":/main/project.png"), tr("Project")))
    {
        //groupProject->setOptionButtonVisible();
        //QAction* act = groupProject->optionButtonAction();
        //act->setText(tr("Clipboard"));
        //act->setIcon(QIcon(":/res/clipboardToolTip.png"));
        //act->setToolTip(tr("Show the Office clipboard Task Pane"));
        //act->setStatusTip(tr("Show the Office clipboard Task Pane"));
        //connect(act, SIGNAL(triggered()), this, SLOT(optionClipboard()));

        QMenu* newPorject = new QMenu(mainwindow);
        QAction *m_actionPaste1 = newPorject->addAction(QIcon(":/res/smallpaste.png"), tr("2D"));
        m_actionPaste1->setPriority(QAction::LowPriority);
        m_actionPaste1->setShortcut(QKeySequence::Paste);

        newPorject->addAction(tr("2D Axisymmetric"));

        QAction *m_actionProject = groupProject->addAction(QIcon(":/main/project.png"),
            tr("&New Project"), Qt::ToolButtonTextUnderIcon, newPorject);
        m_actionProject->setPriority(QAction::LowPriority);
        m_actionProject->setShortcut(QKeySequence::Paste);
        m_actionProject->setToolTip(tr("Create new project."));


        //QAction *m_actionCut = groupProject->addAction(QIcon(":/res/smallcut.png"),
        //    tr("&Cut"), Qt::ToolButtonTextBesideIcon);
        //m_actionCut->setShortcut(QKeySequence::Cut);
        //m_actionCut->setToolTip(tr("Cut the selection and put it on the Clipboard"));

        //QAction *m_actionCopy = groupProject->addAction(QIcon(":/res/smallcopy.png"),
        //    tr("&Copy"), Qt::ToolButtonTextBesideIcon);
        //m_actionCopy->setShortcut(QKeySequence::Copy);

        //QAction *m_actionFormatPointerAction = groupProject->addAction(QIcon(":/res/smallformatpainter.png"),
        //    tr("F&ormat Pointer"), Qt::ToolButtonTextBesideIcon);
    }
}

void PF_WidgetFactory::createGroupLayout(RibbonPage *page)
{
    if(Qtitan::RibbonGroup* groupLayout = page->addGroup(QIcon(":/main/project.png"), tr("Layout")))
    {

    }
}

void PF_WidgetFactory::createGroupHelp(RibbonPage *page)
{
    if(Qtitan::RibbonGroup* groupHelp = page->addGroup(QIcon(":/main/project.png"), tr("Help")))
    {

    }
}

void PF_WidgetFactory::createGroupVariable(RibbonPage *page)
{
    if(Qtitan::RibbonGroup* groupVariable = page->addGroup(QIcon(":/main/project.png"), tr("Variable")))
    {

    }
}

void PF_WidgetFactory::createGroupImExportGeometry(RibbonPage *page)
{
    if(Qtitan::RibbonGroup* groupImExport = page->addGroup(QIcon(":/main/project.png"), tr("Import/Export")))
    {

    }
}

void PF_WidgetFactory::createGroupBuildGeometry(RibbonPage *page)
{
    if(Qtitan::RibbonGroup* groupBuildGeometry = page->addGroup(QIcon(":/main/project.png"), tr("BuildGeometry")))
    {

    }
}

void PF_WidgetFactory::createGroupDrawSetting(RibbonPage *page)
{
    if(Qtitan::RibbonGroup* groupDrawSetting = page->addGroup(QIcon(":/main/project.png"), tr("DrawSetting")))
    {

    }
}

void PF_WidgetFactory::createGroupDraw(RibbonPage *page)
{
    if(Qtitan::RibbonGroup* groupDraw = page->addGroup(QIcon(":/main/project.png"), tr("Draw")))
    {

    }
}

void PF_WidgetFactory::createGroupDrawOperation(RibbonPage *page)
{
    if(Qtitan::RibbonGroup* groupDrawOperation = page->addGroup(QIcon(":/main/project.png"), tr("DrawOperation")))
    {

    }
}

void PF_WidgetFactory::createGroupMaterial(RibbonPage *page)
{
    if(Qtitan::RibbonGroup* groupMaterial = page->addGroup(QIcon(":/main/project.png"), tr("Material")))
    {

    }
}

void PF_WidgetFactory::createGroupBuildMesh(RibbonPage *page)
{
    if(Qtitan::RibbonGroup* groupBuildMesh = page->addGroup(QIcon(":/main/project.png"), tr("BuildMesh")))
    {

    }
}

void PF_WidgetFactory::createGroupGenerator(RibbonPage *page)
{
    if(Qtitan::RibbonGroup* groupGenerator = page->addGroup(QIcon(":/main/project.png"), tr("Mesh Generator")))
    {

    }
}

void PF_WidgetFactory::createGroupImExportMesh(RibbonPage *page)
{
    if(Qtitan::RibbonGroup* groupImExportMesh = page->addGroup(QIcon(":/main/project.png"), tr("Im/ExportMesh")))
    {

    }
}

void PF_WidgetFactory::createGroupStatics(RibbonPage *page)
{
    if(Qtitan::RibbonGroup* groupStatics = page->addGroup(QIcon(":/main/project.png"), tr("Statics")))
    {

    }
}

void PF_WidgetFactory::createGroupClearMesh(RibbonPage *page)
{
    if(Qtitan::RibbonGroup* groupClearMesh = page->addGroup(QIcon(":/main/project.png"), tr("ClearMesh")))
    {

    }
}

void PF_WidgetFactory::createGroupSolve(RibbonPage *page)
{
    if(Qtitan::RibbonGroup* groupSolve = page->addGroup(QIcon(":/main/project.png"), tr("Solve")))
    {

    }
}

void PF_WidgetFactory::createGroupSolverSetting(RibbonPage *page)
{
    if(Qtitan::RibbonGroup* groupSolverSetting = page->addGroup(QIcon(":/main/project.png"), tr("SolverSetting")))
    {

    }
}

void PF_WidgetFactory::createGroupClearSolution(RibbonPage *page)
{
    if(Qtitan::RibbonGroup* groupClearSolution = page->addGroup(QIcon(":/main/project.png"), tr("ClearSolution")))
    {

    }
}

void PF_WidgetFactory::createGroupPlot(RibbonPage *page)
{
    if(Qtitan::RibbonGroup* groupPlot = page->addGroup(QIcon(":/main/project.png"), tr("Plot")))
    {

    }
}
