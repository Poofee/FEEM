#ifndef PF_ACTIONDRAWFACE_H
#define PF_ACTIONDRAWFACE_H

#include "pf_actionpreviewinterface.h"

struct PF_FaceData;

class PF_ActionDrawFace : public PF_ActionPreviewInterface
{
    Q_OBJECT
public:
    enum Status{
        SetFirstLoop,
        SetOtherLoop
    };

public:
    PF_ActionDrawFace(PF_EntityContainer* container,PF_GraphicView* graphicView);
    ~PF_ActionDrawFace() override;

    void reset();

    void init(int status=0) override;
    void trigger() override;

    void mouseMoveEvent(QMouseEvent* e) override;
    void mouseReleaseEvent(QMouseEvent* e) override;

    void hideOptions() override;
    void showOptions() override;

    void updateMouseButtonHints() override;
    void updateMouseCursor() override;

protected:
    PF_FaceData* data;
};

#endif // PF_ACTIONDRAWFACE_H
