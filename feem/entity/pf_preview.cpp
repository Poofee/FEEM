#include "pf_preview.h"
#include <QDebug>

PF_Preview::PF_Preview(PF_EntityContainer *parent)
    :PF_EntityContainer(parent,true)
{

}

void PF_Preview::addEntity(PF_Entity *entity)
{
    if(!entity){
        return;
    }
    PF_EntityContainer::addEntity(entity);
    qDebug()<<"PF_Preview::addEntity: OK.";
}

void PF_Preview::draw(QPainter *painter, PF_GraphicView *view)
{
    qDebug()<<"PF_Preview::draw";
    if(!(painter && view)){
        return;
    }

    for(int i=0;i < entities.size();++i){
        entities.at(i)->draw(painter,view);
    }
    qDebug()<<"PF_Preview::draw: OK.";
}
