#include "pf_entitycontainer.h"

PF_EntityContainer::PF_EntityContainer(PF_EntityContainer *parent, bool owner)
    :PF_Entity(parent)
{
    autoDelete = owner;
}

PF_EntityContainer::~PF_EntityContainer()
{
    if(autoDelete){
        while(!entities.isEmpty()){
            delete entities.takeFirst();
        }
    }else{
        entities.clear();
    }
}
