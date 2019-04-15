#include "pf_line.h"
#include "pf_graphicview.h"
#include <QPainter>


PF_Line::PF_Line(PF_EntityContainer *parent, PF_GraphicView *view, const PF_LineData &d)
    :PF_AtomicEntity(parent,view)
    ,data(d)
{

}


PF_Line::PF_Line(PF_EntityContainer* parent,PF_GraphicView *view, const PF_Vector &pStart, const PF_Vector &pEnd)
    :PF_AtomicEntity(parent,view)
{
    data.startpoint = pStart;
    data.endpoint = pEnd;
}

PF_VectorSolutions PF_Line::getRefPoints() const
{
    return PF_VectorSolutions({data.startpoint, data.endpoint});
}

PF_Vector PF_Line::getMiddlePoint() const
{
    return (getStartpoint() + getEndpoint())*0.5;
}

PF_Vector PF_Line::getNearestEndpoint(const PF_Vector &coord, double *dist) const
{
    double dist1((data.startpoint-coord).squared());
    double dist2((data.endpoint-coord).squared());

    if (dist2<dist1) {
        if (dist) {
            *dist = sqrt(dist2);
        }
        return data.endpoint;
    } else {
        if (dist) {
            *dist = sqrt(dist1);
        }
        return data.startpoint;
    }
}

PF_Vector PF_Line::getNearestPointOnEntity(const PF_Vector &coord, bool onEntity, double *dist, PF_Entity **entity) const
{
    if (entity) {
        *entity = const_cast<PF_Line*>(this);
    }

    PF_Vector direction {data.endpoint - data.startpoint};
    PF_Vector vpc {coord - data.startpoint};
    double a {direction.squared()};

    if( a < PF_TOLERANCE2) {
        //line too short
        vpc = getMiddlePoint();
    }
    else {
        //find projection on line
        const double t {PF_Vector::dotP( vpc, direction) / a};
        if( //!isConstruction()
                 onEntity
                && ( t <= -PF_TOLERANCE
                     || t >= 1. + PF_TOLERANCE ) ) {
            //projection point not within range, find the nearest endpoint
            return getNearestEndpoint( coord, dist);
        }

        vpc = data.startpoint + direction * t;
    }

    if (dist) {
        *dist = vpc.distanceTo( coord);
    }

    return vpc;
}

PF_Vector PF_Line::getNearestCenter(const PF_Vector &coord, double *dist) const
{
    PF_Vector p = (data.startpoint + data.endpoint) * 0.5;

    if(dist)
        *dist = p.distanceTo(coord);

    return p;
}

PF_Vector PF_Line::getNearestMiddle(const PF_Vector &coord, double *dist, int middlePoints) const
{

}

PF_Vector PF_Line::getNearestDist(double distance, const PF_Vector &coord, double *dist) const
{

}

PF_Vector PF_Line::getNearestDist(double distance, bool startp) const
{

}

PF_Vector PF_Line::getNearestOrthTan(const PF_Vector &coord, const PF_Line &normal, bool onEntity) const
{

}

bool PF_Line::offset(const PF_Vector &coord, const double &distance)
{

}

void PF_Line::move(const PF_Vector &offset)
{

}

void PF_Line::rotate(const PF_Vector &center, const double &angle)
{

}

void PF_Line::rotate(const PF_Vector &center, const PF_Vector &angleVector)
{

}

void PF_Line::scale(const PF_Vector &center, const PF_Vector &factor)
{

}

void PF_Line::mirror(const PF_Vector &axisPoint1, const PF_Vector &axisPoint2)
{

}

void PF_Line::moveRef(const PF_Vector &ref, const PF_Vector &offset)
{

}

void PF_Line::draw(QCPPainter *painter)
{
    if(!(painter && mParentPlot)){
        qDebug()<<Q_FUNC_INFO<<":NULL";
        return;
    }
    painter->drawLine(QPointF(mParentPlot->toGuiX(data.startpoint.x),mParentPlot->toGuiY(data.startpoint.y)),
                      QPointF(mParentPlot->toGuiX(data.endpoint.x),mParentPlot->toGuiY(data.endpoint.y)));
}

PF_LineData::PF_LineData(PF_Vector &startpoint, PF_Vector &endpoint)
    :startpoint(startpoint)
    ,endpoint(endpoint)
{

}
