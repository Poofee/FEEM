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

PF_VectorSolutions PF_Circle::getRefPoints() const
{

}

PF_Vector PF_Circle::getMiddlePoint() const
{

}

PF_Vector PF_Circle::getNearestEndpoint(const PF_Vector &coord, double *dist) const
{

}

PF_Vector PF_Circle::getNearestPointOnEntity(const PF_Vector &coord, bool onEntity, double *dist, PF_Entity **entity) const
{

}

PF_Vector PF_Circle::getNearestCenter(const PF_Vector &coord, double *dist) const
{

}

PF_Vector PF_Circle::getNearestMiddle(const PF_Vector &coord, double *dist, int middlePoints) const
{

}

PF_Vector PF_Circle::getNearestDist(double distance, const PF_Vector &coord, double *dist) const
{

}

PF_Vector PF_Circle::getNearestDist(double distance, bool startp) const
{

}

PF_Vector PF_Circle::getNearestOrthTan(const PF_Vector &coord, const PF_Line &normal, bool onEntity) const
{

}

bool PF_Circle::offset(const PF_Vector &coord, const double &distance)
{

}

void PF_Circle::move(const PF_Vector &offset)
{

}

void PF_Circle::rotate(const PF_Vector &center, const double &angle)
{

}

void PF_Circle::rotate(const PF_Vector &center, const PF_Vector &angleVector)
{

}

void PF_Circle::scale(const PF_Vector &center, const PF_Vector &factor)
{

}

void PF_Circle::mirror(const PF_Vector &axisPoint1, const PF_Vector &axisPoint2)
{

}

void PF_Circle::moveRef(const PF_Vector &ref, const PF_Vector &offset)
{

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
