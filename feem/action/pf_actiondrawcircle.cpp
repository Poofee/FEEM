#include <QMouseEvent>
#include <QDebug>
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

    PF_Circle* circle = new PF_Circle(container,*data);

    container->addEntity(circle);
    view->redraw(PF::RedrawDrawing);
    setStatus(SetCenter);
    reset();

    qDebug()<<"PF_ActionDrawCircle::trigger(): circle added";
}

void PF_ActionDrawCircle::mouseMoveEvent(QMouseEvent *e)
{
    qDebug()<<"PF_ActionDrawCircle::mouseMoveEvent";
    PF_Vector mouse = PF_Snapper::snapPoint(e);

    switch(getStatus()){
    case SetCenter:
        qDebug()<<"SetCenter";
        data->center = mouse;
        break;
    case SetRadius:
        qDebug()<<"SetRadius";
        if(data->center.valid){
            data->radius = data->center.distanceTo(mouse);
            deletePreview();
            preview->addEntity(new PF_Circle(preview,*data));
            drawPreview();
        }
        break;
    }
    qDebug()<<"PF_ActionDrawCircle::mouseMoveEvent: OK.";
}

void PF_ActionDrawCircle::mouseReleaseEvent(QMouseEvent *e)
{
    qDebug()<<"PF_ActionDrawCircle::mouseReleaseEvent";
    if(e->button() == Qt::LeftButton){
        qDebug()<<"Clicked LeftButton!";
        PF_Vector mouse = PF_Vector(e->x(),e->y(),0);

        switch(getStatus()){
        case SetCenter:
            data->center = mouse;
            setStatus(SetRadius);
            qDebug()<<"Set Center:("<<e->x()<<","<<e->y()<<")";
            break;
        case SetRadius:
            if(data->center.valid){
                data->radius = data->center.distanceTo(mouse);
                qDebug()<<"Set Radius:"<<data->radius;
                trigger();
            }
            break;
        default:
            break;
        }
    }else if(e->button() == Qt::RightButton){
        qDebug()<<"Clicked RightButton!";
        init(getStatus()-1);
    }
    qDebug()<<"PF_ActionDrawCircle::mouseReleaseEvent: OK.";
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
