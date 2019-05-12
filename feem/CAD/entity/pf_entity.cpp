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

PF_Vector PF_Entity::getCenter() const
{
    return PF_Vector{};
}

double PF_Entity::getRadius() const
{
    return PF_MAXDOUBLE;
}

void PF_Entity::resetBorders()
{
    // TODO: Check that. windoze XP crashes with MAXDOUBLE
    double maxd = PF_MAXDOUBLE;
    double mind = PF_MINDOUBLE;

    minV.set(maxd, maxd);
    maxV.set(mind, mind);
}

void PF_Entity::moveBorders(const PF_Vector &offset)
{
    minV.move(offset);
    maxV.move(offset);
}

void PF_Entity::scaleBorders(const PF_Vector &center, const PF_Vector &factor)
{
    minV.scale(center,factor);
    maxV.scale(center,factor);
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

bool PF_Entity::setHighlighted(bool Highlighted)
{
    if(Highlighted)
        setFlag(PF::FlagHighlighted);
    else
        delFlag(PF::FlagHighlighted);

    return  true;
}

bool PF_Entity::toggleHighlighted()
{
    return setHighlighted(!isHighlighted());
}

bool PF_Entity::isHighlighted() const
{
    return getFlag(PF::FlagHighlighted);
}

bool PF_Entity::isParentHighlighted() const
{
    return false;
}

bool PF_Entity::isVisible() const
{
    if(!getFlag(PF::FlagVisible))
        return false;

    return true;
}

void PF_Entity::setVisible(bool v)
{
    if(v)
        setFlag(PF::FlagVisible);
    else
        delFlag(PF::FlagVisible);
}

PF_Vector PF_Entity::getSize() const
{
    return maxV-minV;
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
    /** 右值引用，由于函数的返回值的存储是不稳定的，立马被释放掉的
     * 这里如果正常写的话，应该建立一个变量来保存函数的返回值，
     * 而右值引用可以避免这样的拷贝。
    **/
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

PF_Vector PF_Entity::getNearestOrthTan(const PF_Vector &, const PF_Line &, bool onEntity) const
{
    return PF_Vector{false};
}

double PF_Entity::getDistanceToPoint(const PF_Vector &coord, PF_Entity **entity, PF::ResolveLevel level, double solidDist) const
{
    if (entity) {
        *entity=const_cast<PF_Entity*>(this);
    }
    double dToEntity = PF_MAXDOUBLE;
    (void) getNearestPointOnEntity(coord, true, &dToEntity, entity);

    // RVT 6 Jan 2011 : Add selection by center point
    if(getCenter().valid){
        double dToCenter=getCenter().distanceTo(coord);
        return std::min(dToEntity,dToCenter);
    }else
        return dToEntity;
}

bool PF_Entity::isPointOnEntity(const PF_Vector &coord, double tolerance) const
{
    double dist = getDistanceToPoint(coord, nullptr, PF::ResolveNone);
    return (dist<=fabs(tolerance));
}
