#ifndef PF_ACTIONPREVIEWINTERFACE_H
#define PF_ACTIONPREVIEWINTERFACE_H

#include "pf_actioninterface.h"

class PF_ActionPreviewInterface: public PF_ActionInterface
{
public:
    PF_ActionPreviewInterface(PF_GraphicView* view);
    ~PF_ActionPreviewInterface() override;

    void init(int status=0) override;
    void finish() override;
    void resume() override;
    void suspend() override;
    void trigger() override;

    void drawPreview();
    void deletePreview();
};

#endif // PF_ACTIONPREVIEWINTERFACE_H
