#ifndef PF_ENTITY_H
#define PF_ENTITY_H

#include "pf.h"
#include "pf_vector.h"

#include "pf_plot.h"
//2018-02-11
//by Poofee
/**实体类，基类**/
class QPainter;
class PF_GraphicView;
class PF_Vector;
class PF_EntityContainer;

class PF_Entity : public QCPLayerable
{
public:
    PF_Entity()=default;
    PF_Entity(PF_EntityContainer*parent, PF_GraphicView *plot);
    virtual ~PF_Entity()=default;

    void init();

    virtual bool isContainer() const = 0;/**纯虚函数**/
    virtual bool isAtomic() const = 0;

    virtual PF_Vector getStartpoint() const;
    virtual PF_Vector getEndpoint() const;

    /**继承的非虚函数**/
    virtual QRect clipRect() const;

    /**继承的虚函数**/
    virtual void applyDefaultAntialiasingHint(QCPPainter *painter) const Q_DECL_OVERRIDE;
    virtual void draw(QCPPainter *painter) Q_DECL_OVERRIDE = 0;

    //virtual void draw(QPainter* painter, PF_GraphicView* view)=0;
protected:
    PF_EntityContainer* parent = nullptr;
};

#endif // PF_ENTITY_H
