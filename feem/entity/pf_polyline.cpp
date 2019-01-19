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

}

PF_Entity *PF_Polyline::addVertex(const PF_Vector &v, double bulge, bool prepend)
{
    return nullptr;
}

void PF_Polyline::appendVertexs(const std::vector<std::pair<PF_Vector, double> > &vl)
{

}

void PF_Polyline::addEntity(PF_Entity *entity)
{

}


/**
 * @brief 移除多边形当中的最后一个顶点
 *
 */
void PF_Polyline::removeLastVertex()
{

}

void PF_Polyline::endPolyline()
{

}

void PF_Polyline::draw(QCPPainter *p)
{

}

PF_Entity *PF_Polyline::createVertex(const PF_Vector &v, double bulge, bool prepend)
{
    return nullptr;
}


