#include "pf_circle.h"
#include <QPainter>
#include "pf_graphicview.h"


PF_Vector PF_Circle::getCenter() const
{
    return data.center;
}

double PF_Circle::getRadius() const
{
    return data.radius;
}

void PF_Circle::draw(QPainter *painter, PF_GraphicView *view)
{
    painter->drawEllipse(QPointF(getCenter().x,getCenter().y),getRadius(),getRadius());
}

PF_CircleData::PF_CircleData(PF_Vector *center, double radius)
    :center(center)
    ,radius(radius)
{

}
