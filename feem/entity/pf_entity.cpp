#include "pf_entity.h"
#include "pf_graphicview.h"

PF_Entity::PF_Entity(PF_EntityContainer *parent, PF_GraphicView *plot)
    :QCPLayerable (plot)
{
    this->parent = parent;
    init();
}

void PF_Entity::init()
{

}

/*!
 \brief 设定绘图的边界，使得图形都显示在坐标轴内部

 \return QRect
*/
QRect PF_Entity::clipRect() const
{
    if(mParentPlot)
        return mParentPlot->xAxis->axisRect()->rect();
    else
        return QRect();
}

void PF_Entity::applyDefaultAntialiasingHint(QCPPainter *painter) const
{
    applyAntialiasingHint(painter, mAntialiased, QCP::aeItems);
}
