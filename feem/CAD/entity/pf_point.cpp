#include "pf_point.h"
#include "pf_graphicview.h"
#include <QPainter>

PF_Point::PF_Point(PF_EntityContainer *parent, PF_GraphicView *view, const PF_PointData &d)
    :PF_AtomicEntity(parent,view)
    ,data(d)
{

}

PF_VectorSolutions PF_Point::getRefPoints() const
{

}

PF_Vector PF_Point::getMiddlePoint() const
{

}

PF_Vector PF_Point::getNearestEndpoint(const PF_Vector &coord, double *dist) const
{

}

PF_Vector PF_Point::getNearestPointOnEntity(const PF_Vector &coord, bool onEntity, double *dist, PF_Entity **entity) const
{

}

PF_Vector PF_Point::getNearestCenter(const PF_Vector &coord, double *dist) const
{

}

PF_Vector PF_Point::getNearestMiddle(const PF_Vector &coord, double *dist, int middlePoints) const
{

}

PF_Vector PF_Point::getNearestDist(double distance, const PF_Vector &coord, double *dist) const
{

}

PF_Vector PF_Point::getNearestOrthTan(const PF_Vector &coord, const PF_Line &normal, bool onEntity) const
{

}

bool PF_Point::offset(const PF_Vector &coord, const double &distance)
{

}

void PF_Point::move(const PF_Vector &offset)
{

}

void PF_Point::rotate(const PF_Vector &center, const double &angle)
{

}

void PF_Point::rotate(const PF_Vector &center, const PF_Vector &angleVector)
{

}

void PF_Point::scale(const PF_Vector &center, const PF_Vector &factor)
{

}

void PF_Point::mirror(const PF_Vector &axisPoint1, const PF_Vector &axisPoint2)
{

}

void PF_Point::moveRef(const PF_Vector &ref, const PF_Vector &offset)
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
