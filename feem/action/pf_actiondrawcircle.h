#ifndef PF_ACTIONDRAWCIRCLE_H
#define PF_ACTIONDRAWCIRCLE_H

#include "pf_actionpreviewinterface.h"

struct PF_CircleData;

//这样的一个类，它的主要功能是通过鼠标操作，获得将要绘制图形
//的数据，并将图形数据附加到view上，最后还是会调用view的函数
//来实现绘图。
class PF_ActionDrawCircle: public PF_ActionPreviewInterface
{
    Q_OBJECT
public:
    /**
      * Action States.
      */
    enum Status {
        SetCenter,       /**< Setting the center point. */
        SetRadius        /**< Setting the radius. */
    };
public:
    PF_ActionDrawCircle(PF_GraphicView* view);
    ~PF_ActionDrawCircle() override;

    void reset();
    void init(int status=0) override;
    void trigger() override;

    void mouseMoveEvent(QMouseEvent* e) override;
    void mouseReleaseEvent(QMouseEvent* e) override;
protected:
    PF_CircleData* data;
};

#endif // PF_ACTIONDRAWCIRCLE_H
