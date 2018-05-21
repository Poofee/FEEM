#include <QMouseEvent>
#include "pf_actiondrawcircle.h"
#include "pf_circle.h"
#include "pf_graphicview.h"

PF_ActionDrawCircle::PF_ActionDrawCircle(PF_EntityContainer *container, PF_GraphicView *view)
    :PF_ActionPreviewInterface("Draw Circles",container,view)
    ,data(new PF_CircleData())
{
    actionType = PF::ActionDrawCircle;
    reset();
}

PF_ActionDrawCircle::~PF_ActionDrawCircle()
{
    if(data){
        delete data;
        data = nullptr;
    }
}

void PF_ActionDrawCircle::reset()
{
    if(data){
        delete data;
    }
    data = new PF_CircleData();
}

void PF_ActionDrawCircle::init(int status)
{
    PF_ActionPreviewInterface::init(status);

    reset();
}

void PF_ActionDrawCircle::trigger()
{
    PF_ActionPreviewInterface::trigger();
}

void PF_ActionDrawCircle::mouseMoveEvent(QMouseEvent *e)
{

}

void PF_ActionDrawCircle::mouseReleaseEvent(QMouseEvent *e)
{

}

void PF_ActionDrawCircle::hideOptions()
{

}

void PF_ActionDrawCircle::showOptions()
{

}

void PF_ActionDrawCircle::updateMouseButtonHints()
{

}

void PF_ActionDrawCircle::updateMouseCursor()
{

}
