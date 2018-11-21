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
    PF_Point(PF_EntityContainer* parent, const PF_PointData & d);

    void draw(QPainter* painter, PF_GraphicView* view) override;

protected:
    PF_PointData data;
};

#endif // PF_POINT_H
