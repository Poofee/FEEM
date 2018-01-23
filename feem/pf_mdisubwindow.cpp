#include "pf_mdisubwindow.h"

#include <QMdiArea>

#include "pf_graphicview.h"

PF_MdiSubWindow::PF_MdiSubWindow(QWidget* parent)
    :QMdiSubWindow(parent)
{
    graphicView = new PF_GraphicView(this);

    setWidget(graphicView);

    setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
}

PF_MdiSubWindow::~PF_MdiSubWindow(){

}

