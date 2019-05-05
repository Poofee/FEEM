#include "pf_point.h"
#include "pf_graphicview.h"
#include <QPainter>

PF_Point::PF_Point(PF_EntityContainer *parent, PF_GraphicView *view, const PF_PointData &d)
    :PF_AtomicEntity(parent,view)
    ,data(d)
{

}

PF_Vector PF_Point::getCenter() const
{
    return data.pos;
}

double PF_Point::getRadius() const
{
    return 0.;
}

PF_VectorSolutions PF_Point::getRefPoints() const
{
    return PF_VectorSolutions{data.pos};
}

PF_Vector PF_Point::getMiddlePoint() const
{
    return data.pos;
}

PF_Vector PF_Point::getNearestEndpoint(const PF_Vector &coord, double *dist) const
{
    if (dist) {
        *dist = data.pos.distanceTo(coord);
    }

    return data.pos;
}

PF_Vector PF_Point::getNearestPointOnEntity(const PF_Vector &coord, bool onEntity, double *dist, PF_Entity **entity) const
{
    if (dist) {
        *dist = data.pos.distanceTo(coord);
    }
    if (entity) {
        *entity = const_cast<PF_Point*>(this);
    }
    return data.pos;
}

PF_Vector PF_Point::getNearestCenter(const PF_Vector &coord, double *dist) const
{
    if (dist) {
        *dist = data.pos.distanceTo(coord);
    }

    return data.pos;
}

PF_Vector PF_Point::getNearestMiddle(const PF_Vector &coord, double *dist, int middlePoints) const
{
    if (dist) {
        *dist = data.pos.distanceTo(coord);
    }

    return data.pos;
}

PF_Vector PF_Point::getNearestDist(double distance, const PF_Vector &coord, double *dist) const
{
    if (dist) {
        *dist = PF_MAXDOUBLE;
    }
    return PF_Vector(false);
}

//PF_Vector PF_Point::getNearestOrthTan(const PF_Vector &coord, const PF_Line &normal, bool onEntity) const
//{

//}

//bool PF_Point::offset(const PF_Vector &coord, const double &distance)
//{

//}

void PF_Point::move(const PF_Vector &offset)
{
    data.pos.move(offset);
    calculateBorders();
}

void PF_Point::rotate(const PF_Vector &center, const double &angle)
{
    data.pos.rotate(center, angle);
    calculateBorders();
}

void PF_Point::rotate(const PF_Vector &center, const PF_Vector &angleVector)
{
    data.pos.rotate(center, angleVector);
    calculateBorders();
}

void PF_Point::scale(const PF_Vector &center, const PF_Vector &factor)
{
    data.pos.scale(center, factor);
    calculateBorders();
}

void PF_Point::mirror(const PF_Vector &axisPoint1, const PF_Vector &axisPoint2)
{
    data.pos.mirror(axisPoint1, axisPoint2);
    calculateBorders();
}

//void PF_Point::moveRef(const PF_Vector &ref, const PF_Vector &offset)
//{

//}

void PF_Point::draw(QCPPainter *painter)
{
    if(!(painter && mParentPlot)){
        return;
    }
    double x = mParentPlot->toGuiX(data.pos.x);
    double y = mParentPlot->toGuiY(data.pos.y);
    int width = 2;
    for(int i = 0;i <= width*2;i++){
        painter->drawLine(QPoint(x-width,y-width + i),QPoint(x+width,y-width+i));
//        qDebug()<<"line "<<i<<QPoint(x-width,y-width + i)<<QPoint(x+width,y-width+i);
    }
    painter->drawText(QPoint(x,y),toString());
}

void PF_Point::calculateBorders()
{
    minV = maxV = data.pos;
}

QString PF_Point::toString() const
{
    return data.pos.toString();
}
