#include "pf_actiondrawpoint.h"
#include <QAction>
#include <QMouseEvent>
#include "pf_point.h"
#include "pf_graphicview.h"

PF_ActionDrawPoint::PF_ActionDrawPoint(PF_EntityContainer *parent, PF_GraphicView *view)
    :PF_ActionPreviewInterface("Draw Point",parent,view)
    ,pt(new PF_Vector())
{
    actionType = PF::ActionDrawPoint;
}

PF_ActionDrawPoint::~PF_ActionDrawPoint()
{

}

void PF_ActionDrawPoint::trigger()
{
    if(pt->valid){
        PF_Point * point = new PF_Point(container,PF_PointData(*pt));
        container->addEntity(point);

        view->redraw((PF::RedrawMethod) (PF::RedrawDrawing | PF::RedrawOverlay));
    }
}

void PF_ActionDrawPoint::mouseMoveEvent(QMouseEvent *e)
{
    snapPoint(e);
}

void PF_ActionDrawPoint::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton){
        PF_Vector mouse = PF_Snapper::snapPoint(e);
        *pt = mouse;
        trigger();
    }else if(e->button() == Qt::RightButton){
        init(getStatus()-1);
    }
}

void PF_ActionDrawPoint::updateMouseButtonHints()
{

}

void PF_ActionDrawPoint::updateMouseCursor()
{

}
