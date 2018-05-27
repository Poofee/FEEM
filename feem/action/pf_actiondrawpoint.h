#ifndef PF_ACTIONDRAWPOINT_H
#define PF_ACTIONDRAWPOINT_H

#include "pf_actionpreviewinterface.h"

class PF_ActionDrawPoint : public PF_ActionPreviewInterface
{
    Q_OBJECT
public:
    PF_ActionDrawPoint(PF_EntityContainer* parent, PF_GraphicView* view);
    ~PF_ActionDrawPoint() override;

    void trigger() override;

    void mouseMoveEvent(QMouseEvent* e) override;
    void mouseReleaseEvent(QMouseEvent* e) override;

    void updateMouseButtonHints() override;
    void updateMouseCursor() override;

private:
    PF_Vector* pt;
};

#endif // PF_ACTIONDRAWPOINT_H
