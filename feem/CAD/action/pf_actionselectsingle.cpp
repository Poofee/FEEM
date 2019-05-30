#include "pf_actionselectsingle.h"

#include "pf_selection.h"

PF_ActionSelectSingle::PF_ActionSelectSingle(PF_EntityContainer* container,PF_GraphicView* view)
    :PF_ActionInterface("Action Select Single",container,view)
{
    actionType = PF::ActionSelectSingle;
}

PF_ActionSelectSingle::~PF_ActionSelectSingle()
{

}

void PF_ActionSelectSingle::trigger() {
    bool typeMatch{true};
//    if (en && entityTypeList.size())
//        typeMatch = std::find(entityTypeList.begin(), entityTypeList.end(), en->rtti())
//                != entityTypeList.end();
    if (en && typeMatch) {
        PF_Selection s(container, view);
        s.selectSingle(en);

//        PF_DIALOGFACTORY->updateSelectionWidget(container->countSelected(),container->totalSelectedLength());
    } else {
//        PF_DEBUG->print("PF_ActionSelectSingle::trigger: Entity is NULL\n");
    }
}


void PF_ActionSelectSingle::keyPressEvent(QKeyEvent* e)
{
    if (e->key()==Qt::Key_Escape)
    {
        finish();
//        actionSelect->keyPressEvent(e);
    }

//    if (container->countSelected() > 0 && e->key()==Qt::Key_Enter)
//    {
//        finish();
//        actionSelect->keyPressEvent(e);
//    }
}


void PF_ActionSelectSingle::mouseReleaseEvent(QMouseEvent* e)
{
    if (e->button()==Qt::RightButton)
    {
        finish();
//        if (actionSelect->rtti() == PF::ActionSelect)
//            actionSelect->finish();
//        else
//            actionSelect->mouseReleaseEvent(e);
    }
    else
    {
        if (entityTypeList.size()) {
            en = catchEntity(e, entityTypeList);
        }else{
            en = catchEntity(e);
        }
        trigger();
    }
}



void PF_ActionSelectSingle::updateMouseCursor() {
//    view->setMouseCuPFor(PF::SelectCuPFor);
}
