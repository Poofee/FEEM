#include "pf_actionpreviewinterface.h"
#include "pf_graphicview.h"
#include <QDebug>

PF_ActionPreviewInterface::PF_ActionPreviewInterface(const char*name, PF_EntityContainer *container, PF_GraphicView *view)
    :PF_ActionInterface(name,container,view)
    ,preview(new PF_Preview(container))
{
    hasPreview = true;
}

PF_ActionPreviewInterface::~PF_ActionPreviewInterface()
{
    deletePreview();
}

void PF_ActionPreviewInterface::init(int status)
{
    deletePreview();
    PF_ActionInterface::init(status);
}

void PF_ActionPreviewInterface::finish()
{
    deletePreview();
    PF_ActionInterface::finish();
}

void PF_ActionPreviewInterface::resume()
{
    PF_ActionInterface::resume();
    deletePreview();
}

void PF_ActionPreviewInterface::suspend()
{
    PF_ActionInterface::suspend();
    deletePreview();
}

void PF_ActionPreviewInterface::trigger()
{
    PF_ActionInterface::trigger();
    deletePreview();
}

void PF_ActionPreviewInterface::drawPreview()
{
    /**获取graphicview中保存preview的entities列表**/
    PF_EntityContainer *ctainer=view->getOverlayContainer(PF::ActionPreviewEntity);
    ctainer->clear();
    /**preview是ctainer中的一个，不能设为自动删除释放**/
    ctainer->setOwner(false); // Little hack for now so we don't delete the preview twice
    ctainer->addEntity(preview);
    //view->redraw(PF::RedrawOverlay);
    view->replot();
    hasPreview=true;
}

void PF_ActionPreviewInterface::deletePreview()
{
    //qDebug()<<"PF_ActionPreviewInterface::deletePreview";
    //if(hasPreview){
        preview->clear();
        hasPreview = false;
        //qDebug()<<"PF_ActionPreviewInterface::deletePreview: OK.";
    //}

}
