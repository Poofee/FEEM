#include "pf_graphicview.h"

#include <QPainter>
#include <QtMath>
#include <QMouseEvent>
#include <QDebug>
#include <QGridLayout>

#include "pf_actioninterface.h"
#include "pf_eventhandler.h"

PF_GraphicView::PF_GraphicView(PF_Document *doc, QWidget *parent)
    : QWidget(parent)
    ,PixmapLayer1(nullptr)
    ,PixmapLayer2(nullptr)
    ,PixmapLayer3(nullptr)
    ,eventHandler(new PF_EventHandler(this))
{
    qDebug()<<"PF_GraphicView::PF_GraphicView";
    if(doc){
        setContainer(doc);
    }
}

PF_GraphicView::~PF_GraphicView()
{
    if(!PixmapLayer1)
        delete PixmapLayer1;
    if(!PixmapLayer2)
        delete PixmapLayer2;
    if(!PixmapLayer3)
        delete PixmapLayer3;
}

void PF_GraphicView::paintEvent(QPaintEvent *e){
    getPixmapForView(&PixmapLayer1);
    getPixmapForView(&PixmapLayer2);
    PixmapLayer1->fill(QColor(250,250,250,80));

    QPainter painter1(PixmapLayer1);
    painter1.setPen(QColor(0,0,0));
    drawLayer1(&painter1);
    painter1.end();

    PixmapLayer2->fill(Qt::transparent);
    QPainter painter2(PixmapLayer2);
    painter2.setRenderHint(QPainter::Antialiasing, true);
    painter2.setPen(QColor(0,0,0));
    drawEntityLayer(&painter2);
    painter2.end();

    QPainter painter(this);
    painter.drawPixmap(0,0,*PixmapLayer1);
    painter.drawPixmap(0,0,*PixmapLayer2);
    painter.end();
}

void PF_GraphicView::mouseMoveEvent(QMouseEvent *e)
{
    e->accept();

    eventHandler->mouseMoveEvent(e);
}

void PF_GraphicView::mouseDoubleClickEvent(QMouseEvent *e)
{
    switch (e->button()) {
    case Qt::MiddleButton:
        //zoom action
        break;
    case Qt::LeftButton:

        break;
    default:
        break;
    }
    e->accept();
}

void PF_GraphicView::mouseReleaseEvent(QMouseEvent *e)
{
    e->accept();

    switch (e->button()) {
    case Qt::RightButton:
        //Ctrl+Right-Click

        //Shift+Right-Click

        //Right-Click menu

        back();
        break;
    default:
        eventHandler->mouseReleaseEvent(e);
        break;
    }
}

void PF_GraphicView::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::MiddleButton){
        //zoom action
    }
    eventHandler->mousePressEvent(e);
}

void PF_GraphicView::tabletEvent(QTabletEvent *e)
{

}

void PF_GraphicView::leaveEvent(QEvent *)
{

}

void PF_GraphicView::enterEvent(QEvent *)
{

}

void PF_GraphicView::focusInEvent(QFocusEvent *)
{

}

void PF_GraphicView::focusOutEvent(QFocusEvent *)
{

}

void PF_GraphicView::wheelEvent(QWheelEvent *e)
{

}

void PF_GraphicView::keyPressEvent(QKeyEvent *e)
{

}

void PF_GraphicView::keyReleaseEvent(QKeyEvent *e)
{

}

void PF_GraphicView::resizeEvent(QResizeEvent *e)
{

}

//绘制坐标轴和网格
void PF_GraphicView::drawLayer1(QPainter * painter){

}

void PF_GraphicView::getPixmapForView(QPixmap **pm)
{
    QSize const s0(width(), height());
    if(*pm){
        if((*pm)->size()==s0){
            return;
        }else{
            delete *pm;
            *pm = nullptr;
        }
    }

    *pm = new QPixmap(width(), height());
}

void PF_GraphicView::back()
{
    if(eventHandler && eventHandler->hasAction()){
        eventHandler->back();
    }
}

void PF_GraphicView::setContainer(PF_EntityContainer *_container)
{
    this->container = _container;
}

void PF_GraphicView::setCurrentAction(PF_ActionInterface *action)
{
    qDebug()<<"PF_GraphicView::setCurrentAction";
    if(eventHandler){
        eventHandler->setCurrentAction(action);
    }
    qDebug()<<"PF_GraphicView::setCurrentAction: OK.";
}

PF_ActionInterface *PF_GraphicView::getCurrentAction()
{
    if(eventHandler){
        return eventHandler->getCurrentAction();
    }else{
        return nullptr;
    }
}

void PF_GraphicView::setDefaultAction(PF_ActionInterface *action)
{
    if(eventHandler){
        eventHandler->setDefaultAction(action);
    }
}

PF_ActionInterface *PF_GraphicView::getDefaultAction()
{
    if(eventHandler){
        return eventHandler->getDefaultAction();
    }else{
        return nullptr;
    }
}

void PF_GraphicView::redraw(PF::RedrawMethod method)
{
    redrawMethod = (PF::RedrawMethod ) (redrawMethod | method);
    update(); // Paint when reeady to pain
    repaint(); //Paint immediate
}

void PF_GraphicView::drawEntity(QPainter *painter, PF_Entity *e)
{
    if(!e){
        return;
    }
    e->draw(painter,this);
}

void PF_GraphicView::drawEntityLayer(QPainter *painter)
{
    drawEntity(painter, container);
}
