#ifndef PF_ENTITY_H
#define PF_ENTITY_H

#include "pf_vector.h"
//2018-02-11
//by Poofee
//实体类，基类
class QPainter;
class PF_GraphicView;
class PF_Vector;

class PF_Entity
{
public:
    PF_Entity()=default;
    virtual ~PF_Entity()=default;

    virtual void draw(QPainter* painter, PF_GraphicView* view)=0;
};

#endif // PF_ENTITY_H
