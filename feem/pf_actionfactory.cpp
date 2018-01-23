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

    action = new QAction(tr("File Open"),agm->file);
    connect(action, SIGNAL(triggered()), action_handler, SLOT(slotFileOpen()));
    action->setObjectName("FileOpen");
    a_map["FileOpen"] = action;
}
