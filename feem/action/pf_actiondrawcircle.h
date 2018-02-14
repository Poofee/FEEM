#ifndef PF_ACTIONDRAWCIRCLE_H
#define PF_ACTIONDRAWCIRCLE_H

#include "pf_actionpreviewinterface.h"

struct PF_CircleData;

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
