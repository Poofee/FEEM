#include "pf_actionselectall.h"

#include "pf_selection.h"

#include <QAction>


PF_ActionSelectAll::PF_ActionSelectAll(PF_EntityContainer* container,
                                       PF_GraphicView* graphicView,
                                       bool select)
        :PF_ActionInterface("Select All Entities", container, graphicView)
        ,select(select)
{
    actionType = PF::ActionSelectAll;
}

void PF_ActionSelectAll::init(int status) {
    PF_ActionInterface::init(status);
    trigger();
    finish();
}

void PF_ActionSelectAll::trigger() {
    PF_Selection s(container, view);
    s.selectAll(select);
}
