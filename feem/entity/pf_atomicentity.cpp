#include "pf_atomicentity.h"

PF_AtomicEntity::PF_AtomicEntity(PF_EntityContainer* parent)
    :PF_Entity(parent)
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
