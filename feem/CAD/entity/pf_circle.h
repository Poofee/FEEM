#ifndef PF_CIRCLE_H
#define PF_CIRCLE_H

#include "pf_atomicentity.h"
//2018-02-11
//by Poofee
/**圆**/

struct PF_CircleData
{
    PF_CircleData()=default;
    PF_CircleData(PF_Vector* center, double radius);

    PF_Vector center;
    double radius;
};

class PF_Circle: public PF_AtomicEntity
{
public:
    PF_Circle()=default;
    PF_Circle(PF_EntityContainer* parent, PF_GraphicView* view, const PF_CircleData &d);
    ~PF_Circle()=default;

    /** @return The center point (x) of this arc */
    PF_Vector getCenter() const override;
    /** Sets new center. */
    void setCenter(const PF_Vector& c);
    /** @return The radius of this arc */
    double getRadius() const override;
    /** Sets new radius. */
    void setRadius(double r);

    /** 继承的虚函数 **/
    PF_VectorSolutions getRefPoints() const override;

    PF_Vector getMiddlePoint(void)const override;
    PF_Vector getNearestEndpoint(const PF_Vector& coord,
                                 double* dist = nullptr) const override;
    PF_Vector getNearestPointOnEntity(const PF_Vector& coord,
                                      bool onEntity = true, double* dist = nullptr, PF_Entity** entity=nullptr)const override;
    PF_Vector getNearestCenter(const PF_Vector& coord,
                               double* dist = nullptr)const override;
    PF_Vector getNearestMiddle(const PF_Vector& coord,
                               double* dist = nullptr,
                               int middlePoints = 1 ) const override;
    PF_Vector getNearestDist(double distance,
                             const PF_Vector& coord,
                             double* dist = nullptr)const override;
    PF_Vector getNearestDist(double distance,
                             bool startp)const override;
    PF_Vector getNearestOrthTan(const PF_Vector& coord,
                                const PF_Line& normal,
                                bool onEntity = false) const override;

    bool offset(const PF_Vector& coord, const double& distance) override;
    //	PF_VectorSolutions getTangentPoint(const PF_Vector& point) const override;//find the tangential points seeing from given point
    //	PF_Vector getTangentDirection(const PF_Vector& point)const override;
    void move(const PF_Vector& offset) override;
    void rotate(const PF_Vector& center, const double& angle) override;
    void rotate(const PF_Vector& center, const PF_Vector& angleVector) override;
    void scale(const PF_Vector& center, const PF_Vector& factor) override;
    void mirror(const PF_Vector& axisPoint1, const PF_Vector& axisPoint2) override;
    void moveRef(const PF_Vector& ref, const PF_Vector& offset) override;

    void draw(QCPPainter* painter) ;

    void calculateBorders() override;
protected:
    PF_CircleData data;
};

#endif // PF_CIRCLE_H
