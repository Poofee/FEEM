#include "pf_actionfactory.h"
#include "pf_actiongroupmanager.h"

#include <QAction>
#include <QActionGroup>

PF_ActionFactory::PF_ActionFactory(QObject *parent, QObject* a_handler)
    : QObject(parent)
    , main_window(parent)
    , action_handler(a_handler)
{

}

void PF_ActionFactory::fillActionContainer(QMap<QString, QAction*>& a_map, PF_ActionGroupManager* agm)
{
    QAction* action;
    //File
    action = new QAction(tr("File Open"),agm->file);
    connect(action, SIGNAL(triggered()), main_window, SLOT(slotFileOpen()));
    action->setObjectName("FileOpen");
    a_map["FileOpen"] = action;

    action = new QAction(tr("File New"),agm->file);
    connect(action, SIGNAL(triggered()), main_window, SLOT(slotFileNew()));
    action->setObjectName("FileNew");
    a_map["FileNew"] = action;

    action = new QAction(tr("File Quit"),agm->file);
    connect(action, SIGNAL(triggered()), main_window, SLOT(slotFileQuit()));
    action->setObjectName("FileQuit");
    a_map["FileQuit"] = action;

    action = new QAction(tr("File Save"),agm->file);
    connect(action, SIGNAL(triggered()), main_window, SLOT(slotFileSave()));
    action->setObjectName("FileSave");
    a_map["FileSave"] = action;

    action = new QAction(tr("File SaveAs"),agm->file);
    connect(action, SIGNAL(triggered()), main_window, SLOT(slotFileSaveAs()));
    action->setObjectName("FileSaveAs");
    a_map["FileSaveAs"] = action;

    action = new QAction(tr("Import DXF"),agm->file);
    connect(action, SIGNAL(triggered()), main_window, SLOT(slotImportDXF()));
    action->setObjectName("ImportDXF");
    a_map["ImportDXF"] = action;

    action = new QAction(tr("Export DXF"),agm->file);
    connect(action, SIGNAL(triggered()), main_window, SLOT(slotExportDXF()));
    action->setObjectName("ExportDXF");
    a_map["ExportDXF"] = action;

    action = new QAction(tr("Print"),agm->file);
    connect(action, SIGNAL(triggered()), main_window, SLOT(slotPrint()));
    action->setObjectName("Print");
    a_map["Print"] = action;

    action = new QAction(tr("PrintView"),agm->file);
    connect(action, SIGNAL(triggered()), main_window, SLOT(slotPrintView()));
    action->setObjectName("PrintView");
    a_map["PrintView"] = action;

	//Edit
	action = new QAction(tr("Undo"), agm->file);
	connect(action, SIGNAL(triggered()), action_handler, SLOT(slotUndo()));
	action->setObjectName("Undo");
	a_map["Undo"] = action;

	action = new QAction(tr("Redo"), agm->file);
	connect(action, SIGNAL(triggered()), action_handler, SLOT(slotRedo()));
	action->setObjectName("Redo");
	a_map["Redo"] = action;

	action = new QAction(tr("Cut"), agm->file);
	connect(action, SIGNAL(triggered()), action_handler, SLOT(slotCut()));
	action->setObjectName("Cut");
	a_map["Cut"] = action;

	action = new QAction(tr("Copy"), agm->file);
	connect(action, SIGNAL(triggered()), action_handler, SLOT(slotCopy()));
	action->setObjectName("Copy");
	a_map["Copy"] = action;

	action = new QAction(tr("Paste"), agm->file);
	connect(action, SIGNAL(triggered()), action_handler, SLOT(slotPaste()));
	action->setObjectName("Paste");
	a_map["Paste"] = action;

	action = new QAction(tr("Delete"), agm->file);
	connect(action, SIGNAL(triggered()), action_handler, SLOT(slotDelete()));
	action->setObjectName("Delete");
	a_map["Delete"] = action;
	//Operation
	action = new QAction(tr("SelectSingle"), agm->file);
	connect(action, SIGNAL(triggered()), action_handler, SLOT(slotSelectSingle()));
	action->setObjectName("SelectSingle");
	a_map["SelectSingle"] = action;

	action = new QAction(tr("SelectAll"), agm->file);
	connect(action, SIGNAL(triggered()), action_handler, SLOT(slotSelectAll()));
	action->setObjectName("SelectAll");
	a_map["SelectAll"] = action;

    /**画点**/
	action = new QAction(tr("DrawPoint"), agm->file);
    action->setIcon(QIcon(":/main/dot.png"));
	connect(action, SIGNAL(triggered()), action_handler, SLOT(slotDrawPoint()));
	action->setObjectName("DrawPoint");
	a_map["DrawPoint"] = action;

    /**画线段**/
	action = new QAction(tr("DrawLine"), agm->file);
    action->setIcon(QIcon(":/main/line.png"));
	connect(action, SIGNAL(triggered()), action_handler, SLOT(slotDrawLine()));
	action->setObjectName("DrawLine");
	a_map["DrawLine"] = action;

    /**画圆弧**/
	action = new QAction(tr("DrawArc"), agm->file);
	connect(action, SIGNAL(triggered()), action_handler, SLOT(slotDrawArc()));
	action->setObjectName("DrawArc");
	a_map["DrawArc"] = action;

    /**画圆**/
    action = new QAction(tr("DrawCircle"), agm->file);
    connect(action, SIGNAL(triggered()), action_handler, SLOT(slotDrawCircle()));
    action->setObjectName("DrawCircle");
    action->setIcon(QIcon(":/main/circle.png"));
    a_map["DrawCircle"] = action;

    /**画长方形**/
    action = new QAction(tr("DrawRectangle"), agm->file);
    connect(action, SIGNAL(triggered()), action_handler, SLOT(slotDrawRectangle()));
    action->setObjectName("DrawRectangle");
    action->setIcon(QIcon(":/main/square.png"));
    a_map["DrawRectangle"] = action;

	//View
	action = new QAction(tr("ZoomIn"), agm->file);
	connect(action, SIGNAL(triggered()), action_handler, SLOT(slotZoomIn()));
	action->setObjectName("ZoomIn");
	a_map["ZoomIn"] = action;

	action = new QAction(tr("ZoomOut"), agm->file);
	connect(action, SIGNAL(triggered()), action_handler, SLOT(slotZoomOut()));
	action->setObjectName("ZoomOut");
	a_map["ZoomOut"] = action;

	action = new QAction(tr("ZoomAuto"), agm->file);
	connect(action, SIGNAL(triggered()), action_handler, SLOT(slotZoomAuto()));
	action->setObjectName("ZoomAuto");
	a_map["ZoomAuto"] = action;

	//Grid
	action = new QAction(tr("ShowGrid"), agm->file);
	connect(action, SIGNAL(triggered()), action_handler, SLOT(slotShowGrid()));
	action->setObjectName("ShowGrid");
	a_map["ShowGrid"] = action;

	action = new QAction(tr("SetGrid"), agm->file);
	connect(action, SIGNAL(triggered()), action_handler, SLOT(slotSetGrid()));
	action->setObjectName("SetGrid");
	a_map["SetGrid"] = action;

	//Mesh
	action = new QAction(tr("DoMesh"), agm->file);
	connect(action, SIGNAL(triggered()), action_handler, SLOT(slotDoMesh()));
	action->setObjectName("DoMesh");
	a_map["DoMesh"] = action;

	action = new QAction(tr("ShowMesh"), agm->file);
	connect(action, SIGNAL(triggered()), action_handler, SLOT(slotShowMesh()));
	action->setObjectName("ShowMesh");
	a_map["ShowMesh"] = action;

	action = new QAction(tr("DelMesh"), agm->file);
	connect(action, SIGNAL(triggered()), action_handler, SLOT(slotDelMesh()));
	action->setObjectName("DelMesh");
	a_map["DelMesh"] = action;

	//Solve
	action = new QAction(tr("Solve"), agm->file);
	connect(action, SIGNAL(triggered()), action_handler, SLOT(slotSolve()));
	action->setObjectName("Solve");
	a_map["Solve"] = action;

	action = new QAction(tr("ShowResult"), agm->file);
	connect(action, SIGNAL(triggered()), action_handler, SLOT(slotShowResult()));
	action->setObjectName("ShowResult");
	a_map["ShowResult"] = action;

	action = new QAction(tr("SolveSetting"), agm->file);
	connect(action, SIGNAL(triggered()), action_handler, SLOT(slotSolveSetting()));
	action->setObjectName("SolveSetting");
	a_map["SolveSetting"] = action;
	
	//About
	action = new QAction(tr("ShowAbout"), agm->file);
	connect(action, SIGNAL(triggered()), action_handler, SLOT(slotShowAbout()));
	action->setObjectName("ShowAbout");
	a_map["ShowAbout"] = action;
}
