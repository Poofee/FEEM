#ifndef PF_GRAPHICVIEW_H
#define PF_GRAPHICVIEW_H

#include <QWidget>

#include "pf.h"
#include "pf_entitycontainer.h"
#include "pf_document.h"
#include "pf_snapper.h"

class QMouseEvent;
class QKeyEvent;
class QGridLayout;
class PF_ActionInterface;
class PF_EventHandler;

class PF_GraphicView : public QWidget
{
    Q_OBJECT
public:
    explicit PF_GraphicView(PF_Document* doc, QWidget *parent = nullptr);
    ~PF_GraphicView();
    void drawLayer1(QPainter * painter);
    void getPixmapForView(QPixmap **pm);

    void setContainer(PF_EntityContainer* _container);

    void setCurrentAction(PF_ActionInterface* action);
    PF_ActionInterface* getCurrentAction();
    void setDefaultAction(PF_ActionInterface* action);
    PF_ActionInterface* getDefaultAction();

    void redraw(PF::RedrawMethod method=PF::RedrawAll);

    void drawEntity(QPainter* painter, PF_Entity* e);

    void drawEntityLayer(QPainter* painter);
signals:

public slots:

protected:
    void paintEvent(QPaintEvent* e) override;
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

    PF_EventHandler* eventHandler;

    PF_EntityContainer* container;//保存所有实体

    PF::RedrawMethod redrawMethod;
};

#endif // PF_GRAPHICVIEW_H
