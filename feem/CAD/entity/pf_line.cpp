#include "pf_line.h"
#include "pf_graphicview.h"
#include <QPainter>

int PF_Line::line_index = 0;
PF_Line::PF_Line(PF_EntityContainer *parent, PF_GraphicView *view, const PF_LineData &d)
    :PF_AtomicEntity(parent,view)
    ,data(d)
{
    m_index = line_index;
//    line_index++;
}


PF_Line::PF_Line(PF_EntityContainer* parent,PF_GraphicView *view, const PF_Vector &pStart, const PF_Vector &pEnd)
    :PF_AtomicEntity(parent,view)
{
    data.startpoint = pStart;
    data.endpoint = pEnd;
    m_index = line_index;
//    line_index++;
}

PF_VectorSolutions PF_Line::getRefPoints() const
{
    return PF_VectorSolutions({data.startpoint,(data.startpoint+data.endpoint)/2, data.endpoint});
}

PF_Vector PF_Line::getMiddlePoint() const
{
    return (getStartpoint() + getEndpoint())*0.5;
}

PF_Vector PF_Line::getNearestEndpoint(const PF_Vector &coord, double *dist) const
{
    double dist1((data.startpoint-coord).squared());
    double dist2((data.endpoint-coord).squared());

    if (dist2 < dist1) {
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
    //        RS_DEBUG->print("RS_Line::getNearestMiddle(): begin\n");
    PF_Vector dvp(getEndpoint() - getStartpoint());
    double l=dvp.magnitude();
    if( l<= PF_TOLERANCE) {
        //line too short
        return const_cast<PF_Line*>(this)->getNearestCenter(coord, dist);
    }
    PF_Vector vp0(getNearestPointOnEntity(coord,true,dist));
    int counts=middlePoints+1;
    int i( static_cast<int>(vp0.distanceTo(getStartpoint())/l*counts+0.5));
    if(!i) i++; // remove end points
    if(i==counts) i--;
    vp0=getStartpoint() + dvp*(double(i)/double(counts));

    if (dist) {
        *dist=vp0.distanceTo(coord);
    }
    //        RS_DEBUG->print("RS_Line::getNearestMiddle(): end\n");
    return vp0;
}

PF_Vector PF_Line::getNearestDist(double distance, const PF_Vector &coord, double *dist) const
{
    PF_Vector dv = PF_Vector::polar(distance, getAngle1());

    PF_Vector ret;
    //if(coord.distanceTo(getStartpoint()) < coord.distanceTo(getEndpoint())) {
    if( (coord-getStartpoint()).squared()<  (coord-getEndpoint()).squared() ) {
        ret = getStartpoint() + dv;
    }else{
        ret = getEndpoint() - dv;
    }
    if (dist)
        *dist=coord.distanceTo(ret);

    return ret;
}

PF_Vector PF_Line::getNearestDist(double distance, bool startp) const
{
    double a1 = getAngle1();

    PF_Vector dv = PF_Vector::polar(distance, a1);
    PF_Vector ret;

    if (startp) {
        ret = data.startpoint + dv;
    }
    else {
        ret = data.endpoint - dv;
    }

    return ret;
}


bool PF_Line::offset(const PF_Vector &coord, const double &distance)
{
    PF_Vector direction{getEndpoint()-getStartpoint()};
    double ds(direction.magnitude());
    if(ds< PF_TOLERANCE) return false;
    direction /= ds;
    PF_Vector vp(coord-getStartpoint());
    //    PF_Vector vp1(getStartpoint() + direction*(PF_Vector::dotP(direction,vp))); //projection
    direction.set(-direction.y,direction.x); //rotate pi/2
    if(PF_Vector::dotP(direction,vp)<0.) {
        direction *= -1.;
    }
    direction*=distance;
    move(direction);
    return true;
}

void PF_Line::move(const PF_Vector &offset)
{
    //    RS_DEBUG->print("RS_Line::move1: sp: %f/%f, ep: %f/%f",
    //                    data.startpoint.x, data.startpoint.y,
    //                    data.endpoint.x, data.endpoint.y);

    //    RS_DEBUG->print("RS_Line::move1: offset: %f/%f", offset.x, offset.y);

    data.startpoint.move(offset);
    data.endpoint.move(offset);
    moveBorders(offset);
    //    RS_DEBUG->print("RS_Line::move2: sp: %f/%f, ep: %f/%f",
    //                    data.startpoint.x, data.startpoint.y,
    //                    data.endpoint.x, data.endpoint.y);
}

void PF_Line::rotate(const PF_Vector &center, const double &angle)
{
    //    RS_DEBUG->print("RS_Line::rotate");
    //    RS_DEBUG->print("RS_Line::rotate1: sp: %f/%f, ep: %f/%f",
    //                    data.startpoint.x, data.startpoint.y,
    //                    data.endpoint.x, data.endpoint.y);
    PF_Vector rvp(angle);
    data.startpoint.rotate(center, rvp);
    data.endpoint.rotate(center, rvp);
    //    RS_DEBUG->print("RS_Line::rotate2: sp: %f/%f, ep: %f/%f",
    //                    data.startpoint.x, data.startpoint.y,
    //                    data.endpoint.x, data.endpoint.y);
    calculateBorders();
    //    RS_DEBUG->print("RS_Line::rotate: OK");
}

void PF_Line::rotate(const PF_Vector &center, const PF_Vector &angleVector)
{
    data.startpoint.rotate(center, angleVector);
    data.endpoint.rotate(center, angleVector);
    calculateBorders();
}

void PF_Line::scale(const PF_Vector &factor)
{
    //    RS_DEBUG->print("RS_Line::scale1: sp: %f/%f, ep: %f/%f",
    //                    data.startpoint.x, data.startpoint.y,
    //                    data.endpoint.x, data.endpoint.y);
    data.startpoint.scale(factor);
    data.endpoint.scale(factor);
    //    RS_DEBUG->print("RS_Line::scale2: sp: %f/%f, ep: %f/%f",
    //                    data.startpoint.x, data.startpoint.y,
    //                    data.endpoint.x, data.endpoint.y);
    calculateBorders();
}

void PF_Line::scale(const PF_Vector &center, const PF_Vector &factor)
{
    //    RS_DEBUG->print("RS_Line::scale1: sp: %f/%f, ep: %f/%f",
    //                    data.startpoint.x, data.startpoint.y,
    //                    data.endpoint.x, data.endpoint.y);
    data.startpoint.scale(center, factor);
    data.endpoint.scale(center, factor);
    //    RS_DEBUG->print("RS_Line::scale2: sp: %f/%f, ep: %f/%f",
    //                    data.startpoint.x, data.startpoint.y,
    //                    data.endpoint.x, data.endpoint.y);
    calculateBorders();
}

void PF_Line::mirror(const PF_Vector &axisPoint1, const PF_Vector &axisPoint2)
{
    data.startpoint.mirror(axisPoint1, axisPoint2);
    data.endpoint.mirror(axisPoint1, axisPoint2);
    calculateBorders();
}

void PF_Line::moveRef(const PF_Vector &ref, const PF_Vector &offset)
{
//    if(  fabs(data.startpoint.x -ref.x)<1.0e-4 &&
//         fabs(data.startpoint.y -ref.y)<1.0e-4 ) {
//        moveStartpoint(data.startpoint+offset);
//    }
//    if(  fabs(data.endpoint.x -ref.x)<1.0e-4 &&
//         fabs(data.endpoint.y -ref.y)<1.0e-4 ) {
//        moveEndpoint(data.endpoint+offset);
//    }
}

void PF_Line::draw(QCPPainter *painter)
{
//    qDebug()<<Q_FUNC_INFO;
    if(!(painter && mParentPlot)){
        qDebug()<<Q_FUNC_INFO<<":NULL";
        return;
    }
    /** set Pen **/
//    QPen oldpen = painter->pen();
//    QBrush oldbursh = painter->brush();
    painter->save();
    if(isSelected()){
        pen.setColor(QColor(0,0,255));
        painter->setPen(pen);
    }
    QPointF start(mParentPlot->toGuiX(data.startpoint.x),mParentPlot->toGuiY(data.startpoint.y));
    QPointF end(mParentPlot->toGuiX(data.endpoint.x),mParentPlot->toGuiY(data.endpoint.y));

    painter->drawLine(start,end);

    painter->drawText(start,data.startpoint.toString());
    painter->drawText(end,data.endpoint.toString());
    painter->drawText((start+end)/2,QString("line:%1").arg(m_index));

    /** 绘制控制点 **/
    if (isSelected() || isHighlighted()) {
//		if (!e->isParentSelected()) {
            PF_VectorSolutions const& s = this->getRefPoints();
            double x,y;
            int size = 4;
            for (size_t i=0; i<s.getNumber(); ++i) {
                x = mParentPlot->toGuiX(s.get(i).x);
                y = mParentPlot->toGuiY(s.get(i).y);

                painter->drawLine(QPointF(x-size,y-size),QPointF(x+size,y-size));
                painter->drawLine(QPointF(x+size,y-size),QPointF(x+size,y+size));
                painter->drawLine(QPointF(x+size,y+size),QPointF(x-size,y+size));
                painter->drawLine(QPointF(x-size,y+size),QPointF(x-size,y-size));
            }
//		}
    }
//    painter->setPen(oldpen);
//    painter->setBrush(oldbursh);
    painter->restore();
}

void PF_Line::calculateBorders()
{
    minV = PF_Vector::minimum(data.startpoint, data.endpoint);
    maxV = PF_Vector::maximum(data.startpoint, data.endpoint);
}

PF_LineData::PF_LineData(PF_Vector &startpoint, PF_Vector &endpoint)
    :startpoint(startpoint)
    ,endpoint(endpoint)
{

}
