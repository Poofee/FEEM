#ifndef PF_ENTITY_H
#define PF_ENTITY_H

#include "pf.h"
#include "pf_vector.h"
//2018-02-11
//by Poofee
//实体类，基类
class QPainter;
class PF_GraphicView;
class PF_Vector;
class PF_EntityContainer;

class PF_Entity
{
public:
    PF_Entity()=default;
    PF_Entity(PF_EntityContainer*parent=nullptr);
    virtual ~PF_Entity()=default;

    void init();

    virtual bool isContainer() const = 0;//纯虚函数
    virtual bool isAtomic() const = 0;

    virtual void draw(QPainter* painter, PF_GraphicView* view)=0;
protected:
    PF_EntityContainer* parent = nullptr;
};

#endif // PF_ENTITY_H
