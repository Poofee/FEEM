#include "pf_graphicview.h"

#include <QPainter>
#include <QtMath>

PF_GraphicView::PF_GraphicView(QWidget *parent)
    : QWidget(parent)
    ,PixmapLayer1(nullptr)
    ,PixmapLayer2(nullptr)
    ,PixmapLayer3(nullptr)
{

}

void PF_GraphicView::paintEvent(QPaintEvent *){   
    getPixmapForView(&PixmapLayer1);
    PixmapLayer1->fill(QColor(250,250,250,80));

    QPainter painter1(PixmapLayer1);
    painter1.setPen(QColor(232,241,246));
    drawLayer1(&painter1);
    painter1.end();

    QPainter painter(this);
    painter.drawPixmap(0,0,*PixmapLayer1);
    painter.end();
}

void PF_GraphicView::mouseMoveEvent(QMouseEvent *e)
{

}

void PF_GraphicView::mouseDoubleClickEvent(QMouseEvent *e)
{

}

void PF_GraphicView::mouseReleaseEvent(QMouseEvent *e)
{

}

void PF_GraphicView::mousePressEvent(QMouseEvent *e)
{

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

    for(int i = 0+50; i < this->width()-50;i+=10){
        for(int j = 0+50; j < this->height()-50;j+=10){
            painter->drawPoint(i,j);
        }
    }
    painter->setPen(Qt::gray);
    int numgridw = 10;
    int numgridh = 5;
    int leftmargin = 50;
    int rightmargin = 50;
    int topmargin = 50;
    int bottommargin = 50;
    int spaceW = (width()-rightmargin-leftmargin)/numgridw;
    int spaceH = (height()-topmargin-bottommargin)/numgridh;

    for(int i = 0; i <= numgridw;i+=1){
        painter->drawLine(QPointF(i*spaceW+leftmargin,0+topmargin),QPointF(i*spaceW+leftmargin,spaceH*numgridh+topmargin));
    }
    for(int i = 0; i <= numgridh;i+=1){
        painter->drawLine(QPointF(0+leftmargin,i*spaceH+topmargin),QPointF(spaceW*numgridw+leftmargin,i*spaceH+topmargin));
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
