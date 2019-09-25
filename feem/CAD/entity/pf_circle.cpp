#include "pf_circle.h"
#include "pf_line.h"

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

void PF_Circle::setCenter(const PF_Vector &c)
{
    data.center = c;
}

double PF_Circle::getRadius() const
{
    return data.radius;
}

void PF_Circle::setRadius(double r)
{
    data.radius = r;
}

PF_VectorSolutions PF_Circle::getRefPoints() const
{
    PF_Vector v1(data.radius, 0.0);
    PF_Vector v2(0.0, data.radius);

    return PF_VectorSolutions ({data.center,
                           data.center+v1, data.center+v2,
                           data.center-v1, data.center-v2});
}

PF_Vector PF_Circle::getMiddlePoint() const
{
    return PF_Vector(false);
}

/*!
 \brief 计算最近的端点，即0,90,180,270方向上的四个点。

 \param coord
 \param dist
 \return PF_Vector
*/
PF_Vector PF_Circle::getNearestEndpoint(const PF_Vector &coord, double *dist) const
{
    return getNearestMiddle( coord, dist, 0);
}

/*!
 \brief

 \param coord
 \param onEntity
 \param dist
 \param entity
 \return PF_Vector
*/
PF_Vector PF_Circle::getNearestPointOnEntity(const PF_Vector &coord, bool onEntity, double *dist, PF_Entity **entity) const
{
    if (entity) {
        *entity = const_cast<PF_Circle*>(this);
    }
    PF_Vector vp(coord - data.center);
    double d(vp.magnitude());
    if( d < PF_TOLERANCE ) return PF_Vector(false);
    vp =data.center+vp*(data.radius/d);
//    RS_DEBUG->print(RS_Debug::D_ERROR, "circle(%g, %g), r=%g: distance to point (%g, %g)\n",data.center.x,data.center.y,coord.x,coord.y);

    if(dist){
        *dist=coord.distanceTo(vp);
//        RS_DEBUG->print(RS_Debug::D_ERROR, "circle(%g, %g), r=%g: distance to point (%g, %g)=%g\n",data.center.x,data.center.y,coord.x,coord.y,*dist);
    }
    return vp;
}

PF_Vector PF_Circle::getNearestCenter(const PF_Vector &coord, double *dist) const
{
    if (dist) {
        *dist = coord.distanceTo(data.center);
    }
    return data.center;
}

/*!
 \brief 计算四分之一圆上的中点。

 \param coord 给定的点坐标
 \param dist 距离
 \param middlePoints
 0 返回0，90，180，270角度上的点
 1 返回45，135，225，315角度上的点
 2 返回30，60，120，150，210，240，300，330角度上的点
 ....
 \return PF_Vector 距离最近的点
*/
PF_Vector PF_Circle::getNearestMiddle(const PF_Vector &coord, double *dist, int middlePoints) const
{
    if( data.radius <= PF_TOLERANCE) {
        //circle too short
        if ( nullptr != dist) {
            *dist = PF_MAXDOUBLE;
        }
        return PF_Vector(false);
    }

    PF_Vector vPoint( getNearestPointOnEntity( coord, true, dist));
    int iCounts = middlePoints + 1;
    double dAngleSteps = M_PI_2 / iCounts;
    double dAngleToPoint = data.center.angleTo(vPoint);
    int iStepCount = static_cast<int>((dAngleToPoint + 0.5 * dAngleSteps) / dAngleSteps);
    if( 0 < middlePoints) {
        // for nearest middle eliminate start/endpoints
        int iQuadrant = static_cast<int>(dAngleToPoint / 0.5 / M_PI);
        int iQuadrantStep = iStepCount - iQuadrant * iCounts;
        if( 0 == iQuadrantStep) {
            ++iStepCount;
        }
        else if( iCounts == iQuadrantStep) {
            --iStepCount;
        }
    }

    vPoint.setPolar( data.radius, dAngleSteps * iStepCount);
    vPoint.move( data.center);

    if(dist) {
        *dist = vPoint.distanceTo( coord);
    }

    return vPoint;
}

PF_Vector PF_Circle::getNearestDist(double distance, const PF_Vector &coord, double *dist) const
{
    if (dist) {
        *dist = PF_MAXDOUBLE;
    }
    return PF_Vector(false);
}

PF_Vector PF_Circle::getNearestDist(double distance, bool startp) const
{
    return PF_Vector(false);
}

PF_Vector PF_Circle::getNearestOrthTan(const PF_Vector &coord, const PF_Line &normal, bool onEntity) const
{
    if ( !coord.valid) {
            return PF_Vector(false);
    }
    PF_Vector vp0(coord-getCenter());
    PF_Vector vp1(normal.getAngle1());
    double d=PF_Vector::dotP(vp0,vp1);
    if(d >= 0. ) {
            return getCenter() + vp1*getRadius();
    }else{
            return getCenter() - vp1*getRadius();
    }
}

/*!
 \brief 偏移操作

 \param coord 偏移的方向向量
 \param distance 偏移的距离
 \return bool 偏移是否成功
*/
bool PF_Circle::offset(const PF_Vector &coord, const double &distance)
{
    double r0(coord.distanceTo(getCenter()));
    if(r0 > getRadius()){
        //external
        r0 = getRadius()+ fabs(distance);
    }else{
        r0 = getRadius()- fabs(distance);
        if(r0<PF_TOLERANCE) {
            return false;
        }
    }
    setRadius(r0);
    calculateBorders();
    return true;
}

/*!
 \brief 移动

 \param offset
*/
void PF_Circle::move(const PF_Vector &offset)
{
    data.center.move(offset);
}

void PF_Circle::rotate(const PF_Vector &center, const double &angle)
{
    data.center.rotate(center, angle);
    calculateBorders();
}

void PF_Circle::rotate(const PF_Vector &center, const PF_Vector &angleVector)
{
    data.center.rotate(center, angleVector);
    calculateBorders();
}

void PF_Circle::scale(const PF_Vector &center, const PF_Vector &factor)
{
    data.center.scale(center, factor);
    //radius always is positive
    data.radius *= fabs(factor.x);
    scaleBorders(center,factor);
}

void PF_Circle::mirror(const PF_Vector &axisPoint1, const PF_Vector &axisPoint2)
{
    data.center.mirror(axisPoint1, axisPoint2);
    calculateBorders();
}

void PF_Circle::moveRef(const PF_Vector &ref, const PF_Vector &offset)
{
    if(ref.distanceTo(data.center)<1.0e-4){
        data.center += offset;
        return;
    }
    PF_Vector v1(data.radius, 0.0);
    PF_VectorSolutions sol;
    sol.push_back(data.center + v1);
    sol.push_back(data.center - v1);
    v1.set(0., data.radius);
    sol.push_back(data.center + v1);
    sol.push_back(data.center - v1);
    double dist;
    v1=sol.getClosest(ref,&dist);
    if(dist>1.0e-4) return;
    data.radius = data.center.distanceTo(v1 + offset);
}

void PF_Circle::draw(QCPPainter *painter)
{
    //qDebug()<<"PF_Circle::draw";
    /** set Pen **/
//    QPen oldpen = painter->pen();
//    QBrush oldbursh = painter->brush();
    painter->save();
    if(isSelected()){
        pen.setColor(QColor(0,0,255));
        painter->setPen(pen);
    }

    painter->drawEllipse(QPointF(mParentPlot->toGuiX(getCenter().x),
                                mParentPlot->toGuiY(getCenter().y)),
                                mParentPlot->toGuiDY(getRadius()),
                                mParentPlot->toGuiDY(getRadius()));
    //qDebug()<<"PF_Circle::draw: OK.";
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

void PF_Circle::calculateBorders()
{
    PF_Vector r(data.radius,data.radius);
    minV = data.center - r;
    maxV = data.center + r;
}

QString PF_Circle::toGeoString()
{
    return QString("");
}

int PF_Circle::index() const
{
    return 0;
}

PF_CircleData::PF_CircleData(PF_Vector *center, double radius)
    :center(center)
    ,radius(radius)
{

}
