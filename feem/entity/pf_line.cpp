#include "pf_line.h"
#include "pf_graphicview.h"
#include <QPainter>


PF_Line::PF_Line(PF_EntityContainer *parent, PF_GraphicView *view, const PF_LineData &d)
    :PF_AtomicEntity(parent)
    ,data(d)
{

}

PF_Line::PF_Line(const PF_Vector &pStart, const PF_Vector &pEnd)
    :PF_AtomicEntity(nullptr)
{
    data.startpoint = pStart;
    data.endpoint = pEnd;
}

PF_Line::PF_Line(PF_EntityContainer *parent, const PF_Vector &pStart, const PF_Vector &pEnd)
    :PF_AtomicEntity(parent)\
{
    data.startpoint = pStart;
    data.endpoint = pEnd;
}

void PF_Line::draw(QCPPainter *painter)
{
    if(!(painter && mParentPlot)){
        return;
    }
    painter->drawLine(QPointF(data.startpoint.x,data.startpoint.y),
                      QPointF(data.endpoint.x,data.endpoint.y));
}

PF_LineData::PF_LineData(PF_Vector &startpoint, PF_Vector &endpoint)
    :startpoint(startpoint)
    ,endpoint(endpoint)
{

}
