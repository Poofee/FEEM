#include "pf_line.h"
#include "pf_graphicview.h"
#include <QPainter>


PF_Line::PF_Line(PF_EntityContainer *parent, PF_GraphicView *view, const PF_LineData &d)
    :PF_AtomicEntity(parent,view)
    ,data(d)
{

}


PF_Line::PF_Line(PF_EntityContainer* parent,PF_GraphicView *view, const PF_Vector &pStart, const PF_Vector &pEnd)
    :PF_AtomicEntity(parent,view)
{
    data.startpoint = pStart;
    data.endpoint = pEnd;
}

void PF_Line::draw(QCPPainter *painter)
{
    if(!(painter && mParentPlot)){
        qDebug()<<Q_FUNC_INFO<<":NULL";
        return;
    }
    painter->drawLine(QPointF(mParentPlot->toGuiX(data.startpoint.x),mParentPlot->toGuiY(data.startpoint.y)),
                      QPointF(mParentPlot->toGuiX(data.endpoint.x),mParentPlot->toGuiY(data.endpoint.y)));
}

PF_LineData::PF_LineData(PF_Vector &startpoint, PF_Vector &endpoint)
    :startpoint(startpoint)
    ,endpoint(endpoint)
{

}
