#include <QMouseEvent>
#include "pf_snapper.h"
#include "pf_graphicview.h"
#include "pf_entitycontainer.h"

PF_Snapper::PF_Snapper(PF_EntityContainer *container, PF_GraphicView *view)
    :view(view)
    ,container(container)
{

}

PF_Snapper::~PF_Snapper()
{

}

void PF_Snapper::init()
{

}

void PF_Snapper::finish()
{

}

PF_Vector PF_Snapper::snapPoint(QMouseEvent *e)
{
    return PF_Vector(e->x(),e->y(),0);
}

void PF_Snapper::suspend()
{

}

void PF_Snapper::resume()
{

}

void PF_Snapper::hideOptions()
{

}

void PF_Snapper::showOptions()
{

}
