#include <QAction>
#include <QMouseEvent>
#include <QDebug>
#include "pf_actiondrawline.h"
#include "pf_graphicview.h"
#include "pf_line.h"
#include "pf_preview.h"


PF_ActionDrawLine::PF_ActionDrawLine(PF_EntityContainer *container, PF_GraphicView *graphicView)
    :PF_ActionPreviewInterface("Draw Line",container,graphicView)
    ,data(new PF_LineData())
{
    actionType = PF::ActionDrawLine;
    reset();
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

    PF_Line * line = new PF_Line(container, *data);
    container->addEntity(line);
    setStatus(SetStartpoint);
    reset();

    view->redraw(PF::RedrawDrawing);
}

void PF_ActionDrawLine::mouseMoveEvent(QMouseEvent *e)
{
    PF_Vector mouse = snapPoint(e);
    //只有起始点设置好之后才有预览
    if(getStatus() == SetEndpoint && data->startpoint.valid){
        deletePreview();

        PF_Line* line = new PF_Line(data->startpoint,mouse);
        preview->addEntity(line);
        drawPreview();
    }
}

void PF_ActionDrawLine::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton){
        PF_Vector mouse = snapPoint(e);

        switch(getStatus()){
        case SetStartpoint:
            data->startpoint = mouse;

            setStatus(SetEndpoint);
            updateMouseButtonHints();
            break;
        case SetEndpoint:
            data->endpoint = mouse;
            trigger();
            break;
        default:
            break;
        }
    }else if(e->button() == Qt::RightButton){
        deletePreview();
        init(getStatus()-1);
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
