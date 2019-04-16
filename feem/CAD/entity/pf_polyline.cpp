#include "pf_line.h"
#include "pf_polyline.h"


PF_PolylineData::PF_PolylineData()
    :startPoint(false)
    ,endPoint(false)
    ,isClosed(true)
{

}

PF_PolylineData::PF_PolylineData(const PF_Vector &startpoint, const PF_Vector &endpoint, bool closed)
    :startPoint(startpoint)
    ,endPoint(endpoint)
    ,isClosed(closed)
{

}

PF_Polyline::PF_Polyline(PF_EntityContainer *parent, PF_GraphicView *view)
    :PF_EntityContainer(parent,view,true)
    ,closingEntity(nullptr)
    ,nextBulge(0.)
{

}

PF_Polyline::PF_Polyline(PF_EntityContainer *parent, PF_GraphicView *view, const PF_PolylineData &d)
    :PF_EntityContainer(parent,view,true)
    ,data(d)
    ,closingEntity(nullptr)
    ,nextBulge(0.)
{

}

void PF_Polyline::setStartpoint(const PF_Vector &v)
{
    data.startPoint = v;
    if (!data.endPoint.valid) {
        data.endPoint = v;
    }
}

PF_Vector PF_Polyline::getStartpoint() const
{
    return data.startPoint;
}

void PF_Polyline::setEndpoint(const PF_Vector &v)
{
    data.endPoint = v;
}

PF_Vector PF_Polyline::getEndpoint() const
{
    return data.endPoint;
}

double PF_Polyline::getClosingBulge() const
{
    return 0.0;
}

void PF_Polyline::updateEndpoints()
{
//    PF_Entity* e1 = firstEntity();
//    if (e1 && e1->isAtomic()) {
//        PF_Vector const& v = e1->getStartpoint();
//        setStartpoint(v);
//    }

//    PF_Entity const* e2 = last();
//    if (isClosed()) {
//        e2 = prevEntity();
//    }
//    if (e2 && e2->isAtomic()) {
//        PF_Vector const& v = e2->getEndpoint();
//        setEndpoint(v);
//    }
}

bool PF_Polyline::isClosed() const
{
    return data.isClosed;
}

void PF_Polyline::setClosed(bool cl)
{
    data.isClosed = cl;
}

void PF_Polyline::setClosed(bool cl, double bulge)
{
    Q_UNUSED(bulge);
    bool areClosed = isClosed();
    setClosed(cl);
    if (isClosed()) {
        endPolyline();
    } else if (areClosed){
        removeLastVertex();
    }
}

PF_Entity *PF_Polyline::addVertex(const PF_Vector &v, double bulge, bool prepend)
{
    PF_Entity* entity=nullptr;
    //static double nextBulge = 0.0;

    // very first vertex:
    if (!data.startPoint.valid) {
        data.startPoint = data.endPoint = v;
        nextBulge = bulge;
    }

    // consequent vertices:
    else {
        // add entity to the polyline:
        entity = createVertex(v, nextBulge, prepend);
        if (entity) {
            if (!prepend) {
                PF_EntityContainer::addEntity(entity);
                data.endPoint = v;
            }
            else {
                PF_EntityContainer::insertEntity(0, entity);
                data.startPoint = v;
            }
        }
        nextBulge = bulge;
        endPolyline();
    }
    //data.endpoint = v;

    return entity;
}

void PF_Polyline::appendVertexs(const std::vector<std::pair<PF_Vector, double> > &vl)
{
    PF_Entity* entity=nullptr;
    //static double nextBulge = 0.0;
    if (!vl.size()) return;
    size_t idx = 0;
    // very first vertex:
    if (!data.startPoint.valid) {
        data.startPoint = data.endPoint = vl.at(idx).first;
        nextBulge = vl.at(idx++).second;
    }

    // consequent vertices:
    for (; idx< vl.size();idx++){
        entity = createVertex(vl.at(idx).first, nextBulge, false);
        data.endPoint = entity->getEndpoint();
        PF_EntityContainer::addEntity(entity);
        nextBulge = vl.at(idx).second;
    }

    endPolyline();
}


/*!
 \brief 删除默认操作

 \param entity
*/
void PF_Polyline::addEntity(PF_Entity *entity)
{
    Q_UNUSED(entity);
    qDebug()<<Q_FUNC_INFO<<"should never be called here!";
}


/**
 * @brief 移除多边形当中的最后一个顶点
 *
 */
void PF_Polyline::removeLastVertex()
{
    PF_Entity* l = last();
    if (l) {
        removeEntity(l);
        l = last();
        if (l) {
            if (l->isAtomic()) {
                data.endPoint = l->getEndpoint();
            }
            else {
                qDebug()<<Q_FUNC_INFO<<"polyline contains non-atomic entity";
            }
        }
    }
}


/*!
 \brief 结束线段绘制。如果为闭合线段，则添加最后的部分。

*/
void PF_Polyline::endPolyline()
{
    if(isClosed()){
        // remove old closing entity:
        if (closingEntity) {
            removeEntity(closingEntity);
        }

        // add closing entity to the polyline:
        closingEntity = createVertex(data.startPoint, nextBulge);
        if (closingEntity) {
            PF_EntityContainer::addEntity(closingEntity);
        }
    }
}

PF_VectorSolutions PF_Polyline::getRefPoints() const
{
    PF_VectorSolutions ret{{data.startPoint}};
    for(auto e: *this){
        if (e->isAtomic()) {
            ret.push_back(e->getEndpoint());
        }
    }

    ret.push_back( data.endPoint);

    return ret;
}

PF_Vector PF_Polyline::getNearestRef(const PF_Vector &coord, double *dist) const
{
    return PF_Entity::getNearestRef( coord, dist);
}

PF_Vector PF_Polyline::getNearestSelectedRef(const PF_Vector &coord, double *dist) const
{
    return PF_Entity::getNearestSelectedRef( coord, dist);
}

bool PF_Polyline::offset(const PF_Vector &coord, const double &distance)
{
    return true;
}

void PF_Polyline::move(const PF_Vector &offset)
{
    PF_EntityContainer::move(offset);
    data.startPoint.move(offset);
    data.endPoint.move(offset);
    calculateBorders();
}

void PF_Polyline::rotate(const PF_Vector &center, const double &angle)
{
    rotate(center, PF_Vector(angle));
}

void PF_Polyline::rotate(const PF_Vector &center, const PF_Vector &angleVector)
{
    data.startPoint.rotate(center, angleVector);
    data.endPoint.rotate(center, angleVector);
    calculateBorders();
}

void PF_Polyline::scale(const PF_Vector &center, const PF_Vector &factor)
{
    data.startPoint.scale(center, factor);
    data.endPoint.scale(center, factor);
    calculateBorders();
}

void PF_Polyline::mirror(const PF_Vector &axisPoint1, const PF_Vector &axisPoint2)
{
    data.startPoint.mirror(axisPoint1, axisPoint2);
    data.endPoint.mirror(axisPoint1, axisPoint2);
    calculateBorders();
}

//void PF_Polyline::stretch(const PF_Vector &firstCorner, const PF_Vector &secondCorner, const PF_Vector &offset)
//{

//}

void PF_Polyline::moveRef(const PF_Vector &ref, const PF_Vector &offset)
{
    if (ref.distanceTo(data.startPoint)<1.0e-4) {
       data.startPoint.move(offset);
    }
    if (ref.distanceTo(data.endPoint)<1.0e-4) {
       data.endPoint.move(offset);
    }
    calculateBorders();
    //update();
}

void PF_Polyline::revertDirection()
{
    PF_EntityContainer::revertDirection();
    PF_Vector tmp = data.startPoint;
    data.startPoint = data.endPoint;
    data.endPoint = tmp;
}



void PF_Polyline::draw(QCPPainter *p)
{
    PF_EntityContainer::draw(p);
}


PF_Entity *PF_Polyline::createVertex(const PF_Vector &v, double bulge, bool prepend)
{
    PF_Entity * entity = nullptr;

    /**先只考虑直线的情况**/
    if (prepend) {
        entity = new PF_Line{this,mParentPlot, v, data.startPoint};
    } else {
        entity = new PF_Line{this,mParentPlot, data.endPoint, v};
    }

    return entity;
}


