#include "pf_graphicview.h"

#include <QPainter>
#include <QtMath>
#include <QMouseEvent>

#include "pf_eventhandler.h"

PF_GraphicView::PF_GraphicView(QWidget *parent)
    : QWidget(parent)
    ,PixmapLayer1(nullptr)
    ,PixmapLayer2(nullptr)
    ,PixmapLayer3(nullptr)
    ,eventHandler(new PF_EventHandler{this})
{

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

void PF_GraphicView::paintEvent(QPaintEvent *){   
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
    painter2.drawEllipse(QPointF(width()/2,height()/2),100,100);
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

void PF_GraphicView::drawLayer1(QPainter * painter){
    int numgridw = 10;
    int numgridh = 5;
    int leftmargin = 50;
    int rightmargin = 50;
    int topmargin = 50;
    int bottommargin = 50;
    int spaceW = (width()-rightmargin-leftmargin)/numgridw;
    int spaceH = (height()-topmargin-bottommargin)/numgridh;
    int subspaceW = spaceW/5;
    int subspaceH = spaceH/5;

//    for(int i = 0+leftmargin; i < this->width()-rightmargin;i+=10){
//        for(int j = 0+topmargin; j < this->height()-bottommargin;j+=10){
//            painter->drawPoint(i,j);
//        }
//    }
    painter->setPen(Qt::gray);

    for(int i = 1; i < numgridw;i+=1){
        painter->drawLine(QPointF(i*spaceW+leftmargin,0+topmargin),QPointF(i*spaceW+leftmargin,spaceH*numgridh+topmargin));
    }
    for(int i = 1; i < numgridh;i+=1){
        painter->drawLine(QPointF(0+leftmargin,i*spaceH+topmargin),QPointF(spaceW*numgridw+leftmargin,i*spaceH+topmargin));
    }
    QPen pen1;
    pen1.setColor(Qt::black);
    pen1.setWidth(4);
    painter->setPen(pen1);
    painter->drawRect(leftmargin,topmargin,spaceW*numgridw,spaceH*numgridh);
    int mainTickH = 10;
    int subTickH = 5;
    for(int i = 0; i < numgridw;i+=1){
        pen1.setWidth(4);painter->setPen(pen1);
        painter->drawLine(QPointF(i*spaceW+leftmargin,0+topmargin),QPointF(i*spaceW+leftmargin,mainTickH+topmargin));
        painter->drawLine(QPointF(i*spaceW+leftmargin,spaceH*numgridh+topmargin),QPointF(i*spaceW+leftmargin,spaceH*numgridh-mainTickH+topmargin));
        pen1.setWidth(2);painter->setPen(pen1);
        for(int j = 1; j <= 4;j++){
            painter->drawLine(QPointF(i*spaceW+leftmargin+j*subspaceW,0+topmargin),QPointF(i*spaceW+leftmargin+j*subspaceW,subTickH+topmargin));
            painter->drawLine(QPointF(i*spaceW+leftmargin+j*subspaceW,spaceH*numgridh+topmargin),QPointF(i*spaceW+leftmargin+j*subspaceW,spaceH*numgridh-subTickH+topmargin));
        }
    }
    for(int i = 0; i < numgridh;i+=1){
        pen1.setWidth(4);painter->setPen(pen1);
        painter->drawLine(QPointF(0+leftmargin,i*spaceH+topmargin),QPointF(mainTickH+leftmargin,i*spaceH+topmargin));
        painter->drawLine(QPointF(spaceW*numgridw+leftmargin,i*spaceH+topmargin),QPointF(spaceW*numgridw-mainTickH+leftmargin,i*spaceH+topmargin));
        pen1.setWidth(2);painter->setPen(pen1);
        for(int j = 1; j <= 4;j++){
            painter->drawLine(QPointF(0+leftmargin,i*spaceH+topmargin+j*subspaceH),QPointF(subTickH+leftmargin,i*spaceH+topmargin+j*subspaceH));
            painter->drawLine(QPointF(spaceW*numgridw+leftmargin,i*spaceH+topmargin+j*subspaceH),QPointF(spaceW*numgridw-subTickH+leftmargin,i*spaceH+topmargin+j*subspaceH));
        }
    }

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

void PF_GraphicView::setCurrentAction(PF_ActionInterface *action)
{
    if(eventHandler){
        eventHandler->setCurrentAction(action);
    }
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
