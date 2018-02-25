#ifndef PF_ENTITYCONTAINER_H
#define PF_ENTITYCONTAINER_H

#include "pf_entity.h"
#include <QList>

//2018-02-15
//by Poofee
//该类实现entity的组合功能，也就是一个数组列表

class PF_EntityContainer: public PF_Entity
{
public:
    PF_EntityContainer();

protected:
    QList<PF_Entity*> entities;//保存所有实体
};

#endif // PF_ENTITYCONTAINER_H
