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

}

PF_Vector PF_Line::getMiddlePoint() const
{

}

PF_Vector PF_Line::getNearestEndpoint(const PF_Vector &coord, double *dist) const
{

}

PF_Vector PF_Line::getNearestPointOnEntity(const PF_Vector &coord, bool onEntity, double *dist, PF_Entity **entity) const
{

}

PF_Vector PF_Line::getNearestCenter(const PF_Vector &coord, double *dist) const
{

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
