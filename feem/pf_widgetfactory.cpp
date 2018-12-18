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
    iconLogo.addPixmap(QPixmap(":/res/file.png"));
    iconLogo.addPixmap(QPixmap(":/shared/res/qtitanlogo32x32.png"));
    if(QAction* actionFile = mainwindow->ribbonBar()->addSystemButton(iconLogo, tr("&File")))
    {
        actionFile->setToolTip(tr("Click here to see everything you can do with your document"));
        Qtitan::RibbonSystemPopupBar* popupBar = qobject_cast<Qtitan::RibbonSystemPopupBar*>(actionFile->menu());

        //popupBar->addAction(m_actionFileNew);
        //popupBar->addAction(m_actionOpenFile);
        //popupBar->addAction(m_actionSaveFile);

        QAction* actionSaveAsFile = popupBar->addAction(QIcon(":/res/largeSaveAsFile.png"), tr("Save &As..."));
        actionSaveAsFile->setPriority(QAction::LowPriority);
        actionSaveAsFile->setToolTip(tr("Save As"));
        actionSaveAsFile->setStatusTip(tr("Save the active document with a new name"));
        connect(actionSaveAsFile, SIGNAL(triggered()), this, SLOT(fileSaveAs()));

        popupBar->addSeparator();


        QAction* actionPrepare = popupBar->addAction(QIcon(":/res/largePrepare.png"), tr("Pr&epare"));
        popupBar->addAction(actionPrepare);

        QAction* actionSend = popupBar->addAction(QIcon(":/res/largeSend.png"), tr("Sen&d"));
        popupBar->addAction(actionSend);
        if (RibbonPageSystemPopup* pageSystemPopup = popupBar->addPageSystemPopup(tr("Preview and print the document"), actionSend, false))
        {
            pageSystemPopup->setMinimumWidth(296);
            QAction* actionMail = pageSystemPopup->addAction(QIcon(":/res/largeMail.png"), tr("E-Mail"));
            actionMail->setToolTip(tr("Send the active document by e-mail"));
            pageSystemPopup->addAction(actionMail);

            QAction* actionIntenetFax = pageSystemPopup->addAction(QIcon(":/res/largeInternetfix.png"), tr("Intenet Fax"));
            actionIntenetFax->setToolTip(tr("Use an Internet fax service to fax the document"));
            pageSystemPopup->addAction(actionIntenetFax);
        }

        popupBar->addSeparator();
        QAction* actionClose = popupBar->addAction(QIcon(":/res/largeClose.png"), tr("&Close"));
        actionClose->setShortcut(tr("Ctrl+C"));
        actionClose->setStatusTip(tr("Exit"));
        actionClose->setEnabled(false);

        QAction* actionExit =  popupBar->addPopupBarAction(tr("Exit Sample"));
        connect(actionExit, SIGNAL(triggered()), this, SLOT(close()));

        QAction* actionOption = popupBar->addPopupBarAction(tr("Option"));
        actionOption->setEnabled(false);

        if (RibbonPageSystemRecentFileList* pageRecentFile = popupBar->addPageRecentFile(tr("Recent Documents")))
        {
            Q_UNUSED(pageRecentFile);
        }
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

}

void PF_WidgetFactory::createGroupVariable(RibbonPage *page)
{

}

void PF_WidgetFactory::createGroupImExportGeometry(RibbonPage *page)
{

}

void PF_WidgetFactory::createGroupBuildGeometry(RibbonPage *page)
{

}

void PF_WidgetFactory::createGroupDrawSetting(RibbonPage *page)
{

}

void PF_WidgetFactory::createGroupDraw(RibbonPage *page)
{

}

void PF_WidgetFactory::createGroupDrawOperation(RibbonPage *page)
{

}

void PF_WidgetFactory::createGroupMaterial(RibbonPage *page)
{

}

void PF_WidgetFactory::createGroupBuildMesh(RibbonPage *page)
{

}

void PF_WidgetFactory::createGroupGenerator(RibbonPage *page)
{

}

void PF_WidgetFactory::createGroupImExportMesh(RibbonPage *page)
{

}

void PF_WidgetFactory::createGroupStatics(RibbonPage *page)
{

}

void PF_WidgetFactory::createGroupClearMesh(RibbonPage *page)
{

}

void PF_WidgetFactory::createGroupSolve(RibbonPage *page)
{

}

void PF_WidgetFactory::createGroupSolverSetting(RibbonPage *page)
{

}

void PF_WidgetFactory::createGroupClearSolution(RibbonPage *page)
{

}

void PF_WidgetFactory::createGroupPlot(RibbonPage *page)
{

}
