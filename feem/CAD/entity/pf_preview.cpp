#include "pf_preview.h"
#include <QDebug>

PF_Preview::PF_Preview(PF_EntityContainer *parent, PF_GraphicView *view)
    :PF_EntityContainer(parent,view,true)
{

}

void PF_Preview::addEntity(PF_Entity *entity)
{
    if(!entity){
        return;
    }
    PF_EntityContainer::addEntity(entity);
    //qDebug()<<"PF_Preview::addEntity: OK.";
}

void PF_Preview::draw(QCPPainter *painter)
{
    //qDebug()<<"PF_Preview::draw";
    if(!(painter && mParentPlot)){
        qDebug()<<Q_FUNC_INFO;
        return;
    }
    painter->save();
    painter->setPen(QColor(Qt::red));
    for(int i=0;i < entities.size();++i){
        entities.at(i)->toggleHighlighted();
        entities.at(i)->draw(painter);
    }
    painter->restore();
    //qDebug()<<"PF_Preview::draw: OK.";
}

QString PF_Preview::toGeoString()
{
    return "";
}

int PF_Preview::index() const
{
    return 0;
}
