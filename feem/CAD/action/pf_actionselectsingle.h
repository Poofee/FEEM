#ifndef PF_ACTIONSELECTSINGLE_H
#define PF_ACTIONSELECTSINGLE_H

#include "pf_actioninterface.h"


/*!
 \brief 选择单个几何图形

*/
class PF_ActionSelectSingle : public PF_ActionInterface
{
    Q_OBJECT
public:
    PF_ActionSelectSingle(PF_EntityContainer* container,PF_GraphicView* view);
    ~PF_ActionSelectSingle();

    void trigger() override;
    void keyPressEvent(QKeyEvent* e) override;
    void mouseReleaseEvent(QMouseEvent* e) override;
    void updateMouseCursor() override;

private:
    std::initializer_list<PF::EntityType> const entityTypeList;

    PF_Entity* en;
    PF_ActionInterface* actionSelect;
};

#endif // PF_ACTIONSELECTSINGLE_H
