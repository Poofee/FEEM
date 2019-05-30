#ifndef PF_ACTIONSELECTALL_H
#define PF_ACTIONSELECTALL_H


#include "pf_actioninterface.h"


class PF_ActionSelectAll : public PF_ActionInterface {
    Q_OBJECT
public:
    PF_ActionSelectAll(PF_EntityContainer* container,
                       PF_GraphicView* graphicView,
                       bool select);

    void init(int status) override;
    void trigger() override;

protected:
    bool select;
};

#endif // PF_ACTIONSELECTALL_H
