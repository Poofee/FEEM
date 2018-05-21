#ifndef PF_SNAPPER_H
#define PF_SNAPPER_H

#include "pf.h"

class PF_Entity;
class PF_GraphicView;
class PF_Vector;
class QMouseEvent;
class PF_EntityContainer;

//2018-02-11
//by Poofee
//该类用于实现实体的捕捉
class PF_Snapper
{
public:
    PF_Snapper()=delete;
    PF_Snapper(PF_EntityContainer*container,PF_GraphicView* view);
    virtual ~PF_Snapper();

    void init();
    void finish();

    virtual void suspend();

    virtual void resume();
    virtual void hideOptions();
    virtual void showOptions();
protected:
    PF_GraphicView* view;
    PF_EntityContainer* container;
};

#endif // PF_SNAPPER_H
