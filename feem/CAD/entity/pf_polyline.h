#ifndef PF_POLYLINE_H
#define PF_POLYLINE_H

#include "pf_entitycontainer.h"

/**
 * Holds the data that defines a polyline.
 */
struct PF_PolylineData  {
    PF_PolylineData();
    ~PF_PolylineData()=default;
    PF_PolylineData(const PF_Vector& startpoint,
                    const PF_Vector& endpoint,
                    bool closed);

    PF_Vector startPoint;
    PF_Vector endPoint;
    bool isClosed;
};


/**
 * @brief 绘制多边形的类，绘制原理是绘制一系列的线段或者弧线
 *
 */
class PF_Polyline : public PF_EntityContainer
{
public:
    PF_Polyline() = default;
    PF_Polyline(PF_EntityContainer* parent, PF_GraphicView* view);
    PF_Polyline(PF_EntityContainer* parent, PF_GraphicView* view, const PF_PolylineData &d);
    ~PF_Polyline() = default;

    /** @return Copy of data that defines the polyline. */
    PF_PolylineData getData() const {
        return data;
    }

    /** sets a new start point of the polyline */
    void setStartpoint(PF_Vector const& v);

    /** @return Start point of the entity */
    PF_Vector getStartpoint() const;

    /** sets a new end point of the polyline */
    void setEndpoint(PF_Vector const& v);

    /** @return End point of the entity */
    PF_Vector getEndpoint() const;

    double getClosingBulge() const;

    void updateEndpoints();

    /** @return true if the polyline is closed. false otherwise */
    bool isClosed() const;

    void setClosed(bool cl);

    void setClosed(bool cl, double bulge);//RLZ: rewrite this:

    PF_Entity* addVertex(const PF_Vector& v,
                         double bulge=0.0, bool prepend=false);

    void appendVertexs(const std::vector< std::pair<PF_Vector, double> >& vl);

    void setNextBulge(double bulge) {
        nextBulge = bulge;
    }
    void addEntity(PF_Entity* entity) override;
    //void addSegment(RS_Entity* entity) override;
    void removeLastVertex();
    void endPolyline();

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

    void draw(QCPPainter * p) override;
protected:
    PF_Entity* createVertex(const PF_Vector& v,
                double bulge=0.0, bool prepend=false);
protected:
    PF_PolylineData data;
    PF_Entity* closingEntity;
    double nextBulge;
};

#endif // PF_POLYLINE_H
