#ifndef PF_ENTITY_H
#define PF_ENTITY_H

#include "pf.h"
#include "pf_vector.h"

#include "pf_plot.h"
#include "pf_flag.h"
//2018-02-11
//by Poofee
/**实体类，基类**/
class QPainter;
class PF_GraphicView;
class PF_Vector;
class PF_EntityContainer;
class PF_Line;

class PF_Entity : public QCPLayerable, public PF_Flag
{
public:
    PF_Entity()=default;
    PF_Entity(PF_EntityContainer*parent, PF_GraphicView *plot);
    virtual ~PF_Entity()=default;

    void init();

    virtual bool isContainer() const = 0;/**纯虚函数**/
    virtual bool isAtomic() const = 0;

    virtual PF_Vector getStartpoint() const;
    virtual PF_Vector getEndpoint() const;

    void setPen(const QPen& pen) {
        this->pen = pen;
    }
    QPen getPen(bool resolve = true) const;
    void setBrush(const QBrush& brush){
        this->brush = brush;
    }
    QBrush getBrush(bool resolve = true) const;

    virtual bool setSelected(bool select);
    virtual bool toggleSelected();
    virtual bool isSelected() const;
    bool isParentSelected() const;

    virtual bool isVisible() const;
    virtual void setVisible(bool v);



    /**继承的非虚函数**/
    virtual QRect clipRect() const override;

    /**继承的虚函数**/
    virtual void applyDefaultAntialiasingHint(QCPPainter *painter) const Q_DECL_OVERRIDE;
    virtual void draw(QCPPainter *painter) Q_DECL_OVERRIDE = 0;

    //virtual void draw(QPainter* painter, PF_GraphicView* view)=0;

    /** 一些点和距离的计算函数 **/
    virtual PF_VectorSolutions getRefPoints() const;

    /**
     * Must be overwritten to get the closest endpoint to the
     * given coordinate for this entity.
     *
     * @param coord Coordinate (typically a mouse coordinate)
     * @param dist Pointer to a value which will contain the measured
     * distance between 'coord' and the closest endpoint. The passed
     * pointer can also be nullptr in which case the distance will be
     * lost.
     *
     * @return The closest endpoint.
     */
    virtual PF_Vector getNearestEndpoint(const PF_Vector& coord,
                                         double* dist = nullptr)const = 0;

    /**
     * Must be overwritten to get the closest coordinate to the
    * given coordinate which is on this entity.
     *
     * @param coord Coordinate (typically a mouse coordinate)
     * @param dist Pointer to a value which will contain the measured
     * distance between \p coord and the point. The passed pointer can
     * also be \p nullptr in which case the distance will be lost.
     *
     * @return The closest coordinate.
     */
    virtual PF_Vector getNearestPointOnEntity(const PF_Vector& /*coord*/,
                                              bool onEntity = true, double* dist = nullptr,
                                              PF_Entity** entity = nullptr) const = 0;

    /**
     * Must be overwritten to get the (nearest) center point to the
     * given coordinate for this entity.
     *
     * @param coord Coordinate (typically a mouse coordinate)
     * @param dist Pointer to a value which will contain the measured
     * distance between 'coord' and the closest center point. The passed
     * pointer can also be nullptr in which case the distance will be
     * lost.
     *
     * @return The closest center point.
     */
    virtual PF_Vector getNearestCenter(const PF_Vector& coord,
                                       double* dist = nullptr) const= 0;

    /**
     * Must be overwritten to get the (nearest) middle point to the
     * given coordinate for this entity.
     *
     * @param coord Coordinate (typically a mouse coordinate)
     * @param dist Pointer to a value which will contain the measured
     * distance between 'coord' and the closest middle point. The passed
     * pointer can also be nullptr in which case the distance will be
     * lost.
     *
     * @return The closest middle point.
     */
    virtual PF_Vector getMiddlePoint(void)const{
        return PF_Vector(false);
    }
    virtual PF_Vector getNearestMiddle(const PF_Vector& coord,
                                       double* dist = nullptr,
                                       int middlePoints = 1
            ) const= 0;

    /**
     * Must be overwritten to get the nearest point with a given
     * distance to the endpoint to the given coordinate for this entity.
     *
     * @param distance Distance to endpoint.
     * @param coord Coordinate (typically a mouse coordinate)
     * @param dist Pointer to a value which will contain the measured
     * distance between 'coord' and the closest point. The passed
     * pointer can also be nullptr in which case the distance will be
     * lost.
     *
     * @return The closest point with the given distance to the endpoint.
     */
    virtual PF_Vector getNearestDist(double distance,
                                     const PF_Vector& coord,
                                     double* dist = nullptr) const= 0;

    /**
     * Must be overwritten to get the point with a given
     * distance to the start- or endpoint to the given coordinate for this entity.
     *
     * @param distance Distance to endpoint.
     * @param startp true = measured from Startpoint, false = measured from Endpoint
     *
     * @return The point with the given distance to the start- or endpoint.
     */
    virtual PF_Vector getNearestDist(double /*distance*/,
                                     bool /*startp*/) const{
        return PF_Vector(false);
    }

    /**
     * Must be overwritten to get the nearest reference point for this entity.
     *
     * @param coord Coordinate (typically a mouse coordinate)
     * @param dist Pointer to a value which will contain the measured
     * distance between 'coord' and the closest point. The passed
     * pointer can also be nullptr in which case the distance will be
     * lost.
     *
     * @return The closest point with the given distance to the endpoint.
     */
    virtual PF_Vector getNearestRef(const PF_Vector& coord,
                                    double* dist = nullptr) const;

    /**
     * Gets the nearest reference point of this entity if it is selected.
         * Containers re-implement this method to return the nearest reference
         * point of a selected sub entity.
     *
     * @param coord Coordinate (typically a mouse coordinate)
     * @param dist Pointer to a value which will contain the measured
     * distance between 'coord' and the closest point. The passed
     * pointer can also be nullptr in which case the distance will be
     * lost.
     *
     * @return The closest point with the given distance to the endpoint.
     */
    virtual PF_Vector getNearestSelectedRef(const PF_Vector& coord,
                                            double* dist = nullptr) const;

    /**
     * Must be overwritten to get the shortest distance between this
     * entity and a coordinate.
     *
     * @param coord Coordinate (typically a mouse coordinate)
     * @param entity Pointer which will contain the (sub-)entity which is
     *               closest to the given point or nullptr if the caller is not
     *               interested in this information.
     * @param level The resolve level.
     *
     * @sa RS2::ResolveLevel
     *
     * @return The measured distance between \p coord and the entity.
     */
    virtual PF_Vector getNearestOrthTan(const PF_Vector& /*coord*/,
                                        const PF_Line& /*normal*/,
                                        bool onEntity = false) const;
    virtual double getDistanceToPoint(const PF_Vector& coord,
                                      PF_Entity** entity = nullptr,
                                      PF::ResolveLevel level = PF::ResolveNone,
                                      double solidDist = PF_MAXDOUBLE) const;

    virtual bool isPointOnEntity(const PF_Vector& coord,
                                 double tolerance=20.*PF_TOLERANCE) const;

    /**
     * Implementations must offset the entity by the given direction and distance.
     */
    virtual bool offset(const PF_Vector& /*coord*/, const double& /*distance*/) {return false;}

    /**
     * Implementations must offset the entity by the distance at both directions
     * used to generate tangential circles
     */
    virtual std::vector<PF_Entity* > offsetTwoSides(const double& /*distance*/) const
    {
        return std::vector<PF_Entity* >();
    }
    /**
    * implementations must revert the direction of an atomic entity
    */
    virtual void revertDirection(){}
    /**
     * Implementations must move the entity by the given vector.
     */
    virtual void move(const PF_Vector& offset) = 0;

    /**
     * Implementations must rotate the entity by the given angle around
     * the given center.
     */
    virtual void rotate(const PF_Vector& center, const double& angle) = 0;
    virtual void rotate(const PF_Vector& center, const PF_Vector& angleVector) = 0;

    /**
     * Implementations must scale the entity by the given factors.
     */
    virtual void scale(const PF_Vector& center, const PF_Vector& factor) = 0;

    /**
     * Acts like scale(PF_Vector) but with equal factors.
     * Equal to scale(center, PF_Vector(factor, factor)).
     */
    virtual void scale(const PF_Vector& center, const double& factor) {
        scale(center, PF_Vector(factor, factor));
    }
    virtual void scale(const PF_Vector& factor) {
        scale(PF_Vector(0.,0.), factor);
    }
    /**
     * Implementations must mirror the entity by the given axis.
     */
    virtual void mirror(const PF_Vector& axisPoint1, const PF_Vector& axisPoint2) = 0;

    virtual void stretch(const PF_Vector& firstCorner,
                         const PF_Vector& secondCorner,
                         const PF_Vector& offset);

    /**
         * Implementations must drag the reference point(s) of all
         * (sub-)entities that are very close to ref by offset.
         */
    virtual void moveRef(const PF_Vector& /*ref*/,
                         const PF_Vector& /*offset*/) {
        return;
    }

    /**
         * Implementations must drag the reference point(s) of selected
         * (sub-)entities that are very close to ref by offset.
         */
    virtual void moveSelectedRef(const PF_Vector& /*ref*/,
                                 const PF_Vector& /*offset*/) {
        return;
    }
protected:
    PF_EntityContainer* parent = nullptr;
    QPen pen;
    QBrush brush;
    PF_Vector minV;
    PF_Vector maxV;
};

#endif // PF_ENTITY_H
