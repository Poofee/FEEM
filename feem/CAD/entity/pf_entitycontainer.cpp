#include "pf_entitycontainer.h"
#include "pf_graphicview.h"
#include "pf_line.h"

#include <QDebug>

PF_EntityContainer::PF_EntityContainer(PF_EntityContainer *parent, PF_GraphicView *view, bool owner)
    :PF_Entity(parent,view)
{
    autoDelete = owner;
}

PF_EntityContainer::~PF_EntityContainer()
{
    if(autoDelete){
        while(!entities.isEmpty()){
            delete entities.takeFirst();
        }
    }else{
        entities.clear();
    }
}

void PF_EntityContainer::clear()
{
    //qDebug()<<"PF_EntityContainer::clear";
    if(autoDelete){
        while(!entities.isEmpty()){
            delete entities.takeFirst();
        }
    }else{
        entities.clear();
    }
    //qDebug()<<"PF_EntityContainer::clear: OK.";
}


/**
 * @brief 返回第一个Entity
 *
 * @return PF_Entity
 */
PF_Entity *PF_EntityContainer::first() const
{
    return entities.first();
}


/**
 * @brief 返回最后一个Entity
 *
 * @return PF_Entity
 */
PF_Entity *PF_EntityContainer::last() const
{
    return entities.last();
}

void PF_EntityContainer::addEntity(PF_Entity *entity)
{
    if(!entity){
        return;
    }

    entities.append(entity);
    //qDebug()<<"PF_EntityContainer::addEntity:size:"<<entities.size();
}


/**
 * @brief 在Entity列表的尾部添加entity
 *
 * @param entity
 */
void PF_EntityContainer::appendEntity(PF_Entity *entity)
{
    if (!entity)
            return;
    entities.append(entity);
}

/**
 * @brief 在Entity列表的头部添加entity
 *
 * @param entity
 */
void PF_EntityContainer::prependEntity(PF_Entity *entity)
{
    if (!entity) return;
    entities.prepend(entity);
}

void PF_EntityContainer::moveEntity(int index, QList<PF_Entity *> &entList)
{

}


/**
 * @brief 在Entity列表当中插入某个位置的entity
 *
 * @param index
 * @param entity
 */
void PF_EntityContainer::insertEntity(int index, PF_Entity *entity)
{
    if (!entity) return;

    entities.insert(index, entity);
}


/**
 * @brief 移除某个entity
 *
 * @param entity
 * @return bool
 */
bool PF_EntityContainer::removeEntity(PF_Entity *entity)
{
    bool ret;
    ret = entities.removeOne(entity);

    if (autoDelete && ret) {
        delete entity;
    }
    return ret;
}


/**
 * @brief 在实体当中添加一个矩形
 *
 * @param corner1
 * @param corner2
 */
void PF_EntityContainer::addRectangle(const PF_Vector &corner1, const PF_Vector &corner2)
{
    addEntity(new PF_Line{this,mParentPlot, corner1, {corner2.x, corner1.y}});
    addEntity(new PF_Line{this,mParentPlot, {corner2.x, corner1.y}, corner2});
    addEntity(new PF_Line{this,mParentPlot, corner2, {corner1.x, corner2.y}});
    addEntity(new PF_Line{this,mParentPlot, {corner1.x, corner2.y}, corner1});
}

void PF_EntityContainer::draw(QCPPainter *painter)
{
    if(!(painter && mParentPlot)){
        qDebug()<<Q_FUNC_INFO<<"NULL";
        return;
    }

    for(int i=0;i < entities.size();++i){
        mParentPlot->drawEntity(painter,entities.at(i));
    }
}

const QList<PF_Entity *> &PF_EntityContainer::getEntityList()
{
    return entities;
}

PF_VectorSolutions PF_EntityContainer::getRefPoints() const
{

}

PF_Vector PF_EntityContainer::getMiddlePoint() const
{

}

PF_Vector PF_EntityContainer::getNearestEndpoint(const PF_Vector &coord, double *dist) const
{

}

PF_Vector PF_EntityContainer::getNearestPointOnEntity(const PF_Vector &coord, bool onEntity, double *dist, PF_Entity **entity) const
{

}

PF_Vector PF_EntityContainer::getNearestCenter(const PF_Vector &coord, double *dist) const
{

}

PF_Vector PF_EntityContainer::getNearestMiddle(const PF_Vector &coord, double *dist, int middlePoints) const
{

}

PF_Vector PF_EntityContainer::getNearestDist(double distance, const PF_Vector &coord, double *dist) const
{

}

PF_Vector PF_EntityContainer::getNearestDist(double distance, bool startp) const
{

}

PF_Vector PF_EntityContainer::getNearestOrthTan(const PF_Vector &coord, const PF_Line &normal, bool onEntity) const
{

}

bool PF_EntityContainer::offset(const PF_Vector &coord, const double &distance)
{

}

void PF_EntityContainer::move(const PF_Vector &offset)
{

}

void PF_EntityContainer::rotate(const PF_Vector &center, const double &angle)
{

}

void PF_EntityContainer::rotate(const PF_Vector &center, const PF_Vector &angleVector)
{

}

void PF_EntityContainer::scale(const PF_Vector &center, const PF_Vector &factor)
{

}

void PF_EntityContainer::mirror(const PF_Vector &axisPoint1, const PF_Vector &axisPoint2)
{

}

void PF_EntityContainer::moveRef(const PF_Vector &ref, const PF_Vector &offset)
{

}
