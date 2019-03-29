#ifndef PF_ENTITYCONTAINER_H
#define PF_ENTITYCONTAINER_H

#include "pf_entity.h"
#include <QList>

//2018-02-15
//by Poofee
/**该类实现entity的组合功能，也就是一个数组列表**/

class PF_EntityContainer: public PF_Entity
{
public:
    PF_EntityContainer(PF_EntityContainer * parent=nullptr, PF_GraphicView* view=nullptr, bool owner=true);
    ~PF_EntityContainer() override;

    virtual void clear();

    bool isAtomic() const override{
        return false;
    }
    bool isContainer() const override{
        return true;
    }

    void setOwner(bool owner) {autoDelete=owner;}

    PF_Entity* first() const;
    PF_Entity* last() const;

    /**一系列对Entity的操作**/
    virtual void addEntity(PF_Entity* entity);
    virtual void appendEntity(PF_Entity* entity);
    virtual void prependEntity(PF_Entity* entity);
    virtual void moveEntity(int index, QList<PF_Entity *>& entList);
    virtual void insertEntity(int index, PF_Entity* entity);
    virtual bool removeEntity(PF_Entity* entity);

    virtual void addRectangle(PF_Vector const& corner1, PF_Vector const & corner2);
    void draw(QCPPainter *painter) override;

    const QList<PF_Entity *> &getEntityList();
protected:
    QList<PF_Entity*> entities;/**保存所有实体**/
private:
    bool autoDelete;
};

#endif // PF_ENTITYCONTAINER_H
