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
}
