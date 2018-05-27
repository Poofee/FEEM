#include "pf_line.h"
#include "pf_graphicview.h"
#include <QPainter>


PF_Line::PF_Line(PF_EntityContainer *parent, const PF_LineData &d)
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

void PF_Line::draw(QPainter *painter, PF_GraphicView *view)
{
    if(!(painter && view)){
        return;
    }
    painter->drawLine(data.startpoint.x,data.startpoint.y,
                      data.endpoint.x,data.endpoint.y);
}

PF_LineData::PF_LineData(PF_Vector &startpoint, PF_Vector &endpoint)
    :startpoint(startpoint)
    ,endpoint(endpoint)
{

}
