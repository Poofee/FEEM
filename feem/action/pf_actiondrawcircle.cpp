#include <QMouseEvent>
#include "pf_actiondrawcircle.h"
#include "pf_circle.h"
#include "pf_graphicview.h"

PF_ActionDrawCircle::PF_ActionDrawCircle(PF_GraphicView *view)
    :PF_ActionPreviewInterface(view)
    ,data(new PF_CircleData())
{

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

}

void PF_ActionDrawCircle::init(int status)
{

}

void PF_ActionDrawCircle::trigger()
{

}

void PF_ActionDrawCircle::mouseMoveEvent(QMouseEvent *e)
{

}

void PF_ActionDrawCircle::mouseReleaseEvent(QMouseEvent *e)
{

}
