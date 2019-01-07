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
    PF_Circle(PF_EntityContainer* parent, const PF_CircleData &d);
    ~PF_Circle()=default;

    PF_Vector getCenter() const;
    double getRadius() const;
    void draw(QPainter* painter, PF_GraphicView* view) override;
protected:
    PF_CircleData data;
};

#endif // PF_CIRCLE_H
