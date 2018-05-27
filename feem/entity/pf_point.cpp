#include "pf_point.h"
#include <QPainter>

PF_Point::PF_Point(PF_EntityContainer *parent, const PF_PointData &d)
    :PF_AtomicEntity(parent)
    ,data(d)
{

}

void PF_Point::draw(QPainter *painter, PF_GraphicView *view)
{
    if(!(painter && view)){
        return;
    }
    painter->drawLine(data.pos.x-1,data.pos.y,
                      data.pos.x+1,data.pos.y);
    painter->drawLine(data.pos.x,data.pos.y-1,
                      data.pos.x,data.pos.y+1);
}
