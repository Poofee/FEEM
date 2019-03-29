#include "pf_point.h"
#include "pf_graphicview.h"
#include <QPainter>

PF_Point::PF_Point(PF_EntityContainer *parent, PF_GraphicView *view, const PF_PointData &d)
    :PF_AtomicEntity(parent,view)
    ,data(d)
{

}

void PF_Point::draw(QCPPainter *painter)
{
    if(!(painter && mParentPlot)){
        return;
    }
    double x = mParentPlot->toGuiX(data.pos.x);
    double y = mParentPlot->toGuiY(data.pos.y);
    painter->drawLine(QPointF(x-1,y),QPointF(x+1,y));
    painter->drawLine(QPointF(x,y-1),QPointF(x,y+1));
}
