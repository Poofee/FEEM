#include "pf_atomicentity.h"

PF_AtomicEntity::PF_AtomicEntity(PF_EntityContainer* parent, PF_GraphicView *view)
    :PF_Entity(parent,view)
{

}

bool PF_AtomicEntity::isContainer() const
{
    return false;
}

bool PF_AtomicEntity::isAtomic() const
{
    return true;
}
