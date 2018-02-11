#ifndef PF_H
#define PF_H
//2018-02-07
//by Poofee
//该类定义一些枚举

class PF{
public:
    enum ActionType{
        ActionNone,
        ActionDefault,

        ActionFlieNew,
        ActionFileOpen,
        ActionFileSave,
        ActionFileSaveAs,
        ActionFileClose,
        ActionFileExport,
        ActionFilePrint,
        ActionFilePrintPreview,
        ActionFileQuit,

        ActionEditKillAllActions,
        ActionEditUndo,
        ActionEditRedo,
        ActionEditCut,
        ActionEditCopy,
        ActionEditPaste,
        ActionEditDelete,

        ActionSelectSingle,
        ActionSelectAll,
        ActionDrawPoint,
        ActionDrawCircle,
        ActionDrawLine,
        ActionDrawArc,

        ActionViewZoomIn,
        ActionViewZoomOut,
        ACtionViewZoomAuto,
        ActionViewShowGrid,
        ActionViewHideGrid,
        ActionViewSetGrid,

        ActionMeshDoMesh,
        ActionMeshDeleteMesh,
        ActionMeshShowMesh,
        ActionMeshHideMesh,

        ActionSolve,
        ActionShowResult,
        ActionSolveSetting,
    };

    enum Unit{
        None,
        Millimeter,
        Centimeter,
    };
    enum ZoomDirection {
        In, Out
    };
    enum RedrawMethod {
        RedrawNone = 0,
        RedrawGrid = 1,
        RedrawOverlay = 2,
        RedrawDrawing = 4,
        RedrawAll = 0xffff
    };

};
#endif // PF_H
