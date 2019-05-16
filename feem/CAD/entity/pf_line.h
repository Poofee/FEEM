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
    /**
     * @return Direction 1. The angle at which the line starts at
     * the startpoint.
     */
    double getDirection1() const override{
        return getAngle1();
    }
    /**
     * @return Direction 2. The angle at which the line starts at
     * the endpoint.
     */
    double getDirection2() const override{
        return getAngle2();
    }
    /**
     * @return The angle of the line (from start to endpoint).
     */
    double getAngle1() const{
        return data.startpoint.angleTo(data.endpoint);
    }

    /**
     * @return The angle of the line (from end to startpoint).
     */
    double getAngle2() const{
        return data.endpoint.angleTo(data.startpoint);
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

    bool offset(const PF_Vector& coord, const double& distance) override;
    //	PF_VectorSolutions getTangentPoint(const PF_Vector& point) const override;//find the tangential points seeing from given point
    //	PF_Vector getTangentDirection(const PF_Vector& point)const override;
    void move(const PF_Vector& offset) override;
    void rotate(const PF_Vector& center, const double& angle) override;
    void rotate(const PF_Vector& center, const PF_Vector& angleVector) override;
    void scale(const PF_Vector& factor) override;
    void scale(const PF_Vector& center, const PF_Vector& factor) override;
    void mirror(const PF_Vector& axisPoint1, const PF_Vector& axisPoint2) override;
    void moveRef(const PF_Vector& ref, const PF_Vector& offset) override;

    void draw(QCPPainter *painter) ;

    void calculateBorders() override;
public:
    static int line_index;
protected:
    PF_LineData data;
    int m_index;

};

#endif // PF_LINE_H
