#include "pf_actioninterface.h"
#include "pf_graphicview.h"

PF_ActionInterface::PF_ActionInterface(const char *name, PF_EntityContainer *container, PF_GraphicView *view)
    :PF_Snapper(container,view)
{
    this->name = name;
    status = 0;
    finished = false;
}

PF::ActionType PF_ActionInterface::rtti() const
{
    return actionType;
}

void PF_ActionInterface::setName(const char *_name)
{
    this->name = _name;
}

QString PF_ActionInterface::getName()
{
    return name;
}

void PF_ActionInterface::init(int status)
{
    setStatus(status);
    if(status >= 0){
        updateMouseButtonHints();
        updateMouseCursor();
    }else{

    }
}

void PF_ActionInterface::mouseMoveEvent(QMouseEvent *)
{

}

void PF_ActionInterface::mousePressEvent(QMouseEvent *)
{

}

void PF_ActionInterface::mouseReleaseEvent(QMouseEvent *)
{

}

void PF_ActionInterface::keyPressEvent(QKeyEvent *e)
{

}

void PF_ActionInterface::keyReleaseEvent(QKeyEvent *e)
{

}

void PF_ActionInterface::setStatus(int status)
{
    this->status = status;
    updateMouseButtonHints();
    updateMouseCursor();
    if(status<0)
        finish();
}

int PF_ActionInterface::getStatus()
{
    return status;
}

void PF_ActionInterface::updateMouseButtonHints()
{

}

void PF_ActionInterface::updateMouseCursor()
{

}

void PF_ActionInterface::trigger()
{

}

void PF_ActionInterface::finish()
{
    if(!(rtti() == PF::ActionDefault || rtti() == PF::ActionFilePrintPreview)){
        status = -1;
        finished = true;
        hideOptions();
        PF_Snapper::finish();
    }
}

void PF_ActionInterface::setPredecessor(PF_ActionInterface *pre)
{
    predecessor = pre;
}

void PF_ActionInterface::suspend()
{
    PF_Snapper::suspend();
}

void PF_ActionInterface::resume()
{
    updateMouseButtonHints();
    updateMouseCursor();
    PF_Snapper::resume();
}

void PF_ActionInterface::showOptions()
{
    PF_Snapper::showOptions();
}

void PF_ActionInterface::hideOptions()
{
    PF_Snapper::hideOptions();
}

bool PF_ActionInterface::isFinished()
{
    return finished;
}

void PF_ActionInterface::setFinished()
{
    status = -1;
}

void PF_ActionInterface::setActionType(PF::ActionType actionType)
{
    this->actionType = actionType;
}
