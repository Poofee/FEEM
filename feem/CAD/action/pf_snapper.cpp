#include <QMouseEvent>
#include "pf_snapper.h"
#include "pf_graphicview.h"
#include "pf_entitycontainer.h"
#include "pf_setting.h"

struct PF_Snapper::Indicator
{
    bool lines_state;
    QString lines_type;
    QPen lines_pen;

    bool shape_state;
    QString shape_type;
    QPen shape_pen;
};

struct PF_Snapper::ImpData {
    PF_Vector snapCoord;
    PF_Vector snapSpot;
};

PF_Snapper::PF_Snapper(PF_EntityContainer *container, PF_GraphicView *view)
    :view(view)
    ,container(container)
    ,pImpData(new ImpData{})
    ,snap_indicator(new Indicator{})
{

}

PF_Snapper::~PF_Snapper()
{
    delete snap_indicator;
}

void PF_Snapper::init()
{
//    snapMode = view->getDefaultSnapMode();
    keyEntity = nullptr;
    pImpData->snapSpot = PF_Vector{false};
    pImpData->snapCoord = PF_Vector{false};
    m_SnapDistance = 1.0;

//    RS_SETTINGS->beginGroup("/Appearance");
    snap_indicator->lines_state = 1;//RS_SETTINGS->readNumEntry("/indicator_lines_state", 1);
    snap_indicator->lines_type = "Crosshair";//RS_SETTINGS->readEntry("/indicator_lines_type", "Crosshair");
    snap_indicator->shape_state = 1;//RS_SETTINGS->readNumEntry("/indicator_shape_state", 1);
    snap_indicator->shape_type = "Circle";//RS_SETTINGS->readEntry("/indicator_shape_type", "Circle");
//    RS_SETTINGS->endGroup();

//    RS_SETTINGS->beginGroup("Colors");
    QString snap_color = Colors::snap_indicator;//RS_SETTINGS->readEntry("/snap_indicator", Colors::snap_indicator);
//    RS_SETTINGS->endGroup();

    snap_indicator->lines_pen = QPen(QColor(snap_color), 1, Qt::DashLine);
    snap_indicator->shape_pen = QPen(QColor(snap_color), 1, Qt::SolidLine);
//    snap_indicator->shape_pen.setScreenWidth(1);

//    snapRange = getSnapRange();
}

void PF_Snapper::finish()
{
    finished = true;
}

/**
 * @brief
 *
 * @param coord
 * @param setSpot
 * @return PF_Vector
 */
PF_Vector PF_Snapper::snapPoint(const PF_Vector &coord, bool setSpot)
{
    if(coord.valid){
        pImpData->snapSpot=coord;
        if(setSpot) pImpData->snapCoord = coord;
        drawSnapper();
//		RS_DIALOGFACTORY->updateCoordinateWidget(
//					pImpData->snapCoord,
//					pImpData->snapCoord - graphicView->getRelativeZero());
    }
    return coord;
}


/*!
 \brief 根据当前的模式捕捉点的坐标

 \param e
 \return PF_Vector
*/
PF_Vector PF_Snapper::snapPoint(QMouseEvent *e)
{
    return view->toGraph(PF_Vector(e->x(),e->y(),0));
}

/**
 * @brief
 *
 * @param e
 * @return PF_Vector
 */
PF_Vector PF_Snapper::snapFree(QMouseEvent *e)
{
    if (!e) {
//                RS_DEBUG->print(RS_Debug::D_WARNING,
//						"RS_Snapper::snapFree: event is nullptr");
        return PF_Vector(false);
    }
    pImpData->snapSpot = view->toGraph(e->x(), e->y());
    pImpData->snapCoord = pImpData->snapSpot;
    snap_indicator->lines_state = true;
    return pImpData->snapCoord;
}

/**
 * @brief
 *
 * @param coord
 * @return PF_Vector
 */
PF_Vector PF_Snapper::snapFree(const PF_Vector &coord)
{
    keyEntity = nullptr;
    return coord;
}

PF_Vector PF_Snapper::snapGrid(const PF_Vector &coord)
{

    return PF_Vector{};
}

/**
 * @brief
 *
 * @param coord
 * @return PF_Vector
 */
PF_Vector PF_Snapper::snapEndpoint(const PF_Vector &coord)
{
    PF_Vector vec(false);

    vec = container->getNearestEndpoint(coord,
                                        nullptr/*, &keyEntity*/);
    return vec;
}

/**
 * @brief
 *
 * @param coord
 * @return PF_Vector
 */
PF_Vector PF_Snapper::snapOnEntity(const PF_Vector &coord)
{
    PF_Vector vec{};
    vec = container->getNearestPointOnEntity(coord, true, nullptr, &keyEntity);
    return vec;
}

/**
 * @brief
 *
 * @param coord
 * @return PF_Vector
 */
PF_Vector PF_Snapper::snapCenter(const PF_Vector &coord)
{
    PF_Vector vec{};

    vec = container->getNearestCenter(coord, nullptr);
    return vec;
}

/**
 * @brief
 *
 * @param coord
 * @return PF_Vector
 */
PF_Vector PF_Snapper::snapMiddle(const PF_Vector &coord)
{
    return container->getNearestMiddle(coord,static_cast<double *>(nullptr),middlePoints);
}

/**
 * @brief
 *
 * @param coord
 * @return PF_Vector
 */
PF_Vector PF_Snapper::snapDist(const PF_Vector &coord)
{
    PF_Vector vec;

//std::cout<<" RS_Snapper::snapDist(RS_Vector coord): distance="<<distance<<std::endl;
    vec = container->getNearestDist(m_SnapDistance,
                                    coord,
                                    nullptr);
    return vec;
}

/**
 * @brief
 *
 * @param coord
 * @return PF_Vector
 */
PF_Vector PF_Snapper::snapIntersection(const PF_Vector &coord)
{
    PF_Vector vec{};

    vec = container->getNearestIntersection(coord,
                                            nullptr);
    return vec;
}

/**
 * @brief 捕捉距离pos最近的Entity
 *
 * @param pos
 * @param level
 * @return PF_Entity
 */
PF_Entity *PF_Snapper::catchEntity(const PF_Vector &pos, PF::ResolveLevel level)
{
    // set default distance for points inside solids
    double dist (0.);

    PF_Entity* entity = container->getNearestEntity(pos, &dist, level);

    int idx = -1;
//    if (entity && entity->getParent()) {
//        idx = entity->getParent()->findEntity(entity);
//    }

    if (entity && dist <= getSnapRange()) {
        // highlight:
        return entity;
    } else {
        return nullptr;
    }
}

/**
 * @brief
 *
 * @param e
 * @param level
 * @return PF_Entity
 */
PF_Entity *PF_Snapper::catchEntity(QMouseEvent *e, PF::ResolveLevel level)
{
    return catchEntity(
               PF_Vector(view->toGraphX(e->x()),
                         view->toGraphY(e->y())),
                         level);
}

/**
 * @brief
 *
 * @param pos
 * @param enType
 * @param level
 * @return PF_Entity
 */
PF_Entity *PF_Snapper::catchEntity(const PF_Vector &pos, PF::EntityType enType, PF::ResolveLevel level)
{
    return nullptr;
}

/**
 * @brief
 *
 * @param e
 * @param enType
 * @param level
 * @return PF_Entity
 */
PF_Entity *PF_Snapper::catchEntity(QMouseEvent *e, PF::EntityType enType, PF::ResolveLevel level)
{
    return nullptr;
}

/**
 * @brief
 *
 * @param e
 * @param enTypeList
 * @param level
 * @return PF_Entity
 */
PF_Entity *PF_Snapper::catchEntity(QMouseEvent *e, const std::initializer_list<PF::EntityType> &enTypeList, PF::ResolveLevel level)
{
    return nullptr;
}

void PF_Snapper::setSnapMode(const PF_SnapMode &snapMode)
{
    this->snapMode = snapMode;
}

const PF_SnapMode *PF_Snapper::getSnapMode() const
{
    return &(this->snapMode);
}

PF_SnapMode *PF_Snapper::getSnapMode()
{
    return &(this->snapMode);
}

void PF_Snapper::suspend()
{

}

void PF_Snapper::resume()
{

}

void PF_Snapper::hideOptions()
{

}

void PF_Snapper::showOptions()
{

}

/**
 * @brief
 *
 */
void PF_Snapper::drawSnapper()
{

}

double PF_Snapper::getSnapRange() const
{
    return snapRange;
}

const PF_SnapMode& PF_SnapMode::clear()
{
    snapFree         = false;
    snapGrid         = false;
    snapEndpoint     = false;
    snapMiddle       = false;
    snapDistance     = false;
    snapCenter       = false;
    snapOnEntity     = false;
    snapIntersection = false;

//	restriction = RS2::RestrictNothing;

    return *this;
}

bool PF_SnapMode::operator ==(PF_SnapMode const& rhs) const{
    if ( snapFree != rhs.snapFree)                 return false;
    if ( snapGrid != rhs.snapGrid)                 return false;
    if ( snapEndpoint != rhs.snapEndpoint)         return false;
    if ( snapMiddle != rhs.snapMiddle)             return false;
    if ( snapDistance != rhs.snapDistance)         return false;
    if ( snapCenter != rhs.snapCenter)             return false;
    if ( snapOnEntity != rhs.snapOnEntity)         return false;
    if ( snapIntersection != rhs.snapIntersection) return false;
//	if ( restriction != rhs.restriction) return false;
    return true;
}
