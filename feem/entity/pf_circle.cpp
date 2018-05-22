#include "pf_circle.h"
#include <QPainter>
#include <qDebug>
#include "pf_graphicview.h"


PF_Circle::PF_Circle(PF_EntityContainer *parent, const PF_CircleData &d)
    :PF_AtomicEntity(parent)
    ,data(d)
{

}

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
    //qDebug()<<"PF_Circle::draw";
    painter->drawEllipse(QPointF(getCenter().x,getCenter().y),getRadius(),getRadius());
    //qDebug()<<"PF_Circle::draw: OK.";
}

PF_CircleData::PF_CircleData(PF_Vector *center, double radius)
    :center(center)
    ,radius(radius)
{

}
