#include "pf_entity.h"
#include "pf_graphicview.h"


/*!
 \brief PF_Entity的构造函数。PF_Entity继承于QCPLayerable，但是由于在QCPLayerable
 的构造函数当中会默认将当前实体添加到层当中，连续带来的问题就是container当中的所有元素
 都被添加到了layer的列表当中，所以元素就被重复计算了。导致绘图的时候绘制了两次。为了解
 决这个问题，默认将mParentplot初始化为空，这样在父类构造函数中就不会执行setlayer的函
 数了。然后再在子类构造函数当中进行初始化操作。

 \param parent
 \param plot
*/
PF_Entity::PF_Entity(PF_EntityContainer *parent, PF_GraphicView *plot)
    :QCPLayerable (nullptr)
{
    this->parent = parent;
    /**手动进行初始化**/
    this->setParent(plot);
    mParentPlot = plot;
    init();
}

void PF_Entity::init()
{

}

PF_Vector PF_Entity::getStartpoint() const
{
    return {};
}

PF_Vector PF_Entity::getEndpoint() const
{
    return {};
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
