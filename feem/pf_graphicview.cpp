#include "pf_graphicview.h"

#include <QPainter>

PF_GraphicView::PF_GraphicView(QWidget *parent) : QWidget(parent)
{

}

void PF_GraphicView::paintEvent(QPaintEvent *){
    QPainter painter(this);

    drawLayer1(&painter);

    // 反走样
    painter.setRenderHint(QPainter::Antialiasing, true);
    // 设置画笔颜色、宽度
    painter.setPen(QPen(QColor(0, 160, 230), 2));
    // 设置画刷颜色
    painter.setBrush(QColor(255, 160, 90));
    painter.drawRect(50, 50, 160, 100);
}

void PF_GraphicView::drawLayer1(QPainter * painter){

    for(int i = 0; i < this->width();i+=10){
        for(int j = 0; j < this->height();j+=10){
            painter->drawPoint(i,j);
        }
    }
}
