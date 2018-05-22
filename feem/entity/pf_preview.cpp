#include "pf_preview.h"

PF_Preview::PF_Preview(PF_EntityContainer *parent)
    :PF_EntityContainer(parent)
{

}

void PF_Preview::addEntity(PF_Entity *entity)
{
    if(!entity){
        return;
    }
    PF_EntityContainer::addEntity(entity);
}

void PF_Preview::draw(QPainter *painter, PF_GraphicView *view)
{
    if(!(painter && view)){
        return;
    }

    for(int i=0;i < entities.size();++i){
        entities.at(i)->draw(painter,view);
    }
}
