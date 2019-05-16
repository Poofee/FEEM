#ifndef PF_POINT_H
#define PF_POINT_H

#include "pf_atomicentity.h"

//point data
struct PF_PointData{
    PF_PointData(const PF_Vector & pos): pos(pos){}

    PF_Vector pos;
};

//point entity
class PF_Point : public PF_AtomicEntity
{
public:
    PF_Point(PF_EntityContainer* parent, PF_GraphicView* view, const PF_PointData & d);


    PF_Vector getCenter() const override;
    double getRadius() const override;
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
//    PF_Vector getNearestDist(double distance,
//                             bool startp)const override;
//    PF_Vector getNearestOrthTan(const PF_Vector& coord,
//                                const PF_Line& normal,
//                                bool onEntity = false) const override;

//    bool offset(const PF_Vector& coord, const double& distance) override;
    //	PF_VectorSolutions getTangentPoint(const PF_Vector& point) const override;//find the tangential points seeing from given point
    //	PF_Vector getTangentDirection(const PF_Vector& point)const override;
    void move(const PF_Vector& offset) override;
    void rotate(const PF_Vector& center, const double& angle) override;
    void rotate(const PF_Vector& center, const PF_Vector& angleVector) override;
    void scale(const PF_Vector& center, const PF_Vector& factor) override;
    void mirror(const PF_Vector& axisPoint1, const PF_Vector& axisPoint2) override;
//    void moveRef(const PF_Vector& ref, const PF_Vector& offset) override;

    void draw(QCPPainter* painter) override;

    void calculateBorders() override;

    QString toString() const;
public:
    static int point_index;
protected:
    PF_PointData data;
    int m_index;
};

#endif // PF_POINT_H
