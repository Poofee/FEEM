#include "pf_actionhandler.h"
#include "pf_actiondrawcircle.h"

PF_ActionHandler::PF_ActionHandler(QObject *parent) : QObject(parent)
{

}

PF_ActionInterface *PF_ActionHandler::getCurrentAction()
{

}

PF_ActionInterface *PF_ActionHandler::setCurrentAction(PF::ActionType typeId)
{
    PF_ActionInterface* a = NULL;

    switch (typeId) {
    case PF::ActionDefault:

        break;
    case PF::ActionDrawArc:

        break;
    case PF::ActionDrawCircle:
        a = new PF_ActionDrawCircle(view);
        break;
    case PF::ActionDrawLine:

        break;
    case PF::ActionDrawPoint:

        break;
    case PF::ActionEditCopy:

        break;
    case PF::ActionEditCut:

        break;
    case PF::ActionEditDelete:

        break;
    case PF::ActionEditKillAllActions:

        break;
    case PF::ActionEditPaste:

        break;
    case PF::ActionEditRedo:

        break;
    case PF::ActionEditUndo:

        break;
    case PF::ActionFileClose:

        break;
    case PF::ActionFileExport:

        break;
    case PF::ActionFileOpen:

        break;
    case PF::ActionFilePrint:

        break;
    case PF::ActionFilePrintPreview:

        break;
    case PF::ActionFileQuit:

        break;
    case PF::ActionFileSave:

        break;
    case PF::ActionFileSaveAs:

        break;
    case PF::ActionFlieNew:

        break;
    case PF::ActionMeshDeleteMesh:

        break;
    case PF::ActionMeshDoMesh:

        break;
    case PF::ActionMeshHideMesh:

        break;
    case PF::ActionMeshShowMesh:

        break;
    case PF::ActionSelectAll:

        break;
    case PF::ActionSelectSingle:

        break;
    case PF::ActionShowResult:

        break;
    case PF::ActionSolve:

        break;
    case PF::ActionSolveSetting:

        break;
    case PF::ActionViewHideGrid:

        break;
    case PF::ActionViewShowGrid:

        break;
    case PF::ActionViewSetGrid:

        break;
    case PF::ACtionViewZoomAuto:

        break;
    case PF::ActionViewZoomIn:

        break;
    case PF::ActionViewZoomOut:

        break;
    default:
        break;
    }
}

void PF_ActionHandler::slotUndo() {
    setCurrentAction(PF::ActionEditUndo);
}

void PF_ActionHandler::slotRedo() {
    setCurrentAction(PF::ActionEditRedo);
}

void PF_ActionHandler::slotCut() {
    setCurrentAction(PF::ActionEditCut);
}

void PF_ActionHandler::slotCopy() {
    setCurrentAction(PF::ActionEditCopy);
}

void PF_ActionHandler::slotPaste() {
    setCurrentAction(PF::ActionEditPaste);
}

void PF_ActionHandler::slotDelete() {
    setCurrentAction(PF::ActionEditDelete);
}

void PF_ActionHandler::slotSelectSingle() {
    setCurrentAction(PF::ActionSelectSingle);
}

void PF_ActionHandler::slotSelectAll() {
    setCurrentAction(PF::ActionSelectAll);
}

void PF_ActionHandler::slotDrawPoint() {
    setCurrentAction(PF::ActionDrawPoint);
}

void PF_ActionHandler::slotDrawLine() {
    setCurrentAction(PF::ActionDrawLine);
}

void PF_ActionHandler::slotDrawArc() {
    setCurrentAction(PF::ActionDrawArc);
}

void PF_ActionHandler::slotDrawCircle()
{
    setCurrentAction(PF::ActionDrawCircle);
}

void PF_ActionHandler::slotZoomIn() {
    setCurrentAction(PF::ActionViewZoomIn);
}

void PF_ActionHandler::slotZoomOut() {
    setCurrentAction(PF::ActionViewZoomOut);
}

void PF_ActionHandler::slotZoomAuto() {
    setCurrentAction(PF::ACtionViewZoomAuto);
}

void PF_ActionHandler::slotShowGrid() {
    setCurrentAction(PF::ActionViewShowGrid);
}

void PF_ActionHandler::slotSetGrid() {
    setCurrentAction(PF::ActionViewSetGrid);
}

void PF_ActionHandler::slotDoMesh() {
    setCurrentAction(PF::ActionMeshDoMesh);
}

void PF_ActionHandler::slotShowMesh() {
    setCurrentAction(PF::ActionMeshShowMesh);
}

void PF_ActionHandler::slotDelMesh() {
    setCurrentAction(PF::ActionMeshDeleteMesh);
}

void PF_ActionHandler::slotSolve() {
    setCurrentAction(PF::ActionSolve);
}

void PF_ActionHandler::slotShowResult() {
    setCurrentAction(PF::ActionShowResult);
}

void PF_ActionHandler::slotSolveSetting() {
    setCurrentAction(PF::ActionSolveSetting);
}

void PF_ActionHandler::slotShowAbout() {

}
