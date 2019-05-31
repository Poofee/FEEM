#include "pf_actiondrawpoint.h"
#include <QAction>
#include <QMouseEvent>
#include "pf_point.h"
#include "pf_graphicview.h"

#include "pf_cadwidget.h"

#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QToolButton>

PF_ActionDrawPoint::PF_ActionDrawPoint(PF_EntityContainer *parent, PF_GraphicView *view)
    :PF_ActionPreviewInterface("Draw Point",parent,view)
    ,pt(new PF_Vector())
{
    actionType = PF::ActionDrawPoint;

    QDialog* dialog = new QDialog(view);
    QFormLayout* layout = new QFormLayout();
    textx = new QLineEdit();
    texty = new QLineEdit();

    QToolButton* bt_OK = new QToolButton();
    bt_OK->setText(tr("Add point"));
    connect(bt_OK,&QToolButton::clicked,[this](){
        *pt = {textx->text().toDouble(),texty->text().toDouble()};
        this->trigger();
    });
    QToolButton* bt_Cancel = new QToolButton();
    bt_Cancel->setText(tr("Cancel"));
    connect(bt_Cancel,&QToolButton::clicked,[dialog,this](){
        this->init(getStatus()-1);
        PF_CADWidget::statusbar->clearMessage();
        dialog->reject();
    });

    layout->addRow(tr("position x:"),textx);
    layout->addRow(tr("position y:"),texty);
    QHBoxLayout* hlayout = new QHBoxLayout();
    hlayout->addStretch();
    hlayout->addWidget(bt_OK);
    hlayout->addWidget(bt_Cancel);
    layout->addRow("",hlayout);
    layout->setSpacing(10);
    layout->setLabelAlignment(Qt::AlignLeft);
    dialog->setLayout(layout);
    dialog->setWindowIcon(QIcon());
    dialog->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    dialog->setWindowTitle(tr("Set point's coordinate"));
    dialog->move(QPoint(5,220));
//    qDebug()<<dialog->mapToParent(view->pos());
    dialog->show();
}

PF_ActionDrawPoint::~PF_ActionDrawPoint()
{

}

void PF_ActionDrawPoint::trigger()
{
    if(pt->valid){
        PF_Point * point = new PF_Point(container,view,PF_PointData(*pt));
        container->addEntity(point);

        PF_Point::point_index++;

        //view->redraw((PF::RedrawMethod) (PF::RedrawDrawing | PF::RedrawOverlay));
        view->replot();
    }
}

void PF_ActionDrawPoint::mouseMoveEvent(QMouseEvent *e)
{
    PF_Vector mouse = snapPoint(e);
    textx->setText(QString("%1").arg(mouse.x));
    texty->setText(QString("%1").arg(mouse.y));

//    PF_CADWidget::statusbar->clearMessage();

    PF_CADWidget::statusbar->showMessage(mouse.toString()+QString(tr("Single click to set point.(Press right click to abort)")));
}

void PF_ActionDrawPoint::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton){
        PF_Vector mouse = PF_Snapper::snapPoint(e);
        *pt = mouse;
        trigger();
    }else if(e->button() == Qt::RightButton){
        init(getStatus()-1);

        PF_CADWidget::statusbar->clearMessage();
    }
}

void PF_ActionDrawPoint::updateMouseButtonHints()
{

}

void PF_ActionDrawPoint::updateMouseCursor()
{

}
