#ifndef PF_GRAPHICVIEW_H
#define PF_GRAPHICVIEW_H

#include <QWidget>
#include <QMap>

#include "pf.h"
#include "pf_entitycontainer.h"
#include "pf_document.h"
#include "pf_snapper.h"
#include "pf_plot.h"

class QMouseEvent;
class QKeyEvent;
class QGridLayout;
class PF_ActionInterface;
class PF_EventHandler;

class PF_GraphicView : public QWidget
{
    Q_OBJECT
    /// \cond INCLUDE_QPROPERTIES
    Q_PROPERTY(QRect viewport READ viewport WRITE setViewport)
    Q_PROPERTY(QPixmap background READ background WRITE setBackground)
    Q_PROPERTY(bool backgroundScaled READ backgroundScaled WRITE setBackgroundScaled)
    Q_PROPERTY(Qt::AspectRatioMode backgroundScaledMode READ backgroundScaledMode WRITE setBackgroundScaledMode)
    Q_PROPERTY(QCPLayoutGrid* plotLayout READ plotLayout)
    Q_PROPERTY(bool autoAddPlottableToLegend READ autoAddPlottableToLegend WRITE setAutoAddPlottableToLegend)
    Q_PROPERTY(int selectionTolerance READ selectionTolerance WRITE setSelectionTolerance)
    Q_PROPERTY(bool noAntialiasingOnDrag READ noAntialiasingOnDrag WRITE setNoAntialiasingOnDrag)
    Q_PROPERTY(Qt::KeyboardModifier multiSelectModifier READ multiSelectModifier WRITE setMultiSelectModifier)
    Q_PROPERTY(bool openGl READ openGl WRITE setOpenGl)
    /// \endcond
public:
    /*!
    Defines how a layer should be inserted relative to an other layer.

    \see addLayer, moveLayer
  */
    enum LayerInsertMode { limBelow  ///< Layer is inserted below other layer
                           ,limAbove ///< Layer is inserted above other layer
                         };
    Q_ENUMS(LayerInsertMode)

    /*!
    Defines with what timing the PF_GraphicView surface is refreshed after a replot.

    \see replot
  */
    enum RefreshPriority { rpImmediateRefresh ///< Replots immediately and repaints the widget immediately by calling QWidget::repaint() after the replot
                           ,rpQueuedRefresh   ///< Replots immediately, but queues the widget repaint, by calling QWidget::update() after the replot. This way multiple redundant widget repaints can be avoided.
                           ,rpRefreshHint     ///< Whether to use immediate or queued refresh depends on whether the plotting hint \ref QCP::phImmediateRefresh is set, see \ref setPlottingHints.
                           ,rpQueuedReplot    ///< Queues the entire replot for the next event loop iteration. This way multiple redundant replots can be avoided. The actual replot is then done with \ref rpRefreshHint priority.
                         };
    Q_ENUMS(RefreshPriority)

    explicit PF_GraphicView(PF_Document* doc, QWidget *parent = nullptr);
    ~PF_GraphicView();
    void drawLayer1(QPainter * painter);
    void getPixmapForView(QPixmap **pm);

    void back();
    void setContainer(PF_EntityContainer* _container);

    void setCurrentAction(PF_ActionInterface* action);
    PF_ActionInterface* getCurrentAction();
    void setDefaultAction(PF_ActionInterface* action);
    PF_ActionInterface* getDefaultAction();

    void redraw(PF::RedrawMethod method=PF::RedrawAll);

    void drawEntity(QPainter* painter, PF_Entity* e);

    void drawEntityLayer(QPainter* painter);

    virtual void drawLayer3(QPainter *painter);

    virtual void drawOverlay(QPainter *painter);

    virtual PF_EntityContainer *getOverlayContainer(PF::OverlayGraphics position);

    // getters:
    QRect viewport() const { return mViewport; }
    double bufferDevicePixelRatio() const { return mBufferDevicePixelRatio; }
    QPixmap background() const { return mBackgroundPixmap; }
    bool backgroundScaled() const { return mBackgroundScaled; }
    Qt::AspectRatioMode backgroundScaledMode() const { return mBackgroundScaledMode; }
    QCPLayoutGrid *plotLayout() const { return mPlotLayout; }
    QCP::AntialiasedElements antialiasedElements() const { return mAntialiasedElements; }
    QCP::AntialiasedElements notAntialiasedElements() const { return mNotAntialiasedElements; }
    bool autoAddPlottableToLegend() const { return mAutoAddPlottableToLegend; }
    const QCP::Interactions interactions() const { return mInteractions; }
    int selectionTolerance() const { return mSelectionTolerance; }
    bool noAntialiasingOnDrag() const { return mNoAntialiasingOnDrag; }
    QCP::PlottingHints plottingHints() const { return mPlottingHints; }
    Qt::KeyboardModifier multiSelectModifier() const { return mMultiSelectModifier; }
    QCP::SelectionRectMode selectionRectMode() const { return mSelectionRectMode; }
    QCPSelectionRect *selectionRect() const { return mSelectionRect; }
    bool openGl() const { return mOpenGl; }

    // setters:
    void setViewport(const QRect &rect);
    void setBufferDevicePixelRatio(double ratio);
    void setBackground(const QPixmap &pm);
    void setBackground(const QPixmap &pm, bool scaled, Qt::AspectRatioMode mode=Qt::KeepAspectRatioByExpanding);
    void setBackground(const QBrush &brush);
    void setBackgroundScaled(bool scaled);
    void setBackgroundScaledMode(Qt::AspectRatioMode mode);
    void setAntialiasedElements(const QCP::AntialiasedElements &antialiasedElements);
    void setAntialiasedElement(QCP::AntialiasedElement antialiasedElement, bool enabled=true);
    void setNotAntialiasedElements(const QCP::AntialiasedElements &notAntialiasedElements);
    void setNotAntialiasedElement(QCP::AntialiasedElement notAntialiasedElement, bool enabled=true);
    void setAutoAddPlottableToLegend(bool on);
    void setInteractions(const QCP::Interactions &interactions);
    void setInteraction(const QCP::Interaction &interaction, bool enabled=true);
    void setSelectionTolerance(int pixels);
    void setNoAntialiasingOnDrag(bool enabled);
    void setPlottingHints(const QCP::PlottingHints &hints);
    void setPlottingHint(QCP::PlottingHint hint, bool enabled=true);
    void setMultiSelectModifier(Qt::KeyboardModifier modifier);
    void setSelectionRectMode(QCP::SelectionRectMode mode);
    void setSelectionRect(QCPSelectionRect *selectionRect);
    void setOpenGl(bool enabled, int multisampling=16);

    // non-property methods:
    // plottable interface:
    QCPAbstractPlottable *plottable(int index);
    QCPAbstractPlottable *plottable();
    bool removePlottable(QCPAbstractPlottable *plottable);
    bool removePlottable(int index);
    int clearPlottables();
    int plottableCount() const;
    QList<QCPAbstractPlottable*> selectedPlottables() const;
    QCPAbstractPlottable *plottableAt(const QPointF &pos, bool onlySelectable=false) const;
    bool hasPlottable(QCPAbstractPlottable *plottable) const;

    // specialized interface for QCPGraph:
    QCPGraph *graph(int index) const;
    QCPGraph *graph() const;
    QCPGraph *addGraph(QCPAxis *keyAxis=0, QCPAxis *valueAxis=0);
    bool removeGraph(QCPGraph *graph);
    bool removeGraph(int index);
    int clearGraphs();
    int graphCount() const;
    QList<QCPGraph*> selectedGraphs() const;

    // item interface:
    QCPAbstractItem *item(int index) const;
    QCPAbstractItem *item() const;
    bool removeItem(QCPAbstractItem *item);
    bool removeItem(int index);
    int clearItems();
    int itemCount() const;
    QList<QCPAbstractItem*> selectedItems() const;
    QCPAbstractItem *itemAt(const QPointF &pos, bool onlySelectable=false) const;
    bool hasItem(QCPAbstractItem *item) const;

    // layer interface:
    QCPLayer *layer(const QString &name) const;
    QCPLayer *layer(int index) const;
    QCPLayer *currentLayer() const;
    bool setCurrentLayer(const QString &name);
    bool setCurrentLayer(QCPLayer *layer);
    int layerCount() const;
    bool addLayer(const QString &name, QCPLayer *otherLayer=0, LayerInsertMode insertMode=limAbove);
    bool removeLayer(QCPLayer *layer);
    bool moveLayer(QCPLayer *layer, QCPLayer *otherLayer, LayerInsertMode insertMode=limAbove);

    // axis rect/layout interface:
    int axisRectCount() const;
    QCPAxisRect* axisRect(int index=0) const;
    QList<QCPAxisRect*> axisRects() const;
    QCPLayoutElement* layoutElementAt(const QPointF &pos) const;
    QCPAxisRect* axisRectAt(const QPointF &pos) const;
    Q_SLOT void rescaleAxes(bool onlyVisiblePlottables=false);

    QList<QCPAxis*> selectedAxes() const;
    QList<QCPLegend*> selectedLegends() const;
    Q_SLOT void deselectAll();

    bool savePdf(const QString &fileName, int width=0, int height=0, QCP::ExportPen exportPen=QCP::epAllowCosmetic, const QString &pdfCreator=QString(), const QString &pdfTitle=QString());
    bool savePng(const QString &fileName, int width=0, int height=0, double scale=1.0, int quality=-1, int resolution=96, QCP::ResolutionUnit resolutionUnit=QCP::ruDotsPerInch);
    bool saveJpg(const QString &fileName, int width=0, int height=0, double scale=1.0, int quality=-1, int resolution=96, QCP::ResolutionUnit resolutionUnit=QCP::ruDotsPerInch);
    bool saveBmp(const QString &fileName, int width=0, int height=0, double scale=1.0, int resolution=96, QCP::ResolutionUnit resolutionUnit=QCP::ruDotsPerInch);
    bool saveRastered(const QString &fileName, int width, int height, double scale, const char *format, int quality=-1, int resolution=96, QCP::ResolutionUnit resolutionUnit=QCP::ruDotsPerInch);
    QPixmap toPixmap(int width=0, int height=0, double scale=1.0);
    void toPainter(QCPPainter *painter, int width=0, int height=0);
    Q_SLOT void replot(PF_GraphicView::RefreshPriority refreshPriority=PF_GraphicView::rpRefreshHint);

    QCPAxis *xAxis, *yAxis, *xAxis2, *yAxis2;
    QCPLegend *legend;
signals:
    void mouseDoubleClick(QMouseEvent *event);
    void mousePress(QMouseEvent *event);
    void mouseMove(QMouseEvent *event);
    void mouseRelease(QMouseEvent *event);
    void mouseWheel(QWheelEvent *event);

    void plottableClick(QCPAbstractPlottable *plottable, int dataIndex, QMouseEvent *event);
    void plottableDoubleClick(QCPAbstractPlottable *plottable, int dataIndex, QMouseEvent *event);
    void itemClick(QCPAbstractItem *item, QMouseEvent *event);
    void itemDoubleClick(QCPAbstractItem *item, QMouseEvent *event);
    void axisClick(QCPAxis *axis, QCPAxis::SelectablePart part, QMouseEvent *event);
    void axisDoubleClick(QCPAxis *axis, QCPAxis::SelectablePart part, QMouseEvent *event);
    void legendClick(QCPLegend *legend, QCPAbstractLegendItem *item, QMouseEvent *event);
    void legendDoubleClick(QCPLegend *legend,  QCPAbstractLegendItem *item, QMouseEvent *event);

    void selectionChangedByUser();
    void beforeReplot();
    void afterReplot();

public slots:

protected:
    void paintEvent(QPaintEvent* e) override;
    void mouseMoveEvent(QMouseEvent* e) override;
    void mouseDoubleClickEvent(QMouseEvent* e) override;
    void mouseReleaseEvent(QMouseEvent* e) override;
    void mousePressEvent(QMouseEvent* e) override;
    void tabletEvent(QTabletEvent* e) override;
    void leaveEvent(QEvent*) override;
    void enterEvent(QEvent*) override;
    void focusInEvent(QFocusEvent*) override;
    void focusOutEvent(QFocusEvent*) override;
    void wheelEvent(QWheelEvent* e) override;
    void keyPressEvent(QKeyEvent* e) override;
    void keyReleaseEvent(QKeyEvent* e) override;

    void resizeEvent(QResizeEvent* e) override;


protected:
    QGridLayout* layout;

    QPixmap *PixmapLayer1;/**图层1，显示网格**/
    QPixmap *PixmapLayer2;
    QPixmap *PixmapLayer3;

    PF_EventHandler* eventHandler;

    PF_EntityContainer* container;/**保存所有实体**/

    PF::RedrawMethod redrawMethod;
private:
    /**保存绘图过程当中的实体**/
    QMap<int, PF_EntityContainer *> overlayEntities;
protected:
    // property members:
    QRect mViewport;
    double mBufferDevicePixelRatio;
    QCPLayoutGrid *mPlotLayout;
    bool mAutoAddPlottableToLegend;
    QList<QCPAbstractPlottable*> mPlottables;
    QList<QCPGraph*> mGraphs; // extra list of plottables also in mPlottables that are of type QCPGraph
    QList<QCPAbstractItem*> mItems;
    QList<QCPLayer*> mLayers;
    QCP::AntialiasedElements mAntialiasedElements, mNotAntialiasedElements;
    QCP::Interactions mInteractions;
    int mSelectionTolerance;
    bool mNoAntialiasingOnDrag;
    QBrush mBackgroundBrush;
    QPixmap mBackgroundPixmap;
    QPixmap mScaledBackgroundPixmap;
    bool mBackgroundScaled;
    Qt::AspectRatioMode mBackgroundScaledMode;
    QCPLayer *mCurrentLayer;
    QCP::PlottingHints mPlottingHints;
    Qt::KeyboardModifier mMultiSelectModifier;
    QCP::SelectionRectMode mSelectionRectMode;
    QCPSelectionRect *mSelectionRect;
    bool mOpenGl;

    // non-property members:
    QList<QSharedPointer<QCPAbstractPaintBuffer> > mPaintBuffers;
    QPoint mMousePressPos;
    bool mMouseHasMoved;
    QPointer<QCPLayerable> mMouseEventLayerable;
    QPointer<QCPLayerable> mMouseSignalLayerable;
    QVariant mMouseEventLayerableDetails;
    QVariant mMouseSignalLayerableDetails;
    bool mReplotting;
    bool mReplotQueued;
    int mOpenGlMultisamples;
    QCP::AntialiasedElements mOpenGlAntialiasedElementsBackup;
    bool mOpenGlCacheLabelsBackup;
#ifdef QCP_OPENGL_FBO
    QSharedPointer<QOpenGLContext> mGlContext;
    QSharedPointer<QSurface> mGlSurface;
    QSharedPointer<QOpenGLPaintDevice> mGlPaintDevice;
#endif

    // reimplemented virtual methods:
    virtual QSize minimumSizeHint() const Q_DECL_OVERRIDE;
    virtual QSize sizeHint() const Q_DECL_OVERRIDE;
    //virtual void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    //virtual void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
    //virtual void mouseDoubleClickEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    //virtual void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    //virtual void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    //virtual void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    //virtual void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;

    // introduced virtual methods:
    virtual void draw(QCPPainter *painter);
    virtual void updateLayout();
    virtual void axisRemoved(QCPAxis *axis);
    virtual void legendRemoved(QCPLegend *legend);
    Q_SLOT virtual void processRectSelection(QRect rect, QMouseEvent *event);
    Q_SLOT virtual void processRectZoom(QRect rect, QMouseEvent *event);
    Q_SLOT virtual void processPointSelection(QMouseEvent *event);

    // non-virtual methods:
    bool registerPlottable(QCPAbstractPlottable *plottable);
    bool registerGraph(QCPGraph *graph);
    bool registerItem(QCPAbstractItem* item);
    void updateLayerIndices() const;
    QCPLayerable *layerableAt(const QPointF &pos, bool onlySelectable, QVariant *selectionDetails=0) const;
    QList<QCPLayerable*> layerableListAt(const QPointF &pos, bool onlySelectable, QList<QVariant> *selectionDetails=0) const;
    void drawBackground(QCPPainter *painter);
    void setupPaintBuffers();
    QCPAbstractPaintBuffer *createPaintBuffer();
    bool hasInvalidatedPaintBuffers();
    bool setupOpenGl();
    void freeOpenGl();

    friend class QCPLegend;
    friend class QCPAxis;
    friend class QCPLayer;
    friend class QCPAxisRect;
    friend class QCPAbstractPlottable;
    friend class QCPGraph;
    friend class QCPAbstractItem;
};
Q_DECLARE_METATYPE(PF_GraphicView::LayerInsertMode)
Q_DECLARE_METATYPE(PF_GraphicView::RefreshPriority)
#endif // PF_GRAPHICVIEW_H
