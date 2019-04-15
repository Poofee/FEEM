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
    /** 手动进行初始化 **/
    this->setParent(plot);
    mParentPlot = plot;
    init();
}

void PF_Entity::init()
{
    setFlag(PF::FlagVisible);
    /** 获取默认的画笔 **/

}

PF_Vector PF_Entity::getStartpoint() const
{
    return {};
}

PF_Vector PF_Entity::getEndpoint() const
{
    return {};
}

QPen PF_Entity::getPen(bool resolve) const
{
    return pen;
}

QBrush PF_Entity::getBrush(bool resolve) const
{
    return brush;
}

bool PF_Entity::setSelected(bool select)
{
    if(select)
        setFlag(PF::FlagSelected);
    else
        delFlag(PF::FlagSelected);

    return  true;
}

bool PF_Entity::toggleSelected()
{
    return setSelected(!isSelected());
}

bool PF_Entity::isSelected() const
{
    return isVisible() && getFlag(PF::FlagSelected);
}

bool PF_Entity::isParentSelected() const
{
//    PF_Entity const* p = this;

//	while(p) {
//		p = p->getParent();
//		if (p && p->isSelected()==true) {
//			return true;
//		}
//	}

    return false;
}

bool PF_Entity::isVisible() const
{
    if(!getFlag(PF::FlagVisible))
        return false;

    return false;
}

void PF_Entity::setVisible(bool v)
{
    if(v)
        setFlag(PF::FlagVisible);
    else
        delFlag(PF::FlagVisible);
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

/*!
 \brief 返回实体的所有参考点，子类必须实现。

 \return PF_VectorSolutions
*/
PF_VectorSolutions PF_Entity::getRefPoints() const
{
    return PF_VectorSolutions();
}

PF_Vector PF_Entity::getNearestRef(const PF_Vector& coord,
                                   double* dist) const{
    PF_VectorSolutions const&& s = getRefPoints();

    return s.getClosest(coord, dist);
}

PF_Vector PF_Entity::getNearestSelectedRef(const PF_Vector& coord,
                                           double* dist) const{
    if (isSelected()) {
        return getNearestRef(coord, dist);
    }
    else {
        return PF_Vector(false);
    }
}
