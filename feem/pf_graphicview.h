#ifndef PF_GRAPHICVIEW_H
#define PF_GRAPHICVIEW_H

#include <QWidget>

#include "pf.h"

class QGridLayout;

class PF_GraphicView : public QWidget
{
    Q_OBJECT
public:
    explicit PF_GraphicView(QWidget *parent = nullptr);
    ~PF_GraphicView();
    void drawLayer1(QPainter * painter);
    void getPixmapForView(QPixmap **pm);

signals:

public slots:

protected:
    void paintEvent(QPaintEvent*);
    void mouseMoveEvent(QMouseEvent* e) override;
    void mouseDoubleClickEvent(QMouseEvent* e) override;
    void mouseReleaseEvent(QMouseEvent* e) override;
    void mousePressEvent(QMouseEvent* e) override;
    void tabletEvent(QTabletEvent* e) override;
    void leaveEvent(QEvent*) override;
    void enterEvent(QEvent*) override;
    void focusInEvent(QFocusEvent*) override;
    void focusOutEvent(QFocusEvent*) override;
    void wheelEvent(QWheelEvent* e) override;
    void keyPressEvent(QKeyEvent* e) override;
    void keyReleaseEvent(QKeyEvent* e) override;

    void resizeEvent(QResizeEvent* e) override;

protected:
    QGridLayout* layout;

    QPixmap *PixmapLayer1;//图层1，显示网格
    QPixmap *PixmapLayer2;
    QPixmap *PixmapLayer3;

    PF::RedrawMethod redrawMethod;
};

#endif // PF_GRAPHICVIEW_H
