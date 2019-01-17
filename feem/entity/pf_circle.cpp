#include "pf_circle.h"
#include <QPainter>
#include <qDebug>
#include "pf_graphicview.h"


PF_Circle::PF_Circle(PF_EntityContainer *parent, PF_GraphicView *view, const PF_CircleData &d)
    :PF_AtomicEntity(parent,view)
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

void PF_Circle::draw(QCPPainter *painter)
{
    //qDebug()<<"PF_Circle::draw";
    painter->drawEllipse(QPointF(mParentPlot->toGuiX(getCenter().x),mParentPlot->toGuiY(getCenter().y)),mParentPlot->toGuiDY(getRadius()),mParentPlot->toGuiDY(getRadius()));
    //qDebug()<<"PF_Circle::draw: OK.";
}

PF_CircleData::PF_CircleData(PF_Vector *center, double radius)
    :center(center)
    ,radius(radius)
{

}
