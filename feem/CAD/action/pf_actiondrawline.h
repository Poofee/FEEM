#ifndef PF_ACTIONDRAWLINE_H
#define PF_ACTIONDRAWLINE_H

#include "pf_actionpreviewinterface.h"

struct PF_LineData;

class PF_ActionDrawLine : public PF_ActionPreviewInterface
{
    Q_OBJECT
public:
    enum Status{
        SetStartpoint,
        SetEndpoint
    };

public:
    PF_ActionDrawLine(PF_EntityContainer* container,PF_GraphicView* graphicView);
    ~PF_ActionDrawLine() override;

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
    PF_LineData* data;
};

#endif // PF_ACTIONDRAWLINE_H
