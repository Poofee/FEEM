#ifndef PF_H
#define PF_H

#define PF_MAXDOUBLE 1.0E+10
#define PF_MINDOUBLE -1.0E+10
//tolerance
#define PF_TOLERANCE 1.0e-10
//squared tolerance
#define PF_TOLERANCE15 1.5e-15
#define PF_TOLERANCE2 1.0e-20
#define PF_TOLERANCE_ANGLE 1.0e-8
//2018-02-07
//by Poofee
/**该类定义一些枚举**/

class PF{
public:
    /**
     * Flags.
     */
    enum Flags {
        /** Flag for Undoables. */
        FlagUndone      = 1<<0,
        /** Entity Visibility. */
        FlagVisible     = 1<<1,
        /** Entity attribute (e.g. color) is defined by layer. */
        FlagByLayer     = 1<<2,
        /** Entity attribute (e.g. color) defined by block. */
        FlagByBlock     = 1<<3,
        /** Layer frozen. */
        FlagFrozen      = 1<<4,
        /** Layer frozen by default. */
        FlagDefFrozen   = 1<<5,
        /** Layer locked. */
        FlagLocked      = 1<<6,
        /** Used for invalid pens. */
        FlagInvalid     = 1<<7,
        /** Entity in current selection. */
        FlagSelected    = 1<<8,
        /** Polyline closed? */
        FlagClosed      = 1<<9,
        /** Flag for temporary entities (e.g. hatch) */
        FlagTemp        = 1<<10,
        /** Flag for processed entities (optcontour) */
        FlagProcessed   = 1<<11,
        /** Startpoint selected */
        FlagSelected1   = 1<<12,
        /** Endpoint selected */
        FlagSelected2   = 1<<13,
        /** Entity is highlighted temporarily (as a user action feedback) */
        FlagHighlighted = 1<<14
    };
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

    /**
     * Enum of levels of resolving when iterating through an entity tree.
     */
    enum ResolveLevel {
        /** Groups are not resolved */
        ResolveNone,
        /**
        * Resolve all but not Inserts.
        */
        ResolveAllButInserts,
        /**
         * Resolve all but not Text or MText.
         */
        ResolveAllButTexts,
        /**
         * Resolve no text or images, added as a quick fix for bug#422
         */
        ResolveAllButTextImage,
        /**
         * all Entity Containers are resolved
         * (including Texts, Polylines, ...)
         */
        ResolveAll
    };
};
#endif // PF_H
