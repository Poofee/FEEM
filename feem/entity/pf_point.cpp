#include "pf_point.h"
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
    painter->drawLine(QPointF(data.pos.x-1,data.pos.y),
                      QPointF(data.pos.x+1,data.pos.y));
    painter->drawLine(QPointF(data.pos.x,data.pos.y-1),
                      QPointF(data.pos.x,data.pos.y+1));
}
