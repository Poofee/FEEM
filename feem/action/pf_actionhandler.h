#ifndef PF_ACTIONHANDLER_H
#define PF_ACTIONHANDLER_H

#include <QObject>

#include "pf.h"
#include "pf_actioninterface.h"

//2018-01-23
//by Poofee
/**这个类实现菜单、工具栏的动作**/
class PF_ActionHandler : public QObject
{
    Q_OBJECT
public:
    PF_ActionHandler(QObject *parent = nullptr);
    virtual ~PF_ActionHandler()=default;

    PF_ActionInterface* getCurrentAction();
    PF_ActionInterface* setCurrentAction(PF::ActionType typeId);

    void set_view(PF_GraphicView * graphicView);
    void set_document(PF_Document * _document);

signals:

public slots:
    //File

    //Edit
    void slotUndo();
    void slotRedo();
    void slotCut();
    void slotCopy();
    void slotPaste();
    void slotDelete();

    //Operation
    void slotSelectSingle();
    void slotSelectAll();
    void slotDrawPoint();
    void slotDrawLine();
    void slotDrawArc();
    void slotDrawCircle();

    //View
    void slotZoomIn();
    void slotZoomOut();
    void slotZoomAuto();

    //Problems

    //Grid
    void slotShowGrid();
    void slotSetGrid();

    //Property

    //Mesh
    void slotDoMesh();
    void slotShowMesh();
    void slotDelMesh();

    //Solve
    void slotSolve();
    void slotShowResult();
    void slotSolveSetting();

    //About
    void slotShowAbout();
private:
    PF_GraphicView* view;
    PF_Document * document;
};

#endif // PF_ACTIONHANDLER_H
