#ifndef PF_LINE_H
#define PF_LINE_H

#include "pf_atomicentity.h"

#include "pf_point.h"

struct PF_LineData{
    PF_LineData()=default;
    PF_LineData(PF_Point* startpoint,PF_Point *endpoint);
    PF_LineData(const PF_LineData& data);
    PF_Point* startpoint;
    PF_Point* endpoint;
};

class PF_Line : public PF_AtomicEntity
{
public:
    PF_Line()=default;
    PF_Line(PF_EntityContainer* parent,PF_GraphicView *view, const PF_LineData& d);
    PF_Line(PF_EntityContainer* parent,PF_GraphicView *view, PF_Point* pStart, PF_Point* pEnd);

    /**	@return PF::EntityLine */
    PF::EntityType rtti() const override{
        return PF::EntityLine;
    }
    /** @return Start point of the entity */
    PF_Vector getStartpoint() const override{
        return data.startpoint->getCenter();
    }
    /** @return End point of the entity */
    PF_Vector getEndpoint() const override{
        return data.endpoint->getCenter();
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
        return data.startpoint->getCenter().angleTo(data.endpoint->getCenter());
    }

    /**
     * @return The angle of the line (from end to startpoint).
     */
    double getAngle2() const{
        return data.endpoint->getCenter().angleTo(data.startpoint->getCenter());
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

    void draw(QCPPainter *painter) override;

    void calculateBorders() override;

    QString toGeoString() override;

    int index() const override;
public:
    static int line_index;
public:
    PF_LineData data;
    int m_index;

};

#endif // PF_LINE_H
