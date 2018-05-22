#include "pf_mdisubwindow.h"

#include <QMdiArea>

#include "pf_graphicview.h"
#include "pf_document.h"

PF_MdiSubWindow::PF_MdiSubWindow(PF_Document* doc, QWidget* parent)
    :QMdiSubWindow(parent)
{
    if(doc == nullptr){

    }else{
        document = doc;
    }
    graphicView = new PF_GraphicView(document,this);

    setWidget(graphicView);

    setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
}

PF_MdiSubWindow::~PF_MdiSubWindow(){

}

PF_GraphicView *PF_MdiSubWindow::getGraphicView() const
{
    return (graphicView) ? graphicView : nullptr;
}

PF_Document *PF_MdiSubWindow::getDocument() const
{
    return document;
}

