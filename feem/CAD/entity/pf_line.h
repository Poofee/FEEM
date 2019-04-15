#ifndef PF_LINE_H
#define PF_LINE_H

#include "pf_atomicentity.h"

struct PF_LineData{
    PF_LineData()=default;
    PF_LineData(PF_Vector& startpoint,PF_Vector& endpoint);
    PF_Vector startpoint;
    PF_Vector endpoint;
};

class PF_Line : public PF_AtomicEntity
{
public:
    PF_Line()=default;
    PF_Line(PF_EntityContainer* parent,PF_GraphicView *view, const PF_LineData& d);
    PF_Line(PF_EntityContainer* parent,PF_GraphicView *view, const PF_Vector& pStart, const PF_Vector& pEnd);

    /** @return Start point of the entity */
    PF_Vector getStartpoint() const override{
        return data.startpoint;
    }
    /** @return End point of the entity */
    PF_Vector getEndpoint() const override{
        return data.endpoint;
    }
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

    void draw(QCPPainter *painter) ;
protected:
    PF_LineData data;
};

#endif // PF_LINE_H
