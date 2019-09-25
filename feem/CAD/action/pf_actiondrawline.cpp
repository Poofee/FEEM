#include <QAction>
#include <QMouseEvent>
#include <QDebug>
#include "pf_actiondrawline.h"
#include "pf_graphicview.h"
#include "pf_line.h"
#include "pf_preview.h"

#include "pf_cadwidget.h"


PF_ActionDrawLine::PF_ActionDrawLine(PF_EntityContainer *container, PF_GraphicView *graphicView)
    :PF_ActionPreviewInterface("Draw Line",container,graphicView)
    ,data(new PF_LineData())
{
    actionType = PF::ActionDrawLine;
    reset();
    /** 设置捕捉模式 **/
    this->snapMode.snapGrid = false;
    this->snapMode.snapFree = false;
    this->snapMode.snapEndpoint = true;
}

PF_ActionDrawLine::~PF_ActionDrawLine()
{

}

void PF_ActionDrawLine::reset()
{
    if(data){
        delete data;
    }
    data = new PF_LineData();
}

void PF_ActionDrawLine::init(int status)
{
    PF_ActionPreviewInterface::init(status);
    reset();
}

void PF_ActionDrawLine::trigger()
{
    PF_ActionPreviewInterface::trigger();

    PF_Line * line = new PF_Line(container,view, *data);
    container->addEntity(line);
    setStatus(SetStartpoint);
    reset();

    PF_Line::line_index++;

    //view->redraw(PF::RedrawDrawing);
    view->replot();
}

void PF_ActionDrawLine::mouseMoveEvent(QMouseEvent *e)
{
    PF_Vector mouse = snapPoint(e);
    if(!mouse.valid)
        return;
    PF_Entity* entity = catchEntity(mouse,PF::EntityPoint);
    if(!entity)
        return;

//    PF_CADWidget::statusbar->clearMessage();
    switch(getStatus()){
    case SetStartpoint:
        PF_CADWidget::statusbar->showMessage(mouse.toString()+QString(tr("Set start point.(Press right click to abort)")));
        break;
    case SetEndpoint:
        PF_CADWidget::statusbar->showMessage(mouse.toString()+QString(tr("Set end point.(Press right click to abort)")));
        break;
    default:
        PF_CADWidget::statusbar->showMessage(mouse.toString()+QString(tr("Status error.")));
        break;
    }
    /**只有起始点设置好之后才有预览**/
    if(getStatus() == SetEndpoint && data->startpoint->getCenter().valid){
        deletePreview();
        view->setCurrentLayer(QLatin1String("overlay"));
        PF_Line* line = new PF_Line(container,view,data->startpoint,dynamic_cast<PF_Point*>(entity));
        view->setCurrentLayer(QLatin1String("main"));
        preview->addEntity(line);
        drawPreview();
    }
}

void PF_ActionDrawLine::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton){
        PF_Vector mouse = snapPoint(e);
        if(!mouse.valid)
            return;
        PF_Entity* entity = catchEntity(mouse,PF::EntityPoint);
        if(!entity)
            return;
        switch(getStatus()){
        case SetStartpoint:
            data->startpoint = dynamic_cast<PF_Point*>(entity);
            //qDebug()<<"setstartpoint"<<mouse.x<<","<<mouse.y;

            setStatus(SetEndpoint);
            updateMouseButtonHints();
            break;
        case SetEndpoint:
            data->endpoint = dynamic_cast<PF_Point*>(entity);
            trigger();
            break;
        default:
            break;
        }
    }else if(e->button() == Qt::RightButton){
        deletePreview();
        drawPreview();
        init(getStatus()-1);

        PF_CADWidget::statusbar->clearMessage();
    }
}

void PF_ActionDrawLine::hideOptions()
{

}

void PF_ActionDrawLine::showOptions()
{

}

void PF_ActionDrawLine::updateMouseButtonHints()
{

}

void PF_ActionDrawLine::updateMouseCursor()
{

}
