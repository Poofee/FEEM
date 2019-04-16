#ifndef PF_SNAPPER_H
#define PF_SNAPPER_H

#include <memory>

#include "pf.h"

class PF_Entity;
class PF_GraphicView;
class PF_Vector;
class QMouseEvent;
class PF_EntityContainer;

/**
  * 20190414
  * 保存如何捕捉鼠标信息。
  *
  * @author Kevin Cox
  */
struct PF_SnapMode {
    bool snapFree           = false; /** 是否自由捕捉 **/
    bool snapGrid           = false; /** 是否捕捉网格 **/
    bool snapEndpoint       = false; /** 是否捕捉端点 **/
    bool snapMiddle         = false; /** 是否捕捉中点 **/
    bool snapDistance       = false; /** 是否捕捉到端点的距离 **/
    bool snapCenter         = false; /** 是否捕捉中心点 **/
    bool snapIntersection   = false; /** 是否捕捉交叉点 **/

    bool snapOnEntity       = false; /** 是否捕捉实体上的点 **/

    //    RS2::SnapRestriction restriction= RS2::RestrictNothing; /// The restriction on the free snap.

    double distance         = 5.; /** 默认捕捉距离 **/

    /**
  * 取消所有捕捉
  *
  * 进入自由捕捉状态
  *
  * @returns A reference to itself.
  */
    PF_SnapMode const & clear(void);
    bool operator == (PF_SnapMode const& rhs) const;
};

//2018-02-11
//by Poofee
/**该类用于实现实体的捕捉**/
class PF_Snapper
{
public:
    PF_Snapper()=delete;
    PF_Snapper(PF_EntityContainer*container,PF_GraphicView* view);
    virtual ~PF_Snapper();

    void init();
    void finish();

    PF_Entity* getKeyEntity() const {
        return keyEntity;
    }

    PF_Vector snapPoint(const PF_Vector& coord, bool setSpot = false);
    PF_Vector snapPoint(QMouseEvent* e);
    PF_Vector snapFree(QMouseEvent* e);

    PF_Vector snapFree(const PF_Vector& coord);
    PF_Vector snapGrid(const PF_Vector& coord);
    PF_Vector snapEndpoint(const PF_Vector& coord);
    PF_Vector snapOnEntity(const PF_Vector& coord);
    PF_Vector snapCenter(const PF_Vector& coord);
    PF_Vector snapMiddle(const PF_Vector& coord);
    PF_Vector snapDist(const PF_Vector& coord);
    PF_Vector snapIntersection(const PF_Vector& coord);

    PF_Entity* catchEntity(const PF_Vector& pos,
                           PF::ResolveLevel level=PF::ResolveNone);
    PF_Entity* catchEntity(QMouseEvent* e,
                           PF::ResolveLevel level=PF::ResolveNone);
    // catch Entity closest to pos and of the given entity type of enType, only search for a particular entity type
    PF_Entity* catchEntity(const PF_Vector& pos, PF::EntityType enType,
                           PF::ResolveLevel level=PF::ResolveNone);
    PF_Entity* catchEntity(QMouseEvent* e, PF::EntityType enType,
                           PF::ResolveLevel level=PF::ResolveNone);
    PF_Entity* catchEntity(QMouseEvent* e, const std::initializer_list<PF::EntityType>& enTypeList,
                           PF::ResolveLevel level=PF::ResolveNone);

    void setSnapRange(int r) {
        snapRange = r;
    }

    void setSnapMode(const PF_SnapMode& snapMode);
    PF_SnapMode const* getSnapMode() const;
    PF_SnapMode* getSnapMode();

    virtual void suspend();

    virtual void resume();
    virtual void hideOptions();
    virtual void showOptions();

    void drawSnapper();

    double getSnapRange() const;
protected:
    PF_GraphicView* view;
    PF_EntityContainer* container;
    PF_Entity* keyEntity;
    PF_SnapMode snapMode;
    bool finished{false};

    /**
     * Snap distance for snapping to points with a
     * given distance from endpoints.
     */
    double m_SnapDistance;
    /**
     * Snap to equidistant middle points
     * default to 1, i.e., equidistant to start/end points
     */
    int middlePoints;
    /**
     * 捕捉范围，单位是像素点
     */
    int snapRange;
private:
    struct ImpData;
    std::unique_ptr<ImpData> pImpData;

    struct Indicator;
    Indicator* snap_indicator{nullptr};
};

#endif // PF_SNAPPER_H
