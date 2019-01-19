#ifndef PF_H
#define PF_H
//2018-02-07
//by Poofee
/**该类定义一些枚举**/

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
        ActionDrawRectangle,
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

    //overlay list
    enum OverlayGraphics {
        ActionPreviewEntity, // Action Entities
        Snapper // Snapper
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
    /**
         * Entity types returned by the rtti() method
         */
    enum EntityType {
        EntityUnknown,      /**< Unknown */
        EntityContainer,    /**< Container */
        EntityBlock,        /**< Block (Group definition) */
        EntityFontChar,     /**< Font character */
        EntityInsert,       /**< Insert (Group instance) */
        EntityGraphic,      /**< Graphic with layers */
        EntityPoint,        /**< Point */
        EntityLine,         /**< Line */
        EntityPolyline,     /**< Polyline */
        EntityVertex,       /**< Vertex (part of a polyline) */
        EntityArc,          /**< Arc */
        EntityCircle,       /**< Circle */
        EntityEllipse,      /**< Ellipse */
        EntityHyperbola,      /**< Hyperbola */
        EntitySolid,        /**< Solid */
        EntityConstructionLine, /**< Construction line */
        EntityMText,         /**< Multi-line Text */
        EntityText,         /**< Single-line Text */
        EntityDimAligned,   /**< Aligned Dimension */
        EntityDimLinear,    /**< Linear Dimension */
        EntityDimRadial,    /**< Radial Dimension */
        EntityDimDiametric, /**< Diametric Dimension */
        EntityDimAngular,   /**< Angular Dimension */
        EntityDimLeader,    /**< Leader Dimension */
        EntityHatch,        /**< Hatch */
        EntityImage,        /**< Image */
        EntitySpline,       /**< Spline */
        EntitySplinePoints,       /**< SplinePoints */
        EntityOverlayBox,    /**< OverlayBox */
        EntityPreview,    /**< Preview Container */
        EntityPattern,
        EntityOverlayLine
    };
};
#endif // PF_H
