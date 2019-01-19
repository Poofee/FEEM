#ifndef PF_ACTIONDRAWRECTANGLE_H
#define PF_ACTIONDRAWRECTANGLE_H

#include "pf_actionpreviewinterface.h"


/**
 * @brief 绘制矩形，矩形也是多边形的一种。
 *
 */
class PF_ActionDrawRectangle : public PF_ActionPreviewInterface
{
    Q_OBJECT
public:
    /**
         * Action States.
         */
        enum Status {
            SetCorner1,      /**< Setting the 1st corner.  */
            SetCorner2       /**< Setting the 2nd corner. */
        };

    public:
        PF_ActionDrawRectangle(PF_EntityContainer* container,
                                   PF_GraphicView* graphicView);
        ~PF_ActionDrawRectangle() override;

        void trigger() override;
        void mouseMoveEvent(QMouseEvent* e) override;
        void mouseReleaseEvent(QMouseEvent* e) override;

        //void coordinateEvent(PF_CoordinateEvent* e) override;
        //void commandEvent(RS_CommandEvent* e) override;

        void updateMouseButtonHints() override;
        void updateMouseCursor() override;

    protected:
        struct Points;
        Points * pPoints;
};

#endif // PF_ACTIONDRAWRECTANGLE_H
