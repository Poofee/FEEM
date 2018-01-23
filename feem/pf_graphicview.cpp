#include "pf_graphicview.h"

#include <QPainter>

PF_GraphicView::PF_GraphicView(QWidget *parent) : QWidget(parent)
{

}

void PF_GraphicView::paintEvent(QPaintEvent *){
    QPainter painter(this);

    // 反走样
    painter.setRenderHint(QPainter::Antialiasing, true);
    // 设置画笔颜色、宽度
    painter.setPen(QPen(QColor(0, 160, 230), 2));
    // 设置画刷颜色
    painter.setBrush(QColor(255, 160, 90));
    painter.drawRect(50, 50, 160, 100);
}
