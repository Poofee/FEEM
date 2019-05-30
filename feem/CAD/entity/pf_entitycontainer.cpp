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

unsigned PF_EntityContainer::count() const
{
    return entities.size();
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
    if (entList.isEmpty()) return;
    int ci = 0; //current index for insert without invert order
    bool ret, into = false;
    PF_Entity *mid = nullptr;
    if (index < 1) {
        ci = 0;
    } else if (index >= entities.size() ) {
        ci = entities.size() - entList.size();
    } else {
        into = true;
        mid = entities.at(index);
    }

    for (int i = 0; i < entList.size(); ++i) {
        PF_Entity *e = entList.at(i);
        ret = entities.removeOne(e);
        //if e not exist in entities list remove from entList
        if (!ret) {
            entList.removeAt(i);
        }
    }
    if (into) {
        ci = entities.indexOf(mid);
    }

    for(auto e: entList){
            entities.insert(ci++, e);
    }
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

void PF_EntityContainer::adjustBorders(PF_Entity *entity)
{
    if (entity) {
        // make sure a container is not empty (otherwise the border
        //   would get extended to 0/0):
        if (!entity->isContainer() || entity->count()>0) {
            minV = PF_Vector::minimum(entity->getMin(),minV);
            maxV = PF_Vector::maximum(entity->getMax(),maxV);
        }

        // Notify parents. The border for the parent might
        // also change TODO: Check for efficiency
        //if(parent) {
        //parent->adjustBorders(this);
        //}
    }
}


/**
 * Recalculates the borders of this entity container.
 */
void PF_EntityContainer::calculateBorders() {

    resetBorders();
    for (PF_Entity* e: entities){
        if (e->isVisible() /*&& !(layer && layer->isFrozen())*/) {
            e->calculateBorders();
            adjustBorders(e);
        }
    }

    if (minV.x>maxV.x || minV.x>PF_MAXDOUBLE || maxV.x>PF_MAXDOUBLE
            || minV.x<PF_MINDOUBLE || maxV.x<PF_MINDOUBLE) {

        minV.x = 0.0;
        maxV.x = 0.0;
    }
    if (minV.y>maxV.y || minV.y>PF_MAXDOUBLE || maxV.y>PF_MAXDOUBLE
            || minV.y<PF_MINDOUBLE || maxV.y<PF_MINDOUBLE) {

        minV.y = 0.0;
        maxV.y = 0.0;
    }
}



/**
 * @return The point which is closest to 'coord'
 * (one of the vertexes)
 */
PF_Vector PF_EntityContainer::getNearestEndpoint(const PF_Vector& coord,
                                                 double* dist  )const {

    double minDist = PF_MAXDOUBLE;  // minimum measured distance
    double curDist;                 // currently measured distance
    PF_Vector closestPoint(false);  // closest found endpoint
    PF_Vector point;                // endpoint found

    for (PF_Entity* en: entities){

        if (en->isVisible()
                //&& !en->getParent()->ignoredOnModification()
                ){//no end point for Insert, text, Dim
            point = en->getNearestEndpoint(coord, &curDist);
            if (point.valid && curDist<minDist) {
                closestPoint = point;
                minDist = curDist;
                if (dist) {
                    *dist = minDist;
                }
            }
        }
    }

    return closestPoint;
}



/**
 * @return The point which is closest to 'coord'
 * (one of the vertexes)
 */
PF_Vector PF_EntityContainer::getNearestEndpoint(const PF_Vector& coord,
                                                 double* dist,  PF_Entity** pEntity)const {

    double minDist = PF_MAXDOUBLE;  // minimum measured distance
    double curDist;                 // currently measured distance
    PF_Vector closestPoint(false);  // closest found endpoint
    PF_Vector point;                // endpoint found

    //QListIterator<PF_Entity> it = createIterator();
    //PF_Entity* en;
    //while ( (en = it.current())  ) {
    //    ++it;

    unsigned i0=0;
    for(auto en: entities){
        //if (!en->getParent()->ignoredOnModification() ){//no end point for Insert, text, Dim
//            std::cout<<"find nearest for entity "<<i0<<std::endl;
            point = en->getNearestEndpoint(coord, &curDist);
            if (point.valid && curDist<minDist) {
                closestPoint = point;
                minDist = curDist;
                if (dist) {
                    *dist = minDist;
                }
                if(pEntity){
                    *pEntity=en;
                }
            }
        //}
        i0++;
    }

//    std::cout<<__FILE__<<" : "<<__func__<<" : line "<<__LINE__<<std::endl;
//    std::cout<<"count()="<<const_cast<PF_EntityContainer*>(this)->count()<<"\tminDist= "<<minDist<<"\tclosestPoint="<<closestPoint;
//    if(pEntity ) std::cout<<"\t*pEntity="<<*pEntity;
//    std::cout<<std::endl;
    return closestPoint;
}



PF_Vector PF_EntityContainer::getNearestPointOnEntity(const PF_Vector& coord,
                                                      bool onEntity, double* dist, PF_Entity** entity)const {

    PF_Vector point(false);

    PF_Entity* en = getNearestEntity(coord, dist, PF::ResolveNone);

    if (en && en->isVisible()
            //&& !en->getParent()->ignoredSnap()
            ){
        point = en->getNearestPointOnEntity(coord, onEntity, dist, entity);
    }

    return point;
}



PF_Vector PF_EntityContainer::getNearestCenter(const PF_Vector& coord,
                                               double* dist) const{
    double minDist = PF_MAXDOUBLE;  // minimum measured distance
    double curDist = PF_MAXDOUBLE;  // currently measured distance
    PF_Vector closestPoint(false);  // closest found endpoint
    PF_Vector point;                // endpoint found

    for(auto en: entities){

        if (en->isVisible()
                //&& !en->getParent()->ignoredSnap()
                ){//no center point for spline, text, Dim
            point = en->getNearestCenter(coord, &curDist);
            if (point.valid && curDist<minDist) {
                closestPoint = point;
                minDist = curDist;
            }
        }
    }
    if (dist) {
        *dist = minDist;
    }

    return closestPoint;
}

/** @return the nearest of equidistant middle points of the line. */

PF_Vector PF_EntityContainer::getNearestMiddle(const PF_Vector& coord,
                                               double* dist,
                                               int middlePoints
                                               ) const{
    double minDist = PF_MAXDOUBLE;  // minimum measured distance
    double curDist = PF_MAXDOUBLE;  // currently measured distance
    PF_Vector closestPoint(false);  // closest found endpoint
    PF_Vector point;                // endpoint found

    for(auto en: entities){

        if (en->isVisible()
                //&& !en->getParent()->ignoredSnap()
                ){//no midle point for spline, text, Dim
            point = en->getNearestMiddle(coord, &curDist, middlePoints);
            if (point.valid && curDist<minDist) {
                closestPoint = point;
                minDist = curDist;
            }
        }
    }
    if (dist) {
        *dist = minDist;
    }

    return closestPoint;
}



PF_Vector PF_EntityContainer::getNearestDist(double distance,
                                             const PF_Vector& coord,
                                             double* dist) const{

    PF_Vector point(false);
    PF_Entity* closestEntity;

    closestEntity = getNearestEntity(coord, nullptr, PF::ResolveNone);

    if (closestEntity) {
        point = closestEntity->getNearestDist(distance, coord, dist);
    }

    return point;
}

PF_Vector PF_EntityContainer::getNearestIntersection(const PF_Vector &coord, double *dist)
{
    return PF_Vector{};
}

PF_Vector PF_EntityContainer::getNearestRef(const PF_Vector& coord,
                                            double* dist) const{

    double minDist = PF_MAXDOUBLE;  // minimum measured distance
    double curDist;                 // currently measured distance
    PF_Vector closestPoint(false);  // closest found endpoint
    PF_Vector point;                // endpoint found

    for(auto en: entities){

        if (en->isVisible()) {
            point = en->getNearestRef(coord, &curDist);
            if (point.valid && curDist<minDist) {
                closestPoint = point;
                minDist = curDist;
                if (dist) {
                    *dist = minDist;
                }
            }
        }
    }

    return closestPoint;
}


PF_Vector PF_EntityContainer::getNearestSelectedRef(const PF_Vector& coord,
                                                    double* dist) const{

    double minDist = PF_MAXDOUBLE;  // minimum measured distance
    double curDist;                 // currently measured distance
    PF_Vector closestPoint(false);  // closest found endpoint
    PF_Vector point;                // endpoint found

    for(auto en: entities){

        if (en->isVisible() && en->isSelected() && !en->isParentSelected()) {
            point = en->getNearestSelectedRef(coord, &curDist);
            if (point.valid && curDist<minDist) {
                closestPoint = point;
                minDist = curDist;
                if (dist) {
                    *dist = minDist;
                }
            }
        }
    }

    return closestPoint;
}


/*!
 \brief 计算距离输入点最近的entity以及对应的距离

 \param coord
 \param entity
 \param level
 \param solidDist
 \return double
*/
double PF_EntityContainer::getDistanceToPoint(const PF_Vector& coord,
                                              PF_Entity** entity,
                                              PF::ResolveLevel level,
                                              double solidDist) const{

    // RS_DEBUG->print("PF_EntityContainer::getDistanceToPoint");


    double minDist = PF_MAXDOUBLE;      // minimum measured distance
    double curDist;                     // currently measured distance
    PF_Entity* closestEntity = nullptr;    // closest entity found
    PF_Entity* subEntity = nullptr;

    for(auto e: entities){

        if (e->isVisible()) {
            // RS_DEBUG->print("entity: getDistanceToPoint");
            // RS_DEBUG->print("entity: %d", e->rtti());
            // bug#426, need to ignore Images to find nearest intersections
            if(level==PF::ResolveAllButTextImage /*&& e->rtti()==PF::EntityImage*/) continue;
            curDist = e->getDistanceToPoint(coord, &subEntity, level, solidDist);

            // RS_DEBUG->print("entity: getDistanceToPoint: OK");

            /*
             * By using '<=', we will prefer the *last* item in the container if there are multiple
             * entities that are *exactly* the same distance away, which should tend to be the one
             * drawn most recently, and the one most likely to be visible (as it is also the order
             * that the software draws the entities). This makes a difference when one entity is
             * drawn directly over top of another, and it's reasonable to assume that humans will
             * tend to want to reference entities that they see or have recently drawn as opposed
             * to deeper more forgotten and invisible ones...
             */
            if (curDist<=minDist)
            {
                switch(level){
                case PF::ResolveAll:
                case PF::ResolveAllButTextImage:
                    closestEntity = subEntity;
                    break;
                default:
                    closestEntity = e;
                }
                minDist = curDist;
            }
        }
    }

    if (entity) {
        *entity = closestEntity;
    }
    // RS_DEBUG->print("PF_EntityContainer::getDistanceToPoint: OK");

    return minDist;
}



PF_Entity* PF_EntityContainer::getNearestEntity(const PF_Vector& coord,
                                                double* dist,
                                                PF::ResolveLevel level) const{

    // RS_DEBUG->print("PF_EntityContainer::getNearestEntity");

    PF_Entity* e = nullptr;

    // distance for points inside solids:
    double solidDist = PF_MAXDOUBLE;
    if (dist) {
        solidDist = *dist;
    }

    double d = getDistanceToPoint(coord, &e, level, solidDist);

    if (e && e->isVisible()==false) {
        e = nullptr;
    }

    // if d is negative, use the default distance (used for points inside solids)
    if (dist) {
        *dist = d;
    }
    // RS_DEBUG->print("PF_EntityContainer::getNearestEntity: OK");

    return e;
}



void PF_EntityContainer::move(const PF_Vector& offset) {
    for(auto e: entities){

        e->move(offset);
//        if (autoUpdateBorders) {
//            e->moveBorders(offset);
//        }
    }
//    if (autoUpdateBorders) {
//        moveBorders(offset);
//    }
}



void PF_EntityContainer::rotate(const PF_Vector& center, const double& angle) {
    PF_Vector angleVector(angle);

    for(auto e: entities){
        e->rotate(center, angleVector);
    }
//    if (autoUpdateBorders) {
//        calculateBorders();
//    }
}


void PF_EntityContainer::rotate(const PF_Vector& center, const PF_Vector& angleVector) {

    for(auto e: entities){
        e->rotate(center, angleVector);
    }
//    if (autoUpdateBorders) {
//        calculateBorders();
//    }
}


void PF_EntityContainer::scale(const PF_Vector& center, const PF_Vector& factor) {
    if (fabs(factor.x)>PF_TOLERANCE && fabs(factor.y)>PF_TOLERANCE) {

        for(auto e: entities){
            e->scale(center, factor);
        }
    }
//    if (autoUpdateBorders) {
//        calculateBorders();
//    }
}



void PF_EntityContainer::mirror(const PF_Vector& axisPoint1, const PF_Vector& axisPoint2) {
    if (axisPoint1.distanceTo(axisPoint2)>PF_TOLERANCE) {

        for(auto e: entities){
            e->mirror(axisPoint1, axisPoint2);
        }
    }
}


//void PF_EntityContainer::stretch(const PF_Vector& firstCorner,
//                                 const PF_Vector& secondCorner,
//                                 const PF_Vector& offset) {

//    if (getMin().isInWindow(firstCorner, secondCorner) &&
//            getMax().isInWindow(firstCorner, secondCorner)) {

//        move(offset);
//    } else {

//        for(auto e: entities){
//            e->stretch(firstCorner, secondCorner, offset);
//        }
//    }

    // some entitiycontainers might need an update (e.g. RS_Leader):
//    update();
//}



void PF_EntityContainer::moveRef(const PF_Vector& ref,
                                 const PF_Vector& offset) {


    for(auto e: entities){
        e->moveRef(ref, offset);
    }
//    if (autoUpdateBorders) {
//        calculateBorders();
//    }
}


void PF_EntityContainer::moveSelectedRef(const PF_Vector& ref,
                                         const PF_Vector& offset) {


    for(auto e: entities){
        e->moveSelectedRef(ref, offset);
    }
//    if (autoUpdateBorders) {
//        calculateBorders();
//    }
}

void PF_EntityContainer::revertDirection() {
    for(int k = 0; k < entities.size() / 2; ++k) {
        entities.swap(k, entities.size() - 1 - k);
    }

    for(PF_Entity*const entity: entities) {
        entity->revertDirection();
    }
}

QList<PF_Entity *>::const_iterator PF_EntityContainer::begin() const
{
    return entities.begin();
}

QList<PF_Entity *>::const_iterator PF_EntityContainer::end() const
{
    return entities.end();
}

QList<PF_Entity *>::iterator PF_EntityContainer::begin()
{
    return entities.begin();
}

QList<PF_Entity *>::iterator PF_EntityContainer::end()
{
    return entities.end();
}


const QList<PF_Entity*>& PF_EntityContainer::getEntityList()
{
    return entities;
}
