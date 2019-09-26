#ifndef PF_FACE_H
#define PF_FACE_H

#include "pf_atomicentity.h"

class QPainterPath;
class QPolygonF;
class PF_Line;
/*!
 \brief 保存闭合的曲线数据

*/
class PF_LineLoop{
public:
    PF_LineLoop();

    int index() const;
    static int lineloop_index;
    QList<PF_Line* > lines;/** 保存对应的点的编号 **/
    QPolygonF loop;/** 保存所有的点，闭合模式 **/
    int m_index;
};

/*!
 \brief 保存面的数据，在这里，面是由一些闭合
 多边形所构成的图形。一般的面可以用一个闭合多
边形来表示，对于某些有孔或洞的面，采用多个闭合
多边形来表示。

*/
class PF_FaceData{
public:
    PF_FaceData()=default;

    PF_FaceData(const PF_FaceData& data);

    PF_FaceData& operator=(const PF_FaceData& data);

    QList<PF_LineLoop* > faceData;
};

/*!
 \brief 产生绘制面的效果

*/
class PF_Face : public PF_AtomicEntity
{
public:
    PF_Face()=default;
    PF_Face(PF_EntityContainer* parent, PF_GraphicView* view, const PF_FaceData &d);
    PF_Face(PF_EntityContainer* parent, PF_GraphicView* view, const PF_FaceData &d,PF_Line* mouse);
    ~PF_Face()=default;

    /**	@return PF::EntityFace */
    PF::EntityType rtti() const override{
        return PF::EntityFace;
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

    void draw(QCPPainter* painter) override;

    void calculateBorders() override;

    QString toGeoString() override;
    int index() const override;

    static int face_index;
protected:
    PF_FaceData data;
    int m_index;
};

#endif // PF_FACE_H
