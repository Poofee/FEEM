#include "pf_entitycontainer.h"
#include "pf_graphicview.h"
#include <QDebug>

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

void PF_EntityContainer::clear()
{
    if(autoDelete){
        while(!entities.isEmpty()){
            delete entities.takeFirst();
        }
    }else{
        entities.clear();
    }
}

void PF_EntityContainer::addEntity(PF_Entity *entity)
{
    if(!entity){
        return;
    }

    entities.append(entity);
    qDebug()<<"PF_EntityContainer::addEntity:size:"<<entities.size();
}

void PF_EntityContainer::draw(QPainter *painter, PF_GraphicView *view)
{
    if(!(painter && view)){
        return;
    }

    for(int i=0;i < entities.size();++i){
        view->drawEntity(painter,entities.at(i));
    }
}
