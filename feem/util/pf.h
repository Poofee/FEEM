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

        ActionSelect,
        ActionSelectSingle,
        ActionSelectAll,
        ActionDeSelectAll,
        ActionDrawPoint,
        ActionDrawCircle,
        ActionDrawRectangle,
        ActionDrawLine,
        ActionDrawFace,
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
        EntityFace,         /**< Face */
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

/* Definitions of useful mathematical constants
 * M_E        - e
 * M_LOG2E    - log2(e)
 * M_LOG10E   - log10(e)
 * M_LN2      - ln(2)
 * M_LN10     - ln(10)
 * M_PI       - pi
 * M_PI_2     - pi/2
 * M_PI_4     - pi/4
 * M_1_PI     - 1/pi
 * M_2_PI     - 2/pi
 * M_2_SQRTPI - 2/sqrt(pi)
 * M_SQRT2    - sqrt(2)
 * M_SQRT1_2  - 1/sqrt(2)
 */

#define M_E        2.71828182845904523536
#define M_LOG2E    1.44269504088896340736
#define M_LOG10E   0.434294481903251827651
#define M_LN2      0.693147180559945309417
#define M_LN10     2.30258509299404568402
#define M_PI       3.14159265358979323846
#define M_PI_2     1.57079632679489661923
#define M_PI_4     0.785398163397448309616
#define M_1_PI     0.318309886183790671538
#define M_2_PI     0.636619772367581343076
#define M_2_SQRTPI 1.12837916709551257390
#define M_SQRT2    1.41421356237309504880
#define M_SQRT1_2  0.707106781186547524401

#endif // PF_H
