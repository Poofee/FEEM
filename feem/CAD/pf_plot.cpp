
#include "pf_plot.h"
#include "pf_graphicview.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPVector2D
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPVector2D
  \brief Represents two doubles as a mathematical 2D vector
  
  This class acts as a replacement for QVector2D with the advantage of double precision instead of
  single, and some convenience methods tailored for the PF_GraphicView library.
*/

/* start documentation of inline functions */

/*! \fn void QCPVector2D::setX(double x)
  
  Sets the x coordinate of this vector to \a x.
  
  \see setY
*/

/*! \fn void QCPVector2D::setY(double y)
  
  Sets the y coordinate of this vector to \a y.
  
  \see setX
*/

/*! \fn double QCPVector2D::length() const
  
  Returns the length of this vector.
  
  \see lengthSquared
*/

/*! \fn double QCPVector2D::lengthSquared() const
  
  Returns the squared length of this vector. In some situations, e.g. when just trying to find the
  shortest vector of a group, this is faster than calculating \ref length, because it avoids
  calculation of a square root.
  
  \see length
*/

/*! \fn QPoint QCPVector2D::toPoint() const
  
  Returns a QPoint which has the x and y coordinates of this vector, truncating any floating point
  information.
  
  \see toPointF
*/

/*! \fn QPointF QCPVector2D::toPointF() const
  
  Returns a QPointF which has the x and y coordinates of this vector.
  
  \see toPoint
*/

/*! \fn bool QCPVector2D::isNull() const
  
  Returns whether this vector is null. A vector is null if \c qIsNull returns true for both x and y
  coordinates, i.e. if both are binary equal to 0.
*/

/*! \fn QCPVector2D QCPVector2D::perpendicular() const
  
  Returns a vector perpendicular to this vector, with the same length.
*/

/*! \fn double QCPVector2D::dot() const
  
  Returns the dot/scalar product of this vector with the specified vector \a vec.
*/

/* end documentation of inline functions */

/*!
  Creates a QCPVector2D object and initializes the x and y coordinates to 0.
*/
QCPVector2D::QCPVector2D() :
    mX(0),
    mY(0)
{
}

/*!
  Creates a QCPVector2D object and initializes the \a x and \a y coordinates with the specified
  values.
*/
QCPVector2D::QCPVector2D(double x, double y) :
    mX(x),
    mY(y)
{
}

/*!
  Creates a QCPVector2D object and initializes the x and y coordinates respective coordinates of
  the specified \a point.
*/
QCPVector2D::QCPVector2D(const QPoint &point) :
    mX(point.x()),
    mY(point.y())
{
}

/*!
  Creates a QCPVector2D object and initializes the x and y coordinates respective coordinates of
  the specified \a point.
*/
QCPVector2D::QCPVector2D(const QPointF &point) :
    mX(point.x()),
    mY(point.y())
{
}

/*!
  Normalizes this vector. After this operation, the length of the vector is equal to 1.
  
  \see normalized, length, lengthSquared
*/
void QCPVector2D::normalize()
{
    double len = length();
    mX /= len;
    mY /= len;
}

/*!
  Returns a normalized version of this vector. The length of the returned vector is equal to 1.
  
  \see normalize, length, lengthSquared
*/
QCPVector2D QCPVector2D::normalized() const
{
    QCPVector2D result(mX, mY);
    result.normalize();
    return result;
}

/*! \overload
  
  Returns the squared shortest distance of this vector (interpreted as a point) to the finite line
  segment given by \a start and \a end.
  
  \see distanceToStraightLine
*/
double QCPVector2D::distanceSquaredToLine(const QCPVector2D &start, const QCPVector2D &end) const
{
    QCPVector2D v(end-start);
    double vLengthSqr = v.lengthSquared();
    if (!qFuzzyIsNull(vLengthSqr))
    {
        double mu = v.dot(*this-start)/vLengthSqr;
        if (mu < 0)
            return (*this-start).lengthSquared();
        else if (mu > 1)
            return (*this-end).lengthSquared();
        else
            return ((start + mu*v)-*this).lengthSquared();
    } else
        return (*this-start).lengthSquared();
}

/*! \overload
  
  Returns the squared shortest distance of this vector (interpreted as a point) to the finite line
  segment given by \a line.
  
  \see distanceToStraightLine
*/
double QCPVector2D::distanceSquaredToLine(const QLineF &line) const
{
    return distanceSquaredToLine(QCPVector2D(line.p1()), QCPVector2D(line.p2()));
}

/*!
  Returns the shortest distance of this vector (interpreted as a point) to the infinite straight
  line given by a \a base point and a \a direction vector.
  
  \see distanceSquaredToLine
*/
double QCPVector2D::distanceToStraightLine(const QCPVector2D &base, const QCPVector2D &direction) const
{
    return qAbs((*this-base).dot(direction.perpendicular()))/direction.length();
}

/*!
  Scales this vector by the given \a factor, i.e. the x and y components are multiplied by \a
  factor.
*/
QCPVector2D &QCPVector2D::operator*=(double factor)
{
    mX *= factor;
    mY *= factor;
    return *this;
}

/*!
  Scales this vector by the given \a divisor, i.e. the x and y components are divided by \a
  divisor.
*/
QCPVector2D &QCPVector2D::operator/=(double divisor)
{
    mX /= divisor;
    mY /= divisor;
    return *this;
}

/*!
  Adds the given \a vector to this vector component-wise.
*/
QCPVector2D &QCPVector2D::operator+=(const QCPVector2D &vector)
{
    mX += vector.mX;
    mY += vector.mY;
    return *this;
}

/*!
  subtracts the given \a vector from this vector component-wise.
*/
QCPVector2D &QCPVector2D::operator-=(const QCPVector2D &vector)
{
    mX -= vector.mX;
    mY -= vector.mY;
    return *this;
}
/* end of 'src/vector2d.cpp' */


/* including file 'src/painter.cpp', size 8670                               */
/* commit ce344b3f96a62e5f652585e55f1ae7c7883cd45b 2018-06-25 01:03:39 +0200 */

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPPainter
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPPainter
  \brief QPainter subclass used internally
  
  This QPainter subclass is used to provide some extended functionality e.g. for tweaking position
  consistency between antialiased and non-antialiased painting. Further it provides workarounds
  for QPainter quirks.
  
  \warning This class intentionally hides non-virtual functions of QPainter, e.g. setPen, save and
  restore. So while it is possible to pass a QCPPainter instance to a function that expects a
  QPainter pointer, some of the workarounds and tweaks will be unavailable to the function (because
  it will call the base class implementations of the functions actually hidden by QCPPainter).
*/

/*!
  Creates a new QCPPainter instance and sets default values
*/
QCPPainter::QCPPainter() :
    QPainter(),
    mModes(pmDefault),
    mIsAntialiasing(false)
{
    // don't setRenderHint(QPainter::NonCosmeticDefautPen) here, because painter isn't active yet and
    // a call to begin() will follow
}

/*!
  Creates a new QCPPainter instance on the specified paint \a device and sets default values. Just
  like the analogous QPainter constructor, begins painting on \a device immediately.
  
  Like \ref begin, this method sets QPainter::NonCosmeticDefaultPen in Qt versions before Qt5.
*/
QCPPainter::QCPPainter(QPaintDevice *device) :
    QPainter(device),
    mModes(pmDefault),
    mIsAntialiasing(false)
{
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0) // before Qt5, default pens used to be cosmetic if NonCosmeticDefaultPen flag isn't set. So we set it to get consistency across Qt versions.
    if (isActive())
        setRenderHint(QPainter::NonCosmeticDefaultPen);
#endif
}

/*!
  Sets the pen of the painter and applies certain fixes to it, depending on the mode of this
  QCPPainter.
  
  \note this function hides the non-virtual base class implementation.
*/
void QCPPainter::setPen(const QPen &pen)
{
    QPainter::setPen(pen);
    if (mModes.testFlag(pmNonCosmetic))
        makeNonCosmetic();
}

/*! \overload
  
  Sets the pen (by color) of the painter and applies certain fixes to it, depending on the mode of
  this QCPPainter.
  
  \note this function hides the non-virtual base class implementation.
*/
void QCPPainter::setPen(const QColor &color)
{
    QPainter::setPen(color);
    if (mModes.testFlag(pmNonCosmetic))
        makeNonCosmetic();
}

/*! \overload
  
  Sets the pen (by style) of the painter and applies certain fixes to it, depending on the mode of
  this QCPPainter.
  
  \note this function hides the non-virtual base class implementation.
*/
void QCPPainter::setPen(Qt::PenStyle penStyle)
{
    QPainter::setPen(penStyle);
    if (mModes.testFlag(pmNonCosmetic))
        makeNonCosmetic();
}

/*! \overload
  
  Works around a Qt bug introduced with Qt 4.8 which makes drawing QLineF unpredictable when
  antialiasing is disabled. Thus when antialiasing is disabled, it rounds the \a line to
  integer coordinates and then passes it to the original drawLine.
  
  \note this function hides the non-virtual base class implementation.
*/
void QCPPainter::drawLine(const QLineF &line)
{
    if (mIsAntialiasing || mModes.testFlag(pmVectorized))
        QPainter::drawLine(line);
    else
        QPainter::drawLine(line.toLine());
}

/*!
  Sets whether painting uses antialiasing or not. Use this method instead of using setRenderHint
  with QPainter::Antialiasing directly, as it allows QCPPainter to regain pixel exactness between
  antialiased and non-antialiased painting (Since Qt < 5.0 uses slightly different coordinate systems for
  AA/Non-AA painting).
*/
void QCPPainter::setAntialiasing(bool enabled)
{
    setRenderHint(QPainter::Antialiasing, enabled);
    if (mIsAntialiasing != enabled)
    {
        mIsAntialiasing = enabled;
        if (!mModes.testFlag(pmVectorized)) // antialiasing half-pixel shift only needed for rasterized outputs
        {
            if (mIsAntialiasing)
                translate(0.5, 0.5);
            else
                translate(-0.5, -0.5);
        }
    }
}

/*!
  Sets the mode of the painter. This controls whether the painter shall adjust its
  fixes/workarounds optimized for certain output devices.
*/
void QCPPainter::setModes(QCPPainter::PainterModes modes)
{
    mModes = modes;
}

/*!
  Sets the QPainter::NonCosmeticDefaultPen in Qt versions before Qt5 after beginning painting on \a
  device. This is necessary to get cosmetic pen consistency across Qt versions, because since Qt5,
  all pens are non-cosmetic by default, and in Qt4 this render hint must be set to get that
  behaviour.
  
  The Constructor \ref QCPPainter(QPaintDevice *device) which directly starts painting also sets
  the render hint as appropriate.
  
  \note this function hides the non-virtual base class implementation.
*/
bool QCPPainter::begin(QPaintDevice *device)
{
    bool result = QPainter::begin(device);
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0) // before Qt5, default pens used to be cosmetic if NonCosmeticDefaultPen flag isn't set. So we set it to get consistency across Qt versions.
    if (result)
        setRenderHint(QPainter::NonCosmeticDefaultPen);
#endif
    return result;
}

/*! \overload
  
  Sets the mode of the painter. This controls whether the painter shall adjust its
  fixes/workarounds optimized for certain output devices.
*/
void QCPPainter::setMode(QCPPainter::PainterMode mode, bool enabled)
{
    if (!enabled && mModes.testFlag(mode))
        mModes &= ~mode;
    else if (enabled && !mModes.testFlag(mode))
        mModes |= mode;
}

/*!
  Saves the painter (see QPainter::save). Since QCPPainter adds some new internal state to
  QPainter, the save/restore functions are reimplemented to also save/restore those members.
  
  \note this function hides the non-virtual base class implementation.
  
  \see restore
*/
void QCPPainter::save()
{
    mAntialiasingStack.push(mIsAntialiasing);
    QPainter::save();
}

/*!
  Restores the painter (see QPainter::restore). Since QCPPainter adds some new internal state to
  QPainter, the save/restore functions are reimplemented to also save/restore those members.
  
  \note this function hides the non-virtual base class implementation.
  
  \see save
*/
void QCPPainter::restore()
{
    if (!mAntialiasingStack.isEmpty())
        mIsAntialiasing = mAntialiasingStack.pop();
    else
        qDebug() << Q_FUNC_INFO << "Unbalanced save/restore";
    QPainter::restore();
}

/*!
  Changes the pen width to 1 if it currently is 0. This function is called in the \ref setPen
  overrides when the \ref pmNonCosmetic mode is set.
*/
void QCPPainter::makeNonCosmetic()
{
    if (qFuzzyIsNull(pen().widthF()))
    {
        QPen p = pen();
        p.setWidth(1);
        QPainter::setPen(p);
    }
}
/* end of 'src/painter.cpp' */


/* including file 'src/paintbuffer.cpp', size 18502                          */
/* commit ce344b3f96a62e5f652585e55f1ae7c7883cd45b 2018-06-25 01:03:39 +0200 */

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPAbstractPaintBuffer
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPAbstractPaintBuffer
  \brief The abstract base class for paint buffers, which define the rendering backend

  This abstract base class defines the basic interface that a paint buffer needs to provide in
  order to be usable by PF_GraphicView.

  A paint buffer manages both a surface to draw onto, and the matching paint device. The size of
  the surface can be changed via \ref setSize. External classes (\ref PF_GraphicView and \ref
  QCPLayer) request a painter via \ref startPainting and then perform the draw calls. Once the
  painting is complete, \ref donePainting is called, so the paint buffer implementation can do
  clean up if necessary. Before rendering a frame, each paint buffer is usually filled with a color
  using \ref clear (usually the color is \c Qt::transparent), to remove the contents of the
  previous frame.

  The simplest paint buffer implementation is \ref QCPPaintBufferPixmap which allows regular
  software rendering via the raster engine. Hardware accelerated rendering via pixel buffers and
  frame buffer objects is provided by \ref QCPPaintBufferGlPbuffer and \ref QCPPaintBufferGlFbo.
  They are used automatically if \ref PF_GraphicView::setOpenGl is enabled.
*/

/* start documentation of pure virtual functions */

/*! \fn virtual QCPPainter *QCPAbstractPaintBuffer::startPainting() = 0

  Returns a \ref QCPPainter which is ready to draw to this buffer. The ownership and thus the
  responsibility to delete the painter after the painting operations are complete is given to the
  caller of this method.

  Once you are done using the painter, delete the painter and call \ref donePainting.

  While a painter generated with this method is active, you must not call \ref setSize, \ref
  setDevicePixelRatio or \ref clear.

  This method may return 0, if a painter couldn't be activated on the buffer. This usually
  indicates a problem with the respective painting backend.
*/

/*! \fn virtual void QCPAbstractPaintBuffer::draw(QCPPainter *painter) const = 0

  Draws the contents of this buffer with the provided \a painter. This is the method that is used
  to finally join all paint buffers and draw them onto the screen.
*/

/*! \fn virtual void QCPAbstractPaintBuffer::clear(const QColor &color) = 0

  Fills the entire buffer with the provided \a color. To have an empty transparent buffer, use the
  named color \c Qt::transparent.

  This method must not be called if there is currently a painter (acquired with \ref startPainting)
  active.
*/

/*! \fn virtual void QCPAbstractPaintBuffer::reallocateBuffer() = 0

  Reallocates the internal buffer with the currently configured size (\ref setSize) and device
  pixel ratio, if applicable (\ref setDevicePixelRatio). It is called as soon as any of those
  properties are changed on this paint buffer.

  \note Subclasses of \ref QCPAbstractPaintBuffer must call their reimplementation of this method
  in their constructor, to perform the first allocation (this can not be done by the base class
  because calling pure virtual methods in base class constructors is not possible).
*/

/* end documentation of pure virtual functions */
/* start documentation of inline functions */

/*! \fn virtual void QCPAbstractPaintBuffer::donePainting()

  If you have acquired a \ref QCPPainter to paint onto this paint buffer via \ref startPainting,
  call this method as soon as you are done with the painting operations and have deleted the
  painter.

  paint buffer subclasses may use this method to perform any type of cleanup that is necessary. The
  default implementation does nothing.
*/

/* end documentation of inline functions */

/*!
  Creates a paint buffer and initializes it with the provided \a size and \a devicePixelRatio.

  Subclasses must call their \ref reallocateBuffer implementation in their respective constructors.
*/
QCPAbstractPaintBuffer::QCPAbstractPaintBuffer(const QSize &size, double devicePixelRatio) :
    mSize(size),
    mDevicePixelRatio(devicePixelRatio),
    mInvalidated(true)
{
}

QCPAbstractPaintBuffer::~QCPAbstractPaintBuffer()
{
}

/*!
  Sets the paint buffer size.

  The buffer is reallocated (by calling \ref reallocateBuffer), so any painters that were obtained
  by \ref startPainting are invalidated and must not be used after calling this method.

  If \a size is already the current buffer size, this method does nothing.
*/
void QCPAbstractPaintBuffer::setSize(const QSize &size)
{
    if (mSize != size)
    {
        mSize = size;
        reallocateBuffer();
    }
}

/*!
  Sets the invalidated flag to \a invalidated.

  This mechanism is used internally in conjunction with isolated replotting of \ref QCPLayer
  instances (in \ref QCPLayer::lmBuffered mode). If \ref QCPLayer::replot is called on a buffered
  layer, i.e. an isolated repaint of only that layer (and its dedicated paint buffer) is requested,
  PF_GraphicView will decide depending on the invalidated flags of other paint buffers whether it also
  replots them, instead of only the layer on which the replot was called.

  The invalidated flag is set to true when \ref QCPLayer association has changed, i.e. if layers
  were added or removed from this buffer, or if they were reordered. It is set to false as soon as
  all associated \ref QCPLayer instances are drawn onto the buffer.

  Under normal circumstances, it is not necessary to manually call this method.
*/
void QCPAbstractPaintBuffer::setInvalidated(bool invalidated)
{
    mInvalidated = invalidated;
}

/*!
  Sets the the device pixel ratio to \a ratio. This is useful to render on high-DPI output devices.
  The ratio is automatically set to the device pixel ratio used by the parent PF_GraphicView instance.

  The buffer is reallocated (by calling \ref reallocateBuffer), so any painters that were obtained
  by \ref startPainting are invalidated and must not be used after calling this method.

  \note This method is only available for Qt versions 5.4 and higher.
*/
void QCPAbstractPaintBuffer::setDevicePixelRatio(double ratio)
{
    if (!qFuzzyCompare(ratio, mDevicePixelRatio))
    {
#ifdef QCP_DEVICEPIXELRATIO_SUPPORTED
        mDevicePixelRatio = ratio;
        reallocateBuffer();
#else
        qDebug() << Q_FUNC_INFO << "Device pixel ratios not supported for Qt versions before 5.4";
        mDevicePixelRatio = 1.0;
#endif
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPPaintBufferPixmap
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPPaintBufferPixmap
  \brief A paint buffer based on QPixmap, using software raster rendering

  This paint buffer is the default and fall-back paint buffer which uses software rendering and
  QPixmap as internal buffer. It is used if \ref PF_GraphicView::setOpenGl is false.
*/

/*!
  Creates a pixmap paint buffer instancen with the specified \a size and \a devicePixelRatio, if
  applicable.
*/
QCPPaintBufferPixmap::QCPPaintBufferPixmap(const QSize &size, double devicePixelRatio) :
    QCPAbstractPaintBuffer(size, devicePixelRatio)
{
    QCPPaintBufferPixmap::reallocateBuffer();
}

QCPPaintBufferPixmap::~QCPPaintBufferPixmap()
{
}

/* inherits documentation from base class */
QCPPainter *QCPPaintBufferPixmap::startPainting()
{
    QCPPainter *result = new QCPPainter(&mBuffer);
    result->setRenderHint(QPainter::HighQualityAntialiasing);
    return result;
}

/* inherits documentation from base class */
void QCPPaintBufferPixmap::draw(QCPPainter *painter) const
{
    if (painter && painter->isActive())
        painter->drawPixmap(0, 0, mBuffer);
    else
        qDebug() << Q_FUNC_INFO << "invalid or inactive painter passed";
}

/* inherits documentation from base class */
void QCPPaintBufferPixmap::clear(const QColor &color)
{
    mBuffer.fill(color);
}

/* inherits documentation from base class */
void QCPPaintBufferPixmap::reallocateBuffer()
{
    setInvalidated();
    if (!qFuzzyCompare(1.0, mDevicePixelRatio))
    {
#ifdef QCP_DEVICEPIXELRATIO_SUPPORTED
        mBuffer = QPixmap(mSize*mDevicePixelRatio);
        mBuffer.setDevicePixelRatio(mDevicePixelRatio);
#else
        qDebug() << Q_FUNC_INFO << "Device pixel ratios not supported for Qt versions before 5.4";
        mDevicePixelRatio = 1.0;
        mBuffer = QPixmap(mSize);
#endif
    } else
    {
        mBuffer = QPixmap(mSize);
    }
}


#ifdef QCP_OPENGL_PBUFFER
////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPPaintBufferGlPbuffer
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPPaintBufferGlPbuffer
  \brief A paint buffer based on OpenGL pixel buffers, using hardware accelerated rendering

  This paint buffer is one of the OpenGL paint buffers which facilitate hardware accelerated plot
  rendering. It is based on OpenGL pixel buffers (pbuffer) and is used in Qt versions before 5.0.
  (See \ref QCPPaintBufferGlFbo used in newer Qt versions.)

  The OpenGL paint buffers are used if \ref PF_GraphicView::setOpenGl is set to true, and if they are
  supported by the system.
*/

/*!
  Creates a \ref QCPPaintBufferGlPbuffer instance with the specified \a size and \a
  devicePixelRatio, if applicable.

  The parameter \a multisamples defines how many samples are used per pixel. Higher values thus
  result in higher quality antialiasing. If the specified \a multisamples value exceeds the
  capability of the graphics hardware, the highest supported multisampling is used.
*/
QCPPaintBufferGlPbuffer::QCPPaintBufferGlPbuffer(const QSize &size, double devicePixelRatio, int multisamples) :
    QCPAbstractPaintBuffer(size, devicePixelRatio),
    mGlPBuffer(0),
    mMultisamples(qMax(0, multisamples))
{
    QCPPaintBufferGlPbuffer::reallocateBuffer();
}

QCPPaintBufferGlPbuffer::~QCPPaintBufferGlPbuffer()
{
    if (mGlPBuffer)
        delete mGlPBuffer;
}

/* inherits documentation from base class */
QCPPainter *QCPPaintBufferGlPbuffer::startPainting()
{
    if (!mGlPBuffer->isValid())
    {
        qDebug() << Q_FUNC_INFO << "OpenGL frame buffer object doesn't exist, reallocateBuffer was not called?";
        return 0;
    }

    QCPPainter *result = new QCPPainter(mGlPBuffer);
    result->setRenderHint(QPainter::HighQualityAntialiasing);
    return result;
}

/* inherits documentation from base class */
void QCPPaintBufferGlPbuffer::draw(QCPPainter *painter) const
{
    if (!painter || !painter->isActive())
    {
        qDebug() << Q_FUNC_INFO << "invalid or inactive painter passed";
        return;
    }
    if (!mGlPBuffer->isValid())
    {
        qDebug() << Q_FUNC_INFO << "OpenGL pbuffer isn't valid, reallocateBuffer was not called?";
        return;
    }
    painter->drawImage(0, 0, mGlPBuffer->toImage());
}

/* inherits documentation from base class */
void QCPPaintBufferGlPbuffer::clear(const QColor &color)
{
    if (mGlPBuffer->isValid())
    {
        mGlPBuffer->makeCurrent();
        glClearColor(color.redF(), color.greenF(), color.blueF(), color.alphaF());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        mGlPBuffer->doneCurrent();
    } else
        qDebug() << Q_FUNC_INFO << "OpenGL pbuffer invalid or context not current";
}

/* inherits documentation from base class */
void QCPPaintBufferGlPbuffer::reallocateBuffer()
{
    if (mGlPBuffer)
        delete mGlPBuffer;

    QGLFormat format;
    format.setAlpha(true);
    format.setSamples(mMultisamples);
    mGlPBuffer = new QGLPixelBuffer(mSize, format);
}
#endif // QCP_OPENGL_PBUFFER


#ifdef QCP_OPENGL_FBO
////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPPaintBufferGlFbo
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPPaintBufferGlFbo
  \brief A paint buffer based on OpenGL frame buffers objects, using hardware accelerated rendering

  This paint buffer is one of the OpenGL paint buffers which facilitate hardware accelerated plot
  rendering. It is based on OpenGL frame buffer objects (fbo) and is used in Qt versions 5.0 and
  higher. (See \ref QCPPaintBufferGlPbuffer used in older Qt versions.)

  The OpenGL paint buffers are used if \ref PF_GraphicView::setOpenGl is set to true, and if they are
  supported by the system.
*/

/*!
  Creates a \ref QCPPaintBufferGlFbo instance with the specified \a size and \a devicePixelRatio,
  if applicable.

  All frame buffer objects shall share one OpenGL context and paint device, which need to be set up
  externally and passed via \a glContext and \a glPaintDevice. The set-up is done in \ref
  PF_GraphicView::setupOpenGl and the context and paint device are managed by the parent PF_GraphicView
  instance.
*/
QCPPaintBufferGlFbo::QCPPaintBufferGlFbo(const QSize &size, double devicePixelRatio, QWeakPointer<QOpenGLContext> glContext, QWeakPointer<QOpenGLPaintDevice> glPaintDevice) :
    QCPAbstractPaintBuffer(size, devicePixelRatio),
    mGlContext(glContext),
    mGlPaintDevice(glPaintDevice),
    mGlFrameBuffer(0)
{
    QCPPaintBufferGlFbo::reallocateBuffer();
}

QCPPaintBufferGlFbo::~QCPPaintBufferGlFbo()
{
    if (mGlFrameBuffer)
        delete mGlFrameBuffer;
}

/* inherits documentation from base class */
QCPPainter *QCPPaintBufferGlFbo::startPainting()
{
    if (mGlPaintDevice.isNull())
    {
        qDebug() << Q_FUNC_INFO << "OpenGL paint device doesn't exist";
        return 0;
    }
    if (!mGlFrameBuffer)
    {
        qDebug() << Q_FUNC_INFO << "OpenGL frame buffer object doesn't exist, reallocateBuffer was not called?";
        return 0;
    }

    if (QOpenGLContext::currentContext() != mGlContext.data())
        mGlContext.data()->makeCurrent(mGlContext.data()->surface());
    mGlFrameBuffer->bind();
    QCPPainter *result = new QCPPainter(mGlPaintDevice.data());
    result->setRenderHint(QPainter::HighQualityAntialiasing);
    return result;
}

/* inherits documentation from base class */
void QCPPaintBufferGlFbo::donePainting()
{
    if (mGlFrameBuffer && mGlFrameBuffer->isBound())
        mGlFrameBuffer->release();
    else
        qDebug() << Q_FUNC_INFO << "Either OpenGL frame buffer not valid or was not bound";
}

/* inherits documentation from base class */
void QCPPaintBufferGlFbo::draw(QCPPainter *painter) const
{
    if (!painter || !painter->isActive())
    {
        qDebug() << Q_FUNC_INFO << "invalid or inactive painter passed";
        return;
    }
    if (!mGlFrameBuffer)
    {
        qDebug() << Q_FUNC_INFO << "OpenGL frame buffer object doesn't exist, reallocateBuffer was not called?";
        return;
    }
    painter->drawImage(0, 0, mGlFrameBuffer->toImage());
}

/* inherits documentation from base class */
void QCPPaintBufferGlFbo::clear(const QColor &color)
{
    if (mGlContext.isNull())
    {
        qDebug() << Q_FUNC_INFO << "OpenGL context doesn't exist";
        return;
    }
    if (!mGlFrameBuffer)
    {
        qDebug() << Q_FUNC_INFO << "OpenGL frame buffer object doesn't exist, reallocateBuffer was not called?";
        return;
    }

    if (QOpenGLContext::currentContext() != mGlContext.data())
        mGlContext.data()->makeCurrent(mGlContext.data()->surface());
    mGlFrameBuffer->bind();
    glClearColor(color.redF(), color.greenF(), color.blueF(), color.alphaF());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mGlFrameBuffer->release();
}

/* inherits documentation from base class */
void QCPPaintBufferGlFbo::reallocateBuffer()
{
    // release and delete possibly existing framebuffer:
    if (mGlFrameBuffer)
    {
        if (mGlFrameBuffer->isBound())
            mGlFrameBuffer->release();
        delete mGlFrameBuffer;
        mGlFrameBuffer = 0;
    }

    if (mGlContext.isNull())
    {
        qDebug() << Q_FUNC_INFO << "OpenGL context doesn't exist";
        return;
    }
    if (mGlPaintDevice.isNull())
    {
        qDebug() << Q_FUNC_INFO << "OpenGL paint device doesn't exist";
        return;
    }

    // create new fbo with appropriate size:
    mGlContext.data()->makeCurrent(mGlContext.data()->surface());
    QOpenGLFramebufferObjectFormat frameBufferFormat;
    frameBufferFormat.setSamples(mGlContext.data()->format().samples());
    frameBufferFormat.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    mGlFrameBuffer = new QOpenGLFramebufferObject(mSize*mDevicePixelRatio, frameBufferFormat);
    if (mGlPaintDevice.data()->size() != mSize*mDevicePixelRatio)
        mGlPaintDevice.data()->setSize(mSize*mDevicePixelRatio);
#ifdef QCP_DEVICEPIXELRATIO_SUPPORTED
    mGlPaintDevice.data()->setDevicePixelRatio(mDevicePixelRatio);
#endif
}
#endif // QCP_OPENGL_FBO
/* end of 'src/paintbuffer.cpp' */


/* including file 'src/layer.cpp', size 37304                                */
/* commit ce344b3f96a62e5f652585e55f1ae7c7883cd45b 2018-06-25 01:03:39 +0200 */

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPLayer
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPLayer
  \brief A layer that may contain objects, to control the rendering order

  The Layering system of PF_GraphicView is the mechanism to control the rendering order of the
  elements inside the plot.

  It is based on the two classes QCPLayer and QCPLayerable. PF_GraphicView holds an ordered list of
  one or more instances of QCPLayer (see PF_GraphicView::addLayer, PF_GraphicView::layer,
  PF_GraphicView::moveLayer, etc.). When replotting, PF_GraphicView goes through the list of layers
  bottom to top and successively draws the layerables of the layers into the paint buffer(s).

  A QCPLayer contains an ordered list of QCPLayerable instances. QCPLayerable is an abstract base
  class from which almost all visible objects derive, like axes, grids, graphs, items, etc.

  \section qcplayer-defaultlayers Default layers

  Initially, PF_GraphicView has six layers: "background", "grid", "main", "axes", "legend" and
  "overlay" (in that order). On top is the "overlay" layer, which only contains the PF_GraphicView's
  selection rect (\ref PF_GraphicView::selectionRect). The next two layers "axes" and "legend" contain
  the default axes and legend, so they will be drawn above plottables. In the middle, there is the
  "main" layer. It is initially empty and set as the current layer (see
  PF_GraphicView::setCurrentLayer). This means, all new plottables, items etc. are created on this
  layer by default. Then comes the "grid" layer which contains the QCPGrid instances (which belong
  tightly to QCPAxis, see \ref QCPAxis::grid). The Axis rect background shall be drawn behind
  everything else, thus the default QCPAxisRect instance is placed on the "background" layer. Of
  course, the layer affiliation of the individual objects can be changed as required (\ref
  QCPLayerable::setLayer).

  \section qcplayer-ordering Controlling the rendering order via layers

  Controlling the ordering of layerables in the plot is easy: Create a new layer in the position
  you want the layerable to be in, e.g. above "main", with \ref PF_GraphicView::addLayer. Then set the
  current layer with \ref PF_GraphicView::setCurrentLayer to that new layer and finally create the
  objects normally. They will be placed on the new layer automatically, due to the current layer
  setting. Alternatively you could have also ignored the current layer setting and just moved the
  objects with \ref QCPLayerable::setLayer to the desired layer after creating them.

  It is also possible to move whole layers. For example, If you want the grid to be shown in front
  of all plottables/items on the "main" layer, just move it above "main" with
  PF_GraphicView::moveLayer.

  The rendering order within one layer is simply by order of creation or insertion. The item
  created last (or added last to the layer), is drawn on top of all other objects on that layer.

  When a layer is deleted, the objects on it are not deleted with it, but fall on the layer below
  the deleted layer, see PF_GraphicView::removeLayer.

  \section qcplayer-buffering Replotting only a specific layer

  If the layer mode (\ref setMode) is set to \ref lmBuffered, you can replot only this specific
  layer by calling \ref replot. In certain situations this can provide better replot performance,
  compared with a full replot of all layers. Upon creation of a new layer, the layer mode is
  initialized to \ref lmLogical. The only layer that is set to \ref lmBuffered in a new \ref
  PF_GraphicView instance is the "overlay" layer, containing the selection rect.
*/

/* start documentation of inline functions */

/*! \fn QList<QCPLayerable*> QCPLayer::children() const
  
  Returns a list of all layerables on this layer. The order corresponds to the rendering order:
  layerables with higher indices are drawn above layerables with lower indices.
*/

/*! \fn int QCPLayer::index() const
  
  Returns the index this layer has in the PF_GraphicView. The index is the integer number by which this layer can be
  accessed via \ref PF_GraphicView::layer.
  
  Layers with higher indices will be drawn above layers with lower indices.
*/

/* end documentation of inline functions */

/*!
  Creates a new QCPLayer instance.
  
  Normally you shouldn't directly instantiate layers, use \ref PF_GraphicView::addLayer instead.
  
  \warning It is not checked that \a layerName is actually a unique layer name in \a parentPlot.
  This check is only performed by \ref PF_GraphicView::addLayer.
*/
QCPLayer::QCPLayer(PF_GraphicView *parentPlot, const QString &layerName) :
    QObject(parentPlot),
    mParentPlot(parentPlot),
    mName(layerName),
    mIndex(-1), // will be set to a proper value by the PF_GraphicView layer creation function
    mVisible(true),
    mMode(lmLogical)
{
    // Note: no need to make sure layerName is unique, because layer
    // management is done with PF_GraphicView functions.
}

QCPLayer::~QCPLayer()
{
    // If child layerables are still on this layer, detach them, so they don't try to reach back to this
    // then invalid layer once they get deleted/moved themselves. This only happens when layers are deleted
    // directly, like in the PF_GraphicView destructor. (The regular layer removal procedure for the user is to
    // call PF_GraphicView::removeLayer, which moves all layerables off this layer before deleting it.)

    while (!mChildren.isEmpty())
        mChildren.last()->setLayer(nullptr); // removes itself from mChildren via removeChild()

    if (mParentPlot->currentLayer() == this)
        qDebug() << Q_FUNC_INFO << "The parent plot's mCurrentLayer will be a dangling pointer. Should have been set to a valid layer or 0 beforehand.";
}

/*!
  Sets whether this layer is visible or not. If \a visible is set to false, all layerables on this
  layer will be invisible.

  This function doesn't change the visibility property of the layerables (\ref
  QCPLayerable::setVisible), but the \ref QCPLayerable::realVisibility of each layerable takes the
  visibility of the parent layer into account.
*/
void QCPLayer::setVisible(bool visible)
{
    mVisible = visible;
}

/*!
  Sets the rendering mode of this layer.

  If \a mode is set to \ref lmBuffered for a layer, it will be given a dedicated paint buffer by
  the parent PF_GraphicView instance. This means it may be replotted individually by calling \ref
  QCPLayer::replot, without needing to replot all other layers.

  Layers which are set to \ref lmLogical (the default) are used only to define the rendering order
  and can't be replotted individually.

  Note that each layer which is set to \ref lmBuffered requires additional paint buffers for the
  layers below, above and for the layer itself. This increases the memory consumption and
  (slightly) decreases the repainting speed because multiple paint buffers need to be joined. So
  you should carefully choose which layers benefit from having their own paint buffer. A typical
  example would be a layer which contains certain layerables (e.g. items) that need to be changed
  and thus replotted regularly, while all other layerables on other layers stay static. By default,
  only the topmost layer called "overlay" is in mode \ref lmBuffered, and contains the selection
  rect.

  \see replot
*/
void QCPLayer::setMode(QCPLayer::LayerMode mode)
{
    if (mMode != mode)
    {
        mMode = mode;
        if (!mPaintBuffer.isNull())
            mPaintBuffer.data()->setInvalidated();
    }
}

/*! \internal

  Draws the contents of this layer with the provided \a painter.

  \see replot, drawToPaintBuffer
*/
void QCPLayer::draw(QCPPainter *painter)
{
    foreach (QCPLayerable *child, mChildren)
    {
        if (child->realVisibility())
        {
            painter->save();
            painter->setClipRect(child->clipRect().translated(0, -1));
            child->applyDefaultAntialiasingHint(painter);
            child->draw(painter);
            painter->restore();
        }
    }
}

/*! \internal

  Draws the contents of this layer into the paint buffer which is associated with this layer. The
  association is established by the parent PF_GraphicView, which manages all paint buffers (see \ref
  PF_GraphicView::setupPaintBuffers).

  \see draw
*/
void QCPLayer::drawToPaintBuffer()
{
    if (!mPaintBuffer.isNull())
    {
        if (QCPPainter *painter = mPaintBuffer.data()->startPainting())
        {
            if (painter->isActive())
                draw(painter);
            else
                qDebug() << Q_FUNC_INFO << "paint buffer returned inactive painter";
            delete painter;
            mPaintBuffer.data()->donePainting();
        } else
            qDebug() << Q_FUNC_INFO << "paint buffer returned zero painter";
    } else
        qDebug() << Q_FUNC_INFO << "no valid paint buffer associated with this layer";
}

/*!
  If the layer mode (\ref setMode) is set to \ref lmBuffered, this method allows replotting only
  the layerables on this specific layer, without the need to replot all other layers (as a call to
  \ref PF_GraphicView::replot would do).

  If the layer mode is \ref lmLogical however, this method simply calls \ref PF_GraphicView::replot on
  the parent PF_GraphicView instance.

  PF_GraphicView also makes sure to replot all layers instead of only this one, if the layer ordering
  has changed since the last full replot and the other paint buffers were thus invalidated.

  \see draw
*/
void QCPLayer::replot()
{
    if (mMode == lmBuffered && !mParentPlot->hasInvalidatedPaintBuffers())
    {
        if (!mPaintBuffer.isNull())
        {
            mPaintBuffer.data()->clear(Qt::transparent);
            drawToPaintBuffer();
            mPaintBuffer.data()->setInvalidated(false);
            mParentPlot->update();
        } else
            qDebug() << Q_FUNC_INFO << "no valid paint buffer associated with this layer";
    } else if (mMode == lmLogical)
        mParentPlot->replot();
}

/*! \internal
  
  Adds the \a layerable to the list of this layer. If \a prepend is set to true, the layerable will
  be prepended to the list, i.e. be drawn beneath the other layerables already in the list.
  
  This function does not change the \a mLayer member of \a layerable to this layer. (Use
  QCPLayerable::setLayer to change the layer of an object, not this function.)
  
  \see removeChild
*/
void QCPLayer::addChild(QCPLayerable *layerable, bool prepend)
{
    if (!mChildren.contains(layerable))
    {
        if (prepend)
            mChildren.prepend(layerable);
        else
            mChildren.append(layerable);
        if (!mPaintBuffer.isNull())
            mPaintBuffer.data()->setInvalidated();
    } else
        qDebug() << Q_FUNC_INFO << "layerable is already child of this layer" << reinterpret_cast<quintptr>(layerable);
}

/*! \internal
  
  Removes the \a layerable from the list of this layer.
  
  This function does not change the \a mLayer member of \a layerable. (Use QCPLayerable::setLayer
  to change the layer of an object, not this function.)
  
  \see addChild
*/
void QCPLayer::removeChild(QCPLayerable *layerable)
{
    if (mChildren.removeOne(layerable))
    {
        if (!mPaintBuffer.isNull())
            mPaintBuffer.data()->setInvalidated();
    } else
        qDebug() << Q_FUNC_INFO << "layerable is not child of this layer" << reinterpret_cast<quintptr>(layerable);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPLayerable
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPLayerable
  \brief Base class for all drawable objects
  
  This is the abstract base class most visible objects derive from, e.g. plottables, axes, grid
  etc.

  Every layerable is on a layer (QCPLayer) which allows controlling the rendering order by stacking
  the layers accordingly.
  
  For details about the layering mechanism, see the QCPLayer documentation.
*/

/* start documentation of inline functions */

/*! \fn QCPLayerable *QCPLayerable::parentLayerable() const

  Returns the parent layerable of this layerable. The parent layerable is used to provide
  visibility hierarchies in conjunction with the method \ref realVisibility. This way, layerables
  only get drawn if their parent layerables are visible, too.
  
  Note that a parent layerable is not necessarily also the QObject parent for memory management.
  Further, a layerable doesn't always have a parent layerable, so this function may return 0.
  
  A parent layerable is set implicitly when placed inside layout elements and doesn't need to be
  set manually by the user.
*/

/* end documentation of inline functions */
/* start documentation of pure virtual functions */

/*! \fn virtual void QCPLayerable::applyDefaultAntialiasingHint(QCPPainter *painter) const = 0
  \internal
  
  This function applies the default antialiasing setting to the specified \a painter, using the
  function \ref applyAntialiasingHint. It is the antialiasing state the painter is put in, when
  \ref draw is called on the layerable. If the layerable has multiple entities whose antialiasing
  setting may be specified individually, this function should set the antialiasing state of the
  most prominent entity. In this case however, the \ref draw function usually calls the specialized
  versions of this function before drawing each entity, effectively overriding the setting of the
  default antialiasing hint.
  
  <b>First example:</b> QCPGraph has multiple entities that have an antialiasing setting: The graph
  line, fills and scatters. Those can be configured via QCPGraph::setAntialiased,
  QCPGraph::setAntialiasedFill and QCPGraph::setAntialiasedScatters. Consequently, there isn't only
  the QCPGraph::applyDefaultAntialiasingHint function (which corresponds to the graph line's
  antialiasing), but specialized ones like QCPGraph::applyFillAntialiasingHint and
  QCPGraph::applyScattersAntialiasingHint. So before drawing one of those entities, QCPGraph::draw
  calls the respective specialized applyAntialiasingHint function.
  
  <b>Second example:</b> QCPItemLine consists only of a line so there is only one antialiasing
  setting which can be controlled with QCPItemLine::setAntialiased. (This function is inherited by
  all layerables. The specialized functions, as seen on QCPGraph, must be added explicitly to the
  respective layerable subclass.) Consequently it only has the normal
  QCPItemLine::applyDefaultAntialiasingHint. The \ref QCPItemLine::draw function doesn't need to
  care about setting any antialiasing states, because the default antialiasing hint is already set
  on the painter when the \ref draw function is called, and that's the state it wants to draw the
  line with.
*/

/*! \fn virtual void QCPLayerable::draw(QCPPainter *painter) const = 0
  \internal
  
  This function draws the layerable with the specified \a painter. It is only called by
  PF_GraphicView, if the layerable is visible (\ref setVisible).
  
  Before this function is called, the painter's antialiasing state is set via \ref
  applyDefaultAntialiasingHint, see the documentation there. Further, the clipping rectangle was
  set to \ref clipRect.
*/

/* end documentation of pure virtual functions */
/* start documentation of signals */

/*! \fn void QCPLayerable::layerChanged(QCPLayer *newLayer);
  
  This signal is emitted when the layer of this layerable changes, i.e. this layerable is moved to
  a different layer.
  
  \see setLayer
*/

/* end documentation of signals */

/*!
  Creates a new QCPLayerable instance.
  
  Since QCPLayerable is an abstract base class, it can't be instantiated directly. Use one of the
  derived classes.
  
  If \a plot is provided, it automatically places itself on the layer named \a targetLayer. If \a
  targetLayer is an empty string, it places itself on the current layer of the plot (see \ref
  PF_GraphicView::setCurrentLayer).
  
  It is possible to provide 0 as \a plot. In that case, you should assign a parent plot at a later
  time with \ref initializeParentPlot.
  
  The layerable's parent layerable is set to \a parentLayerable, if provided. Direct layerable
  parents are mainly used to control visibility in a hierarchy of layerables. This means a
  layerable is only drawn, if all its ancestor layerables are also visible. Note that \a
  parentLayerable does not become the QObject-parent (for memory management) of this layerable, \a
  plot does. It is not uncommon to set the QObject-parent to something else in the constructors of
  QCPLayerable subclasses, to guarantee a working destruction hierarchy.
*/
QCPLayerable::QCPLayerable(PF_GraphicView *plot, QString targetLayer, QCPLayerable *parentLayerable) :
    QObject(plot),
    mVisible(true),
    mParentPlot(plot),
    mParentLayerable(parentLayerable),
    mLayer(nullptr),
    mAntialiased(true)
{
    if (mParentPlot)
    {
        if (targetLayer.isEmpty())
            setLayer(mParentPlot->currentLayer());
        else if (!setLayer(targetLayer))
            qDebug() << Q_FUNC_INFO << "setting QCPlayerable initial layer to" << targetLayer << "failed.";
    }
}

QCPLayerable::~QCPLayerable()
{
    if (mLayer)
    {
        mLayer->removeChild(this);
        mLayer = nullptr;
    }
}

/*!
  Sets the visibility of this layerable object. If an object is not visible, it will not be drawn
  on the PF_GraphicView surface, and user interaction with it (e.g. click and selection) is not
  possible.
*/
void QCPLayerable::setVisible(bool on)
{
    mVisible = on;
}

/*!
  Sets the \a layer of this layerable object. The object will be placed on top of the other objects
  already on \a layer.
  
  If \a layer is 0, this layerable will not be on any layer and thus not appear in the plot (or
  interact/receive events).
  
  Returns true if the layer of this layerable was successfully changed to \a layer.
*/
bool QCPLayerable::setLayer(QCPLayer *layer)
{
    return moveToLayer(layer, false);
}

/*! \overload
  Sets the layer of this layerable object by name
  
  Returns true on success, i.e. if \a layerName is a valid layer name.
*/
bool QCPLayerable::setLayer(const QString &layerName)
{
    if (!mParentPlot)
    {
        qDebug() << Q_FUNC_INFO << "no parent PF_GraphicView set";
        return false;
    }
    if (QCPLayer *layer = mParentPlot->layer(layerName))
    {
        return setLayer(layer);
    } else
    {
        qDebug() << Q_FUNC_INFO << "there is no layer with name" << layerName;
        return false;
    }
}

/*!
  Sets whether this object will be drawn antialiased or not.
  
  Note that antialiasing settings may be overridden by PF_GraphicView::setAntialiasedElements and
  PF_GraphicView::setNotAntialiasedElements.
*/
void QCPLayerable::setAntialiased(bool enabled)
{
    mAntialiased = enabled;
}

/*!
  Returns whether this layerable is visible, taking the visibility of the layerable parent and the
  visibility of this layerable's layer into account. This is the method that is consulted to decide
  whether a layerable shall be drawn or not.
  
  If this layerable has a direct layerable parent (usually set via hierarchies implemented in
  subclasses, like in the case of \ref QCPLayoutElement), this function returns true only if this
  layerable has its visibility set to true and the parent layerable's \ref realVisibility returns
  true.
*/
bool QCPLayerable::realVisibility() const
{
    return mVisible && (!mLayer || mLayer->visible()) && (!mParentLayerable || mParentLayerable.data()->realVisibility());
}

/*!
  This function is used to decide whether a click hits a layerable object or not.

  \a pos is a point in pixel coordinates on the PF_GraphicView surface. This function returns the
  shortest pixel distance of this point to the object. If the object is either invisible or the
  distance couldn't be determined, -1.0 is returned. Further, if \a onlySelectable is true and the
  object is not selectable, -1.0 is returned, too.

  If the object is represented not by single lines but by an area like a \ref QCPItemText or the
  bars of a \ref QCPBars plottable, a click inside the area should also be considered a hit. In
  these cases this function thus returns a constant value greater zero but still below the parent
  plot's selection tolerance. (typically the selectionTolerance multiplied by 0.99).
  
  Providing a constant value for area objects allows selecting line objects even when they are
  obscured by such area objects, by clicking close to the lines (i.e. closer than
  0.99*selectionTolerance).
  
  The actual setting of the selection state is not done by this function. This is handled by the
  parent PF_GraphicView when the mouseReleaseEvent occurs, and the finally selected object is notified
  via the \ref selectEvent/\ref deselectEvent methods.
  
  \a details is an optional output parameter. Every layerable subclass may place any information
  in \a details. This information will be passed to \ref selectEvent when the parent PF_GraphicView
  decides on the basis of this selectTest call, that the object was successfully selected. The
  subsequent call to \ref selectEvent will carry the \a details. This is useful for multi-part
  objects (like QCPAxis). This way, a possibly complex calculation to decide which part was clicked
  is only done once in \ref selectTest. The result (i.e. the actually clicked part) can then be
  placed in \a details. So in the subsequent \ref selectEvent, the decision which part was
  selected doesn't have to be done a second time for a single selection operation.
  
  In the case of 1D Plottables (\ref QCPAbstractPlottable1D, like \ref QCPGraph or \ref QCPBars) \a
  details will be set to a \ref QCPDataSelection, describing the closest data point to \a pos.
  
  You may pass 0 as \a details to indicate that you are not interested in those selection details.
  
  \see selectEvent, deselectEvent, mousePressEvent, wheelEvent, PF_GraphicView::setInteractions,
  QCPAbstractPlottable1D::selectTestRect
*/
double QCPLayerable::selectTest(const QPointF &pos, bool onlySelectable, QVariant *details) const
{
    Q_UNUSED(pos)
    Q_UNUSED(onlySelectable)
    Q_UNUSED(details)
    return -1.0;
}

/*! \internal
  
  Sets the parent plot of this layerable. Use this function once to set the parent plot if you have
  passed 0 in the constructor. It can not be used to move a layerable from one PF_GraphicView to
  another one.
  
  Note that, unlike when passing a non-null parent plot in the constructor, this function does not
  make \a parentPlot the QObject-parent of this layerable. If you want this, call
  QObject::setParent(\a parentPlot) in addition to this function.
  
  Further, you will probably want to set a layer (\ref setLayer) after calling this function, to
  make the layerable appear on the PF_GraphicView.
  
  The parent plot change will be propagated to subclasses via a call to \ref parentPlotInitialized
  so they can react accordingly (e.g. also initialize the parent plot of child layerables, like
  QCPLayout does).
*/
void QCPLayerable::initializeParentPlot(PF_GraphicView *parentPlot)
{
    if (mParentPlot)
    {
        qDebug() << Q_FUNC_INFO << "called with mParentPlot already initialized";
        return;
    }

    if (!parentPlot)
        qDebug() << Q_FUNC_INFO << "called with parentPlot zero";

    mParentPlot = parentPlot;
    parentPlotInitialized(mParentPlot);
}

/*! \internal
  
  Sets the parent layerable of this layerable to \a parentLayerable. Note that \a parentLayerable does not
  become the QObject-parent (for memory management) of this layerable.
  
  The parent layerable has influence on the return value of the \ref realVisibility method. Only
  layerables with a fully visible parent tree will return true for \ref realVisibility, and thus be
  drawn.
  
  \see realVisibility
*/
void QCPLayerable::setParentLayerable(QCPLayerable *parentLayerable)
{
    mParentLayerable = parentLayerable;
}

/*! \internal
  
  Moves this layerable object to \a layer. If \a prepend is true, this object will be prepended to
  the new layer's list, i.e. it will be drawn below the objects already on the layer. If it is
  false, the object will be appended.
  
  Returns true on success, i.e. if \a layer is a valid layer.
*/
bool QCPLayerable::moveToLayer(QCPLayer *layer, bool prepend)
{
    if (layer && !mParentPlot)
    {
        qDebug() << Q_FUNC_INFO << "no parent PF_GraphicView set";
        return false;
    }
    if (layer && layer->parentPlot() != mParentPlot)
    {
        qDebug() << Q_FUNC_INFO << "layer" << layer->name() << "is not in same PF_GraphicView as this layerable";
        return false;
    }

    QCPLayer *oldLayer = mLayer;
    if (mLayer)
        mLayer->removeChild(this);
    mLayer = layer;
    if (mLayer)
        mLayer->addChild(this, prepend);
    if (mLayer != oldLayer)
        emit layerChanged(mLayer);
    return true;
}

/*! \internal

  Sets the QCPainter::setAntialiasing state on the provided \a painter, depending on the \a
  localAntialiased value as well as the overrides \ref PF_GraphicView::setAntialiasedElements and \ref
  PF_GraphicView::setNotAntialiasedElements. Which override enum this function takes into account is
  controlled via \a overrideElement.
*/
void QCPLayerable::applyAntialiasingHint(QCPPainter *painter, bool localAntialiased, QCP::AntialiasedElement overrideElement) const
{
    if (mParentPlot && mParentPlot->notAntialiasedElements().testFlag(overrideElement))
        painter->setAntialiasing(false);
    else if (mParentPlot && mParentPlot->antialiasedElements().testFlag(overrideElement))
        painter->setAntialiasing(true);
    else
        painter->setAntialiasing(localAntialiased);
}

/*! \internal

  This function is called by \ref initializeParentPlot, to allow subclasses to react on the setting
  of a parent plot. This is the case when 0 was passed as parent plot in the constructor, and the
  parent plot is set at a later time.
  
  For example, QCPLayoutElement/QCPLayout hierarchies may be created independently of any
  PF_GraphicView at first. When they are then added to a layout inside the PF_GraphicView, the top level
  element of the hierarchy gets its parent plot initialized with \ref initializeParentPlot. To
  propagate the parent plot to all the children of the hierarchy, the top level element then uses
  this function to pass the parent plot on to its child elements.
  
  The default implementation does nothing.
  
  \see initializeParentPlot
*/
void QCPLayerable::parentPlotInitialized(PF_GraphicView *parentPlot)
{
    Q_UNUSED(parentPlot)
}

/*! \internal

  Returns the selection category this layerable shall belong to. The selection category is used in
  conjunction with \ref PF_GraphicView::setInteractions to control which objects are selectable and
  which aren't.
  
  Subclasses that don't fit any of the normal \ref QCP::Interaction values can use \ref
  QCP::iSelectOther. This is what the default implementation returns.
  
  \see PF_GraphicView::setInteractions
*/
QCP::Interaction QCPLayerable::selectionCategory() const
{
    return QCP::iSelectOther;
}

/*! \internal
  
  Returns the clipping rectangle of this layerable object. By default, this is the viewport of the
  parent PF_GraphicView. Specific subclasses may reimplement this function to provide different
  clipping rects.
  
  The returned clipping rect is set on the painter before the draw function of the respective
  object is called.
*/
QRect QCPLayerable::clipRect() const
{
    if (mParentPlot)
        return mParentPlot->viewport();
    else
        return QRect();
}

/*! \internal
  
  This event is called when the layerable shall be selected, as a consequence of a click by the
  user. Subclasses should react to it by setting their selection state appropriately. The default
  implementation does nothing.
  
  \a event is the mouse event that caused the selection. \a additive indicates, whether the user
  was holding the multi-select-modifier while performing the selection (see \ref
  PF_GraphicView::setMultiSelectModifier). if \a additive is true, the selection state must be toggled
  (i.e. become selected when unselected and unselected when selected).
  
  Every selectEvent is preceded by a call to \ref selectTest, which has returned positively (i.e.
  returned a value greater than 0 and less than the selection tolerance of the parent PF_GraphicView).
  The \a details data you output from \ref selectTest is fed back via \a details here. You may
  use it to transport any kind of information from the selectTest to the possibly subsequent
  selectEvent. Usually \a details is used to transfer which part was clicked, if it is a layerable
  that has multiple individually selectable parts (like QCPAxis). This way selectEvent doesn't need
  to do the calculation again to find out which part was actually clicked.
  
  \a selectionStateChanged is an output parameter. If the pointer is non-null, this function must
  set the value either to true or false, depending on whether the selection state of this layerable
  was actually changed. For layerables that only are selectable as a whole and not in parts, this
  is simple: if \a additive is true, \a selectionStateChanged must also be set to true, because the
  selection toggles. If \a additive is false, \a selectionStateChanged is only set to true, if the
  layerable was previously unselected and now is switched to the selected state.
  
  \see selectTest, deselectEvent
*/
void QCPLayerable::selectEvent(QMouseEvent *event, bool additive, const QVariant &details, bool *selectionStateChanged)
{
    Q_UNUSED(event)
    Q_UNUSED(additive)
    Q_UNUSED(details)
    Q_UNUSED(selectionStateChanged)
}

/*! \internal
  
  This event is called when the layerable shall be deselected, either as consequence of a user
  interaction or a call to \ref PF_GraphicView::deselectAll. Subclasses should react to it by
  unsetting their selection appropriately.
  
  just as in \ref selectEvent, the output parameter \a selectionStateChanged (if non-null), must
  return true or false when the selection state of this layerable has changed or not changed,
  respectively.
  
  \see selectTest, selectEvent
*/
void QCPLayerable::deselectEvent(bool *selectionStateChanged)
{
    Q_UNUSED(selectionStateChanged)
}

/*!
  This event gets called when the user presses a mouse button while the cursor is over the
  layerable. Whether a cursor is over the layerable is decided by a preceding call to \ref
  selectTest.

  The current pixel position of the cursor on the PF_GraphicView widget is accessible via \c
  event->pos(). The parameter \a details contains layerable-specific details about the hit, which
  were generated in the previous call to \ref selectTest. For example, One-dimensional plottables
  like \ref QCPGraph or \ref QCPBars convey the clicked data point in the \a details parameter, as
  \ref QCPDataSelection packed as QVariant. Multi-part objects convey the specific \c
  SelectablePart that was hit (e.g. \ref QCPAxis::SelectablePart in the case of axes).

  PF_GraphicView uses an event propagation system that works the same as Qt's system. If your
  layerable doesn't reimplement the \ref mousePressEvent or explicitly calls \c event->ignore() in
  its reimplementation, the event will be propagated to the next layerable in the stacking order.

  Once a layerable has accepted the \ref mousePressEvent, it is considered the mouse grabber and
  will receive all following calls to \ref mouseMoveEvent or \ref mouseReleaseEvent for this mouse
  interaction (a "mouse interaction" in this context ends with the release).

  The default implementation does nothing except explicitly ignoring the event with \c
  event->ignore().

  \see mouseMoveEvent, mouseReleaseEvent, mouseDoubleClickEvent, wheelEvent
*/
void QCPLayerable::mousePressEvent(QMouseEvent *event, const QVariant &details)
{
    Q_UNUSED(details)
    event->ignore();
}

/*!
  This event gets called when the user moves the mouse while holding a mouse button, after this
  layerable has become the mouse grabber by accepting the preceding \ref mousePressEvent.

  The current pixel position of the cursor on the PF_GraphicView widget is accessible via \c
  event->pos(). The parameter \a startPos indicates the position where the initial \ref
  mousePressEvent occured, that started the mouse interaction.

  The default implementation does nothing.

  \see mousePressEvent, mouseReleaseEvent, mouseDoubleClickEvent, wheelEvent
*/
void QCPLayerable::mouseMoveEvent(QMouseEvent *event, const QPointF &startPos)
{
    Q_UNUSED(startPos)
    event->ignore();
}

/*!
  This event gets called when the user releases the mouse button, after this layerable has become
  the mouse grabber by accepting the preceding \ref mousePressEvent.

  The current pixel position of the cursor on the PF_GraphicView widget is accessible via \c
  event->pos(). The parameter \a startPos indicates the position where the initial \ref
  mousePressEvent occured, that started the mouse interaction.

  The default implementation does nothing.

  \see mousePressEvent, mouseMoveEvent, mouseDoubleClickEvent, wheelEvent
*/
void QCPLayerable::mouseReleaseEvent(QMouseEvent *event, const QPointF &startPos)
{
    Q_UNUSED(startPos)
    event->ignore();
}

/*!
  This event gets called when the user presses the mouse button a second time in a double-click,
  while the cursor is over the layerable. Whether a cursor is over the layerable is decided by a
  preceding call to \ref selectTest.

  The \ref mouseDoubleClickEvent is called instead of the second \ref mousePressEvent. So in the
  case of a double-click, the event succession is
  <i>pressEvent &ndash; releaseEvent &ndash; doubleClickEvent &ndash; releaseEvent</i>.

  The current pixel position of the cursor on the PF_GraphicView widget is accessible via \c
  event->pos(). The parameter \a details contains layerable-specific details about the hit, which
  were generated in the previous call to \ref selectTest. For example, One-dimensional plottables
  like \ref QCPGraph or \ref QCPBars convey the clicked data point in the \a details parameter, as
  \ref QCPDataSelection packed as QVariant. Multi-part objects convey the specific \c
  SelectablePart that was hit (e.g. \ref QCPAxis::SelectablePart in the case of axes).

  Similarly to \ref mousePressEvent, once a layerable has accepted the \ref mouseDoubleClickEvent,
  it is considered the mouse grabber and will receive all following calls to \ref mouseMoveEvent
  and \ref mouseReleaseEvent for this mouse interaction (a "mouse interaction" in this context ends
  with the release).

  The default implementation does nothing except explicitly ignoring the event with \c
  event->ignore().

  \see mousePressEvent, mouseMoveEvent, mouseReleaseEvent, wheelEvent
*/
void QCPLayerable::mouseDoubleClickEvent(QMouseEvent *event, const QVariant &details)
{
    Q_UNUSED(details)
    event->ignore();
}

/*!
  This event gets called when the user turns the mouse scroll wheel while the cursor is over the
  layerable. Whether a cursor is over the layerable is decided by a preceding call to \ref
  selectTest.

  The current pixel position of the cursor on the PF_GraphicView widget is accessible via \c
  event->pos().

  The \c event->delta() indicates how far the mouse wheel was turned, which is usually +/- 120 for
  single rotation steps. However, if the mouse wheel is turned rapidly, multiple steps may
  accumulate to one event, making \c event->delta() larger. On the other hand, if the wheel has
  very smooth steps or none at all, the delta may be smaller.

  The default implementation does nothing.

  \see mousePressEvent, mouseMoveEvent, mouseReleaseEvent, mouseDoubleClickEvent
*/
void QCPLayerable::wheelEvent(QWheelEvent *event)
{
    event->ignore();
}
/* end of 'src/layer.cpp' */


/* including file 'src/axis/range.cpp', size 12221                           */
/* commit ce344b3f96a62e5f652585e55f1ae7c7883cd45b 2018-06-25 01:03:39 +0200 */

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPRange
////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \class QCPRange
  \brief Represents the range an axis is encompassing.
  
  contains a \a lower and \a upper double value and provides convenience input, output and
  modification functions.
  
  \see QCPAxis::setRange
*/

/* start of documentation of inline functions */

/*! \fn double QCPRange::size() const

  Returns the size of the range, i.e. \a upper-\a lower
*/

/*! \fn double QCPRange::center() const

  Returns the center of the range, i.e. (\a upper+\a lower)*0.5
*/

/*! \fn void QCPRange::normalize()

  Makes sure \a lower is numerically smaller than \a upper. If this is not the case, the values are
  swapped.
*/

/*! \fn bool QCPRange::contains(double value) const

  Returns true when \a value lies within or exactly on the borders of the range.
*/

/*! \fn QCPRange &QCPRange::operator+=(const double& value)

  Adds \a value to both boundaries of the range.
*/

/*! \fn QCPRange &QCPRange::operator-=(const double& value)

  Subtracts \a value from both boundaries of the range.
*/

/*! \fn QCPRange &QCPRange::operator*=(const double& value)

  Multiplies both boundaries of the range by \a value.
*/

/*! \fn QCPRange &QCPRange::operator/=(const double& value)

  Divides both boundaries of the range by \a value.
*/

/* end of documentation of inline functions */

/*!
  Minimum range size (\a upper - \a lower) the range changing functions will accept. Smaller
  intervals would cause errors due to the 11-bit exponent of double precision numbers,
  corresponding to a minimum magnitude of roughly 1e-308.

  \warning Do not use this constant to indicate "arbitrarily small" values in plotting logic (as
  values that will appear in the plot)! It is intended only as a bound to compare against, e.g. to
  prevent axis ranges from obtaining underflowing ranges.

  \see validRange, maxRange
*/
const double QCPRange::minRange = 1e-280;

/*!
  Maximum values (negative and positive) the range will accept in range-changing functions.
  Larger absolute values would cause errors due to the 11-bit exponent of double precision numbers,
  corresponding to a maximum magnitude of roughly 1e308.

  \warning Do not use this constant to indicate "arbitrarily large" values in plotting logic (as
  values that will appear in the plot)! It is intended only as a bound to compare against, e.g. to
  prevent axis ranges from obtaining overflowing ranges.

  \see validRange, minRange
*/
const double QCPRange::maxRange = 1e250;

/*!
  Constructs a range with \a lower and \a upper set to zero.
*/
QCPRange::QCPRange() :
    lower(0),
    upper(0)
{
}

/*! \overload

  Constructs a range with the specified \a lower and \a upper values.

  The resulting range will be normalized (see \ref normalize), so if \a lower is not numerically
  smaller than \a upper, they will be swapped.
*/
QCPRange::QCPRange(double lower, double upper) :
    lower(lower),
    upper(upper)
{
    normalize();
}

/*! \overload

  Expands this range such that \a otherRange is contained in the new range. It is assumed that both
  this range and \a otherRange are normalized (see \ref normalize).

  If this range contains NaN as lower or upper bound, it will be replaced by the respective bound
  of \a otherRange.

  If \a otherRange is already inside the current range, this function does nothing.

  \see expanded
*/
void QCPRange::expand(const QCPRange &otherRange)
{
    if (lower > otherRange.lower || qIsNaN(lower))
        lower = otherRange.lower;
    if (upper < otherRange.upper || qIsNaN(upper))
        upper = otherRange.upper;
}

/*! \overload

  Expands this range such that \a includeCoord is contained in the new range. It is assumed that
  this range is normalized (see \ref normalize).

  If this range contains NaN as lower or upper bound, the respective bound will be set to \a
  includeCoord.

  If \a includeCoord is already inside the current range, this function does nothing.

  \see expand
*/
void QCPRange::expand(double includeCoord)
{
    if (lower > includeCoord || qIsNaN(lower))
        lower = includeCoord;
    if (upper < includeCoord || qIsNaN(upper))
        upper = includeCoord;
}


/*! \overload

  Returns an expanded range that contains this and \a otherRange. It is assumed that both this
  range and \a otherRange are normalized (see \ref normalize).

  If this range contains NaN as lower or upper bound, the returned range's bound will be taken from
  \a otherRange.

  \see expand
*/
QCPRange QCPRange::expanded(const QCPRange &otherRange) const
{
    QCPRange result = *this;
    result.expand(otherRange);
    return result;
}

/*! \overload

  Returns an expanded range that includes the specified \a includeCoord. It is assumed that this
  range is normalized (see \ref normalize).

  If this range contains NaN as lower or upper bound, the returned range's bound will be set to \a
  includeCoord.

  \see expand
*/
QCPRange QCPRange::expanded(double includeCoord) const
{
    QCPRange result = *this;
    result.expand(includeCoord);
    return result;
}

/*!
  Returns this range, possibly modified to not exceed the bounds provided as \a lowerBound and \a
  upperBound. If possible, the size of the current range is preserved in the process.
  
  If the range shall only be bounded at the lower side, you can set \a upperBound to \ref
  QCPRange::maxRange. If it shall only be bounded at the upper side, set \a lowerBound to -\ref
  QCPRange::maxRange.
*/
QCPRange QCPRange::bounded(double lowerBound, double upperBound) const
{
    if (lowerBound > upperBound)
        qSwap(lowerBound, upperBound);

    QCPRange result(lower, upper);
    if (result.lower < lowerBound)
    {
        result.lower = lowerBound;
        result.upper = lowerBound + size();
        if (result.upper > upperBound || qFuzzyCompare(size(), upperBound-lowerBound))
            result.upper = upperBound;
    } else if (result.upper > upperBound)
    {
        result.upper = upperBound;
        result.lower = upperBound - size();
        if (result.lower < lowerBound || qFuzzyCompare(size(), upperBound-lowerBound))
            result.lower = lowerBound;
    }

    return result;
}

/*!
  Returns a sanitized version of the range. Sanitized means for logarithmic scales, that
  the range won't span the positive and negative sign domain, i.e. contain zero. Further
  \a lower will always be numerically smaller (or equal) to \a upper.
  
  If the original range does span positive and negative sign domains or contains zero,
  the returned range will try to approximate the original range as good as possible.
  If the positive interval of the original range is wider than the negative interval, the
  returned range will only contain the positive interval, with lower bound set to \a rangeFac or
  \a rangeFac *\a upper, whichever is closer to zero. Same procedure is used if the negative interval
  is wider than the positive interval, this time by changing the \a upper bound.
*/
QCPRange QCPRange::sanitizedForLogScale() const
{
    double rangeFac = 1e-3;
    QCPRange sanitizedRange(lower, upper);
    sanitizedRange.normalize();
    // can't have range spanning negative and positive values in log plot, so change range to fix it
    //if (qFuzzyCompare(sanitizedRange.lower+1, 1) && !qFuzzyCompare(sanitizedRange.upper+1, 1))
    if (sanitizedRange.lower == 0.0 && sanitizedRange.upper != 0.0)
    {
        // case lower is 0
        if (rangeFac < sanitizedRange.upper*rangeFac)
            sanitizedRange.lower = rangeFac;
        else
            sanitizedRange.lower = sanitizedRange.upper*rangeFac;
    } //else if (!qFuzzyCompare(lower+1, 1) && qFuzzyCompare(upper+1, 1))
    else if (sanitizedRange.lower != 0.0 && sanitizedRange.upper == 0.0)
    {
        // case upper is 0
        if (-rangeFac > sanitizedRange.lower*rangeFac)
            sanitizedRange.upper = -rangeFac;
        else
            sanitizedRange.upper = sanitizedRange.lower*rangeFac;
    } else if (sanitizedRange.lower < 0 && sanitizedRange.upper > 0)
    {
        // find out whether negative or positive interval is wider to decide which sign domain will be chosen
        if (-sanitizedRange.lower > sanitizedRange.upper)
        {
            // negative is wider, do same as in case upper is 0
            if (-rangeFac > sanitizedRange.lower*rangeFac)
                sanitizedRange.upper = -rangeFac;
            else
                sanitizedRange.upper = sanitizedRange.lower*rangeFac;
        } else
        {
            // positive is wider, do same as in case lower is 0
            if (rangeFac < sanitizedRange.upper*rangeFac)
                sanitizedRange.lower = rangeFac;
            else
                sanitizedRange.lower = sanitizedRange.upper*rangeFac;
        }
    }
    // due to normalization, case lower>0 && upper<0 should never occur, because that implies upper<lower
    return sanitizedRange;
}

/*!
  Returns a sanitized version of the range. Sanitized means for linear scales, that
  \a lower will always be numerically smaller (or equal) to \a upper.
*/
QCPRange QCPRange::sanitizedForLinScale() const
{
    QCPRange sanitizedRange(lower, upper);
    sanitizedRange.normalize();
    return sanitizedRange;
}

/*!
  Checks, whether the specified range is within valid bounds, which are defined
  as QCPRange::maxRange and QCPRange::minRange.
  A valid range means:
  \li range bounds within -maxRange and maxRange
  \li range size above minRange
  \li range size below maxRange
*/
bool QCPRange::validRange(double lower, double upper)
{
    return (lower > -maxRange &&
            upper < maxRange &&
            qAbs(lower-upper) > minRange &&
            qAbs(lower-upper) < maxRange &&
            !(lower > 0 && qIsInf(upper/lower)) &&
            !(upper < 0 && qIsInf(lower/upper)));
}

/*!
  \overload
  Checks, whether the specified range is within valid bounds, which are defined
  as QCPRange::maxRange and QCPRange::minRange.
  A valid range means:
  \li range bounds within -maxRange and maxRange
  \li range size above minRange
  \li range size below maxRange
*/
bool QCPRange::validRange(const QCPRange &range)
{
    return (range.lower > -maxRange &&
            range.upper < maxRange &&
            qAbs(range.lower-range.upper) > minRange &&
            qAbs(range.lower-range.upper) < maxRange &&
            !(range.lower > 0 && qIsInf(range.upper/range.lower)) &&
            !(range.upper < 0 && qIsInf(range.lower/range.upper)));
}
/* end of 'src/axis/range.cpp' */


/* including file 'src/selection.cpp', size 21941                            */
/* commit ce344b3f96a62e5f652585e55f1ae7c7883cd45b 2018-06-25 01:03:39 +0200 */

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPDataRange
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPDataRange
  \brief Describes a data range given by begin and end index
  
  QCPDataRange holds two integers describing the begin (\ref setBegin) and end (\ref setEnd) index
  of a contiguous set of data points. The end index points to the data point just after the last
  data point that's part of the data range, similarly to the nomenclature used in standard
  iterators.
  
  Data Ranges are not bound to a certain plottable, thus they can be freely exchanged, created and
  modified. If a non-contiguous data set shall be described, the class \ref QCPDataSelection is
  used, which holds and manages multiple instances of \ref QCPDataRange. In most situations, \ref
  QCPDataSelection is thus used.
  
  Both \ref QCPDataRange and \ref QCPDataSelection offer convenience methods to work with them,
  e.g. \ref bounded, \ref expanded, \ref intersects, \ref intersection, \ref adjusted, \ref
  contains. Further, addition and subtraction operators (defined in \ref QCPDataSelection) can be
  used to join/subtract data ranges and data selections (or mixtures), to retrieve a corresponding
  \ref QCPDataSelection.
  
  %PF_GraphicView's \ref dataselection "data selection mechanism" is based on \ref QCPDataSelection and
  QCPDataRange.
  
  \note Do not confuse \ref QCPDataRange with \ref QCPRange. A \ref QCPRange describes an interval
  in floating point plot coordinates, e.g. the current axis range.
*/

/* start documentation of inline functions */

/*! \fn int QCPDataRange::size() const
  
  Returns the number of data points described by this data range. This is equal to the end index
  minus the begin index.
  
  \see length
*/

/*! \fn int QCPDataRange::length() const
  
  Returns the number of data points described by this data range. Equivalent to \ref size.
*/

/*! \fn void QCPDataRange::setBegin(int begin)
  
  Sets the begin of this data range. The \a begin index points to the first data point that is part
  of the data range.
  
  No checks or corrections are made to ensure the resulting range is valid (\ref isValid).
  
  \see setEnd
*/

/*! \fn void QCPDataRange::setEnd(int end)
  
  Sets the end of this data range. The \a end index points to the data point just after the last
  data point that is part of the data range.
  
  No checks or corrections are made to ensure the resulting range is valid (\ref isValid).
  
  \see setBegin
*/

/*! \fn bool QCPDataRange::isValid() const
  
  Returns whether this range is valid. A valid range has a begin index greater or equal to 0, and
  an end index greater or equal to the begin index.
  
  \note Invalid ranges should be avoided and are never the result of any of PF_GraphicView's methods
  (unless they are themselves fed with invalid ranges). Do not pass invalid ranges to PF_GraphicView's
  methods. The invalid range is not inherently prevented in QCPDataRange, to allow temporary
  invalid begin/end values while manipulating the range. An invalid range is not necessarily empty
  (\ref isEmpty), since its \ref length can be negative and thus non-zero.
*/

/*! \fn bool QCPDataRange::isEmpty() const
  
  Returns whether this range is empty, i.e. whether its begin index equals its end index.
  
  \see size, length
*/

/*! \fn QCPDataRange QCPDataRange::adjusted(int changeBegin, int changeEnd) const
  
  Returns a data range where \a changeBegin and \a changeEnd were added to the begin and end
  indices, respectively.
*/

/* end documentation of inline functions */

/*!
  Creates an empty QCPDataRange, with begin and end set to 0.
*/
QCPDataRange::QCPDataRange() :
    mBegin(0),
    mEnd(0)
{
}

/*!
  Creates a QCPDataRange, initialized with the specified \a begin and \a end.
  
  No checks or corrections are made to ensure the resulting range is valid (\ref isValid).
*/
QCPDataRange::QCPDataRange(int begin, int end) :
    mBegin(begin),
    mEnd(end)
{
}

/*!
  Returns a data range that matches this data range, except that parts exceeding \a other are
  excluded.
  
  This method is very similar to \ref intersection, with one distinction: If this range and the \a
  other range share no intersection, the returned data range will be empty with begin and end set
  to the respective boundary side of \a other, at which this range is residing. (\ref intersection
  would just return a range with begin and end set to 0.)
*/
QCPDataRange QCPDataRange::bounded(const QCPDataRange &other) const
{
    QCPDataRange result(intersection(other));
    if (result.isEmpty()) // no intersection, preserve respective bounding side of otherRange as both begin and end of return value
    {
        if (mEnd <= other.mBegin)
            result = QCPDataRange(other.mBegin, other.mBegin);
        else
            result = QCPDataRange(other.mEnd, other.mEnd);
    }
    return result;
}

/*!
  Returns a data range that contains both this data range as well as \a other.
*/
QCPDataRange QCPDataRange::expanded(const QCPDataRange &other) const
{
    return QCPDataRange(qMin(mBegin, other.mBegin), qMax(mEnd, other.mEnd));
}

/*!
  Returns the data range which is contained in both this data range and \a other.
  
  This method is very similar to \ref bounded, with one distinction: If this range and the \a other
  range share no intersection, the returned data range will be empty with begin and end set to 0.
  (\ref bounded would return a range with begin and end set to one of the boundaries of \a other,
  depending on which side this range is on.)
  
  \see QCPDataSelection::intersection
*/
QCPDataRange QCPDataRange::intersection(const QCPDataRange &other) const
{
    QCPDataRange result(qMax(mBegin, other.mBegin), qMin(mEnd, other.mEnd));
    if (result.isValid())
        return result;
    else
        return QCPDataRange();
}

/*!
  Returns whether this data range and \a other share common data points.
  
  \see intersection, contains
*/
bool QCPDataRange::intersects(const QCPDataRange &other) const
{
    return !( (mBegin > other.mBegin && mBegin >= other.mEnd) ||
              (mEnd <= other.mBegin && mEnd < other.mEnd) );
}

/*!
  Returns whether all data points of \a other are also contained inside this data range.
  
  \see intersects
*/
bool QCPDataRange::contains(const QCPDataRange &other) const
{
    return mBegin <= other.mBegin && mEnd >= other.mEnd;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPDataSelection
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPDataSelection
  \brief Describes a data set by holding multiple QCPDataRange instances
  
  QCPDataSelection manages multiple instances of QCPDataRange in order to represent any (possibly
  disjoint) set of data selection.
  
  The data selection can be modified with addition and subtraction operators which take
  QCPDataSelection and QCPDataRange instances, as well as methods such as \ref addDataRange and
  \ref clear. Read access is provided by \ref dataRange, \ref dataRanges, \ref dataRangeCount, etc.
  
  The method \ref simplify is used to join directly adjacent or even overlapping QCPDataRange
  instances. QCPDataSelection automatically simplifies when using the addition/subtraction
  operators. The only case when \ref simplify is left to the user, is when calling \ref
  addDataRange, with the parameter \a simplify explicitly set to false. This is useful if many data
  ranges will be added to the selection successively and the overhead for simplifying after each
  iteration shall be avoided. In this case, you should make sure to call \ref simplify after
  completing the operation.
  
  Use \ref enforceType to bring the data selection into a state complying with the constraints for
  selections defined in \ref QCP::SelectionType.
  
  %PF_GraphicView's \ref dataselection "data selection mechanism" is based on QCPDataSelection and
  QCPDataRange.
  
  \section qcpdataselection-iterating Iterating over a data selection
  
  As an example, the following code snippet calculates the average value of a graph's data
  \ref QCPAbstractPlottable::selection "selection":
  
  \snippet documentation/doc-code-snippets/mainwindow.cpp qcpdataselection-iterating-1
  
*/

/* start documentation of inline functions */

/*! \fn int QCPDataSelection::dataRangeCount() const
  
  Returns the number of ranges that make up the data selection. The ranges can be accessed by \ref
  dataRange via their index.
  
  \see dataRange, dataPointCount
*/

/*! \fn QList<QCPDataRange> QCPDataSelection::dataRanges() const
  
  Returns all data ranges that make up the data selection. If the data selection is simplified (the
  usual state of the selection, see \ref simplify), the ranges are sorted by ascending data point
  index.
  
  \see dataRange
*/

/*! \fn bool QCPDataSelection::isEmpty() const
  
  Returns true if there are no data ranges, and thus no data points, in this QCPDataSelection
  instance.
  
  \see dataRangeCount
*/

/* end documentation of inline functions */

/*!
  Creates an empty QCPDataSelection.
*/
QCPDataSelection::QCPDataSelection()
{
}

/*!
  Creates a QCPDataSelection containing the provided \a range.
*/
QCPDataSelection::QCPDataSelection(const QCPDataRange &range)
{
    mDataRanges.append(range);
}

/*!
  Returns true if this selection is identical (contains the same data ranges with the same begin
  and end indices) to \a other.

  Note that both data selections must be in simplified state (the usual state of the selection, see
  \ref simplify) for this operator to return correct results.
*/
bool QCPDataSelection::operator==(const QCPDataSelection &other) const
{
    if (mDataRanges.size() != other.mDataRanges.size())
        return false;
    for (int i=0; i<mDataRanges.size(); ++i)
    {
        if (mDataRanges.at(i) != other.mDataRanges.at(i))
            return false;
    }
    return true;
}

/*!
  Adds the data selection of \a other to this data selection, and then simplifies this data
  selection (see \ref simplify).
*/
QCPDataSelection &QCPDataSelection::operator+=(const QCPDataSelection &other)
{
    mDataRanges << other.mDataRanges;
    simplify();
    return *this;
}

/*!
  Adds the data range \a other to this data selection, and then simplifies this data selection (see
  \ref simplify).
*/
QCPDataSelection &QCPDataSelection::operator+=(const QCPDataRange &other)
{
    addDataRange(other);
    return *this;
}

/*!
  Removes all data point indices that are described by \a other from this data selection.
*/
QCPDataSelection &QCPDataSelection::operator-=(const QCPDataSelection &other)
{
    for (int i=0; i<other.dataRangeCount(); ++i)
        *this -= other.dataRange(i);

    return *this;
}

/*!
  Removes all data point indices that are described by \a other from this data selection.
*/
QCPDataSelection &QCPDataSelection::operator-=(const QCPDataRange &other)
{
    if (other.isEmpty() || isEmpty())
        return *this;

    simplify();
    int i=0;
    while (i < mDataRanges.size())
    {
        const int thisBegin = mDataRanges.at(i).begin();
        const int thisEnd = mDataRanges.at(i).end();
        if (thisBegin >= other.end())
            break; // since data ranges are sorted after the simplify() call, no ranges which contain other will come after this

        if (thisEnd > other.begin()) // ranges which don't fulfill this are entirely before other and can be ignored
        {
            if (thisBegin >= other.begin()) // range leading segment is encompassed
            {
                if (thisEnd <= other.end()) // range fully encompassed, remove completely
                {
                    mDataRanges.removeAt(i);
                    continue;
                } else // only leading segment is encompassed, trim accordingly
                    mDataRanges[i].setBegin(other.end());
            } else // leading segment is not encompassed
            {
                if (thisEnd <= other.end()) // only trailing segment is encompassed, trim accordingly
                {
                    mDataRanges[i].setEnd(other.begin());
                } else // other lies inside this range, so split range
                {
                    mDataRanges[i].setEnd(other.begin());
                    mDataRanges.insert(i+1, QCPDataRange(other.end(), thisEnd));
                    break; // since data ranges are sorted (and don't overlap) after simplify() call, we're done here
                }
            }
        }
        ++i;
    }

    return *this;
}

/*!
  Returns the total number of data points contained in all data ranges that make up this data
  selection.
*/
int QCPDataSelection::dataPointCount() const
{
    int result = 0;
    for (int i=0; i<mDataRanges.size(); ++i)
        result += mDataRanges.at(i).length();
    return result;
}

/*!
  Returns the data range with the specified \a index.
  
  If the data selection is simplified (the usual state of the selection, see \ref simplify), the
  ranges are sorted by ascending data point index.
  
  \see dataRangeCount
*/
QCPDataRange QCPDataSelection::dataRange(int index) const
{
    if (index >= 0 && index < mDataRanges.size())
    {
        return mDataRanges.at(index);
    } else
    {
        qDebug() << Q_FUNC_INFO << "index out of range:" << index;
        return QCPDataRange();
    }
}

/*!
  Returns a \ref QCPDataRange which spans the entire data selection, including possible
  intermediate segments which are not part of the original data selection.
*/
QCPDataRange QCPDataSelection::span() const
{
    if (isEmpty())
        return QCPDataRange();
    else
        return QCPDataRange(mDataRanges.first().begin(), mDataRanges.last().end());
}

/*!
  Adds the given \a dataRange to this data selection. This is equivalent to the += operator but
  allows disabling immediate simplification by setting \a simplify to false. This can improve
  performance if adding a very large amount of data ranges successively. In this case, make sure to
  call \ref simplify manually, after the operation.
*/
void QCPDataSelection::addDataRange(const QCPDataRange &dataRange, bool simplify)
{
    mDataRanges.append(dataRange);
    if (simplify)
        this->simplify();
}

/*!
  Removes all data ranges. The data selection then contains no data points.
  
  \ref isEmpty
*/
void QCPDataSelection::clear()
{
    mDataRanges.clear();
}

/*!
  Sorts all data ranges by range begin index in ascending order, and then joins directly adjacent
  or overlapping ranges. This can reduce the number of individual data ranges in the selection, and
  prevents possible double-counting when iterating over the data points held by the data ranges.

  This method is automatically called when using the addition/subtraction operators. The only case
  when \ref simplify is left to the user, is when calling \ref addDataRange, with the parameter \a
  simplify explicitly set to false.
*/
void QCPDataSelection::simplify()
{
    // remove any empty ranges:
    for (int i=mDataRanges.size()-1; i>=0; --i)
    {
        if (mDataRanges.at(i).isEmpty())
            mDataRanges.removeAt(i);
    }
    if (mDataRanges.isEmpty())
        return;

    // sort ranges by starting value, ascending:
    std::sort(mDataRanges.begin(), mDataRanges.end(), lessThanDataRangeBegin);

    // join overlapping/contiguous ranges:
    int i = 1;
    while (i < mDataRanges.size())
    {
        if (mDataRanges.at(i-1).end() >= mDataRanges.at(i).begin()) // range i overlaps/joins with i-1, so expand range i-1 appropriately and remove range i from list
        {
            mDataRanges[i-1].setEnd(qMax(mDataRanges.at(i-1).end(), mDataRanges.at(i).end()));
            mDataRanges.removeAt(i);
        } else
            ++i;
    }
}

/*!
  Makes sure this data selection conforms to the specified \a type selection type. Before the type
  is enforced, \ref simplify is called.
  
  Depending on \a type, enforcing means adding new data points that were previously not part of the
  selection, or removing data points from the selection. If the current selection already conforms
  to \a type, the data selection is not changed.
  
  \see QCP::SelectionType
*/
void QCPDataSelection::enforceType(QCP::SelectionType type)
{
    simplify();
    switch (type)
    {
    case QCP::stNone:
    {
        mDataRanges.clear();
        break;
    }
    case QCP::stWhole:
    {
        // whole selection isn't defined by data range, so don't change anything (is handled in plottable methods)
        break;
    }
    case QCP::stSingleData:
    {
        // reduce all data ranges to the single first data point:
        if (!mDataRanges.isEmpty())
        {
            if (mDataRanges.size() > 1)
                mDataRanges = QList<QCPDataRange>() << mDataRanges.first();
            if (mDataRanges.first().length() > 1)
                mDataRanges.first().setEnd(mDataRanges.first().begin()+1);
        }
        break;
    }
    case QCP::stDataRange:
    {
        if (!isEmpty())
            mDataRanges = QList<QCPDataRange>() << span();
        break;
    }
    case QCP::stMultipleDataRanges:
    {
        // this is the selection type that allows all concievable combinations of ranges, so do nothing
        break;
    }
    }
}

/*!
  Returns true if the data selection \a other is contained entirely in this data selection, i.e.
  all data point indices that are in \a other are also in this data selection.
  
  \see QCPDataRange::contains
*/
bool QCPDataSelection::contains(const QCPDataSelection &other) const
{
    if (other.isEmpty()) return false;

    int otherIndex = 0;
    int thisIndex = 0;
    while (thisIndex < mDataRanges.size() && otherIndex < other.mDataRanges.size())
    {
        if (mDataRanges.at(thisIndex).contains(other.mDataRanges.at(otherIndex)))
            ++otherIndex;
        else
            ++thisIndex;
    }
    return thisIndex < mDataRanges.size(); // if thisIndex ran all the way to the end to find a containing range for the current otherIndex, other is not contained in this
}

/*!
  Returns a data selection containing the points which are both in this data selection and in the
  data range \a other.

  A common use case is to limit an unknown data selection to the valid range of a data container,
  using \ref QCPDataContainer::dataRange as \a other. One can then safely iterate over the returned
  data selection without exceeding the data container's bounds.
*/
QCPDataSelection QCPDataSelection::intersection(const QCPDataRange &other) const
{
    QCPDataSelection result;
    for (int i=0; i<mDataRanges.size(); ++i)
        result.addDataRange(mDataRanges.at(i).intersection(other), false);
    result.simplify();
    return result;
}

/*!
  Returns a data selection containing the points which are both in this data selection and in the
  data selection \a other.
*/
QCPDataSelection QCPDataSelection::intersection(const QCPDataSelection &other) const
{
    QCPDataSelection result;
    for (int i=0; i<other.dataRangeCount(); ++i)
        result += intersection(other.dataRange(i));
    result.simplify();
    return result;
}

/*!
  Returns a data selection which is the exact inverse of this data selection, with \a outerRange
  defining the base range on which to invert. If \a outerRange is smaller than the \ref span of
  this data selection, it is expanded accordingly.

  For example, this method can be used to retrieve all unselected segments by setting \a outerRange
  to the full data range of the plottable, and calling this method on a data selection holding the
  selected segments.
*/
QCPDataSelection QCPDataSelection::inverse(const QCPDataRange &outerRange) const
{
    if (isEmpty())
        return QCPDataSelection(outerRange);
    QCPDataRange fullRange = outerRange.expanded(span());

    QCPDataSelection result;
    // first unselected segment:
    if (mDataRanges.first().begin() != fullRange.begin())
        result.addDataRange(QCPDataRange(fullRange.begin(), mDataRanges.first().begin()), false);
    // intermediate unselected segments:
    for (int i=1; i<mDataRanges.size(); ++i)
        result.addDataRange(QCPDataRange(mDataRanges.at(i-1).end(), mDataRanges.at(i).begin()), false);
    // last unselected segment:
    if (mDataRanges.last().end() != fullRange.end())
        result.addDataRange(QCPDataRange(mDataRanges.last().end(), fullRange.end()), false);
    result.simplify();
    return result;
}
/* end of 'src/selection.cpp' */


/* including file 'src/selectionrect.cpp', size 9224                         */
/* commit ce344b3f96a62e5f652585e55f1ae7c7883cd45b 2018-06-25 01:03:39 +0200 */

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPSelectionRect
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPSelectionRect
  \brief Provides rect/rubber-band data selection and range zoom interaction
  
  QCPSelectionRect is used by PF_GraphicView when the \ref PF_GraphicView::setSelectionRectMode is not
  \ref QCP::srmNone. When the user drags the mouse across the plot, the current selection rect
  instance (\ref PF_GraphicView::setSelectionRect) is forwarded these events and makes sure an
  according rect shape is drawn. At the begin, during, and after completion of the interaction, it
  emits the corresponding signals \ref started, \ref changed, \ref canceled, and \ref accepted.
  
  The PF_GraphicView instance connects own slots to the current selection rect instance, in order to
  react to an accepted selection rect interaction accordingly.
  
  \ref isActive can be used to check whether the selection rect is currently active. An ongoing
  selection interaction can be cancelled programmatically via calling \ref cancel at any time.
  
  The appearance of the selection rect can be controlled via \ref setPen and \ref setBrush.

  If you wish to provide custom behaviour, e.g. a different visual representation of the selection
  rect (\ref QCPSelectionRect::draw), you can subclass QCPSelectionRect and pass an instance of
  your subclass to \ref PF_GraphicView::setSelectionRect.
*/

/* start of documentation of inline functions */

/*! \fn bool QCPSelectionRect::isActive() const

  Returns true if there is currently a selection going on, i.e. the user has started dragging a
  selection rect, but hasn't released the mouse button yet.

  \see cancel
*/

/* end of documentation of inline functions */
/* start documentation of signals */

/*! \fn void QCPSelectionRect::started(QMouseEvent *event);

  This signal is emitted when a selection rect interaction was initiated, i.e. the user just
  started dragging the selection rect with the mouse.
*/

/*! \fn void QCPSelectionRect::changed(const QRect &rect, QMouseEvent *event);
  
  This signal is emitted while the selection rect interaction is ongoing and the \a rect has
  changed its size due to the user moving the mouse.
  
  Note that \a rect may have a negative width or height, if the selection is being dragged to the
  upper or left side of the selection rect origin.
*/

/*! \fn void QCPSelectionRect::canceled(const QRect &rect, QInputEvent *event);
  
  This signal is emitted when the selection interaction was cancelled. Note that \a event is 0 if
  the selection interaction was cancelled programmatically, by a call to \ref cancel.
  
  The user may cancel the selection interaction by pressing the escape key. In this case, \a event
  holds the respective input event.
  
  Note that \a rect may have a negative width or height, if the selection is being dragged to the
  upper or left side of the selection rect origin.
*/

/*! \fn void QCPSelectionRect::accepted(const QRect &rect, QMouseEvent *event);
  
  This signal is emitted when the selection interaction was completed by the user releasing the
  mouse button.

  Note that \a rect may have a negative width or height, if the selection is being dragged to the
  upper or left side of the selection rect origin.
*/

/* end documentation of signals */

/*!
  Creates a new QCPSelectionRect instance. To make PF_GraphicView use the selection rect instance,
  pass it to \ref PF_GraphicView::setSelectionRect. \a parentPlot should be set to the same
  PF_GraphicView widget.
*/
QCPSelectionRect::QCPSelectionRect(PF_GraphicView *parentPlot) :
    QCPLayerable(parentPlot),
    mPen(QBrush(Qt::gray), 0, Qt::DashLine),
    mBrush(Qt::NoBrush),
    mActive(false)
{
}

QCPSelectionRect::~QCPSelectionRect()
{
    cancel();
}

/*!
  A convenience function which returns the coordinate range of the provided \a axis, that this
  selection rect currently encompasses.
*/
QCPRange QCPSelectionRect::range(const QCPAxis *axis) const
{
    if (axis)
    {
        if (axis->orientation() == Qt::Horizontal)
            return QCPRange(axis->pixelToCoord(mRect.left()), axis->pixelToCoord(mRect.left()+mRect.width()));
        else
            return QCPRange(axis->pixelToCoord(mRect.top()+mRect.height()), axis->pixelToCoord(mRect.top()));
    } else
    {
        qDebug() << Q_FUNC_INFO << "called with axis zero";
        return QCPRange();
    }
}

/*!
  Sets the pen that will be used to draw the selection rect outline.
  
  \see setBrush
*/
void QCPSelectionRect::setPen(const QPen &pen)
{
    mPen = pen;
}

/*!
  Sets the brush that will be used to fill the selection rect. By default the selection rect is not
  filled, i.e. \a brush is <tt>Qt::NoBrush</tt>.
  
  \see setPen
*/
void QCPSelectionRect::setBrush(const QBrush &brush)
{
    mBrush = brush;
}

/*!
  If there is currently a selection interaction going on (\ref isActive), the interaction is
  canceled. The selection rect will emit the \ref canceled signal.
*/
void QCPSelectionRect::cancel()
{
    if (mActive)
    {
        mActive = false;
        emit canceled(mRect, nullptr);
    }
}

/*! \internal
  
  This method is called by PF_GraphicView to indicate that a selection rect interaction was initiated.
  The default implementation sets the selection rect to active, initializes the selection rect
  geometry and emits the \ref started signal.
*/
void QCPSelectionRect::startSelection(QMouseEvent *event)
{
    mActive = true;
    mRect = QRect(event->pos(), event->pos());
    emit started(event);
}

/*! \internal
  
  This method is called by PF_GraphicView to indicate that an ongoing selection rect interaction needs
  to update its geometry. The default implementation updates the rect and emits the \ref changed
  signal.
*/
void QCPSelectionRect::moveSelection(QMouseEvent *event)
{
    mRect.setBottomRight(event->pos());
    emit changed(mRect, event);
    layer()->replot();
}

/*! \internal
  
  This method is called by PF_GraphicView to indicate that an ongoing selection rect interaction has
  finished by the user releasing the mouse button. The default implementation deactivates the
  selection rect and emits the \ref accepted signal.
*/
void QCPSelectionRect::endSelection(QMouseEvent *event)
{
    mRect.setBottomRight(event->pos());
    mActive = false;
    emit accepted(mRect, event);
}

/*! \internal
  
  This method is called by PF_GraphicView when a key has been pressed by the user while the selection
  rect interaction is active. The default implementation allows to \ref cancel the interaction by
  hitting the escape key.
*/
void QCPSelectionRect::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape && mActive)
    {
        mActive = false;
        emit canceled(mRect, event);
    }
}

/* inherits documentation from base class */
void QCPSelectionRect::applyDefaultAntialiasingHint(QCPPainter *painter) const
{
    applyAntialiasingHint(painter, mAntialiased, QCP::aeOther);
}

/*! \internal
  
  If the selection rect is active (\ref isActive), draws the selection rect defined by \a mRect.
  
  \seebaseclassmethod
*/
void QCPSelectionRect::draw(QCPPainter *painter)
{
    if (mActive)
    {
        painter->setPen(mPen);
        painter->setBrush(mBrush);
        painter->drawRect(mRect);
    }
}
/* end of 'src/selectionrect.cpp' */


/* including file 'src/layout.cpp', size 79139                               */
/* commit ce344b3f96a62e5f652585e55f1ae7c7883cd45b 2018-06-25 01:03:39 +0200 */

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPMarginGroup
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPMarginGroup
  \brief A margin group allows synchronization of margin sides if working with multiple layout elements.
  
  QCPMarginGroup allows you to tie a margin side of two or more layout elements together, such that
  they will all have the same size, based on the largest required margin in the group.
  
  \n
  \image html QCPMarginGroup.png "Demonstration of QCPMarginGroup"
  \n
  
  In certain situations it is desirable that margins at specific sides are synchronized across
  layout elements. For example, if one QCPAxisRect is below another one in a grid layout, it will
  provide a cleaner look to the user if the left and right margins of the two axis rects are of the
  same size. The left axis of the top axis rect will then be at the same horizontal position as the
  left axis of the lower axis rect, making them appear aligned. The same applies for the right
  axes. This is what QCPMarginGroup makes possible.
  
  To add/remove a specific side of a layout element to/from a margin group, use the \ref
  QCPLayoutElement::setMarginGroup method. To completely break apart the margin group, either call
  \ref clear, or just delete the margin group.
  
  \section QCPMarginGroup-example Example
  
  First create a margin group:
  \snippet documentation/doc-code-snippets/mainwindow.cpp qcpmargingroup-creation-1
  Then set this group on the layout element sides:
  \snippet documentation/doc-code-snippets/mainwindow.cpp qcpmargingroup-creation-2
  Here, we've used the first two axis rects of the plot and synchronized their left margins with
  each other and their right margins with each other.
*/

/* start documentation of inline functions */

/*! \fn QList<QCPLayoutElement*> QCPMarginGroup::elements(QCP::MarginSide side) const
  
  Returns a list of all layout elements that have their margin \a side associated with this margin
  group.
*/

/* end documentation of inline functions */

/*!
  Creates a new QCPMarginGroup instance in \a parentPlot.
*/
QCPMarginGroup::QCPMarginGroup(PF_GraphicView *parentPlot) :
    QObject(parentPlot),
    mParentPlot(parentPlot)
{
    mChildren.insert(QCP::msLeft, QList<QCPLayoutElement*>());
    mChildren.insert(QCP::msRight, QList<QCPLayoutElement*>());
    mChildren.insert(QCP::msTop, QList<QCPLayoutElement*>());
    mChildren.insert(QCP::msBottom, QList<QCPLayoutElement*>());
}

QCPMarginGroup::~QCPMarginGroup()
{
    clear();
}

/*!
  Returns whether this margin group is empty. If this function returns true, no layout elements use
  this margin group to synchronize margin sides.
*/
bool QCPMarginGroup::isEmpty() const
{
    QHashIterator<QCP::MarginSide, QList<QCPLayoutElement*> > it(mChildren);
    while (it.hasNext())
    {
        it.next();
        if (!it.value().isEmpty())
            return false;
    }
    return true;
}

/*!
  Clears this margin group. The synchronization of the margin sides that use this margin group is
  lifted and they will use their individual margin sizes again.
*/
void QCPMarginGroup::clear()
{
    // make all children remove themselves from this margin group:
    QHashIterator<QCP::MarginSide, QList<QCPLayoutElement*> > it(mChildren);
    while (it.hasNext())
    {
        it.next();
        const QList<QCPLayoutElement*> elements = it.value();
        for (int i=elements.size()-1; i>=0; --i)
            elements.at(i)->setMarginGroup(it.key(), nullptr); // removes itself from mChildren via removeChild
    }
}

/*! \internal
  
  Returns the synchronized common margin for \a side. This is the margin value that will be used by
  the layout element on the respective side, if it is part of this margin group.
  
  The common margin is calculated by requesting the automatic margin (\ref
  QCPLayoutElement::calculateAutoMargin) of each element associated with \a side in this margin
  group, and choosing the largest returned value. (QCPLayoutElement::minimumMargins is taken into
  account, too.)
*/
int QCPMarginGroup::commonMargin(QCP::MarginSide side) const
{
    // query all automatic margins of the layout elements in this margin group side and find maximum:
    int result = 0;
    const QList<QCPLayoutElement*> elements = mChildren.value(side);
    for (int i=0; i<elements.size(); ++i)
    {
        if (!elements.at(i)->autoMargins().testFlag(side))
            continue;
        int m = qMax(elements.at(i)->calculateAutoMargin(side), QCP::getMarginValue(elements.at(i)->minimumMargins(), side));
        if (m > result)
            result = m;
    }
    return result;
}

/*! \internal
  
  Adds \a element to the internal list of child elements, for the margin \a side.
  
  This function does not modify the margin group property of \a element.
*/
void QCPMarginGroup::addChild(QCP::MarginSide side, QCPLayoutElement *element)
{
    if (!mChildren[side].contains(element))
        mChildren[side].append(element);
    else
        qDebug() << Q_FUNC_INFO << "element is already child of this margin group side" << reinterpret_cast<quintptr>(element);
}

/*! \internal
  
  Removes \a element from the internal list of child elements, for the margin \a side.
  
  This function does not modify the margin group property of \a element.
*/
void QCPMarginGroup::removeChild(QCP::MarginSide side, QCPLayoutElement *element)
{
    if (!mChildren[side].removeOne(element))
        qDebug() << Q_FUNC_INFO << "element is not child of this margin group side" << reinterpret_cast<quintptr>(element);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPLayoutElement
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPLayoutElement
  \brief The abstract base class for all objects that form \ref thelayoutsystem "the layout system".
  
  This is an abstract base class. As such, it can't be instantiated directly, rather use one of its subclasses.
  
  A Layout element is a rectangular object which can be placed in layouts. It has an outer rect
  (QCPLayoutElement::outerRect) and an inner rect (\ref QCPLayoutElement::rect). The difference
  between outer and inner rect is called its margin. The margin can either be set to automatic or
  manual (\ref setAutoMargins) on a per-side basis. If a side is set to manual, that margin can be
  set explicitly with \ref setMargins and will stay fixed at that value. If it's set to automatic,
  the layout element subclass will control the value itself (via \ref calculateAutoMargin).
  
  Layout elements can be placed in layouts (base class QCPLayout) like QCPLayoutGrid. The top level
  layout is reachable via \ref PF_GraphicView::plotLayout, and is a \ref QCPLayoutGrid. Since \ref
  QCPLayout itself derives from \ref QCPLayoutElement, layouts can be nested.
  
  Thus in PF_GraphicView one can divide layout elements into two categories: The ones that are
  invisible by themselves, because they don't draw anything. Their only purpose is to manage the
  position and size of other layout elements. This category of layout elements usually use
  QCPLayout as base class. Then there is the category of layout elements which actually draw
  something. For example, QCPAxisRect, QCPLegend and QCPTextElement are of this category. This does
  not necessarily mean that the latter category can't have child layout elements. QCPLegend for
  instance, actually derives from QCPLayoutGrid and the individual legend items are child layout
  elements in the grid layout.
*/

/* start documentation of inline functions */

/*! \fn QCPLayout *QCPLayoutElement::layout() const
  
  Returns the parent layout of this layout element.
*/

/*! \fn QRect QCPLayoutElement::rect() const
  
  Returns the inner rect of this layout element. The inner rect is the outer rect (\ref outerRect, \ref
  setOuterRect) shrinked by the margins (\ref setMargins, \ref setAutoMargins).
  
  In some cases, the area between outer and inner rect is left blank. In other cases the margin
  area is used to display peripheral graphics while the main content is in the inner rect. This is
  where automatic margin calculation becomes interesting because it allows the layout element to
  adapt the margins to the peripheral graphics it wants to draw. For example, \ref QCPAxisRect
  draws the axis labels and tick labels in the margin area, thus needs to adjust the margins (if
  \ref setAutoMargins is enabled) according to the space required by the labels of the axes.
  
  \see outerRect
*/

/*! \fn QRect QCPLayoutElement::outerRect() const
  
  Returns the outer rect of this layout element. The outer rect is the inner rect expanded by the
  margins (\ref setMargins, \ref setAutoMargins). The outer rect is used (and set via \ref
  setOuterRect) by the parent \ref QCPLayout to control the size of this layout element.
  
  \see rect
*/

/* end documentation of inline functions */

/*!
  Creates an instance of QCPLayoutElement and sets default values.
*/
QCPLayoutElement::QCPLayoutElement(PF_GraphicView *parentPlot) :
    QCPLayerable(parentPlot), // parenthood is changed as soon as layout element gets inserted into a layout (except for top level layout)
    mParentLayout(nullptr),
    mMinimumSize(),
    mMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX),
    mSizeConstraintRect(scrInnerRect),
    mRect(0, 0, 0, 0),
    mOuterRect(0, 0, 0, 0),
    mMargins(0, 0, 0, 0),
    mMinimumMargins(0, 0, 0, 0),
    mAutoMargins(QCP::msAll)
{
}

QCPLayoutElement::~QCPLayoutElement()
{
    setMarginGroup(QCP::msAll, nullptr); // unregister at margin groups, if there are any
    // unregister at layout:
    if (qobject_cast<QCPLayout*>(mParentLayout)) // the qobject_cast is just a safeguard in case the layout forgets to call clear() in its dtor and this dtor is called by QObject dtor
        mParentLayout->take(this);
}

/*!
  Sets the outer rect of this layout element. If the layout element is inside a layout, the layout
  sets the position and size of this layout element using this function.
  
  Calling this function externally has no effect, since the layout will overwrite any changes to
  the outer rect upon the next replot.
  
  The layout element will adapt its inner \ref rect by applying the margins inward to the outer rect.
  
  \see rect
*/
void QCPLayoutElement::setOuterRect(const QRect &rect)
{
    if (mOuterRect != rect)
    {
        mOuterRect = rect;
        mRect = mOuterRect.adjusted(mMargins.left(), mMargins.top(), -mMargins.right(), -mMargins.bottom());
    }
}

/*!
  Sets the margins of this layout element. If \ref setAutoMargins is disabled for some or all
  sides, this function is used to manually set the margin on those sides. Sides that are still set
  to be handled automatically are ignored and may have any value in \a margins.
  
  The margin is the distance between the outer rect (controlled by the parent layout via \ref
  setOuterRect) and the inner \ref rect (which usually contains the main content of this layout
  element).
  
  \see setAutoMargins
*/
void QCPLayoutElement::setMargins(const QMargins &margins)
{
    if (mMargins != margins)
    {
        mMargins = margins;
        mRect = mOuterRect.adjusted(mMargins.left(), mMargins.top(), -mMargins.right(), -mMargins.bottom());
    }
}

/*!
  If \ref setAutoMargins is enabled on some or all margins, this function is used to provide
  minimum values for those margins.
  
  The minimum values are not enforced on margin sides that were set to be under manual control via
  \ref setAutoMargins.
  
  \see setAutoMargins
*/
void QCPLayoutElement::setMinimumMargins(const QMargins &margins)
{
    if (mMinimumMargins != margins)
    {
        mMinimumMargins = margins;
    }
}

/*!
  Sets on which sides the margin shall be calculated automatically. If a side is calculated
  automatically, a minimum margin value may be provided with \ref setMinimumMargins. If a side is
  set to be controlled manually, the value may be specified with \ref setMargins.
  
  Margin sides that are under automatic control may participate in a \ref QCPMarginGroup (see \ref
  setMarginGroup), to synchronize (align) it with other layout elements in the plot.
  
  \see setMinimumMargins, setMargins, QCP::MarginSide
*/
void QCPLayoutElement::setAutoMargins(QCP::MarginSides sides)
{
    mAutoMargins = sides;
}

/*!
  Sets the minimum size of this layout element. A parent layout tries to respect the \a size here
  by changing row/column sizes in the layout accordingly.
  
  If the parent layout size is not sufficient to satisfy all minimum size constraints of its child
  layout elements, the layout may set a size that is actually smaller than \a size. PF_GraphicView
  propagates the layout's size constraints to the outside by setting its own minimum QWidget size
  accordingly, so violations of \a size should be exceptions.
  
  Whether this constraint applies to the inner or the outer rect can be specified with \ref
  setSizeConstraintRect (see \ref rect and \ref outerRect).
*/
void QCPLayoutElement::setMinimumSize(const QSize &size)
{
    if (mMinimumSize != size)
    {
        mMinimumSize = size;
        if (mParentLayout)
            mParentLayout->sizeConstraintsChanged();
    }
}

/*! \overload
  
  Sets the minimum size of this layout element.
  
  Whether this constraint applies to the inner or the outer rect can be specified with \ref
  setSizeConstraintRect (see \ref rect and \ref outerRect).
*/
void QCPLayoutElement::setMinimumSize(int width, int height)
{
    setMinimumSize(QSize(width, height));
}

/*!
  Sets the maximum size of this layout element. A parent layout tries to respect the \a size here
  by changing row/column sizes in the layout accordingly.
  
  Whether this constraint applies to the inner or the outer rect can be specified with \ref
  setSizeConstraintRect (see \ref rect and \ref outerRect).
*/
void QCPLayoutElement::setMaximumSize(const QSize &size)
{
    if (mMaximumSize != size)
    {
        mMaximumSize = size;
        if (mParentLayout)
            mParentLayout->sizeConstraintsChanged();
    }
}

/*! \overload
  
  Sets the maximum size of this layout element.
  
  Whether this constraint applies to the inner or the outer rect can be specified with \ref
  setSizeConstraintRect (see \ref rect and \ref outerRect).
*/
void QCPLayoutElement::setMaximumSize(int width, int height)
{
    setMaximumSize(QSize(width, height));
}

/*!
  Sets to which rect of a layout element the size constraints apply. Size constraints can be set
  via \ref setMinimumSize and \ref setMaximumSize.
  
  The outer rect (\ref outerRect) includes the margins (e.g. in the case of a QCPAxisRect the axis
  labels), whereas the inner rect (\ref rect) does not.
  
  \see setMinimumSize, setMaximumSize
*/
void QCPLayoutElement::setSizeConstraintRect(SizeConstraintRect constraintRect)
{
    if (mSizeConstraintRect != constraintRect)
    {
        mSizeConstraintRect = constraintRect;
        if (mParentLayout)
            mParentLayout->sizeConstraintsChanged();
    }
}

/*!
  Sets the margin \a group of the specified margin \a sides.
  
  Margin groups allow synchronizing specified margins across layout elements, see the documentation
  of \ref QCPMarginGroup.
  
  To unset the margin group of \a sides, set \a group to 0.
  
  Note that margin groups only work for margin sides that are set to automatic (\ref
  setAutoMargins).
  
  \see QCP::MarginSide
*/
void QCPLayoutElement::setMarginGroup(QCP::MarginSides sides, QCPMarginGroup *group)
{
    QVector<QCP::MarginSide> sideVector;
    if (sides.testFlag(QCP::msLeft)) sideVector.append(QCP::msLeft);
    if (sides.testFlag(QCP::msRight)) sideVector.append(QCP::msRight);
    if (sides.testFlag(QCP::msTop)) sideVector.append(QCP::msTop);
    if (sides.testFlag(QCP::msBottom)) sideVector.append(QCP::msBottom);

    for (int i=0; i<sideVector.size(); ++i)
    {
        QCP::MarginSide side = sideVector.at(i);
        if (marginGroup(side) != group)
        {
            QCPMarginGroup *oldGroup = marginGroup(side);
            if (oldGroup) // unregister at old group
                oldGroup->removeChild(side, this);

            if (!group) // if setting to 0, remove hash entry. Else set hash entry to new group and register there
            {
                mMarginGroups.remove(side);
            } else // setting to a new group
            {
                mMarginGroups[side] = group;
                group->addChild(side, this);
            }
        }
    }
}

/*!
  Updates the layout element and sub-elements. This function is automatically called before every
  replot by the parent layout element. It is called multiple times, once for every \ref
  UpdatePhase. The phases are run through in the order of the enum values. For details about what
  happens at the different phases, see the documentation of \ref UpdatePhase.
  
  Layout elements that have child elements should call the \ref update method of their child
  elements, and pass the current \a phase unchanged.
  
  The default implementation executes the automatic margin mechanism in the \ref upMargins phase.
  Subclasses should make sure to call the base class implementation.
*/
void QCPLayoutElement::update(UpdatePhase phase)
{
    if (phase == upMargins)
    {
        if (mAutoMargins != QCP::msNone)
        {
            // set the margins of this layout element according to automatic margin calculation, either directly or via a margin group:
            QMargins newMargins = mMargins;
            QList<QCP::MarginSide> allMarginSides = QList<QCP::MarginSide>() << QCP::msLeft << QCP::msRight << QCP::msTop << QCP::msBottom;
            foreach (QCP::MarginSide side, allMarginSides)
            {
                if (mAutoMargins.testFlag(side)) // this side's margin shall be calculated automatically
                {
                    if (mMarginGroups.contains(side))
                        QCP::setMarginValue(newMargins, side, mMarginGroups[side]->commonMargin(side)); // this side is part of a margin group, so get the margin value from that group
                    else
                        QCP::setMarginValue(newMargins, side, calculateAutoMargin(side)); // this side is not part of a group, so calculate the value directly
                    // apply minimum margin restrictions:
                    if (QCP::getMarginValue(newMargins, side) < QCP::getMarginValue(mMinimumMargins, side))
                        QCP::setMarginValue(newMargins, side, QCP::getMarginValue(mMinimumMargins, side));
                }
            }
            setMargins(newMargins);
        }
    }
}

/*!
  Returns the suggested minimum size this layout element (the \ref outerRect) may be compressed to,
  if no manual minimum size is set.
  
  if a minimum size (\ref setMinimumSize) was not set manually, parent layouts use the returned size
  (usually indirectly through \ref QCPLayout::getFinalMinimumOuterSize) to determine the minimum
  allowed size of this layout element.

  A manual minimum size is considered set if it is non-zero.
  
  The default implementation simply returns the sum of the horizontal margins for the width and the
  sum of the vertical margins for the height. Reimplementations may use their detailed knowledge
  about the layout element's content to provide size hints.
*/
QSize QCPLayoutElement::minimumOuterSizeHint() const
{
    return QSize(mMargins.left()+mMargins.right(), mMargins.top()+mMargins.bottom());
}

/*!
  Returns the suggested maximum size this layout element (the \ref outerRect) may be expanded to,
  if no manual maximum size is set.
  
  if a maximum size (\ref setMaximumSize) was not set manually, parent layouts use the returned
  size (usually indirectly through \ref QCPLayout::getFinalMaximumOuterSize) to determine the
  maximum allowed size of this layout element.

  A manual maximum size is considered set if it is smaller than Qt's \c QWIDGETSIZE_MAX.
  
  The default implementation simply returns \c QWIDGETSIZE_MAX for both width and height, implying
  no suggested maximum size. Reimplementations may use their detailed knowledge about the layout
  element's content to provide size hints.
*/
QSize QCPLayoutElement::maximumOuterSizeHint() const
{
    return QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
}

/*!
  Returns a list of all child elements in this layout element. If \a recursive is true, all
  sub-child elements are included in the list, too.
  
  \warning There may be entries with value 0 in the returned list. (For example, QCPLayoutGrid may have
  empty cells which yield 0 at the respective index.)
*/
QList<QCPLayoutElement*> QCPLayoutElement::elements(bool recursive) const
{
    Q_UNUSED(recursive)
    return QList<QCPLayoutElement*>();
}

/*!
  Layout elements are sensitive to events inside their outer rect. If \a pos is within the outer
  rect, this method returns a value corresponding to 0.99 times the parent plot's selection
  tolerance. However, layout elements are not selectable by default. So if \a onlySelectable is
  true, -1.0 is returned.
  
  See \ref QCPLayerable::selectTest for a general explanation of this virtual method.
  
  QCPLayoutElement subclasses may reimplement this method to provide more specific selection test
  behaviour.
*/
double QCPLayoutElement::selectTest(const QPointF &pos, bool onlySelectable, QVariant *details) const
{
    Q_UNUSED(details)

    if (onlySelectable)
        return -1;

    if (QRectF(mOuterRect).contains(pos))
    {
        if (mParentPlot)
            return mParentPlot->selectionTolerance()*0.99;
        else
        {
            qDebug() << Q_FUNC_INFO << "parent plot not defined";
            return -1;
        }
    } else
        return -1;
}

/*! \internal
  
  propagates the parent plot initialization to all child elements, by calling \ref
  QCPLayerable::initializeParentPlot on them.
*/
void QCPLayoutElement::parentPlotInitialized(PF_GraphicView *parentPlot)
{
    foreach (QCPLayoutElement* el, elements(false))
    {
        if (!el->parentPlot())
            el->initializeParentPlot(parentPlot);
    }
}

/*! \internal
  
  Returns the margin size for this \a side. It is used if automatic margins is enabled for this \a
  side (see \ref setAutoMargins). If a minimum margin was set with \ref setMinimumMargins, the
  returned value will not be smaller than the specified minimum margin.
  
  The default implementation just returns the respective manual margin (\ref setMargins) or the
  minimum margin, whichever is larger.
*/
int QCPLayoutElement::calculateAutoMargin(QCP::MarginSide side)
{
    return qMax(QCP::getMarginValue(mMargins, side), QCP::getMarginValue(mMinimumMargins, side));
}

/*! \internal
  
  This virtual method is called when this layout element was moved to a different QCPLayout, or
  when this layout element has changed its logical position (e.g. row and/or column) within the
  same QCPLayout. Subclasses may use this to react accordingly.
  
  Since this method is called after the completion of the move, you can access the new parent
  layout via \ref layout().
  
  The default implementation does nothing.
*/
void QCPLayoutElement::layoutChanged()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPLayout
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPLayout
  \brief The abstract base class for layouts
  
  This is an abstract base class for layout elements whose main purpose is to define the position
  and size of other child layout elements. In most cases, layouts don't draw anything themselves
  (but there are exceptions to this, e.g. QCPLegend).
  
  QCPLayout derives from QCPLayoutElement, and thus can itself be nested in other layouts.
  
  QCPLayout introduces a common interface for accessing and manipulating the child elements. Those
  functions are most notably \ref elementCount, \ref elementAt, \ref takeAt, \ref take, \ref
  simplify, \ref removeAt, \ref remove and \ref clear. Individual subclasses may add more functions
  to this interface which are more specialized to the form of the layout. For example, \ref
  QCPLayoutGrid adds functions that take row and column indices to access cells of the layout grid
  more conveniently.
  
  Since this is an abstract base class, you can't instantiate it directly. Rather use one of its
  subclasses like QCPLayoutGrid or QCPLayoutInset.
  
  For a general introduction to the layout system, see the dedicated documentation page \ref
  thelayoutsystem "The Layout System".
*/

/* start documentation of pure virtual functions */

/*! \fn virtual int QCPLayout::elementCount() const = 0
  
  Returns the number of elements/cells in the layout.
  
  \see elements, elementAt
*/

/*! \fn virtual QCPLayoutElement* QCPLayout::elementAt(int index) const = 0
  
  Returns the element in the cell with the given \a index. If \a index is invalid, returns 0.
  
  Note that even if \a index is valid, the respective cell may be empty in some layouts (e.g.
  QCPLayoutGrid), so this function may return 0 in those cases. You may use this function to check
  whether a cell is empty or not.
  
  \see elements, elementCount, takeAt
*/

/*! \fn virtual QCPLayoutElement* QCPLayout::takeAt(int index) = 0
  
  Removes the element with the given \a index from the layout and returns it.
  
  If the \a index is invalid or the cell with that index is empty, returns 0.
  
  Note that some layouts don't remove the respective cell right away but leave an empty cell after
  successful removal of the layout element. To collapse empty cells, use \ref simplify.
  
  \see elementAt, take
*/

/*! \fn virtual bool QCPLayout::take(QCPLayoutElement* element) = 0
  
  Removes the specified \a element from the layout and returns true on success.
  
  If the \a element isn't in this layout, returns false.
  
  Note that some layouts don't remove the respective cell right away but leave an empty cell after
  successful removal of the layout element. To collapse empty cells, use \ref simplify.
  
  \see takeAt
*/

/* end documentation of pure virtual functions */

/*!
  Creates an instance of QCPLayout and sets default values. Note that since QCPLayout
  is an abstract base class, it can't be instantiated directly.
*/
QCPLayout::QCPLayout()
{
}

/*!
  If \a phase is \ref upLayout, calls \ref updateLayout, which subclasses may reimplement to
  reposition and resize their cells.
  
  Finally, the call is propagated down to all child \ref QCPLayoutElement "QCPLayoutElements".
  
  For details about this method and the update phases, see the documentation of \ref
  QCPLayoutElement::update.
*/
void QCPLayout::update(UpdatePhase phase)
{
    QCPLayoutElement::update(phase);

    // set child element rects according to layout:
    if (phase == upLayout)
        updateLayout();

    // propagate update call to child elements:
    const int elCount = elementCount();
    for (int i=0; i<elCount; ++i)
    {
        if (QCPLayoutElement *el = elementAt(i))
            el->update(phase);
    }
}

/* inherits documentation from base class */
QList<QCPLayoutElement*> QCPLayout::elements(bool recursive) const
{
    const int c = elementCount();
    QList<QCPLayoutElement*> result;
#if QT_VERSION >= QT_VERSION_CHECK(4, 7, 0)
    result.reserve(c);
#endif
    for (int i=0; i<c; ++i)
        result.append(elementAt(i));
    if (recursive)
    {
        for (int i=0; i<c; ++i)
        {
            if (result.at(i))
                result << result.at(i)->elements(recursive);
        }
    }
    return result;
}

/*!
  Simplifies the layout by collapsing empty cells. The exact behavior depends on subclasses, the
  default implementation does nothing.
  
  Not all layouts need simplification. For example, QCPLayoutInset doesn't use explicit
  simplification while QCPLayoutGrid does.
*/
void QCPLayout::simplify()
{
}

/*!
  Removes and deletes the element at the provided \a index. Returns true on success. If \a index is
  invalid or points to an empty cell, returns false.
  
  This function internally uses \ref takeAt to remove the element from the layout and then deletes
  the returned element. Note that some layouts don't remove the respective cell right away but leave an
  empty cell after successful removal of the layout element. To collapse empty cells, use \ref
  simplify.
  
  \see remove, takeAt
*/
bool QCPLayout::removeAt(int index)
{
    if (QCPLayoutElement *el = takeAt(index))
    {
        delete el;
        return true;
    } else
        return false;
}

/*!
  Removes and deletes the provided \a element. Returns true on success. If \a element is not in the
  layout, returns false.
  
  This function internally uses \ref takeAt to remove the element from the layout and then deletes
  the element. Note that some layouts don't remove the respective cell right away but leave an
  empty cell after successful removal of the layout element. To collapse empty cells, use \ref
  simplify.
  
  \see removeAt, take
*/
bool QCPLayout::remove(QCPLayoutElement *element)
{
    if (take(element))
    {
        delete element;
        return true;
    } else
        return false;
}

/*!
  Removes and deletes all layout elements in this layout. Finally calls \ref simplify to make sure
  all empty cells are collapsed.
  
  \see remove, removeAt
*/
void QCPLayout::clear()
{
    for (int i=elementCount()-1; i>=0; --i)
    {
        if (elementAt(i))
            removeAt(i);
    }
    simplify();
}

/*!
  Subclasses call this method to report changed (minimum/maximum) size constraints.
  
  If the parent of this layout is again a QCPLayout, forwards the call to the parent's \ref
  sizeConstraintsChanged. If the parent is a QWidget (i.e. is the \ref PF_GraphicView::plotLayout of
  PF_GraphicView), calls QWidget::updateGeometry, so if the PF_GraphicView widget is inside a Qt QLayout,
  it may update itself and resize cells accordingly.
*/
void QCPLayout::sizeConstraintsChanged() const
{
    if (QWidget *w = qobject_cast<QWidget*>(parent()))
        w->updateGeometry();
    else if (QCPLayout *l = qobject_cast<QCPLayout*>(parent()))
        l->sizeConstraintsChanged();
}

/*! \internal
  
  Subclasses reimplement this method to update the position and sizes of the child elements/cells
  via calling their \ref QCPLayoutElement::setOuterRect. The default implementation does nothing.
  
  The geometry used as a reference is the inner \ref rect of this layout. Child elements should stay
  within that rect.
  
  \ref getSectionSizes may help with the reimplementation of this function.
  
  \see update
*/
void QCPLayout::updateLayout()
{
}


/*! \internal
  
  Associates \a el with this layout. This is done by setting the \ref QCPLayoutElement::layout, the
  \ref QCPLayerable::parentLayerable and the QObject parent to this layout.
  
  Further, if \a el didn't previously have a parent plot, calls \ref
  QCPLayerable::initializeParentPlot on \a el to set the paret plot.
  
  This method is used by subclass specific methods that add elements to the layout. Note that this
  method only changes properties in \a el. The removal from the old layout and the insertion into
  the new layout must be done additionally.
*/
void QCPLayout::adoptElement(QCPLayoutElement *el)
{
    if (el)
    {
        el->mParentLayout = this;
        el->setParentLayerable(this);
        el->setParent(this);
        if (!el->parentPlot())
            el->initializeParentPlot(mParentPlot);
        el->layoutChanged();
    } else
        qDebug() << Q_FUNC_INFO << "Null element passed";
}

/*! \internal
  
  Disassociates \a el from this layout. This is done by setting the \ref QCPLayoutElement::layout
  and the \ref QCPLayerable::parentLayerable to zero. The QObject parent is set to the parent
  PF_GraphicView.
  
  This method is used by subclass specific methods that remove elements from the layout (e.g. \ref
  take or \ref takeAt). Note that this method only changes properties in \a el. The removal from
  the old layout must be done additionally.
*/
void QCPLayout::releaseElement(QCPLayoutElement *el)
{
    if (el)
    {
        el->mParentLayout = nullptr;
        el->setParentLayerable(nullptr);
        el->setParent(mParentPlot);
        // Note: Don't initializeParentPlot(0) here, because layout element will stay in same parent plot
    } else
        qDebug() << Q_FUNC_INFO << "Null element passed";
}

/*! \internal
  
  This is a helper function for the implementation of \ref updateLayout in subclasses.
  
  It calculates the sizes of one-dimensional sections with provided constraints on maximum section
  sizes, minimum section sizes, relative stretch factors and the final total size of all sections.
  
  The QVector entries refer to the sections. Thus all QVectors must have the same size.
  
  \a maxSizes gives the maximum allowed size of each section. If there shall be no maximum size
  imposed, set all vector values to Qt's QWIDGETSIZE_MAX.
  
  \a minSizes gives the minimum allowed size of each section. If there shall be no minimum size
  imposed, set all vector values to zero. If the \a minSizes entries add up to a value greater than
  \a totalSize, sections will be scaled smaller than the proposed minimum sizes. (In other words,
  not exceeding the allowed total size is taken to be more important than not going below minimum
  section sizes.)
  
  \a stretchFactors give the relative proportions of the sections to each other. If all sections
  shall be scaled equally, set all values equal. If the first section shall be double the size of
  each individual other section, set the first number of \a stretchFactors to double the value of
  the other individual values (e.g. {2, 1, 1, 1}).
  
  \a totalSize is the value that the final section sizes will add up to. Due to rounding, the
  actual sum may differ slightly. If you want the section sizes to sum up to exactly that value,
  you could distribute the remaining difference on the sections.
  
  The return value is a QVector containing the section sizes.
*/
QVector<int> QCPLayout::getSectionSizes(QVector<int> maxSizes, QVector<int> minSizes, QVector<double> stretchFactors, int totalSize) const
{
    if (maxSizes.size() != minSizes.size() || minSizes.size() != stretchFactors.size())
    {
        qDebug() << Q_FUNC_INFO << "Passed vector sizes aren't equal:" << maxSizes << minSizes << stretchFactors;
        return QVector<int>();
    }
    if (stretchFactors.isEmpty())
        return QVector<int>();
    int sectionCount = stretchFactors.size();
    QVector<double> sectionSizes(sectionCount);
    // if provided total size is forced smaller than total minimum size, ignore minimum sizes (squeeze sections):
    int minSizeSum = 0;
    for (int i=0; i<sectionCount; ++i)
        minSizeSum += minSizes.at(i);
    if (totalSize < minSizeSum)
    {
        // new stretch factors are minimum sizes and minimum sizes are set to zero:
        for (int i=0; i<sectionCount; ++i)
        {
            stretchFactors[i] = minSizes.at(i);
            minSizes[i] = 0;
        }
    }

    QList<int> minimumLockedSections;
    QList<int> unfinishedSections;
    for (int i=0; i<sectionCount; ++i)
        unfinishedSections.append(i);
    double freeSize = totalSize;

    int outerIterations = 0;
    while (!unfinishedSections.isEmpty() && outerIterations < sectionCount*2) // the iteration check ist just a failsafe in case something really strange happens
    {
        ++outerIterations;
        int innerIterations = 0;
        while (!unfinishedSections.isEmpty() && innerIterations < sectionCount*2) // the iteration check ist just a failsafe in case something really strange happens
        {
            ++innerIterations;
            // find section that hits its maximum next:
            int nextId = -1;
            double nextMax = 1e12;
            for (int i=0; i<unfinishedSections.size(); ++i)
            {
                int secId = unfinishedSections.at(i);
                double hitsMaxAt = (maxSizes.at(secId)-sectionSizes.at(secId))/stretchFactors.at(secId);
                if (hitsMaxAt < nextMax)
                {
                    nextMax = hitsMaxAt;
                    nextId = secId;
                }
            }
            // check if that maximum is actually within the bounds of the total size (i.e. can we stretch all remaining sections so far that the found section
            // actually hits its maximum, without exceeding the total size when we add up all sections)
            double stretchFactorSum = 0;
            for (int i=0; i<unfinishedSections.size(); ++i)
                stretchFactorSum += stretchFactors.at(unfinishedSections.at(i));
            double nextMaxLimit = freeSize/stretchFactorSum;
            if (nextMax < nextMaxLimit) // next maximum is actually hit, move forward to that point and fix the size of that section
            {
                for (int i=0; i<unfinishedSections.size(); ++i)
                {
                    sectionSizes[unfinishedSections.at(i)] += nextMax*stretchFactors.at(unfinishedSections.at(i)); // increment all sections
                    freeSize -= nextMax*stretchFactors.at(unfinishedSections.at(i));
                }
                unfinishedSections.removeOne(nextId); // exclude the section that is now at maximum from further changes
            } else // next maximum isn't hit, just distribute rest of free space on remaining sections
            {
                for (int i=0; i<unfinishedSections.size(); ++i)
                    sectionSizes[unfinishedSections.at(i)] += nextMaxLimit*stretchFactors.at(unfinishedSections.at(i)); // increment all sections
                unfinishedSections.clear();
            }
        }
        if (innerIterations == sectionCount*2)
            qDebug() << Q_FUNC_INFO << "Exceeded maximum expected inner iteration count, layouting aborted. Input was:" << maxSizes << minSizes << stretchFactors << totalSize;

        // now check whether the resulting section sizes violate minimum restrictions:
        bool foundMinimumViolation = false;
        for (int i=0; i<sectionSizes.size(); ++i)
        {
            if (minimumLockedSections.contains(i))
                continue;
            if (sectionSizes.at(i) < minSizes.at(i)) // section violates minimum
            {
                sectionSizes[i] = minSizes.at(i); // set it to minimum
                foundMinimumViolation = true; // make sure we repeat the whole optimization process
                minimumLockedSections.append(i);
            }
        }
        if (foundMinimumViolation)
        {
            freeSize = totalSize;
            for (int i=0; i<sectionCount; ++i)
            {
                if (!minimumLockedSections.contains(i)) // only put sections that haven't hit their minimum back into the pool
                    unfinishedSections.append(i);
                else
                    freeSize -= sectionSizes.at(i); // remove size of minimum locked sections from available space in next round
            }
            // reset all section sizes to zero that are in unfinished sections (all others have been set to their minimum):
            for (int i=0; i<unfinishedSections.size(); ++i)
                sectionSizes[unfinishedSections.at(i)] = 0;
        }
    }
    if (outerIterations == sectionCount*2)
        qDebug() << Q_FUNC_INFO << "Exceeded maximum expected outer iteration count, layouting aborted. Input was:" << maxSizes << minSizes << stretchFactors << totalSize;

    QVector<int> result(sectionCount);
    for (int i=0; i<sectionCount; ++i)
        result[i] = qRound(sectionSizes.at(i));
    return result;
}

/*! \internal
  
  This is a helper function for the implementation of subclasses.
  
  It returns the minimum size that should finally be used for the outer rect of the passed layout
  element \a el.
  
  It takes into account whether a manual minimum size is set (\ref
  QCPLayoutElement::setMinimumSize), which size constraint is set (\ref
  QCPLayoutElement::setSizeConstraintRect), as well as the minimum size hint, if no manual minimum
  size was set (\ref QCPLayoutElement::minimumOuterSizeHint).
*/
QSize QCPLayout::getFinalMinimumOuterSize(const QCPLayoutElement *el)
{
    QSize minOuterHint = el->minimumOuterSizeHint();
    QSize minOuter = el->minimumSize(); // depending on sizeConstraitRect this might be with respect to inner rect, so possibly add margins in next four lines (preserving unset minimum of 0)
    if (minOuter.width() > 0 && el->sizeConstraintRect() == QCPLayoutElement::scrInnerRect)
        minOuter.rwidth() += el->margins().left() + el->margins().right();
    if (minOuter.height() > 0 && el->sizeConstraintRect() == QCPLayoutElement::scrInnerRect)
        minOuter.rheight() += el->margins().top() + el->margins().bottom();

    return QSize(minOuter.width() > 0 ? minOuter.width() : minOuterHint.width(),
                 minOuter.height() > 0 ? minOuter.height() : minOuterHint.height());;
}

/*! \internal
  
  This is a helper function for the implementation of subclasses.
  
  It returns the maximum size that should finally be used for the outer rect of the passed layout
  element \a el.
  
  It takes into account whether a manual maximum size is set (\ref
  QCPLayoutElement::setMaximumSize), which size constraint is set (\ref
  QCPLayoutElement::setSizeConstraintRect), as well as the maximum size hint, if no manual maximum
  size was set (\ref QCPLayoutElement::maximumOuterSizeHint).
*/
QSize QCPLayout::getFinalMaximumOuterSize(const QCPLayoutElement *el)
{
    QSize maxOuterHint = el->maximumOuterSizeHint();
    QSize maxOuter = el->maximumSize(); // depending on sizeConstraitRect this might be with respect to inner rect, so possibly add margins in next four lines (preserving unset maximum of QWIDGETSIZE_MAX)
    if (maxOuter.width() < QWIDGETSIZE_MAX && el->sizeConstraintRect() == QCPLayoutElement::scrInnerRect)
        maxOuter.rwidth() += el->margins().left() + el->margins().right();
    if (maxOuter.height() < QWIDGETSIZE_MAX && el->sizeConstraintRect() == QCPLayoutElement::scrInnerRect)
        maxOuter.rheight() += el->margins().top() + el->margins().bottom();

    return QSize(maxOuter.width() < QWIDGETSIZE_MAX ? maxOuter.width() : maxOuterHint.width(),
                 maxOuter.height() < QWIDGETSIZE_MAX ? maxOuter.height() : maxOuterHint.height());
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPLayoutGrid
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPLayoutGrid
  \brief A layout that arranges child elements in a grid

  Elements are laid out in a grid with configurable stretch factors (\ref setColumnStretchFactor,
  \ref setRowStretchFactor) and spacing (\ref setColumnSpacing, \ref setRowSpacing).

  Elements can be added to cells via \ref addElement. The grid is expanded if the specified row or
  column doesn't exist yet. Whether a cell contains a valid layout element can be checked with \ref
  hasElement, that element can be retrieved with \ref element. If rows and columns that only have
  empty cells shall be removed, call \ref simplify. Removal of elements is either done by just
  adding the element to a different layout or by using the QCPLayout interface \ref take or \ref
  remove.

  If you use \ref addElement(QCPLayoutElement*) without explicit parameters for \a row and \a
  column, the grid layout will choose the position according to the current \ref setFillOrder and
  the wrapping (\ref setWrap).

  Row and column insertion can be performed with \ref insertRow and \ref insertColumn.
*/

/* start documentation of inline functions */

/*! \fn int QCPLayoutGrid::rowCount() const

  Returns the number of rows in the layout.

  \see columnCount
*/

/*! \fn int QCPLayoutGrid::columnCount() const

  Returns the number of columns in the layout.

  \see rowCount
*/

/* end documentation of inline functions */

/*!
  Creates an instance of QCPLayoutGrid and sets default values.
*/
QCPLayoutGrid::QCPLayoutGrid() :
    mColumnSpacing(5),
    mRowSpacing(5),
    mWrap(0),
    mFillOrder(foColumnsFirst)
{
}

QCPLayoutGrid::~QCPLayoutGrid()
{
    // clear all child layout elements. This is important because only the specific layouts know how
    // to handle removing elements (clear calls virtual removeAt method to do that).
    clear();
}

/*!
  Returns the element in the cell in \a row and \a column.
  
  Returns 0 if either the row/column is invalid or if the cell is empty. In those cases, a qDebug
  message is printed. To check whether a cell exists and isn't empty, use \ref hasElement.
  
  \see addElement, hasElement
*/
QCPLayoutElement *QCPLayoutGrid::element(int row, int column) const
{
    if (row >= 0 && row < mElements.size())
    {
        if (column >= 0 && column < mElements.first().size())
        {
            if (QCPLayoutElement *result = mElements.at(row).at(column))
                return result;
            else
                qDebug() << Q_FUNC_INFO << "Requested cell is empty. Row:" << row << "Column:" << column;
        } else
            qDebug() << Q_FUNC_INFO << "Invalid column. Row:" << row << "Column:" << column;
    } else
        qDebug() << Q_FUNC_INFO << "Invalid row. Row:" << row << "Column:" << column;
    return nullptr;
}


/*! \overload

  Adds the \a element to cell with \a row and \a column. If \a element is already in a layout, it
  is first removed from there. If \a row or \a column don't exist yet, the layout is expanded
  accordingly.

  Returns true if the element was added successfully, i.e. if the cell at \a row and \a column
  didn't already have an element.

  Use the overload of this method without explicit row/column index to place the element according
  to the configured fill order and wrapping settings.

  \see element, hasElement, take, remove
*/
bool QCPLayoutGrid::addElement(int row, int column, QCPLayoutElement *element)
{
    if (!hasElement(row, column))
    {
        if (element && element->layout()) // remove from old layout first
            element->layout()->take(element);
        expandTo(row+1, column+1);
        mElements[row][column] = element;
        if (element)
            adoptElement(element);
        return true;
    } else
        qDebug() << Q_FUNC_INFO << "There is already an element in the specified row/column:" << row << column;
    return false;
}

/*! \overload

  Adds the \a element to the next empty cell according to the current fill order (\ref
  setFillOrder) and wrapping (\ref setWrap). If \a element is already in a layout, it is first
  removed from there. If necessary, the layout is expanded to hold the new element.

  Returns true if the element was added successfully.

  \see setFillOrder, setWrap, element, hasElement, take, remove
*/
bool QCPLayoutGrid::addElement(QCPLayoutElement *element)
{
    int rowIndex = 0;
    int colIndex = 0;
    if (mFillOrder == foColumnsFirst)
    {
        while (hasElement(rowIndex, colIndex))
        {
            ++colIndex;
            if (colIndex >= mWrap && mWrap > 0)
            {
                colIndex = 0;
                ++rowIndex;
            }
        }
    } else
    {
        while (hasElement(rowIndex, colIndex))
        {
            ++rowIndex;
            if (rowIndex >= mWrap && mWrap > 0)
            {
                rowIndex = 0;
                ++colIndex;
            }
        }
    }
    return addElement(rowIndex, colIndex, element);
}

/*!
  Returns whether the cell at \a row and \a column exists and contains a valid element, i.e. isn't
  empty.
  
  \see element
*/
bool QCPLayoutGrid::hasElement(int row, int column)
{
    if (row >= 0 && row < rowCount() && column >= 0 && column < columnCount())
        return mElements.at(row).at(column);
    else
        return false;
}

/*!
  Sets the stretch \a factor of \a column.
  
  Stretch factors control the relative sizes of rows and columns. Cells will not be resized beyond
  their minimum and maximum widths/heights, regardless of the stretch factor. (see \ref
  QCPLayoutElement::setMinimumSize, \ref QCPLayoutElement::setMaximumSize, \ref
  QCPLayoutElement::setSizeConstraintRect.)
  
  The default stretch factor of newly created rows/columns is 1.
  
  \see setColumnStretchFactors, setRowStretchFactor
*/
void QCPLayoutGrid::setColumnStretchFactor(int column, double factor)
{
    if (column >= 0 && column < columnCount())
    {
        if (factor > 0)
            mColumnStretchFactors[column] = factor;
        else
            qDebug() << Q_FUNC_INFO << "Invalid stretch factor, must be positive:" << factor;
    } else
        qDebug() << Q_FUNC_INFO << "Invalid column:" << column;
}

/*!
  Sets the stretch \a factors of all columns. \a factors must have the size \ref columnCount.
  
  Stretch factors control the relative sizes of rows and columns. Cells will not be resized beyond
  their minimum and maximum widths/heights, regardless of the stretch factor. (see \ref
  QCPLayoutElement::setMinimumSize, \ref QCPLayoutElement::setMaximumSize, \ref
  QCPLayoutElement::setSizeConstraintRect.)
  
  The default stretch factor of newly created rows/columns is 1.
  
  \see setColumnStretchFactor, setRowStretchFactors
*/
void QCPLayoutGrid::setColumnStretchFactors(const QList<double> &factors)
{
    if (factors.size() == mColumnStretchFactors.size())
    {
        mColumnStretchFactors = factors;
        for (int i=0; i<mColumnStretchFactors.size(); ++i)
        {
            if (mColumnStretchFactors.at(i) <= 0)
            {
                qDebug() << Q_FUNC_INFO << "Invalid stretch factor, must be positive:" << mColumnStretchFactors.at(i);
                mColumnStretchFactors[i] = 1;
            }
        }
    } else
        qDebug() << Q_FUNC_INFO << "Column count not equal to passed stretch factor count:" << factors;
}

/*!
  Sets the stretch \a factor of \a row.
  
  Stretch factors control the relative sizes of rows and columns. Cells will not be resized beyond
  their minimum and maximum widths/heights, regardless of the stretch factor. (see \ref
  QCPLayoutElement::setMinimumSize, \ref QCPLayoutElement::setMaximumSize, \ref
  QCPLayoutElement::setSizeConstraintRect.)
  
  The default stretch factor of newly created rows/columns is 1.
  
  \see setColumnStretchFactors, setRowStretchFactor
*/
void QCPLayoutGrid::setRowStretchFactor(int row, double factor)
{
    if (row >= 0 && row < rowCount())
    {
        if (factor > 0)
            mRowStretchFactors[row] = factor;
        else
            qDebug() << Q_FUNC_INFO << "Invalid stretch factor, must be positive:" << factor;
    } else
        qDebug() << Q_FUNC_INFO << "Invalid row:" << row;
}

/*!
  Sets the stretch \a factors of all rows. \a factors must have the size \ref rowCount.
  
  Stretch factors control the relative sizes of rows and columns. Cells will not be resized beyond
  their minimum and maximum widths/heights, regardless of the stretch factor. (see \ref
  QCPLayoutElement::setMinimumSize, \ref QCPLayoutElement::setMaximumSize, \ref
  QCPLayoutElement::setSizeConstraintRect.)
  
  The default stretch factor of newly created rows/columns is 1.
  
  \see setRowStretchFactor, setColumnStretchFactors
*/
void QCPLayoutGrid::setRowStretchFactors(const QList<double> &factors)
{
    if (factors.size() == mRowStretchFactors.size())
    {
        mRowStretchFactors = factors;
        for (int i=0; i<mRowStretchFactors.size(); ++i)
        {
            if (mRowStretchFactors.at(i) <= 0)
            {
                qDebug() << Q_FUNC_INFO << "Invalid stretch factor, must be positive:" << mRowStretchFactors.at(i);
                mRowStretchFactors[i] = 1;
            }
        }
    } else
        qDebug() << Q_FUNC_INFO << "Row count not equal to passed stretch factor count:" << factors;
}

/*!
  Sets the gap that is left blank between columns to \a pixels.
  
  \see setRowSpacing
*/
void QCPLayoutGrid::setColumnSpacing(int pixels)
{
    mColumnSpacing = pixels;
}

/*!
  Sets the gap that is left blank between rows to \a pixels.
  
  \see setColumnSpacing
*/
void QCPLayoutGrid::setRowSpacing(int pixels)
{
    mRowSpacing = pixels;
}

/*!
  Sets the maximum number of columns or rows that are used, before new elements added with \ref
  addElement(QCPLayoutElement*) will start to fill the next row or column, respectively. It depends
  on \ref setFillOrder, whether rows or columns are wrapped.

  If \a count is set to zero, no wrapping will ever occur.
  
  If you wish to re-wrap the elements currently in the layout, call \ref setFillOrder with \a
  rearrange set to true (the actual fill order doesn't need to be changed for the rearranging to be
  done).

  Note that the method \ref addElement(int row, int column, QCPLayoutElement *element) with
  explicitly stated row and column is not subject to wrapping and can place elements even beyond
  the specified wrapping point.

  \see setFillOrder
*/
void QCPLayoutGrid::setWrap(int count)
{
    mWrap = qMax(0, count);
}

/*!
  Sets the filling order and wrapping behaviour that is used when adding new elements with the
  method \ref addElement(QCPLayoutElement*).

  The specified \a order defines whether rows or columns are filled first. Using \ref setWrap, you
  can control at which row/column count wrapping into the next column/row will occur. If you set it
  to zero, no wrapping will ever occur. Changing the fill order also changes the meaning of the
  linear index used e.g. in \ref elementAt and \ref takeAt. The default fill order for \ref
  QCPLayoutGrid is \ref foColumnsFirst.

  If you want to have all current elements arranged in the new order, set \a rearrange to true. The
  elements will be rearranged in a way that tries to preserve their linear index. However, empty
  cells are skipped during build-up of the new cell order, which shifts the succeeding element's
  index. The rearranging is performed even if the specified \a order is already the current fill
  order. Thus this method can be used to re-wrap the current elements.

  If \a rearrange is false, the current element arrangement is not changed, which means the
  linear indexes change (because the linear index is dependent on the fill order).

  Note that the method \ref addElement(int row, int column, QCPLayoutElement *element) with
  explicitly stated row and column is not subject to wrapping and can place elements even beyond
  the specified wrapping point.

  \see setWrap, addElement(QCPLayoutElement*)
*/
void QCPLayoutGrid::setFillOrder(FillOrder order, bool rearrange)
{
    // if rearranging, take all elements via linear index of old fill order:
    const int elCount = elementCount();
    QVector<QCPLayoutElement*> tempElements;
    if (rearrange)
    {
        tempElements.reserve(elCount);
        for (int i=0; i<elCount; ++i)
        {
            if (elementAt(i))
                tempElements.append(takeAt(i));
        }
        simplify();
    }
    // change fill order as requested:
    mFillOrder = order;
    // if rearranging, re-insert via linear index according to new fill order:
    if (rearrange)
    {
        for (int i=0; i<tempElements.size(); ++i)
            addElement(tempElements.at(i));
    }
}

/*!
  Expands the layout to have \a newRowCount rows and \a newColumnCount columns. So the last valid
  row index will be \a newRowCount-1, the last valid column index will be \a newColumnCount-1.
  
  If the current column/row count is already larger or equal to \a newColumnCount/\a newRowCount,
  this function does nothing in that dimension.
  
  Newly created cells are empty, new rows and columns have the stretch factor 1.
  
  Note that upon a call to \ref addElement, the layout is expanded automatically to contain the
  specified row and column, using this function.
  
  \see simplify
*/
void QCPLayoutGrid::expandTo(int newRowCount, int newColumnCount)
{
    // add rows as necessary:
    while (rowCount() < newRowCount)
    {
        mElements.append(QList<QCPLayoutElement*>());
        mRowStretchFactors.append(1);
    }
    // go through rows and expand columns as necessary:
    int newColCount = qMax(columnCount(), newColumnCount);
    for (int i=0; i<rowCount(); ++i)
    {
        while (mElements.at(i).size() < newColCount)
            mElements[i].append(nullptr);
    }
    while (mColumnStretchFactors.size() < newColCount)
        mColumnStretchFactors.append(1);
}

/*!
  Inserts a new row with empty cells at the row index \a newIndex. Valid values for \a newIndex
  range from 0 (inserts a row at the top) to \a rowCount (appends a row at the bottom).
  
  \see insertColumn
*/
void QCPLayoutGrid::insertRow(int newIndex)
{
    if (mElements.isEmpty() || mElements.first().isEmpty()) // if grid is completely empty, add first cell
    {
        expandTo(1, 1);
        return;
    }

    if (newIndex < 0)
        newIndex = 0;
    if (newIndex > rowCount())
        newIndex = rowCount();

    mRowStretchFactors.insert(newIndex, 1);
    QList<QCPLayoutElement*> newRow;
    for (int col=0; col<columnCount(); ++col)
        newRow.append((QCPLayoutElement*)nullptr);
    mElements.insert(newIndex, newRow);
}

/*!
  Inserts a new column with empty cells at the column index \a newIndex. Valid values for \a
  newIndex range from 0 (inserts a column at the left) to \a columnCount (appends a column at the
  right).
  
  \see insertRow
*/
void QCPLayoutGrid::insertColumn(int newIndex)
{
    if (mElements.isEmpty() || mElements.first().isEmpty()) // if grid is completely empty, add first cell
    {
        expandTo(1, 1);
        return;
    }

    if (newIndex < 0)
        newIndex = 0;
    if (newIndex > columnCount())
        newIndex = columnCount();

    mColumnStretchFactors.insert(newIndex, 1);
    for (int row=0; row<rowCount(); ++row)
        mElements[row].insert(newIndex, (QCPLayoutElement*)nullptr);
}

/*!
  Converts the given \a row and \a column to the linear index used by some methods of \ref
  QCPLayoutGrid and \ref QCPLayout.

  The way the cells are indexed depends on \ref setFillOrder. If it is \ref foRowsFirst, the
  indices increase left to right and then top to bottom. If it is \ref foColumnsFirst, the indices
  increase top to bottom and then left to right.

  For the returned index to be valid, \a row and \a column must be valid indices themselves, i.e.
  greater or equal to zero and smaller than the current \ref rowCount/\ref columnCount.

  \see indexToRowCol
*/
int QCPLayoutGrid::rowColToIndex(int row, int column) const
{
    if (row >= 0 && row < rowCount())
    {
        if (column >= 0 && column < columnCount())
        {
            switch (mFillOrder)
            {
            case foRowsFirst: return column*rowCount() + row;
            case foColumnsFirst: return row*columnCount() + column;
            }
        } else
            qDebug() << Q_FUNC_INFO << "row index out of bounds:" << row;
    } else
        qDebug() << Q_FUNC_INFO << "column index out of bounds:" << column;
    return 0;
}

/*!
  Converts the linear index to row and column indices and writes the result to \a row and \a
  column.

  The way the cells are indexed depends on \ref setFillOrder. If it is \ref foRowsFirst, the
  indices increase left to right and then top to bottom. If it is \ref foColumnsFirst, the indices
  increase top to bottom and then left to right.

  If there are no cells (i.e. column or row count is zero), sets \a row and \a column to -1.

  For the retrieved \a row and \a column to be valid, the passed \a index must be valid itself,
  i.e. greater or equal to zero and smaller than the current \ref elementCount.

  \see rowColToIndex
*/
void QCPLayoutGrid::indexToRowCol(int index, int &row, int &column) const
{
    row = -1;
    column = -1;
    const int nCols = columnCount();
    const int nRows = rowCount();
    if (nCols == 0 || nRows == 0)
        return;
    if (index < 0 || index >= elementCount())
    {
        qDebug() << Q_FUNC_INFO << "index out of bounds:" << index;
        return;
    }

    switch (mFillOrder)
    {
    case foRowsFirst:
    {
        column = index / nRows;
        row = index % nRows;
        break;
    }
    case foColumnsFirst:
    {
        row = index / nCols;
        column = index % nCols;
        break;
    }
    }
}

/* inherits documentation from base class */
void QCPLayoutGrid::updateLayout()
{
    QVector<int> minColWidths, minRowHeights, maxColWidths, maxRowHeights;
    getMinimumRowColSizes(&minColWidths, &minRowHeights);
    getMaximumRowColSizes(&maxColWidths, &maxRowHeights);

    int totalRowSpacing = (rowCount()-1) * mRowSpacing;
    int totalColSpacing = (columnCount()-1) * mColumnSpacing;
    QVector<int> colWidths = getSectionSizes(maxColWidths, minColWidths, mColumnStretchFactors.toVector(), mRect.width()-totalColSpacing);
    QVector<int> rowHeights = getSectionSizes(maxRowHeights, minRowHeights, mRowStretchFactors.toVector(), mRect.height()-totalRowSpacing);

    // go through cells and set rects accordingly:
    int yOffset = mRect.top();
    for (int row=0; row<rowCount(); ++row)
    {
        if (row > 0)
            yOffset += rowHeights.at(row-1)+mRowSpacing;
        int xOffset = mRect.left();
        for (int col=0; col<columnCount(); ++col)
        {
            if (col > 0)
                xOffset += colWidths.at(col-1)+mColumnSpacing;
            if (mElements.at(row).at(col))
                mElements.at(row).at(col)->setOuterRect(QRect(xOffset, yOffset, colWidths.at(col), rowHeights.at(row)));
        }
    }
}

/*!
  \seebaseclassmethod

  Note that the association of the linear \a index to the row/column based cells depends on the
  current setting of \ref setFillOrder.

  \see rowColToIndex
*/
QCPLayoutElement *QCPLayoutGrid::elementAt(int index) const
{
    if (index >= 0 && index < elementCount())
    {
        int row, col;
        indexToRowCol(index, row, col);
        return mElements.at(row).at(col);
    } else
        return nullptr;
}

/*!
  \seebaseclassmethod

  Note that the association of the linear \a index to the row/column based cells depends on the
  current setting of \ref setFillOrder.

  \see rowColToIndex
*/
QCPLayoutElement *QCPLayoutGrid::takeAt(int index)
{
    if (QCPLayoutElement *el = elementAt(index))
    {
        releaseElement(el);
        int row, col;
        indexToRowCol(index, row, col);
        mElements[row][col] = nullptr;
        return el;
    } else
    {
        qDebug() << Q_FUNC_INFO << "Attempt to take invalid index:" << index;
        return nullptr;
    }
}

/* inherits documentation from base class */
bool QCPLayoutGrid::take(QCPLayoutElement *element)
{
    if (element)
    {
        for (int i=0; i<elementCount(); ++i)
        {
            if (elementAt(i) == element)
            {
                takeAt(i);
                return true;
            }
        }
        qDebug() << Q_FUNC_INFO << "Element not in this layout, couldn't take";
    } else
        qDebug() << Q_FUNC_INFO << "Can't take null element";
    return false;
}

/* inherits documentation from base class */
QList<QCPLayoutElement*> QCPLayoutGrid::elements(bool recursive) const
{
    QList<QCPLayoutElement*> result;
    const int elCount = elementCount();
#if QT_VERSION >= QT_VERSION_CHECK(4, 7, 0)
    result.reserve(elCount);
#endif
    for (int i=0; i<elCount; ++i)
        result.append(elementAt(i));
    if (recursive)
    {
        for (int i=0; i<elCount; ++i)
        {
            if (result.at(i))
                result << result.at(i)->elements(recursive);
        }
    }
    return result;
}

/*!
  Simplifies the layout by collapsing rows and columns which only contain empty cells.
*/
void QCPLayoutGrid::simplify()
{
    // remove rows with only empty cells:
    for (int row=rowCount()-1; row>=0; --row)
    {
        bool hasElements = false;
        for (int col=0; col<columnCount(); ++col)
        {
            if (mElements.at(row).at(col))
            {
                hasElements = true;
                break;
            }
        }
        if (!hasElements)
        {
            mRowStretchFactors.removeAt(row);
            mElements.removeAt(row);
            if (mElements.isEmpty()) // removed last element, also remove stretch factor (wouldn't happen below because also columnCount changed to 0 now)
                mColumnStretchFactors.clear();
        }
    }

    // remove columns with only empty cells:
    for (int col=columnCount()-1; col>=0; --col)
    {
        bool hasElements = false;
        for (int row=0; row<rowCount(); ++row)
        {
            if (mElements.at(row).at(col))
            {
                hasElements = true;
                break;
            }
        }
        if (!hasElements)
        {
            mColumnStretchFactors.removeAt(col);
            for (int row=0; row<rowCount(); ++row)
                mElements[row].removeAt(col);
        }
    }
}

/* inherits documentation from base class */
QSize QCPLayoutGrid::minimumOuterSizeHint() const
{
    QVector<int> minColWidths, minRowHeights;
    getMinimumRowColSizes(&minColWidths, &minRowHeights);
    QSize result(0, 0);
    for (int i=0; i<minColWidths.size(); ++i)
        result.rwidth() += minColWidths.at(i);
    for (int i=0; i<minRowHeights.size(); ++i)
        result.rheight() += minRowHeights.at(i);
    result.rwidth() += qMax(0, columnCount()-1) * mColumnSpacing;
    result.rheight() += qMax(0, rowCount()-1) * mRowSpacing;
    result.rwidth() += mMargins.left()+mMargins.right();
    result.rheight() += mMargins.top()+mMargins.bottom();
    return result;
}

/* inherits documentation from base class */
QSize QCPLayoutGrid::maximumOuterSizeHint() const
{
    QVector<int> maxColWidths, maxRowHeights;
    getMaximumRowColSizes(&maxColWidths, &maxRowHeights);

    QSize result(0, 0);
    for (int i=0; i<maxColWidths.size(); ++i)
        result.setWidth(qMin(result.width()+maxColWidths.at(i), QWIDGETSIZE_MAX));
    for (int i=0; i<maxRowHeights.size(); ++i)
        result.setHeight(qMin(result.height()+maxRowHeights.at(i), QWIDGETSIZE_MAX));
    result.rwidth() += qMax(0, columnCount()-1) * mColumnSpacing;
    result.rheight() += qMax(0, rowCount()-1) * mRowSpacing;
    result.rwidth() += mMargins.left()+mMargins.right();
    result.rheight() += mMargins.top()+mMargins.bottom();
    if (result.height() > QWIDGETSIZE_MAX)
        result.setHeight(QWIDGETSIZE_MAX);
    if (result.width() > QWIDGETSIZE_MAX)
        result.setWidth(QWIDGETSIZE_MAX);
    return result;
}

/*! \internal
  
  Places the minimum column widths and row heights into \a minColWidths and \a minRowHeights
  respectively.
  
  The minimum height of a row is the largest minimum height of any element's outer rect in that
  row. The minimum width of a column is the largest minimum width of any element's outer rect in
  that column.
  
  This is a helper function for \ref updateLayout.
  
  \see getMaximumRowColSizes
*/
void QCPLayoutGrid::getMinimumRowColSizes(QVector<int> *minColWidths, QVector<int> *minRowHeights) const
{
    *minColWidths = QVector<int>(columnCount(), 0);
    *minRowHeights = QVector<int>(rowCount(), 0);
    for (int row=0; row<rowCount(); ++row)
    {
        for (int col=0; col<columnCount(); ++col)
        {
            if (QCPLayoutElement *el = mElements.at(row).at(col))
            {
                QSize minSize = getFinalMinimumOuterSize(el);
                if (minColWidths->at(col) < minSize.width())
                    (*minColWidths)[col] = minSize.width();
                if (minRowHeights->at(row) < minSize.height())
                    (*minRowHeights)[row] = minSize.height();
            }
        }
    }
}

/*! \internal
  
  Places the maximum column widths and row heights into \a maxColWidths and \a maxRowHeights
  respectively.
  
  The maximum height of a row is the smallest maximum height of any element's outer rect in that
  row. The maximum width of a column is the smallest maximum width of any element's outer rect in
  that column.
  
  This is a helper function for \ref updateLayout.
  
  \see getMinimumRowColSizes
*/
void QCPLayoutGrid::getMaximumRowColSizes(QVector<int> *maxColWidths, QVector<int> *maxRowHeights) const
{
    *maxColWidths = QVector<int>(columnCount(), QWIDGETSIZE_MAX);
    *maxRowHeights = QVector<int>(rowCount(), QWIDGETSIZE_MAX);
    for (int row=0; row<rowCount(); ++row)
    {
        for (int col=0; col<columnCount(); ++col)
        {
            if (QCPLayoutElement *el = mElements.at(row).at(col))
            {
                QSize maxSize = getFinalMaximumOuterSize(el);
                if (maxColWidths->at(col) > maxSize.width())
                    (*maxColWidths)[col] = maxSize.width();
                if (maxRowHeights->at(row) > maxSize.height())
                    (*maxRowHeights)[row] = maxSize.height();
            }
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPLayoutInset
////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \class QCPLayoutInset
  \brief A layout that places child elements aligned to the border or arbitrarily positioned
  
  Elements are placed either aligned to the border or at arbitrary position in the area of the
  layout. Which placement applies is controlled with the \ref InsetPlacement (\ref
  setInsetPlacement).

  Elements are added via \ref addElement(QCPLayoutElement *element, Qt::Alignment alignment) or
  addElement(QCPLayoutElement *element, const QRectF &rect). If the first method is used, the inset
  placement will default to \ref ipBorderAligned and the element will be aligned according to the
  \a alignment parameter. The second method defaults to \ref ipFree and allows placing elements at
  arbitrary position and size, defined by \a rect.
  
  The alignment or rect can be set via \ref setInsetAlignment or \ref setInsetRect, respectively.
  
  This is the layout that every QCPAxisRect has as \ref QCPAxisRect::insetLayout.
*/

/* start documentation of inline functions */

/*! \fn virtual void QCPLayoutInset::simplify()
  
  The QCPInsetLayout does not need simplification since it can never have empty cells due to its
  linear index structure. This method does nothing.
*/

/* end documentation of inline functions */

/*!
  Creates an instance of QCPLayoutInset and sets default values.
*/
QCPLayoutInset::QCPLayoutInset()
{
}

QCPLayoutInset::~QCPLayoutInset()
{
    // clear all child layout elements. This is important because only the specific layouts know how
    // to handle removing elements (clear calls virtual removeAt method to do that).
    clear();
}

/*!
  Returns the placement type of the element with the specified \a index.
*/
QCPLayoutInset::InsetPlacement QCPLayoutInset::insetPlacement(int index) const
{
    if (elementAt(index))
        return mInsetPlacement.at(index);
    else
    {
        qDebug() << Q_FUNC_INFO << "Invalid element index:" << index;
        return ipFree;
    }
}

/*!
  Returns the alignment of the element with the specified \a index. The alignment only has a
  meaning, if the inset placement (\ref setInsetPlacement) is \ref ipBorderAligned.
*/
Qt::Alignment QCPLayoutInset::insetAlignment(int index) const
{
    if (elementAt(index))
        return mInsetAlignment.at(index);
    else
    {
        qDebug() << Q_FUNC_INFO << "Invalid element index:" << index;
        return nullptr;
    }
}

/*!
  Returns the rect of the element with the specified \a index. The rect only has a
  meaning, if the inset placement (\ref setInsetPlacement) is \ref ipFree.
*/
QRectF QCPLayoutInset::insetRect(int index) const
{
    if (elementAt(index))
        return mInsetRect.at(index);
    else
    {
        qDebug() << Q_FUNC_INFO << "Invalid element index:" << index;
        return QRectF();
    }
}

/*!
  Sets the inset placement type of the element with the specified \a index to \a placement.
  
  \see InsetPlacement
*/
void QCPLayoutInset::setInsetPlacement(int index, QCPLayoutInset::InsetPlacement placement)
{
    if (elementAt(index))
        mInsetPlacement[index] = placement;
    else
        qDebug() << Q_FUNC_INFO << "Invalid element index:" << index;
}

/*!
  If the inset placement (\ref setInsetPlacement) is \ref ipBorderAligned, this function
  is used to set the alignment of the element with the specified \a index to \a alignment.
  
  \a alignment is an or combination of the following alignment flags: Qt::AlignLeft,
  Qt::AlignHCenter, Qt::AlighRight, Qt::AlignTop, Qt::AlignVCenter, Qt::AlignBottom. Any other
  alignment flags will be ignored.
*/
void QCPLayoutInset::setInsetAlignment(int index, Qt::Alignment alignment)
{
    if (elementAt(index))
        mInsetAlignment[index] = alignment;
    else
        qDebug() << Q_FUNC_INFO << "Invalid element index:" << index;
}

/*!
  If the inset placement (\ref setInsetPlacement) is \ref ipFree, this function is used to set the
  position and size of the element with the specified \a index to \a rect.
  
  \a rect is given in fractions of the whole inset layout rect. So an inset with rect (0, 0, 1, 1)
  will span the entire layout. An inset with rect (0.6, 0.1, 0.35, 0.35) will be in the top right
  corner of the layout, with 35% width and height of the parent layout.
  
  Note that the minimum and maximum sizes of the embedded element (\ref
  QCPLayoutElement::setMinimumSize, \ref QCPLayoutElement::setMaximumSize) are enforced.
*/
void QCPLayoutInset::setInsetRect(int index, const QRectF &rect)
{
    if (elementAt(index))
        mInsetRect[index] = rect;
    else
        qDebug() << Q_FUNC_INFO << "Invalid element index:" << index;
}

/* inherits documentation from base class */
void QCPLayoutInset::updateLayout()
{
    for (int i=0; i<mElements.size(); ++i)
    {
        QCPLayoutElement *el = mElements.at(i);
        QRect insetRect;
        QSize finalMinSize = getFinalMinimumOuterSize(el);
        QSize finalMaxSize = getFinalMaximumOuterSize(el);
        if (mInsetPlacement.at(i) == ipFree)
        {
            insetRect = QRect(rect().x()+rect().width()*mInsetRect.at(i).x(),
                              rect().y()+rect().height()*mInsetRect.at(i).y(),
                              rect().width()*mInsetRect.at(i).width(),
                              rect().height()*mInsetRect.at(i).height());
            if (insetRect.size().width() < finalMinSize.width())
                insetRect.setWidth(finalMinSize.width());
            if (insetRect.size().height() < finalMinSize.height())
                insetRect.setHeight(finalMinSize.height());
            if (insetRect.size().width() > finalMaxSize.width())
                insetRect.setWidth(finalMaxSize.width());
            if (insetRect.size().height() > finalMaxSize.height())
                insetRect.setHeight(finalMaxSize.height());
        } else if (mInsetPlacement.at(i) == ipBorderAligned)
        {
            insetRect.setSize(finalMinSize);
            Qt::Alignment al = mInsetAlignment.at(i);
            if (al.testFlag(Qt::AlignLeft)) insetRect.moveLeft(rect().x());
            else if (al.testFlag(Qt::AlignRight)) insetRect.moveRight(rect().x()+rect().width());
            else insetRect.moveLeft(rect().x()+rect().width()*0.5-finalMinSize.width()*0.5); // default to Qt::AlignHCenter
            if (al.testFlag(Qt::AlignTop)) insetRect.moveTop(rect().y());
            else if (al.testFlag(Qt::AlignBottom)) insetRect.moveBottom(rect().y()+rect().height());
            else insetRect.moveTop(rect().y()+rect().height()*0.5-finalMinSize.height()*0.5); // default to Qt::AlignVCenter
        }
        mElements.at(i)->setOuterRect(insetRect);
    }
}

/* inherits documentation from base class */
int QCPLayoutInset::elementCount() const
{
    return mElements.size();
}

/* inherits documentation from base class */
QCPLayoutElement *QCPLayoutInset::elementAt(int index) const
{
    if (index >= 0 && index < mElements.size())
        return mElements.at(index);
    else
        return nullptr;
}

/* inherits documentation from base class */
QCPLayoutElement *QCPLayoutInset::takeAt(int index)
{
    if (QCPLayoutElement *el = elementAt(index))
    {
        releaseElement(el);
        mElements.removeAt(index);
        mInsetPlacement.removeAt(index);
        mInsetAlignment.removeAt(index);
        mInsetRect.removeAt(index);
        return el;
    } else
    {
        qDebug() << Q_FUNC_INFO << "Attempt to take invalid index:" << index;
        return nullptr;
    }
}

/* inherits documentation from base class */
bool QCPLayoutInset::take(QCPLayoutElement *element)
{
    if (element)
    {
        for (int i=0; i<elementCount(); ++i)
        {
            if (elementAt(i) == element)
            {
                takeAt(i);
                return true;
            }
        }
        qDebug() << Q_FUNC_INFO << "Element not in this layout, couldn't take";
    } else
        qDebug() << Q_FUNC_INFO << "Can't take null element";
    return false;
}

/*!
  The inset layout is sensitive to events only at areas where its (visible) child elements are
  sensitive. If the selectTest method of any of the child elements returns a positive number for \a
  pos, this method returns a value corresponding to 0.99 times the parent plot's selection
  tolerance. The inset layout is not selectable itself by default. So if \a onlySelectable is true,
  -1.0 is returned.
  
  See \ref QCPLayerable::selectTest for a general explanation of this virtual method.
*/
double QCPLayoutInset::selectTest(const QPointF &pos, bool onlySelectable, QVariant *details) const
{
    Q_UNUSED(details)
    if (onlySelectable)
        return -1;

    for (int i=0; i<mElements.size(); ++i)
    {
        // inset layout shall only return positive selectTest, if actually an inset object is at pos
        // else it would block the entire underlying QCPAxisRect with its surface.
        if (mElements.at(i)->realVisibility() && mElements.at(i)->selectTest(pos, onlySelectable) >= 0)
            return mParentPlot->selectionTolerance()*0.99;
    }
    return -1;
}

/*!
  Adds the specified \a element to the layout as an inset aligned at the border (\ref
  setInsetAlignment is initialized with \ref ipBorderAligned). The alignment is set to \a
  alignment.
  
  \a alignment is an or combination of the following alignment flags: Qt::AlignLeft,
  Qt::AlignHCenter, Qt::AlighRight, Qt::AlignTop, Qt::AlignVCenter, Qt::AlignBottom. Any other
  alignment flags will be ignored.
  
  \see addElement(QCPLayoutElement *element, const QRectF &rect)
*/
void QCPLayoutInset::addElement(QCPLayoutElement *element, Qt::Alignment alignment)
{
    if (element)
    {
        if (element->layout()) // remove from old layout first
            element->layout()->take(element);
        mElements.append(element);
        mInsetPlacement.append(ipBorderAligned);
        mInsetAlignment.append(alignment);
        mInsetRect.append(QRectF(0.6, 0.6, 0.4, 0.4));
        adoptElement(element);
    } else
        qDebug() << Q_FUNC_INFO << "Can't add null element";
}

/*!
  Adds the specified \a element to the layout as an inset with free positioning/sizing (\ref
  setInsetAlignment is initialized with \ref ipFree). The position and size is set to \a
  rect.
  
  \a rect is given in fractions of the whole inset layout rect. So an inset with rect (0, 0, 1, 1)
  will span the entire layout. An inset with rect (0.6, 0.1, 0.35, 0.35) will be in the top right
  corner of the layout, with 35% width and height of the parent layout.
  
  \see addElement(QCPLayoutElement *element, Qt::Alignment alignment)
*/
void QCPLayoutInset::addElement(QCPLayoutElement *element, const QRectF &rect)
{
    if (element)
    {
        if (element->layout()) // remove from old layout first
            element->layout()->take(element);
        mElements.append(element);
        mInsetPlacement.append(ipFree);
        mInsetAlignment.append(Qt::AlignRight|Qt::AlignTop);
        mInsetRect.append(rect);
        adoptElement(element);
    } else
        qDebug() << Q_FUNC_INFO << "Can't add null element";
}
/* end of 'src/layout.cpp' */


/* including file 'src/lineending.cpp', size 11536                           */
/* commit ce344b3f96a62e5f652585e55f1ae7c7883cd45b 2018-06-25 01:03:39 +0200 */

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPLineEnding
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPLineEnding
  \brief Handles the different ending decorations for line-like items
  
  \image html QCPLineEnding.png "The various ending styles currently supported"
  
  For every ending a line-like item has, an instance of this class exists. For example, QCPItemLine
  has two endings which can be set with QCPItemLine::setHead and QCPItemLine::setTail.

  The styles themselves are defined via the enum QCPLineEnding::EndingStyle. Most decorations can
  be modified regarding width and length, see \ref setWidth and \ref setLength. The direction of
  the ending decoration (e.g. direction an arrow is pointing) is controlled by the line-like item.
  For example, when both endings of a QCPItemLine are set to be arrows, they will point to opposite
  directions, e.g. "outward". This can be changed by \ref setInverted, which would make the
  respective arrow point inward.
  
  Note that due to the overloaded QCPLineEnding constructor, you may directly specify a
  QCPLineEnding::EndingStyle where actually a QCPLineEnding is expected, e.g.
  \snippet documentation/doc-code-snippets/mainwindow.cpp qcplineending-sethead
*/

/*!
  Creates a QCPLineEnding instance with default values (style \ref esNone).
*/
QCPLineEnding::QCPLineEnding() :
    mStyle(esNone),
    mWidth(8),
    mLength(10),
    mInverted(false)
{
}

/*!
  Creates a QCPLineEnding instance with the specified values.
*/
QCPLineEnding::QCPLineEnding(QCPLineEnding::EndingStyle style, double width, double length, bool inverted) :
    mStyle(style),
    mWidth(width),
    mLength(length),
    mInverted(inverted)
{
}

/*!
  Sets the style of the ending decoration.
*/
void QCPLineEnding::setStyle(QCPLineEnding::EndingStyle style)
{
    mStyle = style;
}

/*!
  Sets the width of the ending decoration, if the style supports it. On arrows, for example, the
  width defines the size perpendicular to the arrow's pointing direction.
  
  \see setLength
*/
void QCPLineEnding::setWidth(double width)
{
    mWidth = width;
}

/*!
  Sets the length of the ending decoration, if the style supports it. On arrows, for example, the
  length defines the size in pointing direction.
  
  \see setWidth
*/
void QCPLineEnding::setLength(double length)
{
    mLength = length;
}

/*!
  Sets whether the ending decoration shall be inverted. For example, an arrow decoration will point
  inward when \a inverted is set to true.

  Note that also the \a width direction is inverted. For symmetrical ending styles like arrows or
  discs, this doesn't make a difference. However, asymmetric styles like \ref esHalfBar are
  affected by it, which can be used to control to which side the half bar points to.
*/
void QCPLineEnding::setInverted(bool inverted)
{
    mInverted = inverted;
}

/*! \internal
  
  Returns the maximum pixel radius the ending decoration might cover, starting from the position
  the decoration is drawn at (typically a line ending/\ref QCPItemPosition of an item).
  
  This is relevant for clipping. Only omit painting of the decoration when the position where the
  decoration is supposed to be drawn is farther away from the clipping rect than the returned
  distance.
*/
double QCPLineEnding::boundingDistance() const
{
    switch (mStyle)
    {
    case esNone:
        return 0;

    case esFlatArrow:
    case esSpikeArrow:
    case esLineArrow:
    case esSkewedBar:
        return qSqrt(mWidth*mWidth+mLength*mLength); // items that have width and length

    case esDisc:
    case esSquare:
    case esDiamond:
    case esBar:
    case esHalfBar:
        return mWidth*1.42; // items that only have a width -> width*sqrt(2)

    }
    return 0;
}

/*!
  Starting from the origin of this line ending (which is style specific), returns the length
  covered by the line ending symbol, in backward direction.
  
  For example, the \ref esSpikeArrow has a shorter real length than a \ref esFlatArrow, even if
  both have the same \ref setLength value, because the spike arrow has an inward curved back, which
  reduces the length along its center axis (the drawing origin for arrows is at the tip).
  
  This function is used for precise, style specific placement of line endings, for example in
  QCPAxes.
*/
double QCPLineEnding::realLength() const
{
    switch (mStyle)
    {
    case esNone:
    case esLineArrow:
    case esSkewedBar:
    case esBar:
    case esHalfBar:
        return 0;

    case esFlatArrow:
        return mLength;

    case esDisc:
    case esSquare:
    case esDiamond:
        return mWidth*0.5;

    case esSpikeArrow:
        return mLength*0.8;
    }
    return 0;
}

/*! \internal
  
  Draws the line ending with the specified \a painter at the position \a pos. The direction of the
  line ending is controlled with \a dir.
*/
void QCPLineEnding::draw(QCPPainter *painter, const QCPVector2D &pos, const QCPVector2D &dir) const
{
    if (mStyle == esNone)
        return;

    QCPVector2D lengthVec = dir.normalized() * mLength*(mInverted ? -1 : 1);
    if (lengthVec.isNull())
        lengthVec = QCPVector2D(1, 0);
    QCPVector2D widthVec = dir.normalized().perpendicular() * mWidth*0.5*(mInverted ? -1 : 1);

    QPen penBackup = painter->pen();
    QBrush brushBackup = painter->brush();
    QPen miterPen = penBackup;
    miterPen.setJoinStyle(Qt::MiterJoin); // to make arrow heads spikey
    QBrush brush(painter->pen().color(), Qt::SolidPattern);
    switch (mStyle)
    {
    case esNone: break;
    case esFlatArrow:
    {
        QPointF points[3] = {pos.toPointF(),
                             (pos-lengthVec+widthVec).toPointF(),
                             (pos-lengthVec-widthVec).toPointF()
                            };
        painter->setPen(miterPen);
        painter->setBrush(brush);
        painter->drawConvexPolygon(points, 3);
        painter->setBrush(brushBackup);
        painter->setPen(penBackup);
        break;
    }
    case esSpikeArrow:
    {
        QPointF points[4] = {pos.toPointF(),
                             (pos-lengthVec+widthVec).toPointF(),
                             (pos-lengthVec*0.8).toPointF(),
                             (pos-lengthVec-widthVec).toPointF()
                            };
        painter->setPen(miterPen);
        painter->setBrush(brush);
        painter->drawConvexPolygon(points, 4);
        painter->setBrush(brushBackup);
        painter->setPen(penBackup);
        break;
    }
    case esLineArrow:
    {
        QPointF points[3] = {(pos-lengthVec+widthVec).toPointF(),
                             pos.toPointF(),
                             (pos-lengthVec-widthVec).toPointF()
                            };
        painter->setPen(miterPen);
        painter->drawPolyline(points, 3);
        painter->setPen(penBackup);
        break;
    }
    case esDisc:
    {
        painter->setBrush(brush);
        painter->drawEllipse(pos.toPointF(),  mWidth*0.5, mWidth*0.5);
        painter->setBrush(brushBackup);
        break;
    }
    case esSquare:
    {
        QCPVector2D widthVecPerp = widthVec.perpendicular();
        QPointF points[4] = {(pos-widthVecPerp+widthVec).toPointF(),
                             (pos-widthVecPerp-widthVec).toPointF(),
                             (pos+widthVecPerp-widthVec).toPointF(),
                             (pos+widthVecPerp+widthVec).toPointF()
                            };
        painter->setPen(miterPen);
        painter->setBrush(brush);
        painter->drawConvexPolygon(points, 4);
        painter->setBrush(brushBackup);
        painter->setPen(penBackup);
        break;
    }
    case esDiamond:
    {
        QCPVector2D widthVecPerp = widthVec.perpendicular();
        QPointF points[4] = {(pos-widthVecPerp).toPointF(),
                             (pos-widthVec).toPointF(),
                             (pos+widthVecPerp).toPointF(),
                             (pos+widthVec).toPointF()
                            };
        painter->setPen(miterPen);
        painter->setBrush(brush);
        painter->drawConvexPolygon(points, 4);
        painter->setBrush(brushBackup);
        painter->setPen(penBackup);
        break;
    }
    case esBar:
    {
        painter->drawLine((pos+widthVec).toPointF(), (pos-widthVec).toPointF());
        break;
    }
    case esHalfBar:
    {
        painter->drawLine((pos+widthVec).toPointF(), pos.toPointF());
        break;
    }
    case esSkewedBar:
    {
        if (qFuzzyIsNull(painter->pen().widthF()) && !painter->modes().testFlag(QCPPainter::pmNonCosmetic))
        {
            // if drawing with cosmetic pen (perfectly thin stroke, happens only in vector exports), draw bar exactly on tip of line
            painter->drawLine((pos+widthVec+lengthVec*0.2*(mInverted?-1:1)).toPointF(),
                              (pos-widthVec-lengthVec*0.2*(mInverted?-1:1)).toPointF());
        } else
        {
            // if drawing with thick (non-cosmetic) pen, shift bar a little in line direction to prevent line from sticking through bar slightly
            painter->drawLine((pos+widthVec+lengthVec*0.2*(mInverted?-1:1)+dir.normalized()*qMax(1.0f, (float)painter->pen().widthF())*0.5f).toPointF(),
                              (pos-widthVec-lengthVec*0.2*(mInverted?-1:1)+dir.normalized()*qMax(1.0f, (float)painter->pen().widthF())*0.5f).toPointF());
        }
        break;
    }
    }
}

/*! \internal
  \overload
  
  Draws the line ending. The direction is controlled with the \a angle parameter in radians.
*/
void QCPLineEnding::draw(QCPPainter *painter, const QCPVector2D &pos, double angle) const
{
    draw(painter, pos, QCPVector2D(qCos(angle), qSin(angle)));
}
/* end of 'src/lineending.cpp' */


/* including file 'src/axis/axisticker.cpp', size 18664                      */
/* commit ce344b3f96a62e5f652585e55f1ae7c7883cd45b 2018-06-25 01:03:39 +0200 */

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPAxisTicker
////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \class QCPAxisTicker
  \brief The base class tick generator used by QCPAxis to create tick positions and tick labels
  
  Each QCPAxis has an internal QCPAxisTicker (or a subclass) in order to generate tick positions
  and tick labels for the current axis range. The ticker of an axis can be set via \ref
  QCPAxis::setTicker. Since that method takes a <tt>QSharedPointer<QCPAxisTicker></tt>, multiple
  axes can share the same ticker instance.
  
  This base class generates normal tick coordinates and numeric labels for linear axes. It picks a
  reasonable tick step (the separation between ticks) which results in readable tick labels. The
  number of ticks that should be approximately generated can be set via \ref setTickCount.
  Depending on the current tick step strategy (\ref setTickStepStrategy), the algorithm either
  sacrifices readability to better match the specified tick count (\ref
  QCPAxisTicker::tssMeetTickCount) or relaxes the tick count in favor of better tick steps (\ref
  QCPAxisTicker::tssReadability), which is the default.
  
  The following more specialized axis ticker subclasses are available, see details in the
  respective class documentation:
  
  <center>
  <table>
  <tr><td style="text-align:right; padding: 0 1em">QCPAxisTickerFixed</td><td>\image html axisticker-fixed.png</td></tr>
  <tr><td style="text-align:right; padding: 0 1em">QCPAxisTickerLog</td><td>\image html axisticker-log.png</td></tr>
  <tr><td style="text-align:right; padding: 0 1em">QCPAxisTickerPi</td><td>\image html axisticker-pi.png</td></tr>
  <tr><td style="text-align:right; padding: 0 1em">QCPAxisTickerText</td><td>\image html axisticker-text.png</td></tr>
  <tr><td style="text-align:right; padding: 0 1em">QCPAxisTickerDateTime</td><td>\image html axisticker-datetime.png</td></tr>
  <tr><td style="text-align:right; padding: 0 1em">QCPAxisTickerTime</td><td>\image html axisticker-time.png
    \image html axisticker-time2.png</td></tr>
  </table>
  </center>
  
  \section axisticker-subclassing Creating own axis tickers
  
  Creating own axis tickers can be achieved very easily by sublassing QCPAxisTicker and
  reimplementing some or all of the available virtual methods.

  In the simplest case you might wish to just generate different tick steps than the other tickers,
  so you only reimplement the method \ref getTickStep. If you additionally want control over the
  string that will be shown as tick label, reimplement \ref getTickLabel.
  
  If you wish to have complete control, you can generate the tick vectors and tick label vectors
  yourself by reimplementing \ref createTickVector and \ref createLabelVector. The default
  implementations use the previously mentioned virtual methods \ref getTickStep and \ref
  getTickLabel, but your reimplementations don't necessarily need to do so. For example in the case
  of unequal tick steps, the method \ref getTickStep loses its usefulness and can be ignored.
  
  The sub tick count between major ticks can be controlled with \ref getSubTickCount. Full sub tick
  placement control is obtained by reimplementing \ref createSubTickVector.
  
  See the documentation of all these virtual methods in QCPAxisTicker for detailed information
  about the parameters and expected return values.
*/

/*!
  Constructs the ticker and sets reasonable default values. Axis tickers are commonly created
  managed by a QSharedPointer, which then can be passed to QCPAxis::setTicker.
*/
QCPAxisTicker::QCPAxisTicker() :
    mTickStepStrategy(tssReadability),
    mTickCount(5),
    mTickOrigin(0),
    mTickStep(1),
    mAutoTickStep(true)
{
}

QCPAxisTicker::~QCPAxisTicker()
{

}

/*!
  Sets which strategy the axis ticker follows when choosing the size of the tick step. For the
  available strategies, see \ref TickStepStrategy.
*/
void QCPAxisTicker::setTickStepStrategy(QCPAxisTicker::TickStepStrategy strategy)
{
    mTickStepStrategy = strategy;
}

/*!
  Sets how many ticks this ticker shall aim to generate across the axis range. Note that \a count
  is not guaranteed to be matched exactly, as generating readable tick intervals may conflict with
  the requested number of ticks.

  Whether the readability has priority over meeting the requested \a count can be specified with
  \ref setTickStepStrategy.
*/
void QCPAxisTicker::setTickCount(int count)
{
    if (count > 0)
        mTickCount = count;
    else
        qDebug() << Q_FUNC_INFO << "tick count must be greater than zero:" << count;
}

/*!
  Sets the mathematical coordinate (or "offset") of the zeroth tick. This tick coordinate is just a
  concept and doesn't need to be inside the currently visible axis range.
  
  By default \a origin is zero, which for example yields ticks {-5, 0, 5, 10, 15,...} when the tick
  step is five. If \a origin is now set to 1 instead, the correspondingly generated ticks would be
  {-4, 1, 6, 11, 16,...}.
*/
void QCPAxisTicker::setTickOrigin(double origin)
{
    mTickOrigin = origin;
}
/*!
  Sets whether the tick step, i.e. the interval between two (major) ticks, is calculated
  automatically. If \a on is set to true, the axis finds a tick step that is reasonable for human
  readable plots.

  The number of ticks the algorithm aims for within the visible range can be specified with \ref
  setAutoTickCount.

  If \a on is set to false, you may set the tick step manually with \ref setTickStep.

  \see setAutoTicks, setAutoSubTicks, setAutoTickCount
*/
void QCPAxisTicker::setAutoTickStep(bool on)
{
    if (mAutoTickStep != on)
    {
        mAutoTickStep = on;
    }
}

/*!
  If \ref setAutoTickStep is set to false, use this function to set the tick step manually.
  The tick step is the interval between (major) ticks, in plot coordinates.
  \see setSubTickCount
*/
void QCPAxisTicker::setTickStep(double step)
{
  if (mTickStep != step)
  {
    mTickStep = step;
  }
}
/*!
  This is the method called by QCPAxis in order to actually generate tick coordinates (\a ticks),
  tick label strings (\a tickLabels) and sub tick coordinates (\a subTicks).
  
  The ticks are generated for the specified \a range. The generated labels typically follow the
  specified \a locale, \a formatChar and number \a precision, however this might be different (or
  even irrelevant) for certain QCPAxisTicker subclasses.
  
  The output parameter \a ticks is filled with the generated tick positions in axis coordinates.
  The output parameters \a subTicks and \a tickLabels are optional (set them to 0 if not needed)
  and are respectively filled with sub tick coordinates, and tick label strings belonging to \a
  ticks by index.
*/
void QCPAxisTicker::generate(const QCPRange &range, const QLocale &locale, QChar formatChar, int precision, QVector<double> &ticks, QVector<double> *subTicks, QVector<QString> *tickLabels)
{
    // generate (major) ticks:
    if(mAutoTickStep)
        mTickStep = getTickStep(range);
    ticks = createTickVector(mTickStep, range);
    trimTicks(range, ticks, true); // trim ticks to visible range plus one outer tick on each side (incase a subclass createTickVector creates more)

    // generate sub ticks between major ticks:
    if (subTicks)
    {
        if (ticks.size() > 0)
        {
            *subTicks = createSubTickVector(getSubTickCount(mTickStep), ticks);
            trimTicks(range, *subTicks, false);
        } else
            *subTicks = QVector<double>();
    }

    // finally trim also outliers (no further clipping happens in axis drawing):
    trimTicks(range, ticks, false);
    // generate labels for visible ticks if requested:
    if (tickLabels)
        *tickLabels = createLabelVector(ticks, locale, formatChar, precision);
}

/*! \internal
  
  Takes the entire currently visible axis range and returns a sensible tick step in
  order to provide readable tick labels as well as a reasonable number of tick counts (see \ref
  setTickCount, \ref setTickStepStrategy).
  
  If a QCPAxisTicker subclass only wants a different tick step behaviour than the default
  implementation, it should reimplement this method. See \ref cleanMantissa for a possible helper
  function.
*/
double QCPAxisTicker::getTickStep(const QCPRange &range)
{
    double exactStep = range.size()/(double)(mTickCount+1e-10); // mTickCount ticks on average, the small addition is to prevent jitter on exact integers
    return cleanMantissa(exactStep);
}

/*! \internal
  
  Takes the \a tickStep, i.e. the distance between two consecutive ticks, and returns
  an appropriate number of sub ticks for that specific tick step.
  
  Note that a returned sub tick count of e.g. 4 will split each tick interval into 5 sections.
*/
int QCPAxisTicker::getSubTickCount(double tickStep)
{
    int result = 1; // default to 1, if no proper value can be found

    // separate integer and fractional part of mantissa:
    double epsilon = 0.01;
    double intPartf;
    int intPart;
    double fracPart = modf(getMantissa(tickStep), &intPartf);
    intPart = intPartf;

    // handle cases with (almost) integer mantissa:
    if (fracPart < epsilon || 1.0-fracPart < epsilon)
    {
        if (1.0-fracPart < epsilon)
            ++intPart;
        switch (intPart)
        {
        case 1: result = 4; break; // 1.0 -> 0.2 substep
        case 2: result = 3; break; // 2.0 -> 0.5 substep
        case 3: result = 2; break; // 3.0 -> 1.0 substep
        case 4: result = 3; break; // 4.0 -> 1.0 substep
        case 5: result = 4; break; // 5.0 -> 1.0 substep
        case 6: result = 2; break; // 6.0 -> 2.0 substep
        case 7: result = 6; break; // 7.0 -> 1.0 substep
        case 8: result = 3; break; // 8.0 -> 2.0 substep
        case 9: result = 2; break; // 9.0 -> 3.0 substep
        }
    } else
    {
        // handle cases with significantly fractional mantissa:
        if (qAbs(fracPart-0.5) < epsilon) // *.5 mantissa
        {
            switch (intPart)
            {
            case 1: result = 2; break; // 1.5 -> 0.5 substep
            case 2: result = 4; break; // 2.5 -> 0.5 substep
            case 3: result = 4; break; // 3.5 -> 0.7 substep
            case 4: result = 2; break; // 4.5 -> 1.5 substep
            case 5: result = 4; break; // 5.5 -> 1.1 substep (won't occur with default getTickStep from here on)
            case 6: result = 4; break; // 6.5 -> 1.3 substep
            case 7: result = 2; break; // 7.5 -> 2.5 substep
            case 8: result = 4; break; // 8.5 -> 1.7 substep
            case 9: result = 4; break; // 9.5 -> 1.9 substep
            }
        }
        // if mantissa fraction isn't 0.0 or 0.5, don't bother finding good sub tick marks, leave default
    }

    return result;
}

/*! \internal
  
  This method returns the tick label string as it should be printed under the \a tick coordinate.
  If a textual number is returned, it should respect the provided \a locale, \a formatChar and \a
  precision.
  
  If the returned value contains exponentials of the form "2e5" and beautifully typeset powers is
  enabled in the QCPAxis number format (\ref QCPAxis::setNumberFormat), the exponential part will
  be formatted accordingly using multiplication symbol and superscript during rendering of the
  label automatically.
*/
QString QCPAxisTicker::getTickLabel(double tick, const QLocale &locale, QChar formatChar, int precision)
{
    return locale.toString(tick, formatChar.toLatin1(), precision);
}

/*! \internal
  
  Returns a vector containing all coordinates of sub ticks that should be drawn. It generates \a
  subTickCount sub ticks between each tick pair given in \a ticks.
  
  If a QCPAxisTicker subclass needs maximal control over the generated sub ticks, it should
  reimplement this method. Depending on the purpose of the subclass it doesn't necessarily need to
  base its result on \a subTickCount or \a ticks.
*/
QVector<double> QCPAxisTicker::createSubTickVector(int subTickCount, const QVector<double> &ticks)
{
    QVector<double> result;
    if (subTickCount <= 0 || ticks.size() < 2)
        return result;

    result.reserve((ticks.size()-1)*subTickCount);
    for (int i=1; i<ticks.size(); ++i)
    {
        double subTickStep = (ticks.at(i)-ticks.at(i-1))/(double)(subTickCount+1);
        for (int k=1; k<=subTickCount; ++k)
            result.append(ticks.at(i-1) + k*subTickStep);
    }
    return result;
}

/*! \internal
  
  Returns a vector containing all coordinates of ticks that should be drawn. The default
  implementation generates ticks with a spacing of \a tickStep (mathematically starting at the tick
  step origin, see \ref setTickOrigin) distributed over the passed \a range.
  
  In order for the axis ticker to generate proper sub ticks, it is necessary that the first and
  last tick coordinates returned by this method are just below/above the provided \a range.
  Otherwise the outer intervals won't contain any sub ticks.
  
  If a QCPAxisTicker subclass needs maximal control over the generated ticks, it should reimplement
  this method. Depending on the purpose of the subclass it doesn't necessarily need to base its
  result on \a tickStep, e.g. when the ticks are spaced unequally like in the case of
  QCPAxisTickerLog.
*/
QVector<double> QCPAxisTicker::createTickVector(double tickStep, const QCPRange &range)
{
    QVector<double> result;
    // Generate tick positions according to tickStep:
    qint64 firstStep = floor((range.lower-mTickOrigin)/tickStep); // do not use qFloor here, or we'll lose 64 bit precision
    qint64 lastStep = ceil((range.upper-mTickOrigin)/tickStep); // do not use qCeil here, or we'll lose 64 bit precision
    int tickcount = lastStep-firstStep+1;
    if (tickcount < 0) tickcount = 0;
    result.resize(tickcount);
    for (int i=0; i<tickcount; ++i)
        result[i] = mTickOrigin + (firstStep+i)*tickStep;
    return result;
}

/*! \internal
  
  Returns a vector containing all tick label strings corresponding to the tick coordinates provided
  in \a ticks. The default implementation calls \ref getTickLabel to generate the respective
  strings.
  
  It is possible but uncommon for QCPAxisTicker subclasses to reimplement this method, as
  reimplementing \ref getTickLabel often achieves the intended result easier.
*/
QVector<QString> QCPAxisTicker::createLabelVector(const QVector<double> &ticks, const QLocale &locale, QChar formatChar, int precision)
{
    QVector<QString> result;
    result.reserve(ticks.size());
    for (int i=0; i<ticks.size(); ++i)
        result.append(getTickLabel(ticks.at(i), locale, formatChar, precision));
    return result;
}

/*! \internal
  
  Removes tick coordinates from \a ticks which lie outside the specified \a range. If \a
  keepOneOutlier is true, it preserves one tick just outside the range on both sides, if present.
  
  The passed \a ticks must be sorted in ascending order.
*/
void QCPAxisTicker::trimTicks(const QCPRange &range, QVector<double> &ticks, bool keepOneOutlier) const
{
    bool lowFound = false;
    bool highFound = false;
    int lowIndex = 0;
    int highIndex = -1;

    for (int i=0; i < ticks.size(); ++i)
    {
        if (ticks.at(i) >= range.lower)
        {
            lowFound = true;
            lowIndex = i;
            break;
        }
    }
    for (int i=ticks.size()-1; i >= 0; --i)
    {
        if (ticks.at(i) <= range.upper)
        {
            highFound = true;
            highIndex = i;
            break;
        }
    }

    if (highFound && lowFound)
    {
        int trimFront = qMax(0, lowIndex-(keepOneOutlier ? 1 : 0));
        int trimBack = qMax(0, ticks.size()-(keepOneOutlier ? 2 : 1)-highIndex);
        if (trimFront > 0 || trimBack > 0)
            ticks = ticks.mid(trimFront, ticks.size()-trimFront-trimBack);
    } else // all ticks are either all below or all above the range
        ticks.clear();
}

/*! \internal
  
  Returns the coordinate contained in \a candidates which is closest to the provided \a target.
  
  This method assumes \a candidates is not empty and sorted in ascending order.
*/
double QCPAxisTicker::pickClosest(double target, const QVector<double> &candidates) const
{
    if (candidates.size() == 1)
        return candidates.first();
    QVector<double>::const_iterator it = std::lower_bound(candidates.constBegin(), candidates.constEnd(), target);
    if (it == candidates.constEnd())
        return *(it-1);
    else if (it == candidates.constBegin())
        return *it;
    else
        return target-*(it-1) < *it-target ? *(it-1) : *it;
}

/*! \internal
  
  Returns the decimal mantissa of \a input. Optionally, if \a magnitude is not set to zero, it also
  returns the magnitude of \a input as a power of 10.
  
  For example, an input of 142.6 will return a mantissa of 1.426 and a magnitude of 100.
*/
double QCPAxisTicker::getMantissa(double input, double *magnitude) const
{
    const double mag = qPow(10.0, qFloor(qLn(input)/qLn(10.0)));
    if (magnitude) *magnitude = mag;
    return input/mag;
}

/*! \internal
  
  Returns a number that is close to \a input but has a clean, easier human readable mantissa. How
  strongly the mantissa is altered, and thus how strong the result deviates from the original \a
  input, depends on the current tick step strategy (see \ref setTickStepStrategy).
*/
double QCPAxisTicker::cleanMantissa(double input) const
{
    double magnitude;
    const double mantissa = getMantissa(input, &magnitude);
    switch (mTickStepStrategy)
    {
    case tssReadability:
    {
        return pickClosest(mantissa, QVector<double>() << 1.0 << 2.0 << 2.5 << 5.0 << 10.0)*magnitude;
    }
    case tssMeetTickCount:
    {
        // this gives effectively a mantissa of 1.0, 1.5, 2.0, 2.5, 3.0, 3.5, 4.0, 4.5, 5.0, 6.0, 8.0, 10.0
        if (mantissa <= 5.0)
            return (int)(mantissa*2)/2.0*magnitude; // round digit after decimal point to 0.5
        else
            return (int)(mantissa/2.0)*2.0*magnitude; // round to first digit in multiples of 2
    }
    }
    return input;
}
/* end of 'src/axis/axisticker.cpp' */


/* including file 'src/axis/axistickerdatetime.cpp', size 14443              */
/* commit ce344b3f96a62e5f652585e55f1ae7c7883cd45b 2018-06-25 01:03:39 +0200 */

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPAxisTickerDateTime
////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \class QCPAxisTickerDateTime
  \brief Specialized axis ticker for calendar dates and times as axis ticks
  
  \image html axisticker-datetime.png
  
  This QCPAxisTicker subclass generates ticks that correspond to real calendar dates and times. The
  plot axis coordinate is interpreted as Unix Time, so seconds since Epoch (January 1, 1970, 00:00
  UTC). This is also used for example by QDateTime in the <tt>toTime_t()/setTime_t()</tt> methods
  with a precision of one second. Since Qt 4.7, millisecond accuracy can be obtained from QDateTime
  by using <tt>QDateTime::fromMSecsSinceEpoch()/1000.0</tt>. The static methods \ref dateTimeToKey
  and \ref keyToDateTime conveniently perform this conversion achieving a precision of one
  millisecond on all Qt versions.
  
  The format of the date/time display in the tick labels is controlled with \ref setDateTimeFormat.
  If a different time spec (time zone) shall be used, see \ref setDateTimeSpec.
  
  This ticker produces unequal tick spacing in order to provide intuitive date and time-of-day
  ticks. For example, if the axis range spans a few years such that there is one tick per year,
  ticks will be positioned on 1. January of every year. This is intuitive but, due to leap years,
  will result in slightly unequal tick intervals (visually unnoticeable). The same can be seen in
  the image above: even though the number of days varies month by month, this ticker generates
  ticks on the same day of each month.
  
  If you would like to change the date/time that is used as a (mathematical) starting date for the
  ticks, use the \ref setTickOrigin(const QDateTime &origin) method overload, which takes a
  QDateTime. If you pass 15. July, 9:45 to this method, the yearly ticks will end up on 15. July at
  9:45 of every year.
  
  The ticker can be created and assigned to an axis like this:
  \snippet documentation/doc-image-generator/mainwindow.cpp axistickerdatetime-creation
  
  \note If you rather wish to display relative times in terms of days, hours, minutes, seconds and
  milliseconds, and are not interested in the intricacies of real calendar dates with months and
  (leap) years, have a look at QCPAxisTickerTime instead.
*/

/*!
  Constructs the ticker and sets reasonable default values. Axis tickers are commonly created
  managed by a QSharedPointer, which then can be passed to QCPAxis::setTicker.
*/
QCPAxisTickerDateTime::QCPAxisTickerDateTime() :
    mDateTimeFormat(QLatin1String("hh:mm:ss\ndd.MM.yy")),
    mDateTimeSpec(Qt::LocalTime),
    mDateStrategy(dsNone)
{
    setTickCount(4);
}

/*!
  Sets the format in which dates and times are displayed as tick labels. For details about the \a
  format string, see the documentation of QDateTime::toString().
  
  Newlines can be inserted with "\n".
  
  \see setDateTimeSpec
*/
void QCPAxisTickerDateTime::setDateTimeFormat(const QString &format)
{
    mDateTimeFormat = format;
}

/*!
  Sets the time spec that is used for creating the tick labels from corresponding dates/times.

  The default value of QDateTime objects (and also QCPAxisTickerDateTime) is
  <tt>Qt::LocalTime</tt>. However, if the date time values passed to PF_GraphicView (e.g. in the form
  of axis ranges or keys of a plottable) are given in the UTC spec, set \a spec to <tt>Qt::UTC</tt>
  to get the correct axis labels.
  
  \see setDateTimeFormat
*/
void QCPAxisTickerDateTime::setDateTimeSpec(Qt::TimeSpec spec)
{
    mDateTimeSpec = spec;
}

/*!
  Sets the tick origin (see \ref QCPAxisTicker::setTickOrigin) in seconds since Epoch (1. Jan 1970,
  00:00 UTC). For the date time ticker it might be more intuitive to use the overload which
  directly takes a QDateTime, see \ref setTickOrigin(const QDateTime &origin).
  
  This is useful to define the month/day/time recurring at greater tick interval steps. For
  example, If you pass 15. July, 9:45 to this method and the tick interval happens to be one tick
  per year, the ticks will end up on 15. July at 9:45 of every year.
*/
void QCPAxisTickerDateTime::setTickOrigin(double origin)
{
    QCPAxisTicker::setTickOrigin(origin);
}

/*!
  Sets the tick origin (see \ref QCPAxisTicker::setTickOrigin) as a QDateTime \a origin.
  
  This is useful to define the month/day/time recurring at greater tick interval steps. For
  example, If you pass 15. July, 9:45 to this method and the tick interval happens to be one tick
  per year, the ticks will end up on 15. July at 9:45 of every year.
*/
void QCPAxisTickerDateTime::setTickOrigin(const QDateTime &origin)
{
    setTickOrigin(dateTimeToKey(origin));
}

/*! \internal
  
  Returns a sensible tick step with intervals appropriate for a date-time-display, such as weekly,
  monthly, bi-monthly, etc.
  
  Note that this tick step isn't used exactly when generating the tick vector in \ref
  createTickVector, but only as a guiding value requiring some correction for each individual tick
  interval. Otherwise this would lead to unintuitive date displays, e.g. jumping between first day
  in the month to the last day in the previous month from tick to tick, due to the non-uniform
  length of months. The same problem arises with leap years.
  
  \seebaseclassmethod
*/
double QCPAxisTickerDateTime::getTickStep(const QCPRange &range)
{
    double result = range.size()/(double)(mTickCount+1e-10); // mTickCount ticks on average, the small addition is to prevent jitter on exact integers

    mDateStrategy = dsNone;
    if (result < 1) // ideal tick step is below 1 second -> use normal clean mantissa algorithm in units of seconds
    {
        result = cleanMantissa(result);
    } else if (result < 86400*30.4375*12) // below a year
    {
        result = pickClosest(result, QVector<double>()
                             << 1 << 2.5 << 5 << 10 << 15 << 30 << 60 << 2.5*60 << 5*60 << 10*60 << 15*60 << 30*60 << 60*60 // second, minute, hour range
                             << 3600*2 << 3600*3 << 3600*6 << 3600*12 << 3600*24 // hour to day range
                             << 86400*2 << 86400*5 << 86400*7 << 86400*14 << 86400*30.4375 << 86400*30.4375*2 << 86400*30.4375*3 << 86400*30.4375*6 << 86400*30.4375*12); // day, week, month range (avg. days per month includes leap years)
        if (result > 86400*30.4375-1) // month tick intervals or larger
            mDateStrategy = dsUniformDayInMonth;
        else if (result > 3600*24-1) // day tick intervals or larger
            mDateStrategy = dsUniformTimeInDay;
    } else // more than a year, go back to normal clean mantissa algorithm but in units of years
    {
        const double secondsPerYear = 86400*30.4375*12; // average including leap years
        result = cleanMantissa(result/secondsPerYear)*secondsPerYear;
        mDateStrategy = dsUniformDayInMonth;
    }
    return result;
}

/*! \internal
  
  Returns a sensible sub tick count with intervals appropriate for a date-time-display, such as weekly,
  monthly, bi-monthly, etc.
  
  \seebaseclassmethod
*/
int QCPAxisTickerDateTime::getSubTickCount(double tickStep)
{
    int result = QCPAxisTicker::getSubTickCount(tickStep);
    switch (qRound(tickStep)) // hand chosen subticks for specific minute/hour/day/week/month range (as specified in getTickStep)
    {
    case 5*60: result = 4; break;
    case 10*60: result = 1; break;
    case 15*60: result = 2; break;
    case 30*60: result = 1; break;
    case 60*60: result = 3; break;
    case 3600*2: result = 3; break;
    case 3600*3: result = 2; break;
    case 3600*6: result = 1; break;
    case 3600*12: result = 3; break;
    case 3600*24: result = 3; break;
    case 86400*2: result = 1; break;
    case 86400*5: result = 4; break;
    case 86400*7: result = 6; break;
    case 86400*14: result = 1; break;
    case (int)(86400*30.4375+0.5): result = 3; break;
    case (int)(86400*30.4375*2+0.5): result = 1; break;
    case (int)(86400*30.4375*3+0.5): result = 2; break;
    case (int)(86400*30.4375*6+0.5): result = 5; break;
    case (int)(86400*30.4375*12+0.5): result = 3; break;
    }
    return result;
}

/*! \internal
  
  Generates a date/time tick label for tick coordinate \a tick, based on the currently set format
  (\ref setDateTimeFormat) and time spec (\ref setDateTimeSpec).
  
  \seebaseclassmethod
*/
QString QCPAxisTickerDateTime::getTickLabel(double tick, const QLocale &locale, QChar formatChar, int precision)
{
    Q_UNUSED(precision)
    Q_UNUSED(formatChar)
    return locale.toString(keyToDateTime(tick).toTimeSpec(mDateTimeSpec), mDateTimeFormat);
}

/*! \internal
  
  Uses the passed \a tickStep as a guiding value and applies corrections in order to obtain
  non-uniform tick intervals but intuitive tick labels, e.g. falling on the same day of each month.
  
  \seebaseclassmethod
*/
QVector<double> QCPAxisTickerDateTime::createTickVector(double tickStep, const QCPRange &range)
{
    QVector<double> result = QCPAxisTicker::createTickVector(tickStep, range);
    if (!result.isEmpty())
    {
        if (mDateStrategy == dsUniformTimeInDay)
        {
            QDateTime uniformDateTime = keyToDateTime(mTickOrigin); // the time of this datetime will be set for all other ticks, if possible
            QDateTime tickDateTime;
            for (int i=0; i<result.size(); ++i)
            {
                tickDateTime = keyToDateTime(result.at(i));
                tickDateTime.setTime(uniformDateTime.time());
                result[i] = dateTimeToKey(tickDateTime);
            }
        } else if (mDateStrategy == dsUniformDayInMonth)
        {
            QDateTime uniformDateTime = keyToDateTime(mTickOrigin); // this day (in month) and time will be set for all other ticks, if possible
            QDateTime tickDateTime;
            for (int i=0; i<result.size(); ++i)
            {
                tickDateTime = keyToDateTime(result.at(i));
                tickDateTime.setTime(uniformDateTime.time());
                int thisUniformDay = uniformDateTime.date().day() <= tickDateTime.date().daysInMonth() ? uniformDateTime.date().day() : tickDateTime.date().daysInMonth(); // don't exceed month (e.g. try to set day 31 in February)
                if (thisUniformDay-tickDateTime.date().day() < -15) // with leap years involved, date month may jump backwards or forwards, and needs to be corrected before setting day
                    tickDateTime = tickDateTime.addMonths(1);
                else if (thisUniformDay-tickDateTime.date().day() > 15) // with leap years involved, date month may jump backwards or forwards, and needs to be corrected before setting day
                    tickDateTime = tickDateTime.addMonths(-1);
                tickDateTime.setDate(QDate(tickDateTime.date().year(), tickDateTime.date().month(), thisUniformDay));
                result[i] = dateTimeToKey(tickDateTime);
            }
        }
    }
    return result;
}

/*!
  A convenience method which turns \a key (in seconds since Epoch 1. Jan 1970, 00:00 UTC) into a
  QDateTime object. This can be used to turn axis coordinates to actual QDateTimes.
  
  The accuracy achieved by this method is one millisecond, irrespective of the used Qt version (it
  works around the lack of a QDateTime::fromMSecsSinceEpoch in Qt 4.6)
  
  \see dateTimeToKey
*/
QDateTime QCPAxisTickerDateTime::keyToDateTime(double key)
{
# if QT_VERSION < QT_VERSION_CHECK(4, 7, 0)
    return QDateTime::fromTime_t(key).addMSecs((key-(qint64)key)*1000);
# else
    return QDateTime::fromMSecsSinceEpoch(key*1000.0);
# endif
}

/*! \overload
  
  A convenience method which turns a QDateTime object into a double value that corresponds to
  seconds since Epoch (1. Jan 1970, 00:00 UTC). This is the format used as axis coordinates by
  QCPAxisTickerDateTime.
  
  The accuracy achieved by this method is one millisecond, irrespective of the used Qt version (it
  works around the lack of a QDateTime::toMSecsSinceEpoch in Qt 4.6)
  
  \see keyToDateTime
*/
double QCPAxisTickerDateTime::dateTimeToKey(const QDateTime dateTime)
{
# if QT_VERSION < QT_VERSION_CHECK(4, 7, 0)
    return dateTime.toTime_t()+dateTime.time().msec()/1000.0;
# else
    return dateTime.toMSecsSinceEpoch()/1000.0;
# endif
}

/*! \overload
  
  A convenience method which turns a QDate object into a double value that corresponds to
  seconds since Epoch (1. Jan 1970, 00:00 UTC). This is the format used as axis coordinates by
  QCPAxisTickerDateTime.
  
  \see keyToDateTime
*/
double QCPAxisTickerDateTime::dateTimeToKey(const QDate date)
{
# if QT_VERSION < QT_VERSION_CHECK(4, 7, 0)
    return QDateTime(date).toTime_t();
# else
    return QDateTime(date).toMSecsSinceEpoch()/1000.0;
# endif
}
/* end of 'src/axis/axistickerdatetime.cpp' */


/* including file 'src/axis/axistickertime.cpp', size 11747                  */
/* commit ce344b3f96a62e5f652585e55f1ae7c7883cd45b 2018-06-25 01:03:39 +0200 */

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPAxisTickerTime
////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \class QCPAxisTickerTime
  \brief Specialized axis ticker for time spans in units of milliseconds to days
  
  \image html axisticker-time.png
  
  This QCPAxisTicker subclass generates ticks that corresponds to time intervals.
  
  The format of the time display in the tick labels is controlled with \ref setTimeFormat and \ref
  setFieldWidth. The time coordinate is in the unit of seconds with respect to the time coordinate
  zero. Unlike with QCPAxisTickerDateTime, the ticks don't correspond to a specific calendar date
  and time.
  
  The time can be displayed in milliseconds, seconds, minutes, hours and days. Depending on the
  largest available unit in the format specified with \ref setTimeFormat, any time spans above will
  be carried in that largest unit. So for example if the format string is "%m:%s" and a tick at
  coordinate value 7815 (being 2 hours, 10 minutes and 15 seconds) is created, the resulting tick
  label will show "130:15" (130 minutes, 15 seconds). If the format string is "%h:%m:%s", the hour
  unit will be used and the label will thus be "02:10:15". Negative times with respect to the axis
  zero will carry a leading minus sign.
  
  The ticker can be created and assigned to an axis like this:
  \snippet documentation/doc-image-generator/mainwindow.cpp axistickertime-creation
  
  Here is an example of a time axis providing time information in days, hours and minutes. Due to
  the axis range spanning a few days and the wanted tick count (\ref setTickCount), the ticker
  decided to use tick steps of 12 hours:
  
  \image html axisticker-time2.png
  
  The format string for this example is
  \snippet documentation/doc-image-generator/mainwindow.cpp axistickertime-creation-2
  
  \note If you rather wish to display calendar dates and times, have a look at QCPAxisTickerDateTime
  instead.
*/

/*!
  Constructs the ticker and sets reasonable default values. Axis tickers are commonly created
  managed by a QSharedPointer, which then can be passed to QCPAxis::setTicker.
*/
QCPAxisTickerTime::QCPAxisTickerTime() :
    mTimeFormat(QLatin1String("%h:%m:%s")),
    mSmallestUnit(tuSeconds),
    mBiggestUnit(tuHours)
{
    setTickCount(4);
    mFieldWidth[tuMilliseconds] = 3;
    mFieldWidth[tuSeconds] = 2;
    mFieldWidth[tuMinutes] = 2;
    mFieldWidth[tuHours] = 2;
    mFieldWidth[tuDays] = 1;

    mFormatPattern[tuMilliseconds] = QLatin1String("%z");
    mFormatPattern[tuSeconds] = QLatin1String("%s");
    mFormatPattern[tuMinutes] = QLatin1String("%m");
    mFormatPattern[tuHours] = QLatin1String("%h");
    mFormatPattern[tuDays] = QLatin1String("%d");
}

/*!
  Sets the format that will be used to display time in the tick labels.
  
  The available patterns are:
  - %%z for milliseconds
  - %%s for seconds
  - %%m for minutes
  - %%h for hours
  - %%d for days
  
  The field width (zero padding) can be controlled for each unit with \ref setFieldWidth.
  
  The largest unit that appears in \a format will carry all the remaining time of a certain tick
  coordinate, even if it overflows the natural limit of the unit. For example, if %%m is the
  largest unit it might become larger than 59 in order to consume larger time values. If on the
  other hand %%h is available, the minutes will wrap around to zero after 59 and the time will
  carry to the hour digit.
*/
void QCPAxisTickerTime::setTimeFormat(const QString &format)
{
    mTimeFormat = format;

    // determine smallest and biggest unit in format, to optimize unit replacement and allow biggest
    // unit to consume remaining time of a tick value and grow beyond its modulo (e.g. min > 59)
    mSmallestUnit = tuMilliseconds;
    mBiggestUnit = tuMilliseconds;
    bool hasSmallest = false;
    for (int i = tuMilliseconds; i <= tuDays; ++i)
    {
        TimeUnit unit = static_cast<TimeUnit>(i);
        if (mTimeFormat.contains(mFormatPattern.value(unit)))
        {
            if (!hasSmallest)
            {
                mSmallestUnit = unit;
                hasSmallest = true;
            }
            mBiggestUnit = unit;
        }
    }
}

/*!
  Sets the field widh of the specified \a unit to be \a width digits, when displayed in the tick
  label. If the number for the specific unit is shorter than \a width, it will be padded with an
  according number of zeros to the left in order to reach the field width.
  
  \see setTimeFormat
*/
void QCPAxisTickerTime::setFieldWidth(QCPAxisTickerTime::TimeUnit unit, int width)
{
    mFieldWidth[unit] = qMax(width, 1);
}

/*! \internal

  Returns the tick step appropriate for time displays, depending on the provided \a range and the
  smallest available time unit in the current format (\ref setTimeFormat). For example if the unit
  of seconds isn't available in the format, this method will not generate steps (like 2.5 minutes)
  that require sub-minute precision to be displayed correctly.
  
  \seebaseclassmethod
*/
double QCPAxisTickerTime::getTickStep(const QCPRange &range)
{
    double result = range.size()/(double)(mTickCount+1e-10); // mTickCount ticks on average, the small addition is to prevent jitter on exact integers

    if (result < 1) // ideal tick step is below 1 second -> use normal clean mantissa algorithm in units of seconds
    {
        if (mSmallestUnit == tuMilliseconds)
            result = qMax(cleanMantissa(result), 0.001); // smallest tick step is 1 millisecond
        else // have no milliseconds available in format, so stick with 1 second tickstep
            result = 1.0;
    } else if (result < 3600*24) // below a day
    {
        // the filling of availableSteps seems a bit contorted but it fills in a sorted fashion and thus saves a post-fill sorting run
        QVector<double> availableSteps;
        // seconds range:
        if (mSmallestUnit <= tuSeconds)
            availableSteps << 1;
        if (mSmallestUnit == tuMilliseconds)
            availableSteps << 2.5; // only allow half second steps if milliseconds are there to display it
        else if (mSmallestUnit == tuSeconds)
            availableSteps << 2;
        if (mSmallestUnit <= tuSeconds)
            availableSteps << 5 << 10 << 15 << 30;
        // minutes range:
        if (mSmallestUnit <= tuMinutes)
            availableSteps << 1*60;
        if (mSmallestUnit <= tuSeconds)
            availableSteps << 2.5*60; // only allow half minute steps if seconds are there to display it
        else if (mSmallestUnit == tuMinutes)
            availableSteps << 2*60;
        if (mSmallestUnit <= tuMinutes)
            availableSteps << 5*60 << 10*60 << 15*60 << 30*60;
        // hours range:
        if (mSmallestUnit <= tuHours)
            availableSteps << 1*3600 << 2*3600 << 3*3600 << 6*3600 << 12*3600 << 24*3600;
        // pick available step that is most appropriate to approximate ideal step:
        result = pickClosest(result, availableSteps);
    } else // more than a day, go back to normal clean mantissa algorithm but in units of days
    {
        const double secondsPerDay = 3600*24;
        result = cleanMantissa(result/secondsPerDay)*secondsPerDay;
    }
    return result;
}

/*! \internal

  Returns the sub tick count appropriate for the provided \a tickStep and time displays.
  
  \seebaseclassmethod
*/
int QCPAxisTickerTime::getSubTickCount(double tickStep)
{
    int result = QCPAxisTicker::getSubTickCount(tickStep);
    switch (qRound(tickStep)) // hand chosen subticks for specific minute/hour/day range (as specified in getTickStep)
    {
    case 5*60: result = 4; break;
    case 10*60: result = 1; break;
    case 15*60: result = 2; break;
    case 30*60: result = 1; break;
    case 60*60: result = 3; break;
    case 3600*2: result = 3; break;
    case 3600*3: result = 2; break;
    case 3600*6: result = 1; break;
    case 3600*12: result = 3; break;
    case 3600*24: result = 3; break;
    }
    return result;
}

/*! \internal
  
  Returns the tick label corresponding to the provided \a tick and the configured format and field
  widths (\ref setTimeFormat, \ref setFieldWidth).
  
  \seebaseclassmethod
*/
QString QCPAxisTickerTime::getTickLabel(double tick, const QLocale &locale, QChar formatChar, int precision)
{
    Q_UNUSED(precision)
    Q_UNUSED(formatChar)
    Q_UNUSED(locale)
    bool negative = tick < 0;
    if (negative) tick *= -1;
    double values[tuDays+1]; // contains the msec/sec/min/... value with its respective modulo (e.g. minute 0..59)
    double restValues[tuDays+1]; // contains the msec/sec/min/... value as if it's the largest available unit and thus consumes the remaining time

    restValues[tuMilliseconds] = tick*1000;
    values[tuMilliseconds] = modf(restValues[tuMilliseconds]/1000, &restValues[tuSeconds])*1000;
    values[tuSeconds] = modf(restValues[tuSeconds]/60, &restValues[tuMinutes])*60;
    values[tuMinutes] = modf(restValues[tuMinutes]/60, &restValues[tuHours])*60;
    values[tuHours] = modf(restValues[tuHours]/24, &restValues[tuDays])*24;
    // no need to set values[tuDays] because days are always a rest value (there is no higher unit so it consumes all remaining time)

    QString result = mTimeFormat;
    for (int i = mSmallestUnit; i <= mBiggestUnit; ++i)
    {
        TimeUnit iUnit = static_cast<TimeUnit>(i);
        replaceUnit(result, iUnit, qRound(iUnit == mBiggestUnit ? restValues[iUnit] : values[iUnit]));
    }
    if (negative)
        result.prepend(QLatin1Char('-'));
    return result;
}

/*! \internal
  
  Replaces all occurrences of the format pattern belonging to \a unit in \a text with the specified
  \a value, using the field width as specified with \ref setFieldWidth for the \a unit.
*/
void QCPAxisTickerTime::replaceUnit(QString &text, QCPAxisTickerTime::TimeUnit unit, int value) const
{
    QString valueStr = QString::number(value);
    while (valueStr.size() < mFieldWidth.value(unit))
        valueStr.prepend(QLatin1Char('0'));

    text.replace(mFormatPattern.value(unit), valueStr);
}
/* end of 'src/axis/axistickertime.cpp' */


/* including file 'src/axis/axistickerfixed.cpp', size 5583                  */
/* commit ce344b3f96a62e5f652585e55f1ae7c7883cd45b 2018-06-25 01:03:39 +0200 */

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPAxisTickerFixed
////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \class QCPAxisTickerFixed
  \brief Specialized axis ticker with a fixed tick step
  
  \image html axisticker-fixed.png
  
  This QCPAxisTicker subclass generates ticks with a fixed tick step set with \ref setTickStep. It
  is also possible to allow integer multiples and integer powers of the specified tick step with
  \ref setScaleStrategy.
  
  A typical application of this ticker is to make an axis only display integers, by setting the
  tick step of the ticker to 1.0 and the scale strategy to \ref ssMultiples.
  
  Another case is when a certain number has a special meaning and axis ticks should only appear at
  multiples of that value. In this case you might also want to consider \ref QCPAxisTickerPi
  because despite the name it is not limited to only pi symbols/values.
  
  The ticker can be created and assigned to an axis like this:
  \snippet documentation/doc-image-generator/mainwindow.cpp axistickerfixed-creation
*/

/*!
  Constructs the ticker and sets reasonable default values. Axis tickers are commonly created
  managed by a QSharedPointer, which then can be passed to QCPAxis::setTicker.
*/
QCPAxisTickerFixed::QCPAxisTickerFixed() :
    mTickStep(1.0),
    mScaleStrategy(ssNone)
{
}

/*!
  Sets the fixed tick interval to \a step.
  
  The axis ticker will only use this tick step when generating axis ticks. This might cause a very
  high tick density and overlapping labels if the axis range is zoomed out. Using \ref
  setScaleStrategy it is possible to relax the fixed step and also allow multiples or powers of \a
  step. This will enable the ticker to reduce the number of ticks to a reasonable amount (see \ref
  setTickCount).
*/
void QCPAxisTickerFixed::setTickStep(double step)
{
    if (step > 0)
        mTickStep = step;
    else
        qDebug() << Q_FUNC_INFO << "tick step must be greater than zero:" << step;
}

/*!
  Sets whether the specified tick step (\ref setTickStep) is absolutely fixed or whether
  modifications may be applied to it before calculating the finally used tick step, such as
  permitting multiples or powers. See \ref ScaleStrategy for details.
  
  The default strategy is \ref ssNone, which means the tick step is absolutely fixed.
*/
void QCPAxisTickerFixed::setScaleStrategy(QCPAxisTickerFixed::ScaleStrategy strategy)
{
    mScaleStrategy = strategy;
}

/*! \internal
  
  Determines the actually used tick step from the specified tick step and scale strategy (\ref
  setTickStep, \ref setScaleStrategy).
  
  This method either returns the specified tick step exactly, or, if the scale strategy is not \ref
  ssNone, a modification of it to allow varying the number of ticks in the current axis range.
  
  \seebaseclassmethod
*/
double QCPAxisTickerFixed::getTickStep(const QCPRange &range)
{
    switch (mScaleStrategy)
    {
    case ssNone:
    {
        return mTickStep;
    }
    case ssMultiples:
    {
        double exactStep = range.size()/(double)(mTickCount+1e-10); // mTickCount ticks on average, the small addition is to prevent jitter on exact integers
        if (exactStep < mTickStep)
            return mTickStep;
        else
            return (qint64)(cleanMantissa(exactStep/mTickStep)+0.5)*mTickStep;
    }
    case ssPowers:
    {
        double exactStep = range.size()/(double)(mTickCount+1e-10); // mTickCount ticks on average, the small addition is to prevent jitter on exact integers
        return qPow(mTickStep, (int)(qLn(exactStep)/qLn(mTickStep)+0.5));
    }
    }
    return mTickStep;
}
/* end of 'src/axis/axistickerfixed.cpp' */


/* including file 'src/axis/axistickertext.cpp', size 8661                   */
/* commit ce344b3f96a62e5f652585e55f1ae7c7883cd45b 2018-06-25 01:03:39 +0200 */

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPAxisTickerText
////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \class QCPAxisTickerText
  \brief Specialized axis ticker which allows arbitrary labels at specified coordinates
  
  \image html axisticker-text.png
  
  This QCPAxisTicker subclass generates ticks which can be directly specified by the user as
  coordinates and associated strings. They can be passed as a whole with \ref setTicks or one at a
  time with \ref addTick. Alternatively you can directly access the internal storage via \ref ticks
  and modify the tick/label data there.
  
  This is useful for cases where the axis represents categories rather than numerical values.
  
  If you are updating the ticks of this ticker regularly and in a dynamic fasion (e.g. dependent on
  the axis range), it is a sign that you should probably create an own ticker by subclassing
  QCPAxisTicker, instead of using this one.
  
  The ticker can be created and assigned to an axis like this:
  \snippet documentation/doc-image-generator/mainwindow.cpp axistickertext-creation
*/

/* start of documentation of inline functions */

/*! \fn QMap<double, QString> &QCPAxisTickerText::ticks()
  
  Returns a non-const reference to the internal map which stores the tick coordinates and their
  labels.

  You can access the map directly in order to add, remove or manipulate ticks, as an alternative to
  using the methods provided by QCPAxisTickerText, such as \ref setTicks and \ref addTick.
*/

/* end of documentation of inline functions */

/*!
  Constructs the ticker and sets reasonable default values. Axis tickers are commonly created
  managed by a QSharedPointer, which then can be passed to QCPAxis::setTicker.
*/
QCPAxisTickerText::QCPAxisTickerText() :
    mSubTickCount(0)
{
}

/*! \overload
  
  Sets the ticks that shall appear on the axis. The map key of \a ticks corresponds to the axis
  coordinate, and the map value is the string that will appear as tick label.
  
  An alternative to manipulate ticks is to directly access the internal storage with the \ref ticks
  getter.
  
  \see addTicks, addTick, clear
*/
void QCPAxisTickerText::setTicks(const QMap<double, QString> &ticks)
{
    mTicks = ticks;
}

/*! \overload
  
  Sets the ticks that shall appear on the axis. The entries of \a positions correspond to the axis
  coordinates, and the entries of \a labels are the respective strings that will appear as tick
  labels.
  
  \see addTicks, addTick, clear
*/
void QCPAxisTickerText::setTicks(const QVector<double> &positions, const QVector<QString> &labels)
{
    clear();
    addTicks(positions, labels);
}

/*!
  Sets the number of sub ticks that shall appear between ticks. For QCPAxisTickerText, there is no
  automatic sub tick count calculation. So if sub ticks are needed, they must be configured with this
  method.
*/
void QCPAxisTickerText::setSubTickCount(int subTicks)
{
    if (subTicks >= 0)
        mSubTickCount = subTicks;
    else
        qDebug() << Q_FUNC_INFO << "sub tick count can't be negative:" << subTicks;
}

/*!
  Clears all ticks.
  
  An alternative to manipulate ticks is to directly access the internal storage with the \ref ticks
  getter.
  
  \see setTicks, addTicks, addTick
*/
void QCPAxisTickerText::clear()
{
    mTicks.clear();
}

/*!
  Adds a single tick to the axis at the given axis coordinate \a position, with the provided tick \a
  label.
  
  \see addTicks, setTicks, clear
*/
void QCPAxisTickerText::addTick(double position, const QString &label)
{
    mTicks.insert(position, label);
}

/*! \overload
  
  Adds the provided \a ticks to the ones already existing. The map key of \a ticks corresponds to
  the axis coordinate, and the map value is the string that will appear as tick label.
  
  An alternative to manipulate ticks is to directly access the internal storage with the \ref ticks
  getter.
  
  \see addTick, setTicks, clear
*/
void QCPAxisTickerText::addTicks(const QMap<double, QString> &ticks)
{
    mTicks.unite(ticks);
}

/*! \overload
  
  Adds the provided ticks to the ones already existing. The entries of \a positions correspond to
  the axis coordinates, and the entries of \a labels are the respective strings that will appear as
  tick labels.
  
  An alternative to manipulate ticks is to directly access the internal storage with the \ref ticks
  getter.
  
  \see addTick, setTicks, clear
*/
void QCPAxisTickerText::addTicks(const QVector<double> &positions, const QVector<QString> &labels)
{
    if (positions.size() != labels.size())
        qDebug() << Q_FUNC_INFO << "passed unequal length vectors for positions and labels:" << positions.size() << labels.size();
    int n = qMin(positions.size(), labels.size());
    for (int i=0; i<n; ++i)
        mTicks.insert(positions.at(i), labels.at(i));
}

/*!
  Since the tick coordinates are provided externally, this method implementation does nothing.
  
  \seebaseclassmethod
*/
double QCPAxisTickerText::getTickStep(const QCPRange &range)
{
    // text axis ticker has manual tick positions, so doesn't need this method
    Q_UNUSED(range)
    return 1.0;
}

/*!
  Returns the sub tick count that was configured with \ref setSubTickCount.
  
  \seebaseclassmethod
*/
int QCPAxisTickerText::getSubTickCount(double tickStep)
{
    Q_UNUSED(tickStep)
    return mSubTickCount;
}

/*!
  Returns the tick label which corresponds to the key \a tick in the internal tick storage. Since
  the labels are provided externally, \a locale, \a formatChar, and \a precision are ignored.
  
  \seebaseclassmethod
*/
QString QCPAxisTickerText::getTickLabel(double tick, const QLocale &locale, QChar formatChar, int precision)
{
    Q_UNUSED(locale)
    Q_UNUSED(formatChar)
    Q_UNUSED(precision)
    return mTicks.value(tick);
}

/*!
  Returns the externally provided tick coordinates which are in the specified \a range. If
  available, one tick above and below the range is provided in addition, to allow possible sub tick
  calculation. The parameter \a tickStep is ignored.
  
  \seebaseclassmethod
*/
QVector<double> QCPAxisTickerText::createTickVector(double tickStep, const QCPRange &range)
{
    Q_UNUSED(tickStep)
    QVector<double> result;
    if (mTicks.isEmpty())
        return result;

    QMap<double, QString>::const_iterator start = mTicks.lowerBound(range.lower);
    QMap<double, QString>::const_iterator end = mTicks.upperBound(range.upper);
    // this method should try to give one tick outside of range so proper subticks can be generated:
    if (start != mTicks.constBegin()) --start;
    if (end != mTicks.constEnd()) ++end;
    for (QMap<double, QString>::const_iterator it = start; it != end; ++it)
        result.append(it.key());

    return result;
}
/* end of 'src/axis/axistickertext.cpp' */


/* including file 'src/axis/axistickerpi.cpp', size 11170                    */
/* commit ce344b3f96a62e5f652585e55f1ae7c7883cd45b 2018-06-25 01:03:39 +0200 */

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPAxisTickerPi
////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \class QCPAxisTickerPi
  \brief Specialized axis ticker to display ticks in units of an arbitrary constant, for example pi
  
  \image html axisticker-pi.png
  
  This QCPAxisTicker subclass generates ticks that are expressed with respect to a given symbolic
  constant with a numerical value specified with \ref setPiValue and an appearance in the tick
  labels specified with \ref setPiSymbol.
  
  Ticks may be generated at fractions of the symbolic constant. How these fractions appear in the
  tick label can be configured with \ref setFractionStyle.
  
  The ticker can be created and assigned to an axis like this:
  \snippet documentation/doc-image-generator/mainwindow.cpp axistickerpi-creation
*/

/*!
  Constructs the ticker and sets reasonable default values. Axis tickers are commonly created
  managed by a QSharedPointer, which then can be passed to QCPAxis::setTicker.
*/
QCPAxisTickerPi::QCPAxisTickerPi() :
    mPiSymbol(QLatin1String(" ")+QChar(0x03C0)),
    mPiValue(M_PI),
    mPeriodicity(0),
    mFractionStyle(fsUnicodeFractions),
    mPiTickStep(0)
{
    setTickCount(4);
}

/*!
  Sets how the symbol part (which is always a suffix to the number) shall appear in the axis tick
  label.
  
  If a space shall appear between the number and the symbol, make sure the space is contained in \a
  symbol.
*/
void QCPAxisTickerPi::setPiSymbol(QString symbol)
{
    mPiSymbol = symbol;
}

/*!
  Sets the numerical value that the symbolic constant has.

  This will be used to place the appropriate fractions of the symbol at the respective axis
  coordinates.
*/
void QCPAxisTickerPi::setPiValue(double pi)
{
    mPiValue = pi;
}

/*!
  Sets whether the axis labels shall appear periodicly and if so, at which multiplicity of the
  symbolic constant.
  
  To disable periodicity, set \a multiplesOfPi to zero.
  
  For example, an axis that identifies 0 with 2pi would set \a multiplesOfPi to two.
*/
void QCPAxisTickerPi::setPeriodicity(int multiplesOfPi)
{
    mPeriodicity = qAbs(multiplesOfPi);
}

/*!
  Sets how the numerical/fractional part preceding the symbolic constant is displayed in tick
  labels. See \ref FractionStyle for the various options.
*/
void QCPAxisTickerPi::setFractionStyle(QCPAxisTickerPi::FractionStyle style)
{
    mFractionStyle = style;
}

/*! \internal
  
  Returns the tick step, using the constant's value (\ref setPiValue) as base unit. In consequence
  the numerical/fractional part preceding the symbolic constant is made to have a readable
  mantissa.
  
  \seebaseclassmethod
*/
double QCPAxisTickerPi::getTickStep(const QCPRange &range)
{
    mPiTickStep = range.size()/mPiValue/(double)(mTickCount+1e-10); // mTickCount ticks on average, the small addition is to prevent jitter on exact integers
    mPiTickStep = cleanMantissa(mPiTickStep);
    return mPiTickStep*mPiValue;
}

/*! \internal
  
  Returns the sub tick count, using the constant's value (\ref setPiValue) as base unit. In
  consequence the sub ticks divide the numerical/fractional part preceding the symbolic constant
  reasonably, and not the total tick coordinate.
  
  \seebaseclassmethod
*/
int QCPAxisTickerPi::getSubTickCount(double tickStep)
{
    return QCPAxisTicker::getSubTickCount(tickStep/mPiValue);
}

/*! \internal
  
  Returns the tick label as a fractional/numerical part and a symbolic string as suffix. The
  formatting of the fraction is done according to the specified \ref setFractionStyle. The appended
  symbol is specified with \ref setPiSymbol.
  
  \seebaseclassmethod
*/
QString QCPAxisTickerPi::getTickLabel(double tick, const QLocale &locale, QChar formatChar, int precision)
{
    double tickInPis = tick/mPiValue;
    if (mPeriodicity > 0)
        tickInPis = fmod(tickInPis, mPeriodicity);

    if (mFractionStyle != fsFloatingPoint && mPiTickStep > 0.09 && mPiTickStep < 50)
    {
        // simply construct fraction from decimal like 1.234 -> 1234/1000 and then simplify fraction, smaller digits are irrelevant due to mPiTickStep conditional above
        int denominator = 1000;
        int numerator = qRound(tickInPis*denominator);
        simplifyFraction(numerator, denominator);
        if (qAbs(numerator) == 1 && denominator == 1)
            return (numerator < 0 ? QLatin1String("-") : QLatin1String("")) + mPiSymbol.trimmed();
        else if (numerator == 0)
            return QLatin1String("0");
        else
            return fractionToString(numerator, denominator) + mPiSymbol;
    } else
    {
        if (qFuzzyIsNull(tickInPis))
            return QLatin1String("0");
        else if (qFuzzyCompare(qAbs(tickInPis), 1.0))
            return (tickInPis < 0 ? QLatin1String("-") : QLatin1String("")) + mPiSymbol.trimmed();
        else
            return QCPAxisTicker::getTickLabel(tickInPis, locale, formatChar, precision) + mPiSymbol;
    }
}

/*! \internal
  
  Takes the fraction given by \a numerator and \a denominator and modifies the values to make sure
  the fraction is in irreducible form, i.e. numerator and denominator don't share any common
  factors which could be cancelled.
*/
void QCPAxisTickerPi::simplifyFraction(int &numerator, int &denominator) const
{
    if (numerator == 0 || denominator == 0)
        return;

    int num = numerator;
    int denom = denominator;
    while (denom != 0) // euclidean gcd algorithm
    {
        int oldDenom = denom;
        denom = num % denom;
        num = oldDenom;
    }
    // num is now gcd of numerator and denominator
    numerator /= num;
    denominator /= num;
}

/*! \internal
  
  Takes the fraction given by \a numerator and \a denominator and returns a string representation.
  The result depends on the configured fraction style (\ref setFractionStyle).
  
  This method is used to format the numerical/fractional part when generating tick labels. It
  simplifies the passed fraction to an irreducible form using \ref simplifyFraction and factors out
  any integer parts of the fraction (e.g. "10/4" becomes "2 1/2").
*/
QString QCPAxisTickerPi::fractionToString(int numerator, int denominator) const
{
    if (denominator == 0)
    {
        qDebug() << Q_FUNC_INFO << "called with zero denominator";
        return QString();
    }
    if (mFractionStyle == fsFloatingPoint) // should never be the case when calling this function
    {
        qDebug() << Q_FUNC_INFO << "shouldn't be called with fraction style fsDecimal";
        return QString::number(numerator/(double)denominator); // failsafe
    }
    int sign = numerator*denominator < 0 ? -1 : 1;
    numerator = qAbs(numerator);
    denominator = qAbs(denominator);

    if (denominator == 1)
    {
        return QString::number(sign*numerator);
    } else
    {
        int integerPart = numerator/denominator;
        int remainder = numerator%denominator;
        if (remainder == 0)
        {
            return QString::number(sign*integerPart);
        } else
        {
            if (mFractionStyle == fsAsciiFractions)
            {
                return QString(QLatin1String("%1%2%3/%4"))
                        .arg(sign == -1 ? QLatin1String("-") : QLatin1String(""))
                        .arg(integerPart > 0 ? QString::number(integerPart)+QLatin1String(" ") : QLatin1String(""))
                        .arg(remainder)
                        .arg(denominator);
            } else if (mFractionStyle == fsUnicodeFractions)
            {
                return QString(QLatin1String("%1%2%3"))
                        .arg(sign == -1 ? QLatin1String("-") : QLatin1String(""))
                        .arg(integerPart > 0 ? QString::number(integerPart) : QLatin1String(""))
                        .arg(unicodeFraction(remainder, denominator));
            }
        }
    }
    return QString();
}

/*! \internal
  
  Returns the unicode string representation of the fraction given by \a numerator and \a
  denominator. This is the representation used in \ref fractionToString when the fraction style
  (\ref setFractionStyle) is \ref fsUnicodeFractions.
  
  This method doesn't use the single-character common fractions but builds each fraction from a
  superscript unicode number, the unicode fraction character, and a subscript unicode number.
*/
QString QCPAxisTickerPi::unicodeFraction(int numerator, int denominator) const
{
    return unicodeSuperscript(numerator)+QChar(0x2044)+unicodeSubscript(denominator);
}

/*! \internal
  
  Returns the unicode string representing \a number as superscript. This is used to build
  unicode fractions in \ref unicodeFraction.
*/
QString QCPAxisTickerPi::unicodeSuperscript(int number) const
{
    if (number == 0)
        return QString(QChar(0x2070));

    QString result;
    while (number > 0)
    {
        const int digit = number%10;
        switch (digit)
        {
        case 1: { result.prepend(QChar(0x00B9)); break; }
        case 2: { result.prepend(QChar(0x00B2)); break; }
        case 3: { result.prepend(QChar(0x00B3)); break; }
        default: { result.prepend(QChar(0x2070+digit)); break; }
        }
        number /= 10;
    }
    return result;
}

/*! \internal
  
  Returns the unicode string representing \a number as subscript. This is used to build unicode
  fractions in \ref unicodeFraction.
*/
QString QCPAxisTickerPi::unicodeSubscript(int number) const
{
    if (number == 0)
        return QString(QChar(0x2080));

    QString result;
    while (number > 0)
    {
        result.prepend(QChar(0x2080+number%10));
        number /= 10;
    }
    return result;
}
/* end of 'src/axis/axistickerpi.cpp' */


/* including file 'src/axis/axistickerlog.cpp', size 7106                    */
/* commit ce344b3f96a62e5f652585e55f1ae7c7883cd45b 2018-06-25 01:03:39 +0200 */

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPAxisTickerLog
////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \class QCPAxisTickerLog
  \brief Specialized axis ticker suited for logarithmic axes
  
  \image html axisticker-log.png
  
  This QCPAxisTicker subclass generates ticks with unequal tick intervals suited for logarithmic
  axis scales. The ticks are placed at powers of the specified log base (\ref setLogBase).
  
  Especially in the case of a log base equal to 10 (the default), it might be desirable to have
  tick labels in the form of powers of ten without mantissa display. To achieve this, set the
  number precision (\ref QCPAxis::setNumberPrecision) to zero and the number format (\ref
  QCPAxis::setNumberFormat) to scientific (exponential) display with beautifully typeset decimal
  powers, so a format string of <tt>"eb"</tt>. This will result in the following axis tick labels:
  
  \image html axisticker-log-powers.png

  The ticker can be created and assigned to an axis like this:
  \snippet documentation/doc-image-generator/mainwindow.cpp axistickerlog-creation
*/

/*!
  Constructs the ticker and sets reasonable default values. Axis tickers are commonly created
  managed by a QSharedPointer, which then can be passed to QCPAxis::setTicker.
*/
QCPAxisTickerLog::QCPAxisTickerLog() :
    mLogBase(10.0),
    mSubTickCount(8), // generates 10 intervals
    mLogBaseLnInv(1.0/qLn(mLogBase))
{
}

/*!
  Sets the logarithm base used for tick coordinate generation. The ticks will be placed at integer
  powers of \a base.
*/
void QCPAxisTickerLog::setLogBase(double base)
{
    if (base > 0)
    {
        mLogBase = base;
        mLogBaseLnInv = 1.0/qLn(mLogBase);
    } else
        qDebug() << Q_FUNC_INFO << "log base has to be greater than zero:" << base;
}

/*!
  Sets the number of sub ticks in a tick interval. Within each interval, the sub ticks are spaced
  linearly to provide a better visual guide, so the sub tick density increases toward the higher
  tick.
  
  Note that \a subTicks is the number of sub ticks (not sub intervals) in one tick interval. So in
  the case of logarithm base 10 an intuitive sub tick spacing would be achieved with eight sub
  ticks (the default). This means e.g. between the ticks 10 and 100 there will be eight ticks,
  namely at 20, 30, 40, 50, 60, 70, 80 and 90.
*/
void QCPAxisTickerLog::setSubTickCount(int subTicks)
{
    if (subTicks >= 0)
        mSubTickCount = subTicks;
    else
        qDebug() << Q_FUNC_INFO << "sub tick count can't be negative:" << subTicks;
}

/*! \internal
  
  Since logarithmic tick steps are necessarily different for each tick interval, this method does
  nothing in the case of QCPAxisTickerLog
  
  \seebaseclassmethod
*/
double QCPAxisTickerLog::getTickStep(const QCPRange &range)
{
    // Logarithmic axis ticker has unequal tick spacing, so doesn't need this method
    Q_UNUSED(range)
    return 1.0;
}

/*! \internal
  
  Returns the sub tick count specified in \ref setSubTickCount. For QCPAxisTickerLog, there is no
  automatic sub tick count calculation necessary.
  
  \seebaseclassmethod
*/
int QCPAxisTickerLog::getSubTickCount(double tickStep)
{
    Q_UNUSED(tickStep)
    return mSubTickCount;
}

/*! \internal
  
  Creates ticks with a spacing given by the logarithm base and an increasing integer power in the
  provided \a range. The step in which the power increases tick by tick is chosen in order to keep
  the total number of ticks as close as possible to the tick count (\ref setTickCount). The
  parameter \a tickStep is ignored for QCPAxisTickerLog
  
  \seebaseclassmethod
*/
QVector<double> QCPAxisTickerLog::createTickVector(double tickStep, const QCPRange &range)
{
    Q_UNUSED(tickStep)
    QVector<double> result;
    if (range.lower > 0 && range.upper > 0) // positive range
    {
        double exactPowerStep =  qLn(range.upper/range.lower)*mLogBaseLnInv/(double)(mTickCount+1e-10);
        double newLogBase = qPow(mLogBase, qMax((int)cleanMantissa(exactPowerStep), 1));
        double currentTick = qPow(newLogBase, qFloor(qLn(range.lower)/qLn(newLogBase)));
        result.append(currentTick);
        while (currentTick < range.upper && currentTick > 0) // currentMag might be zero for ranges ~1e-300, just cancel in that case
        {
            currentTick *= newLogBase;
            result.append(currentTick);
        }
    } else if (range.lower < 0 && range.upper < 0) // negative range
    {
        double exactPowerStep =  qLn(range.lower/range.upper)*mLogBaseLnInv/(double)(mTickCount+1e-10);
        double newLogBase = qPow(mLogBase, qMax((int)cleanMantissa(exactPowerStep), 1));
        double currentTick = -qPow(newLogBase, qCeil(qLn(-range.lower)/qLn(newLogBase)));
        result.append(currentTick);
        while (currentTick < range.upper && currentTick < 0) // currentMag might be zero for ranges ~1e-300, just cancel in that case
        {
            currentTick /= newLogBase;
            result.append(currentTick);
        }
    } else // invalid range for logarithmic scale, because lower and upper have different sign
    {
        qDebug() << Q_FUNC_INFO << "Invalid range for logarithmic plot: " << range.lower << ".." << range.upper;
    }

    return result;
}
/* end of 'src/axis/axistickerlog.cpp' */


/* including file 'src/axis/axis.cpp', size 99515                            */
/* commit ce344b3f96a62e5f652585e55f1ae7c7883cd45b 2018-06-25 01:03:39 +0200 */


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPGrid
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPGrid
  \brief Responsible for drawing the grid of a QCPAxis.
  
  This class is tightly bound to QCPAxis. Every axis owns a grid instance and uses it to draw the
  grid lines, sub grid lines and zero-line. You can interact with the grid of an axis via \ref
  QCPAxis::grid. Normally, you don't need to create an instance of QCPGrid yourself.
  
  The axis and grid drawing was split into two classes to allow them to be placed on different
  layers (both QCPAxis and QCPGrid inherit from QCPLayerable). Thus it is possible to have the grid
  in the background and the axes in the foreground, and any plottables/items in between. This
  described situation is the default setup, see the QCPLayer documentation.
*/

/*!
  Creates a QCPGrid instance and sets default values.
  
  You shouldn't instantiate grids on their own, since every QCPAxis brings its own QCPGrid.
*/
QCPGrid::QCPGrid(QCPAxis *parentAxis) :
    QCPLayerable(parentAxis->parentPlot(), QString(), parentAxis),
    mParentAxis(parentAxis)
{
    // warning: this is called in QCPAxis constructor, so parentAxis members should not be accessed/called
    setParent(parentAxis);
    setPen(QPen(QColor(200,200,200), 0, Qt::DotLine));
    setSubGridPen(QPen(QColor(220,220,220), 0, Qt::DotLine));
    setZeroLinePen(QPen(QColor(200,200,200), 0, Qt::SolidLine));
    setSubGridVisible(false);
    setAntialiased(false);
    setAntialiasedSubGrid(false);
    setAntialiasedZeroLine(false);
}

/*!
  Sets whether grid lines at sub tick marks are drawn.
  
  \see setSubGridPen
*/
void QCPGrid::setSubGridVisible(bool visible)
{
    mSubGridVisible = visible;
}

/*!
  Sets whether sub grid lines are drawn antialiased.
*/
void QCPGrid::setAntialiasedSubGrid(bool enabled)
{
    mAntialiasedSubGrid = enabled;
}

/*!
  Sets whether zero lines are drawn antialiased.
*/
void QCPGrid::setAntialiasedZeroLine(bool enabled)
{
    mAntialiasedZeroLine = enabled;
}

/*!
  Sets the pen with which (major) grid lines are drawn.
*/
void QCPGrid::setPen(const QPen &pen)
{
    mPen = pen;
}

/*!
  Sets the pen with which sub grid lines are drawn.
*/
void QCPGrid::setSubGridPen(const QPen &pen)
{
    mSubGridPen = pen;
}

/*!
  Sets the pen with which zero lines are drawn.
  
  Zero lines are lines at value coordinate 0 which may be drawn with a different pen than other grid
  lines. To disable zero lines and just draw normal grid lines at zero, set \a pen to Qt::NoPen.
*/
void QCPGrid::setZeroLinePen(const QPen &pen)
{
    mZeroLinePen = pen;
}

/*! \internal

  A convenience function to easily set the QPainter::Antialiased hint on the provided \a painter
  before drawing the major grid lines.

  This is the antialiasing state the painter passed to the \ref draw method is in by default.
  
  This function takes into account the local setting of the antialiasing flag as well as the
  overrides set with \ref PF_GraphicView::setAntialiasedElements and \ref
  PF_GraphicView::setNotAntialiasedElements.
  
  \see setAntialiased
*/
void QCPGrid::applyDefaultAntialiasingHint(QCPPainter *painter) const
{
    applyAntialiasingHint(painter, mAntialiased, QCP::aeGrid);
}

/*! \internal
  
  Draws grid lines and sub grid lines at the positions of (sub) ticks of the parent axis, spanning
  over the complete axis rect. Also draws the zero line, if appropriate (\ref setZeroLinePen).
*/
void QCPGrid::draw(QCPPainter *painter)
{
    if (!mParentAxis) { qDebug() << Q_FUNC_INFO << "invalid parent axis"; return; }

    if (mParentAxis->subTicks() && mSubGridVisible)
        drawSubGridLines(painter);
    drawGridLines(painter);
}

/*! \internal
  
  Draws the main grid lines and possibly a zero line with the specified painter.
  
  This is a helper function called by \ref draw.
*/
void QCPGrid::drawGridLines(QCPPainter *painter) const
{
    if (!mParentAxis) { qDebug() << Q_FUNC_INFO << "invalid parent axis"; return; }

    const int tickCount = mParentAxis->mTickVector.size();
    double t; // helper variable, result of coordinate-to-pixel transforms
    if (mParentAxis->orientation() == Qt::Horizontal)
    {
        // draw zeroline:
        int zeroLineIndex = -1;
        if (mZeroLinePen.style() != Qt::NoPen && mParentAxis->mRange.lower < 0 && mParentAxis->mRange.upper > 0)
        {
            applyAntialiasingHint(painter, mAntialiasedZeroLine, QCP::aeZeroLine);
            painter->setPen(mZeroLinePen);
            double epsilon = mParentAxis->range().size()*1E-6; // for comparing double to zero
            for (int i=0; i<tickCount; ++i)
            {
                if (qAbs(mParentAxis->mTickVector.at(i)) < epsilon)
                {
                    zeroLineIndex = i;
                    t = mParentAxis->coordToPixel(mParentAxis->mTickVector.at(i)); // x
                    painter->drawLine(QLineF(t, mParentAxis->mAxisRect->bottom(), t, mParentAxis->mAxisRect->top()));
                    break;
                }
            }
        }
        // draw grid lines:
        applyDefaultAntialiasingHint(painter);
        painter->setPen(mPen);
        for (int i=0; i<tickCount; ++i)
        {
            if (i == zeroLineIndex) continue; // don't draw a gridline on top of the zeroline
            t = mParentAxis->coordToPixel(mParentAxis->mTickVector.at(i)); // x
            painter->drawLine(QLineF(t, mParentAxis->mAxisRect->bottom(), t, mParentAxis->mAxisRect->top()));
        }
    } else
    {
        // draw zeroline:
        int zeroLineIndex = -1;
        if (mZeroLinePen.style() != Qt::NoPen && mParentAxis->mRange.lower < 0 && mParentAxis->mRange.upper > 0)
        {
            applyAntialiasingHint(painter, mAntialiasedZeroLine, QCP::aeZeroLine);
            painter->setPen(mZeroLinePen);
            double epsilon = mParentAxis->mRange.size()*1E-6; // for comparing double to zero
            for (int i=0; i<tickCount; ++i)
            {
                if (qAbs(mParentAxis->mTickVector.at(i)) < epsilon)
                {
                    zeroLineIndex = i;
                    t = mParentAxis->coordToPixel(mParentAxis->mTickVector.at(i)); // y
                    painter->drawLine(QLineF(mParentAxis->mAxisRect->left(), t, mParentAxis->mAxisRect->right(), t));
                    break;
                }
            }
        }
        // draw grid lines:
        applyDefaultAntialiasingHint(painter);
        painter->setPen(mPen);
        for (int i=0; i<tickCount; ++i)
        {
            if (i == zeroLineIndex) continue; // don't draw a gridline on top of the zeroline
            t = mParentAxis->coordToPixel(mParentAxis->mTickVector.at(i)); // y
            painter->drawLine(QLineF(mParentAxis->mAxisRect->left(), t, mParentAxis->mAxisRect->right(), t));
        }
    }
}

/*! \internal
  
  Draws the sub grid lines with the specified painter.
  
  This is a helper function called by \ref draw.
*/
void QCPGrid::drawSubGridLines(QCPPainter *painter) const
{
    if (!mParentAxis) { qDebug() << Q_FUNC_INFO << "invalid parent axis"; return; }

    applyAntialiasingHint(painter, mAntialiasedSubGrid, QCP::aeSubGrid);
    double t; // helper variable, result of coordinate-to-pixel transforms
    painter->setPen(mSubGridPen);
    if (mParentAxis->orientation() == Qt::Horizontal)
    {
        for (int i=0; i<mParentAxis->mSubTickVector.size(); ++i)
        {
            t = mParentAxis->coordToPixel(mParentAxis->mSubTickVector.at(i)); // x
            painter->drawLine(QLineF(t, mParentAxis->mAxisRect->bottom(), t, mParentAxis->mAxisRect->top()));
        }
    } else
    {
        for (int i=0; i<mParentAxis->mSubTickVector.size(); ++i)
        {
            t = mParentAxis->coordToPixel(mParentAxis->mSubTickVector.at(i)); // y
            painter->drawLine(QLineF(mParentAxis->mAxisRect->left(), t, mParentAxis->mAxisRect->right(), t));
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPAxis
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPAxis
  \brief Manages a single axis inside a PF_GraphicView.

  Usually doesn't need to be instantiated externally. Access %PF_GraphicView's default four axes via
  PF_GraphicView::xAxis (bottom), PF_GraphicView::yAxis (left), PF_GraphicView::xAxis2 (top) and
  PF_GraphicView::yAxis2 (right).
  
  Axes are always part of an axis rect, see QCPAxisRect.
  \image html AxisNamesOverview.png
  <center>Naming convention of axis parts</center>
  \n

  \image html AxisRectSpacingOverview.png
  <center>Overview of the spacings and paddings that define the geometry of an axis. The dashed gray line
  on the left represents the PF_GraphicView widget border.</center>
  
  Each axis holds an instance of QCPAxisTicker which is used to generate the tick coordinates and
  tick labels. You can access the currently installed \ref ticker or set a new one (possibly one of
  the specialized subclasses, or your own subclass) via \ref setTicker. For details, see the
  documentation of QCPAxisTicker.
*/

/* start of documentation of inline functions */

/*! \fn Qt::Orientation QCPAxis::orientation() const

  Returns the orientation of this axis. The axis orientation (horizontal or vertical) is deduced
  from the axis type (left, top, right or bottom).

  \see orientation(AxisType type), pixelOrientation
*/

/*! \fn QCPGrid *QCPAxis::grid() const
  
  Returns the \ref QCPGrid instance belonging to this axis. Access it to set details about the way the
  grid is displayed.
*/

/*! \fn static Qt::Orientation QCPAxis::orientation(AxisType type)

  Returns the orientation of the specified axis type

  \see orientation(), pixelOrientation
*/

/*! \fn int QCPAxis::pixelOrientation() const

  Returns which direction points towards higher coordinate values/keys, in pixel space.

  This method returns either 1 or -1. If it returns 1, then going in the positive direction along
  the orientation of the axis in pixels corresponds to going from lower to higher axis coordinates.
  On the other hand, if this method returns -1, going to smaller pixel values corresponds to going
  from lower to higher axis coordinates.

  For example, this is useful to easily shift axis coordinates by a certain amount given in pixels,
  without having to care about reversed or vertically aligned axes:

  \code
  double newKey = keyAxis->pixelToCoord(keyAxis->coordToPixel(oldKey)+10*keyAxis->pixelOrientation());
  \endcode

  \a newKey will then contain a key that is ten pixels towards higher keys, starting from \a oldKey.
*/

/*! \fn QSharedPointer<QCPAxisTicker> QCPAxis::ticker() const

  Returns a modifiable shared pointer to the currently installed axis ticker. The axis ticker is
  responsible for generating the tick positions and tick labels of this axis. You can access the
  \ref QCPAxisTicker with this method and modify basic properties such as the approximate tick count
  (\ref QCPAxisTicker::setTickCount).

  You can gain more control over the axis ticks by setting a different \ref QCPAxisTicker subclass, see
  the documentation there. A new axis ticker can be set with \ref setTicker.

  Since the ticker is stored in the axis as a shared pointer, multiple axes may share the same axis
  ticker simply by passing the same shared pointer to multiple axes.

  \see setTicker
*/

/* end of documentation of inline functions */
/* start of documentation of signals */

/*! \fn void QCPAxis::rangeChanged(const QCPRange &newRange)

  This signal is emitted when the range of this axis has changed. You can connect it to the \ref
  setRange slot of another axis to communicate the new range to the other axis, in order for it to
  be synchronized.
  
  You may also manipulate/correct the range with \ref setRange in a slot connected to this signal.
  This is useful if for example a maximum range span shall not be exceeded, or if the lower/upper
  range shouldn't go beyond certain values (see \ref QCPRange::bounded). For example, the following
  slot would limit the x axis to ranges between 0 and 10:
  \code
  customPlot->xAxis->setRange(newRange.bounded(0, 10))
  \endcode
*/

/*! \fn void QCPAxis::rangeChanged(const QCPRange &newRange, const QCPRange &oldRange)
  \overload
  
  Additionally to the new range, this signal also provides the previous range held by the axis as
  \a oldRange.
*/

/*! \fn void QCPAxis::scaleTypeChanged(QCPAxis::ScaleType scaleType);
  
  This signal is emitted when the scale type changes, by calls to \ref setScaleType
*/

/*! \fn void QCPAxis::selectionChanged(QCPAxis::SelectableParts selection)
  
  This signal is emitted when the selection state of this axis has changed, either by user interaction
  or by a direct call to \ref setSelectedParts.
*/

/*! \fn void QCPAxis::selectableChanged(const QCPAxis::SelectableParts &parts);
  
  This signal is emitted when the selectability changes, by calls to \ref setSelectableParts
*/

/* end of documentation of signals */

/*!
  Constructs an Axis instance of Type \a type for the axis rect \a parent.
  
  Usually it isn't necessary to instantiate axes directly, because you can let PF_GraphicView create
  them for you with \ref QCPAxisRect::addAxis. If you want to use own QCPAxis-subclasses however,
  create them manually and then inject them also via \ref QCPAxisRect::addAxis.
*/
QCPAxis::QCPAxis(QCPAxisRect *parent, AxisType type) :
    QCPLayerable(parent->parentPlot(), QString(), parent),
    // axis base:
    mAxisType(type),
    mAxisRect(parent),
    mPadding(5),
    mOrientation(orientation(type)),
    mSelectableParts(spAxis | spTickLabels | spAxisLabel),
    mSelectedParts(spNone),
    mBasePen(QPen(Qt::black, 0, Qt::SolidLine, Qt::SquareCap)),
    mSelectedBasePen(QPen(Qt::blue, 2)),
    // axis label:
    mLabel(),
    mLabelFont(mParentPlot->font()),
    mSelectedLabelFont(QFont(mLabelFont.family(), mLabelFont.pointSize(), QFont::Bold)),
    mLabelColor(Qt::black),
    mSelectedLabelColor(Qt::blue),
    // tick labels:
    mTickLabels(true),
    mTickLabelFont(mParentPlot->font()),
    mSelectedTickLabelFont(QFont(mTickLabelFont.family(), mTickLabelFont.pointSize(), QFont::Bold)),
    mTickLabelColor(Qt::black),
    mSelectedTickLabelColor(Qt::blue),
    mNumberPrecision(6),
    mNumberFormatChar('g'),
    mNumberBeautifulPowers(true),
    // ticks and subticks:
    mTicks(true),
    mSubTicks(true),
    mTickPen(QPen(Qt::black, 0, Qt::SolidLine, Qt::SquareCap)),
    mSelectedTickPen(QPen(Qt::blue, 2)),
    mSubTickPen(QPen(Qt::black, 0, Qt::SolidLine, Qt::SquareCap)),
    mSelectedSubTickPen(QPen(Qt::blue, 2)),
    // scale and range:
    mRange(0, 5),
    mRangeReversed(false),
    mScaleType(stLinear),
    // internal members:
    mGrid(new QCPGrid(this)),
    mAxisPainter(new QCPAxisPainterPrivate(parent->parentPlot())),
    mTicker(new QCPAxisTicker),
    mCachedMarginValid(false),
    mCachedMargin(0)
{
    setParent(parent);
    mGrid->setVisible(false);
    setAntialiased(false);
    setLayer(mParentPlot->currentLayer()); // it's actually on that layer already, but we want it in front of the grid, so we place it on there again

    if (type == atTop)
    {
        setTickLabelPadding(3);
        setLabelPadding(6);
    } else if (type == atRight)
    {
        setTickLabelPadding(7);
        setLabelPadding(12);
    } else if (type == atBottom)
    {
        setTickLabelPadding(3);
        setLabelPadding(3);
    } else if (type == atLeft)
    {
        setTickLabelPadding(5);
        setLabelPadding(10);
    }
}

QCPAxis::~QCPAxis()
{
    delete mAxisPainter;
    delete mGrid; // delete grid here instead of via parent ~QObject for better defined deletion order
}

/* No documentation as it is a property getter */
int QCPAxis::tickLabelPadding() const
{
    return mAxisPainter->tickLabelPadding;
}

/* No documentation as it is a property getter */
double QCPAxis::tickLabelRotation() const
{
    return mAxisPainter->tickLabelRotation;
}

/* No documentation as it is a property getter */
QCPAxis::LabelSide QCPAxis::tickLabelSide() const
{
    return mAxisPainter->tickLabelSide;
}

/* No documentation as it is a property getter */
QString QCPAxis::numberFormat() const
{
    QString result;
    result.append(mNumberFormatChar);
    if (mNumberBeautifulPowers)
    {
        result.append(QLatin1Char('b'));
        if (mAxisPainter->numberMultiplyCross)
            result.append(QLatin1Char('c'));
    }
    return result;
}

/* No documentation as it is a property getter */
int QCPAxis::tickLengthIn() const
{
    return mAxisPainter->tickLengthIn;
}

/* No documentation as it is a property getter */
int QCPAxis::tickLengthOut() const
{
    return mAxisPainter->tickLengthOut;
}

/* No documentation as it is a property getter */
int QCPAxis::subTickLengthIn() const
{
    return mAxisPainter->subTickLengthIn;
}

/* No documentation as it is a property getter */
int QCPAxis::subTickLengthOut() const
{
    return mAxisPainter->subTickLengthOut;
}

/* No documentation as it is a property getter */
int QCPAxis::labelPadding() const
{
    return mAxisPainter->labelPadding;
}

/* No documentation as it is a property getter */
int QCPAxis::offset() const
{
    return mAxisPainter->offset;
}

/* No documentation as it is a property getter */
QCPLineEnding QCPAxis::lowerEnding() const
{
    return mAxisPainter->lowerEnding;
}

/* No documentation as it is a property getter */
QCPLineEnding QCPAxis::upperEnding() const
{
    return mAxisPainter->upperEnding;
}

/*!
  Sets whether the axis uses a linear scale or a logarithmic scale.
  
  Note that this method controls the coordinate transformation. For logarithmic scales, you will
  likely also want to use a logarithmic tick spacing and labeling, which can be achieved by setting
  the axis ticker to an instance of \ref QCPAxisTickerLog :
  
  \snippet documentation/doc-code-snippets/mainwindow.cpp qcpaxisticker-log-creation
  
  See the documentation of \ref QCPAxisTickerLog about the details of logarithmic axis tick
  creation.
  
  \ref setNumberPrecision
*/
void QCPAxis::setScaleType(QCPAxis::ScaleType type)
{
    if (mScaleType != type)
    {
        mScaleType = type;
        if (mScaleType == stLogarithmic)
            setRange(mRange.sanitizedForLogScale());
        mCachedMarginValid = false;
        emit scaleTypeChanged(mScaleType);
    }
}

/*!
  Sets the range of the axis.
  
  This slot may be connected with the \ref rangeChanged signal of another axis so this axis
  is always synchronized with the other axis range, when it changes.
  
  To invert the direction of an axis, use \ref setRangeReversed.
*/
void QCPAxis::setRange(const QCPRange &range)
{
    if (range.lower == mRange.lower && range.upper == mRange.upper)
        return;

    if (!QCPRange::validRange(range)) return;
    QCPRange oldRange = mRange;
    if (mScaleType == stLogarithmic)
    {
        mRange = range.sanitizedForLogScale();
    } else
    {
        mRange = range.sanitizedForLinScale();
    }
    emit rangeChanged(mRange);
    emit rangeChanged(mRange, oldRange);
}

/*!
  Sets whether the user can (de-)select the parts in \a selectable by clicking on the PF_GraphicView surface.
  (When \ref PF_GraphicView::setInteractions contains iSelectAxes.)
  
  However, even when \a selectable is set to a value not allowing the selection of a specific part,
  it is still possible to set the selection of this part manually, by calling \ref setSelectedParts
  directly.
  
  \see SelectablePart, setSelectedParts
*/
void QCPAxis::setSelectableParts(const SelectableParts &selectable)
{
    if (mSelectableParts != selectable)
    {
        mSelectableParts = selectable;
        emit selectableChanged(mSelectableParts);
    }
}

/*!
  Sets the selected state of the respective axis parts described by \ref SelectablePart. When a part
  is selected, it uses a different pen/font.
  
  The entire selection mechanism for axes is handled automatically when \ref
  PF_GraphicView::setInteractions contains iSelectAxes. You only need to call this function when you
  wish to change the selection state manually.
  
  This function can change the selection state of a part, independent of the \ref setSelectableParts setting.
  
  emits the \ref selectionChanged signal when \a selected is different from the previous selection state.
  
  \see SelectablePart, setSelectableParts, selectTest, setSelectedBasePen, setSelectedTickPen, setSelectedSubTickPen,
  setSelectedTickLabelFont, setSelectedLabelFont, setSelectedTickLabelColor, setSelectedLabelColor
*/
void QCPAxis::setSelectedParts(const SelectableParts &selected)
{
    if (mSelectedParts != selected)
    {
        mSelectedParts = selected;
        emit selectionChanged(mSelectedParts);
    }
}

/*!
  \overload
  
  Sets the lower and upper bound of the axis range.
  
  To invert the direction of an axis, use \ref setRangeReversed.
  
  There is also a slot to set a range, see \ref setRange(const QCPRange &range).
*/
void QCPAxis::setRange(double lower, double upper)
{
    if (lower == mRange.lower && upper == mRange.upper)
        return;

    if (!QCPRange::validRange(lower, upper)) return;
    QCPRange oldRange = mRange;
    mRange.lower = lower;
    mRange.upper = upper;
    if (mScaleType == stLogarithmic)
    {
        mRange = mRange.sanitizedForLogScale();
    } else
    {
        mRange = mRange.sanitizedForLinScale();
    }
    emit rangeChanged(mRange);
    emit rangeChanged(mRange, oldRange);
}

/*!
  \overload
  
  Sets the range of the axis.
  
  The \a position coordinate indicates together with the \a alignment parameter, where the new
  range will be positioned. \a size defines the size of the new axis range. \a alignment may be
  Qt::AlignLeft, Qt::AlignRight or Qt::AlignCenter. This will cause the left border, right border,
  or center of the range to be aligned with \a position. Any other values of \a alignment will
  default to Qt::AlignCenter.
*/
void QCPAxis::setRange(double position, double size, Qt::AlignmentFlag alignment)
{
    if (alignment == Qt::AlignLeft)
        setRange(position, position+size);
    else if (alignment == Qt::AlignRight)
        setRange(position-size, position);
    else // alignment == Qt::AlignCenter
        setRange(position-size/2.0, position+size/2.0);
}

/*!
  Sets the lower bound of the axis range. The upper bound is not changed.
  \see setRange
*/
void QCPAxis::setRangeLower(double lower)
{
    if (mRange.lower == lower)
        return;

    QCPRange oldRange = mRange;
    mRange.lower = lower;
    if (mScaleType == stLogarithmic)
    {
        mRange = mRange.sanitizedForLogScale();
    } else
    {
        mRange = mRange.sanitizedForLinScale();
    }
    emit rangeChanged(mRange);
    emit rangeChanged(mRange, oldRange);
}

/*!
  Sets the upper bound of the axis range. The lower bound is not changed.
  \see setRange
*/
void QCPAxis::setRangeUpper(double upper)
{
    if (mRange.upper == upper)
        return;

    QCPRange oldRange = mRange;
    mRange.upper = upper;
    if (mScaleType == stLogarithmic)
    {
        mRange = mRange.sanitizedForLogScale();
    } else
    {
        mRange = mRange.sanitizedForLinScale();
    }
    emit rangeChanged(mRange);
    emit rangeChanged(mRange, oldRange);
}

/*!
  Sets whether the axis range (direction) is displayed reversed. Normally, the values on horizontal
  axes increase left to right, on vertical axes bottom to top. When \a reversed is set to true, the
  direction of increasing values is inverted.

  Note that the range and data interface stays the same for reversed axes, e.g. the \a lower part
  of the \ref setRange interface will still reference the mathematically smaller number than the \a
  upper part.
*/
void QCPAxis::setRangeReversed(bool reversed)
{
    mRangeReversed = reversed;
}

/*!
  The axis ticker is responsible for generating the tick positions and tick labels. See the
  documentation of QCPAxisTicker for details on how to work with axis tickers.
  
  You can change the tick positioning/labeling behaviour of this axis by setting a different
  QCPAxisTicker subclass using this method. If you only wish to modify the currently installed axis
  ticker, access it via \ref ticker.
  
  Since the ticker is stored in the axis as a shared pointer, multiple axes may share the same axis
  ticker simply by passing the same shared pointer to multiple axes.
  
  \see ticker
*/
void QCPAxis::setTicker(QSharedPointer<QCPAxisTicker> ticker)
{
    if (ticker)
        mTicker = ticker;
    else
        qDebug() << Q_FUNC_INFO << "can not set 0 as axis ticker";
    // no need to invalidate margin cache here because produced tick labels are checked for changes in setupTickVector
}

/*!
  Sets whether tick marks are displayed.

  Note that setting \a show to false does not imply that tick labels are invisible, too. To achieve
  that, see \ref setTickLabels.
  
  \see setSubTicks
*/
void QCPAxis::setTicks(bool show)
{
    if (mTicks != show)
    {
        mTicks = show;
        mCachedMarginValid = false;
    }
}

/*!
  Sets whether tick labels are displayed. Tick labels are the numbers drawn next to tick marks.
*/
void QCPAxis::setTickLabels(bool show)
{
    if (mTickLabels != show)
    {
        mTickLabels = show;
        mCachedMarginValid = false;
        if (!mTickLabels)
            mTickVectorLabels.clear();
    }
}


/*!
  Sets the distance between the axis base line (including any outward ticks) and the tick labels.
  \see setLabelPadding, setPadding
*/
void QCPAxis::setTickLabelPadding(int padding)
{
    if (mAxisPainter->tickLabelPadding != padding)
    {
        mAxisPainter->tickLabelPadding = padding;
        mCachedMarginValid = false;
    }
}

/*!
  Sets the font of the tick labels.
  
  \see setTickLabels, setTickLabelColor
*/
void QCPAxis::setTickLabelFont(const QFont &font)
{
    if (font != mTickLabelFont)
    {
        mTickLabelFont = font;
        mCachedMarginValid = false;
    }
}

/*!
  Sets the color of the tick labels.
  
  \see setTickLabels, setTickLabelFont
*/
void QCPAxis::setTickLabelColor(const QColor &color)
{
    mTickLabelColor = color;
}

/*!
  Sets the rotation of the tick labels. If \a degrees is zero, the labels are drawn normally. Else,
  the tick labels are drawn rotated by \a degrees clockwise. The specified angle is bound to values
  from -90 to 90 degrees.
  
  If \a degrees is exactly -90, 0 or 90, the tick labels are centered on the tick coordinate. For
  other angles, the label is drawn with an offset such that it seems to point toward or away from
  the tick mark.
*/
void QCPAxis::setTickLabelRotation(double degrees)
{
    if (!qFuzzyIsNull(degrees-mAxisPainter->tickLabelRotation))
    {
        mAxisPainter->tickLabelRotation = qBound(-90.0, degrees, 90.0);
        mCachedMarginValid = false;
    }
}

/*!
  Sets whether the tick labels (numbers) shall appear inside or outside the axis rect.
  
  The usual and default setting is \ref lsOutside. Very compact plots sometimes require tick labels
  to be inside the axis rect, to save space. If \a side is set to \ref lsInside, the tick labels
  appear on the inside are additionally clipped to the axis rect.
*/
void QCPAxis::setTickLabelSide(LabelSide side)
{
    mAxisPainter->tickLabelSide = side;
    mCachedMarginValid = false;
}

/*!
  Sets the number format for the numbers in tick labels. This \a formatCode is an extended version
  of the format code used e.g. by QString::number() and QLocale::toString(). For reference about
  that, see the "Argument Formats" section in the detailed description of the QString class.
  
  \a formatCode is a string of one, two or three characters. The first character is identical to
  the normal format code used by Qt. In short, this means: 'e'/'E' scientific format, 'f' fixed
  format, 'g'/'G' scientific or fixed, whichever is shorter.
  
  The second and third characters are optional and specific to PF_GraphicView:\n
  If the first char was 'e' or 'g', numbers are/might be displayed in the scientific format, e.g.
  "5.5e9", which is ugly in a plot. So when the second char of \a formatCode is set to 'b' (for
  "beautiful"), those exponential numbers are formatted in a more natural way, i.e. "5.5
  [multiplication sign] 10 [superscript] 9". By default, the multiplication sign is a centered dot.
  If instead a cross should be shown (as is usual in the USA), the third char of \a formatCode can
  be set to 'c'. The inserted multiplication signs are the UTF-8 characters 215 (0xD7) for the
  cross and 183 (0xB7) for the dot.
  
  Examples for \a formatCode:
  \li \c g normal format code behaviour. If number is small, fixed format is used, if number is large,
  normal scientific format is used
  \li \c gb If number is small, fixed format is used, if number is large, scientific format is used with
  beautifully typeset decimal powers and a dot as multiplication sign
  \li \c ebc All numbers are in scientific format with beautifully typeset decimal power and a cross as
  multiplication sign
  \li \c fb illegal format code, since fixed format doesn't support (or need) beautifully typeset decimal
  powers. Format code will be reduced to 'f'.
  \li \c hello illegal format code, since first char is not 'e', 'E', 'f', 'g' or 'G'. Current format
  code will not be changed.
*/
void QCPAxis::setNumberFormat(const QString &formatCode)
{
    if (formatCode.isEmpty())
    {
        qDebug() << Q_FUNC_INFO << "Passed formatCode is empty";
        return;
    }
    mCachedMarginValid = false;

    // interpret first char as number format char:
    QString allowedFormatChars(QLatin1String("eEfgG"));
    if (allowedFormatChars.contains(formatCode.at(0)))
    {
        mNumberFormatChar = QLatin1Char(formatCode.at(0).toLatin1());
    } else
    {
        qDebug() << Q_FUNC_INFO << "Invalid number format code (first char not in 'eEfgG'):" << formatCode;
        return;
    }
    if (formatCode.length() < 2)
    {
        mNumberBeautifulPowers = false;
        mAxisPainter->numberMultiplyCross = false;
        return;
    }

    // interpret second char as indicator for beautiful decimal powers:
    if (formatCode.at(1) == QLatin1Char('b') && (mNumberFormatChar == QLatin1Char('e') || mNumberFormatChar == QLatin1Char('g')))
    {
        mNumberBeautifulPowers = true;
    } else
    {
        qDebug() << Q_FUNC_INFO << "Invalid number format code (second char not 'b' or first char neither 'e' nor 'g'):" << formatCode;
        return;
    }
    if (formatCode.length() < 3)
    {
        mAxisPainter->numberMultiplyCross = false;
        return;
    }

    // interpret third char as indicator for dot or cross multiplication symbol:
    if (formatCode.at(2) == QLatin1Char('c'))
    {
        mAxisPainter->numberMultiplyCross = true;
    } else if (formatCode.at(2) == QLatin1Char('d'))
    {
        mAxisPainter->numberMultiplyCross = false;
    } else
    {
        qDebug() << Q_FUNC_INFO << "Invalid number format code (third char neither 'c' nor 'd'):" << formatCode;
        return;
    }
}

/*!
  Sets the precision of the tick label numbers. See QLocale::toString(double i, char f, int prec)
  for details. The effect of precisions are most notably for number Formats starting with 'e', see
  \ref setNumberFormat
*/
void QCPAxis::setNumberPrecision(int precision)
{
    if (mNumberPrecision != precision)
    {
        mNumberPrecision = precision;
        mCachedMarginValid = false;
    }
}

/*!
  Sets the length of the ticks in pixels. \a inside is the length the ticks will reach inside the
  plot and \a outside is the length they will reach outside the plot. If \a outside is greater than
  zero, the tick labels and axis label will increase their distance to the axis accordingly, so
  they won't collide with the ticks.
  
  \see setSubTickLength, setTickLengthIn, setTickLengthOut
*/
void QCPAxis::setTickLength(int inside, int outside)
{
    setTickLengthIn(inside);
    setTickLengthOut(outside);
}

/*!
  Sets the length of the inward ticks in pixels. \a inside is the length the ticks will reach
  inside the plot.
  
  \see setTickLengthOut, setTickLength, setSubTickLength
*/
void QCPAxis::setTickLengthIn(int inside)
{
    if (mAxisPainter->tickLengthIn != inside)
    {
        mAxisPainter->tickLengthIn = inside;
    }
}

/*!
  Sets the length of the outward ticks in pixels. \a outside is the length the ticks will reach
  outside the plot. If \a outside is greater than zero, the tick labels and axis label will
  increase their distance to the axis accordingly, so they won't collide with the ticks.
  
  \see setTickLengthIn, setTickLength, setSubTickLength
*/
void QCPAxis::setTickLengthOut(int outside)
{
    if (mAxisPainter->tickLengthOut != outside)
    {
        mAxisPainter->tickLengthOut = outside;
        mCachedMarginValid = false; // only outside tick length can change margin
    }
}

/*!
  Sets whether sub tick marks are displayed.
  
  Sub ticks are only potentially visible if (major) ticks are also visible (see \ref setTicks)
  
  \see setTicks
*/
void QCPAxis::setSubTicks(bool show)
{
    if (mSubTicks != show)
    {
        mSubTicks = show;
        mCachedMarginValid = false;
    }
}

/*!
  Sets the length of the subticks in pixels. \a inside is the length the subticks will reach inside
  the plot and \a outside is the length they will reach outside the plot. If \a outside is greater
  than zero, the tick labels and axis label will increase their distance to the axis accordingly,
  so they won't collide with the ticks.
  
  \see setTickLength, setSubTickLengthIn, setSubTickLengthOut
*/
void QCPAxis::setSubTickLength(int inside, int outside)
{
    setSubTickLengthIn(inside);
    setSubTickLengthOut(outside);
}

/*!
  Sets the length of the inward subticks in pixels. \a inside is the length the subticks will reach inside
  the plot.
  
  \see setSubTickLengthOut, setSubTickLength, setTickLength
*/
void QCPAxis::setSubTickLengthIn(int inside)
{
    if (mAxisPainter->subTickLengthIn != inside)
    {
        mAxisPainter->subTickLengthIn = inside;
    }
}

/*!
  Sets the length of the outward subticks in pixels. \a outside is the length the subticks will reach
  outside the plot. If \a outside is greater than zero, the tick labels will increase their
  distance to the axis accordingly, so they won't collide with the ticks.
  
  \see setSubTickLengthIn, setSubTickLength, setTickLength
*/
void QCPAxis::setSubTickLengthOut(int outside)
{
    if (mAxisPainter->subTickLengthOut != outside)
    {
        mAxisPainter->subTickLengthOut = outside;
        mCachedMarginValid = false; // only outside tick length can change margin
    }
}

/*!
  Sets the pen, the axis base line is drawn with.
  
  \see setTickPen, setSubTickPen
*/
void QCPAxis::setBasePen(const QPen &pen)
{
    mBasePen = pen;
}

/*!
  Sets the pen, tick marks will be drawn with.
  
  \see setTickLength, setBasePen
*/
void QCPAxis::setTickPen(const QPen &pen)
{
    mTickPen = pen;
}

/*!
  Sets the pen, subtick marks will be drawn with.
  
  \see setSubTickCount, setSubTickLength, setBasePen
*/
void QCPAxis::setSubTickPen(const QPen &pen)
{
    mSubTickPen = pen;
}

/*!
  Sets the font of the axis label.
  
  \see setLabelColor
*/
void QCPAxis::setLabelFont(const QFont &font)
{
    if (mLabelFont != font)
    {
        mLabelFont = font;
        mCachedMarginValid = false;
    }
}

/*!
  Sets the color of the axis label.
  
  \see setLabelFont
*/
void QCPAxis::setLabelColor(const QColor &color)
{
    mLabelColor = color;
}

/*!
  Sets the text of the axis label that will be shown below/above or next to the axis, depending on
  its orientation. To disable axis labels, pass an empty string as \a str.
*/
void QCPAxis::setLabel(const QString &str)
{
    if (mLabel != str)
    {
        mLabel = str;
        mCachedMarginValid = false;
    }
}

/*!
  Sets the distance between the tick labels and the axis label.
  
  \see setTickLabelPadding, setPadding
*/
void QCPAxis::setLabelPadding(int padding)
{
    if (mAxisPainter->labelPadding != padding)
    {
        mAxisPainter->labelPadding = padding;
        mCachedMarginValid = false;
    }
}

/*!
  Sets the padding of the axis.

  When \ref QCPAxisRect::setAutoMargins is enabled, the padding is the additional outer most space,
  that is left blank.
  
  The axis padding has no meaning if \ref QCPAxisRect::setAutoMargins is disabled.
  
  \see setLabelPadding, setTickLabelPadding
*/
void QCPAxis::setPadding(int padding)
{
    if (mPadding != padding)
    {
        mPadding = padding;
        mCachedMarginValid = false;
    }
}

/*!
  Sets the offset the axis has to its axis rect side.
  
  If an axis rect side has multiple axes and automatic margin calculation is enabled for that side,
  only the offset of the inner most axis has meaning (even if it is set to be invisible). The
  offset of the other, outer axes is controlled automatically, to place them at appropriate
  positions.
*/
void QCPAxis::setOffset(int offset)
{
    mAxisPainter->offset = offset;
}

/*!
  Sets the font that is used for tick labels when they are selected.
  
  \see setTickLabelFont, setSelectableParts, setSelectedParts, PF_GraphicView::setInteractions
*/
void QCPAxis::setSelectedTickLabelFont(const QFont &font)
{
    if (font != mSelectedTickLabelFont)
    {
        mSelectedTickLabelFont = font;
        // don't set mCachedMarginValid to false here because margin calculation is always done with non-selected fonts
    }
}

/*!
  Sets the font that is used for the axis label when it is selected.
  
  \see setLabelFont, setSelectableParts, setSelectedParts, PF_GraphicView::setInteractions
*/
void QCPAxis::setSelectedLabelFont(const QFont &font)
{
    mSelectedLabelFont = font;
    // don't set mCachedMarginValid to false here because margin calculation is always done with non-selected fonts
}

/*!
  Sets the color that is used for tick labels when they are selected.
  
  \see setTickLabelColor, setSelectableParts, setSelectedParts, PF_GraphicView::setInteractions
*/
void QCPAxis::setSelectedTickLabelColor(const QColor &color)
{
    if (color != mSelectedTickLabelColor)
    {
        mSelectedTickLabelColor = color;
    }
}

/*!
  Sets the color that is used for the axis label when it is selected.
  
  \see setLabelColor, setSelectableParts, setSelectedParts, PF_GraphicView::setInteractions
*/
void QCPAxis::setSelectedLabelColor(const QColor &color)
{
    mSelectedLabelColor = color;
}

/*!
  Sets the pen that is used to draw the axis base line when selected.
  
  \see setBasePen, setSelectableParts, setSelectedParts, PF_GraphicView::setInteractions
*/
void QCPAxis::setSelectedBasePen(const QPen &pen)
{
    mSelectedBasePen = pen;
}

/*!
  Sets the pen that is used to draw the (major) ticks when selected.
  
  \see setTickPen, setSelectableParts, setSelectedParts, PF_GraphicView::setInteractions
*/
void QCPAxis::setSelectedTickPen(const QPen &pen)
{
    mSelectedTickPen = pen;
}

/*!
  Sets the pen that is used to draw the subticks when selected.
  
  \see setSubTickPen, setSelectableParts, setSelectedParts, PF_GraphicView::setInteractions
*/
void QCPAxis::setSelectedSubTickPen(const QPen &pen)
{
    mSelectedSubTickPen = pen;
}

/*!
  Sets the style for the lower axis ending. See the documentation of QCPLineEnding for available
  styles.
  
  For horizontal axes, this method refers to the left ending, for vertical axes the bottom ending.
  Note that this meaning does not change when the axis range is reversed with \ref
  setRangeReversed.
  
  \see setUpperEnding
*/
void QCPAxis::setLowerEnding(const QCPLineEnding &ending)
{
    mAxisPainter->lowerEnding = ending;
}

/*!
  Sets the style for the upper axis ending. See the documentation of QCPLineEnding for available
  styles.
  
  For horizontal axes, this method refers to the right ending, for vertical axes the top ending.
  Note that this meaning does not change when the axis range is reversed with \ref
  setRangeReversed.
  
  \see setLowerEnding
*/
void QCPAxis::setUpperEnding(const QCPLineEnding &ending)
{
    mAxisPainter->upperEnding = ending;
}

/*!
  If the scale type (\ref setScaleType) is \ref stLinear, \a diff is added to the lower and upper
  bounds of the range. The range is simply moved by \a diff.
  
  If the scale type is \ref stLogarithmic, the range bounds are multiplied by \a diff. This
  corresponds to an apparent "linear" move in logarithmic scaling by a distance of log(diff).
*/
void QCPAxis::moveRange(double diff)
{
    QCPRange oldRange = mRange;
    if (mScaleType == stLinear)
    {
        mRange.lower += diff;
        mRange.upper += diff;
    } else // mScaleType == stLogarithmic
    {
        mRange.lower *= diff;
        mRange.upper *= diff;
    }
    emit rangeChanged(mRange);
    emit rangeChanged(mRange, oldRange);
}

/*!
  Scales the range of this axis by \a factor around the center of the current axis range. For
  example, if \a factor is 2.0, then the axis range will double its size, and the point at the axis
  range center won't have changed its position in the PF_GraphicView widget (i.e. coordinates around
  the center will have moved symmetrically closer).

  If you wish to scale around a different coordinate than the current axis range center, use the
  overload \ref scaleRange(double factor, double center).
*/
void QCPAxis::scaleRange(double factor)
{
    scaleRange(factor, range().center());
}

/*! \overload

  Scales the range of this axis by \a factor around the coordinate \a center. For example, if \a
  factor is 2.0, \a center is 1.0, then the axis range will double its size, and the point at
  coordinate 1.0 won't have changed its position in the PF_GraphicView widget (i.e. coordinates
  around 1.0 will have moved symmetrically closer to 1.0).

  \see scaleRange(double factor)
*/
void QCPAxis::scaleRange(double factor, double center)
{
    QCPRange oldRange = mRange;
    if (mScaleType == stLinear)
    {
        QCPRange newRange;
        newRange.lower = (mRange.lower-center)*factor + center;
        newRange.upper = (mRange.upper-center)*factor + center;
        if (QCPRange::validRange(newRange))
            mRange = newRange.sanitizedForLinScale();
    } else // mScaleType == stLogarithmic
    {
        if ((mRange.upper < 0 && center < 0) || (mRange.upper > 0 && center > 0)) // make sure center has same sign as range
        {
            QCPRange newRange;
            newRange.lower = qPow(mRange.lower/center, factor)*center;
            newRange.upper = qPow(mRange.upper/center, factor)*center;
            if (QCPRange::validRange(newRange))
                mRange = newRange.sanitizedForLogScale();
        } else
            qDebug() << Q_FUNC_INFO << "Center of scaling operation doesn't lie in same logarithmic sign domain as range:" << center;
    }
    emit rangeChanged(mRange);
    emit rangeChanged(mRange, oldRange);
}

/*!
  Scales the range of this axis to have a certain scale \a ratio to \a otherAxis. The scaling will
  be done around the center of the current axis range.

  For example, if \a ratio is 1, this axis is the \a yAxis and \a otherAxis is \a xAxis, graphs
  plotted with those axes will appear in a 1:1 aspect ratio, independent of the aspect ratio the
  axis rect has.

  This is an operation that changes the range of this axis once, it doesn't fix the scale ratio
  indefinitely. Note that calling this function in the constructor of the PF_GraphicView's parent
  won't have the desired effect, since the widget dimensions aren't defined yet, and a resizeEvent
  will follow.
*/
void QCPAxis::setScaleRatio(const QCPAxis *otherAxis, double ratio)
{
    int otherPixelSize, ownPixelSize;

    if (otherAxis->orientation() == Qt::Horizontal)
        otherPixelSize = otherAxis->axisRect()->width();
    else
        otherPixelSize = otherAxis->axisRect()->height();

    if (orientation() == Qt::Horizontal)
        ownPixelSize = axisRect()->width();
    else
        ownPixelSize = axisRect()->height();

    double newRangeSize = ratio*otherAxis->range().size()*ownPixelSize/(double)otherPixelSize;
    setRange(range().center(), newRangeSize, Qt::AlignCenter);
    mTicker->setAutoTickStep(false);
    mTicker->setTickStep(otherAxis->mTicker->getTickStep(otherAxis->range()));
}

/*!
  Changes the axis range such that all plottables associated with this axis are fully visible in
  that dimension.
  
  \see QCPAbstractPlottable::rescaleAxes, PF_GraphicView::rescaleAxes
*/
void QCPAxis::rescale(bool onlyVisiblePlottables)
{
    QList<QCPAbstractPlottable*> p = plottables();
    QCPRange newRange;
    bool haveRange = false;
    for (int i=0; i<p.size(); ++i)
    {
        if (!p.at(i)->realVisibility() && onlyVisiblePlottables)
            continue;
        QCPRange plottableRange;
        bool currentFoundRange;
        QCP::SignDomain signDomain = QCP::sdBoth;
        if (mScaleType == stLogarithmic)
            signDomain = (mRange.upper < 0 ? QCP::sdNegative : QCP::sdPositive);
        if (p.at(i)->keyAxis() == this)
            plottableRange = p.at(i)->getKeyRange(currentFoundRange, signDomain);
        else
            plottableRange = p.at(i)->getValueRange(currentFoundRange, signDomain);
        if (currentFoundRange)
        {
            if (!haveRange)
                newRange = plottableRange;
            else
                newRange.expand(plottableRange);
            haveRange = true;
        }
    }
    if (haveRange)
    {
        if (!QCPRange::validRange(newRange)) // likely due to range being zero (plottable has only constant data in this axis dimension), shift current range to at least center the plottable
        {
            double center = (newRange.lower+newRange.upper)*0.5; // upper and lower should be equal anyway, but just to make sure, incase validRange returned false for other reason
            if (mScaleType == stLinear)
            {
                newRange.lower = center-mRange.size()/2.0;
                newRange.upper = center+mRange.size()/2.0;
            } else // mScaleType == stLogarithmic
            {
                newRange.lower = center/qSqrt(mRange.upper/mRange.lower);
                newRange.upper = center*qSqrt(mRange.upper/mRange.lower);
            }
        }
        setRange(newRange);
    }
}

/*!
  Transforms \a value, in pixel coordinates of the PF_GraphicView widget, to axis coordinates.
*/
double QCPAxis::pixelToCoord(double value) const
{
    if (orientation() == Qt::Horizontal)
    {
        if (mScaleType == stLinear)
        {
            if (!mRangeReversed)
                return (value-mAxisRect->left())/(double)mAxisRect->width()*mRange.size()+mRange.lower;
            else
                return -(value-mAxisRect->left())/(double)mAxisRect->width()*mRange.size()+mRange.upper;
        } else // mScaleType == stLogarithmic
        {
            if (!mRangeReversed)
                return qPow(mRange.upper/mRange.lower, (value-mAxisRect->left())/(double)mAxisRect->width())*mRange.lower;
            else
                return qPow(mRange.upper/mRange.lower, (mAxisRect->left()-value)/(double)mAxisRect->width())*mRange.upper;
        }
    } else // orientation() == Qt::Vertical
    {
        if (mScaleType == stLinear)
        {
            if (!mRangeReversed)
                return (mAxisRect->bottom()-value)/(double)mAxisRect->height()*mRange.size()+mRange.lower;
            else
                return -(mAxisRect->bottom()-value)/(double)mAxisRect->height()*mRange.size()+mRange.upper;
        } else // mScaleType == stLogarithmic
        {
            if (!mRangeReversed)
                return qPow(mRange.upper/mRange.lower, (mAxisRect->bottom()-value)/(double)mAxisRect->height())*mRange.lower;
            else
                return qPow(mRange.upper/mRange.lower, (value-mAxisRect->bottom())/(double)mAxisRect->height())*mRange.upper;
        }
    }
}

/*!
  Transforms \a value, in coordinates of the axis, to pixel coordinates of the PF_GraphicView widget.
*/
double QCPAxis::coordToPixel(double value) const
{
    if (orientation() == Qt::Horizontal)
    {
        if (mScaleType == stLinear)
        {
            if (!mRangeReversed)
                return (value-mRange.lower)/mRange.size()*mAxisRect->width()+mAxisRect->left();
            else
                return (mRange.upper-value)/mRange.size()*mAxisRect->width()+mAxisRect->left();
        } else // mScaleType == stLogarithmic
        {
            if (value >= 0.0 && mRange.upper < 0.0) // invalid value for logarithmic scale, just draw it outside visible range
                return !mRangeReversed ? mAxisRect->right()+200 : mAxisRect->left()-200;
            else if (value <= 0.0 && mRange.upper >= 0.0) // invalid value for logarithmic scale, just draw it outside visible range
                return !mRangeReversed ? mAxisRect->left()-200 : mAxisRect->right()+200;
            else
            {
                if (!mRangeReversed)
                    return qLn(value/mRange.lower)/qLn(mRange.upper/mRange.lower)*mAxisRect->width()+mAxisRect->left();
                else
                    return qLn(mRange.upper/value)/qLn(mRange.upper/mRange.lower)*mAxisRect->width()+mAxisRect->left();
            }
        }
    } else // orientation() == Qt::Vertical
    {
        if (mScaleType == stLinear)
        {
            if (!mRangeReversed)
                return mAxisRect->bottom()-(value-mRange.lower)/mRange.size()*mAxisRect->height();
            else
                return mAxisRect->bottom()-(mRange.upper-value)/mRange.size()*mAxisRect->height();
        } else // mScaleType == stLogarithmic
        {
            if (value >= 0.0 && mRange.upper < 0.0) // invalid value for logarithmic scale, just draw it outside visible range
                return !mRangeReversed ? mAxisRect->top()-200 : mAxisRect->bottom()+200;
            else if (value <= 0.0 && mRange.upper >= 0.0) // invalid value for logarithmic scale, just draw it outside visible range
                return !mRangeReversed ? mAxisRect->bottom()+200 : mAxisRect->top()-200;
            else
            {
                if (!mRangeReversed)
                    return mAxisRect->bottom()-qLn(value/mRange.lower)/qLn(mRange.upper/mRange.lower)*mAxisRect->height();
                else
                    return mAxisRect->bottom()-qLn(mRange.upper/value)/qLn(mRange.upper/mRange.lower)*mAxisRect->height();
            }
        }
    }
}

/*!
  Returns the part of the axis that is hit by \a pos (in pixels). The return value of this function
  is independent of the user-selectable parts defined with \ref setSelectableParts. Further, this
  function does not change the current selection state of the axis.
  
  If the axis is not visible (\ref setVisible), this function always returns \ref spNone.
  
  \see setSelectedParts, setSelectableParts, PF_GraphicView::setInteractions
*/
QCPAxis::SelectablePart QCPAxis::getPartAt(const QPointF &pos) const
{
    if (!mVisible)
        return spNone;

    if (mAxisPainter->axisSelectionBox().contains(pos.toPoint()))
        return spAxis;
    else if (mAxisPainter->tickLabelsSelectionBox().contains(pos.toPoint()))
        return spTickLabels;
    else if (mAxisPainter->labelSelectionBox().contains(pos.toPoint()))
        return spAxisLabel;
    else
        return spNone;
}

/* inherits documentation from base class */
double QCPAxis::selectTest(const QPointF &pos, bool onlySelectable, QVariant *details) const
{
    if (!mParentPlot) return -1;
    SelectablePart part = getPartAt(pos);
    if ((onlySelectable && !mSelectableParts.testFlag(part)) || part == spNone)
        return -1;

    if (details)
        details->setValue(part);
    return mParentPlot->selectionTolerance()*0.99;
}

/*!
  Returns a list of all the plottables that have this axis as key or value axis.
  
  If you are only interested in plottables of type QCPGraph, see \ref graphs.
  
  \see graphs, items
*/
QList<QCPAbstractPlottable*> QCPAxis::plottables() const
{
    QList<QCPAbstractPlottable*> result;
    if (!mParentPlot) return result;

    for (int i=0; i<mParentPlot->mPlottables.size(); ++i)
    {
        if (mParentPlot->mPlottables.at(i)->keyAxis() == this ||mParentPlot->mPlottables.at(i)->valueAxis() == this)
            result.append(mParentPlot->mPlottables.at(i));
    }
    return result;
}

/*!
  Returns a list of all the graphs that have this axis as key or value axis.
  
  \see plottables, items
*/
//QList<QCPGraph*> QCPAxis::graphs() const
//{
//    QList<QCPGraph*> result;
//    if (!mParentPlot) return result;

//    for (int i=0; i<mParentPlot->mGraphs.size(); ++i)
//    {
//        if (mParentPlot->mGraphs.at(i)->keyAxis() == this || mParentPlot->mGraphs.at(i)->valueAxis() == this)
//            result.append(mParentPlot->mGraphs.at(i));
//    }
//    return result;
//}

/*!
  Returns a list of all the items that are associated with this axis. An item is considered
  associated with an axis if at least one of its positions uses the axis as key or value axis.
  
  \see plottables, graphs
*/
QList<QCPAbstractItem*> QCPAxis::items() const
{
    QList<QCPAbstractItem*> result;
    if (!mParentPlot) return result;

    for (int itemId=0; itemId<mParentPlot->mItems.size(); ++itemId)
    {
        QList<QCPItemPosition*> positions = mParentPlot->mItems.at(itemId)->positions();
        for (int posId=0; posId<positions.size(); ++posId)
        {
            if (positions.at(posId)->keyAxis() == this || positions.at(posId)->valueAxis() == this)
            {
                result.append(mParentPlot->mItems.at(itemId));
                break;
            }
        }
    }
    return result;
}

/*!
  Transforms a margin side to the logically corresponding axis type. (QCP::msLeft to
  QCPAxis::atLeft, QCP::msRight to QCPAxis::atRight, etc.)
*/
QCPAxis::AxisType QCPAxis::marginSideToAxisType(QCP::MarginSide side)
{
    switch (side)
    {
    case QCP::msLeft: return atLeft;
    case QCP::msRight: return atRight;
    case QCP::msTop: return atTop;
    case QCP::msBottom: return atBottom;
    default: break;
    }
    qDebug() << Q_FUNC_INFO << "Invalid margin side passed:" << (int)side;
    return atLeft;
}

/*!
  Returns the axis type that describes the opposite axis of an axis with the specified \a type.
*/
QCPAxis::AxisType QCPAxis::opposite(QCPAxis::AxisType type)
{
    switch (type)
    {
    case atLeft: return atRight; break;
    case atRight: return atLeft; break;
    case atBottom: return atTop; break;
    case atTop: return atBottom; break;
    default: qDebug() << Q_FUNC_INFO << "invalid axis type"; return atLeft; break;
    }
}

/* inherits documentation from base class */
void QCPAxis::selectEvent(QMouseEvent *event, bool additive, const QVariant &details, bool *selectionStateChanged)
{
    Q_UNUSED(event)
    SelectablePart part = details.value<SelectablePart>();
    if (mSelectableParts.testFlag(part))
    {
        SelectableParts selBefore = mSelectedParts;
        setSelectedParts(additive ? mSelectedParts^part : part);
        if (selectionStateChanged)
            *selectionStateChanged = mSelectedParts != selBefore;
    }
}

/* inherits documentation from base class */
void QCPAxis::deselectEvent(bool *selectionStateChanged)
{
    SelectableParts selBefore = mSelectedParts;
    setSelectedParts(mSelectedParts & ~mSelectableParts);
    if (selectionStateChanged)
        *selectionStateChanged = mSelectedParts != selBefore;
}

/*! \internal
  
  This mouse event reimplementation provides the functionality to let the user drag individual axes
  exclusively, by startig the drag on top of the axis.

  For the axis to accept this event and perform the single axis drag, the parent \ref QCPAxisRect
  must be configured accordingly, i.e. it must allow range dragging in the orientation of this axis
  (\ref QCPAxisRect::setRangeDrag) and this axis must be a draggable axis (\ref
  QCPAxisRect::setRangeDragAxes)
  
  \seebaseclassmethod
  
  \note The dragging of possibly multiple axes at once by starting the drag anywhere in the axis
  rect is handled by the axis rect's mouse event, e.g. \ref QCPAxisRect::mousePressEvent.
*/
void QCPAxis::mousePressEvent(QMouseEvent *event, const QVariant &details)
{
    Q_UNUSED(details)
    if (!mParentPlot->interactions().testFlag(QCP::iRangeDrag) ||
            !mAxisRect->rangeDrag().testFlag(orientation()) ||
            !mAxisRect->rangeDragAxes(orientation()).contains(this))
    {
        event->ignore();
        return;
    }

    if (event->buttons() & Qt::LeftButton)
    {
        mDragging = true;
        // initialize antialiasing backup in case we start dragging:
        if (mParentPlot->noAntialiasingOnDrag())
        {
            mAADragBackup = mParentPlot->antialiasedElements();
            mNotAADragBackup = mParentPlot->notAntialiasedElements();
        }
        // Mouse range dragging interaction:
        if (mParentPlot->interactions().testFlag(QCP::iRangeDrag))
            mDragStartRange = mRange;
    }
}

/*! \internal
  
  This mouse event reimplementation provides the functionality to let the user drag individual axes
  exclusively, by startig the drag on top of the axis.
  
  \seebaseclassmethod
  
  \note The dragging of possibly multiple axes at once by starting the drag anywhere in the axis
  rect is handled by the axis rect's mouse event, e.g. \ref QCPAxisRect::mousePressEvent.
  
  \see QCPAxis::mousePressEvent
*/
void QCPAxis::mouseMoveEvent(QMouseEvent *event, const QPointF &startPos)
{
    if (mDragging)
    {
        const double startPixel = orientation() == Qt::Horizontal ? startPos.x() : startPos.y();
        const double currentPixel = orientation() == Qt::Horizontal ? event->pos().x() : event->pos().y();
        if (mScaleType == QCPAxis::stLinear)
        {
            const double diff = pixelToCoord(startPixel) - pixelToCoord(currentPixel);
            setRange(mDragStartRange.lower+diff, mDragStartRange.upper+diff);
        } else if (mScaleType == QCPAxis::stLogarithmic)
        {
            const double diff = pixelToCoord(startPixel) / pixelToCoord(currentPixel);
            setRange(mDragStartRange.lower*diff, mDragStartRange.upper*diff);
        }

        if (mParentPlot->noAntialiasingOnDrag())
            mParentPlot->setNotAntialiasedElements(QCP::aeAll);
        mParentPlot->replot(PF_GraphicView::rpQueuedReplot);
    }
}

/*! \internal
  
  This mouse event reimplementation provides the functionality to let the user drag individual axes
  exclusively, by startig the drag on top of the axis.
  
  \seebaseclassmethod
  
  \note The dragging of possibly multiple axes at once by starting the drag anywhere in the axis
  rect is handled by the axis rect's mouse event, e.g. \ref QCPAxisRect::mousePressEvent.
  
  \see QCPAxis::mousePressEvent
*/
void QCPAxis::mouseReleaseEvent(QMouseEvent *event, const QPointF &startPos)
{
    Q_UNUSED(event)
    Q_UNUSED(startPos)
    mDragging = false;
    if (mParentPlot->noAntialiasingOnDrag())
    {
        mParentPlot->setAntialiasedElements(mAADragBackup);
        mParentPlot->setNotAntialiasedElements(mNotAADragBackup);
    }
}

/*! \internal
  
  This mouse event reimplementation provides the functionality to let the user zoom individual axes
  exclusively, by performing the wheel event on top of the axis.

  For the axis to accept this event and perform the single axis zoom, the parent \ref QCPAxisRect
  must be configured accordingly, i.e. it must allow range zooming in the orientation of this axis
  (\ref QCPAxisRect::setRangeZoom) and this axis must be a zoomable axis (\ref
  QCPAxisRect::setRangeZoomAxes)
  
  \seebaseclassmethod
  
  \note The zooming of possibly multiple axes at once by performing the wheel event anywhere in the
  axis rect is handled by the axis rect's mouse event, e.g. \ref QCPAxisRect::wheelEvent.
*/
void QCPAxis::wheelEvent(QWheelEvent *event)
{
    // Mouse range zooming interaction:
    if (!mParentPlot->interactions().testFlag(QCP::iRangeZoom) ||
            !mAxisRect->rangeZoom().testFlag(orientation()) ||
            !mAxisRect->rangeZoomAxes(orientation()).contains(this))
    {
        event->ignore();
        return;
    }

    const double wheelSteps = event->delta()/120.0; // a single step delta is +/-120 usually
    const double factor = qPow(mAxisRect->rangeZoomFactor(orientation()), wheelSteps);
    scaleRange(factor, pixelToCoord(orientation() == Qt::Horizontal ? event->pos().x() : event->pos().y()));
    /**设置同步轴的新刻度**/
    mParentPlot->replot();
}

/*! \internal

  A convenience function to easily set the QPainter::Antialiased hint on the provided \a painter
  before drawing axis lines.

  This is the antialiasing state the painter passed to the \ref draw method is in by default.
  
  This function takes into account the local setting of the antialiasing flag as well as the
  overrides set with \ref PF_GraphicView::setAntialiasedElements and \ref
  PF_GraphicView::setNotAntialiasedElements.
  
  \seebaseclassmethod
  
  \see setAntialiased
*/
void QCPAxis::applyDefaultAntialiasingHint(QCPPainter *painter) const
{
    applyAntialiasingHint(painter, mAntialiased, QCP::aeAxes);
}

/*! \internal
  
  Draws the axis with the specified \a painter, using the internal QCPAxisPainterPrivate instance.

  \seebaseclassmethod
*/
void QCPAxis::draw(QCPPainter *painter)
{
    QVector<double> subTickPositions; // the final coordToPixel transformed vector passed to QCPAxisPainter
    QVector<double> tickPositions; // the final coordToPixel transformed vector passed to QCPAxisPainter
    QVector<QString> tickLabels; // the final vector passed to QCPAxisPainter
    tickPositions.reserve(mTickVector.size());
    tickLabels.reserve(mTickVector.size());
    subTickPositions.reserve(mSubTickVector.size());

    if (mTicks)
    {
        for (int i=0; i<mTickVector.size(); ++i)
        {
            tickPositions.append(coordToPixel(mTickVector.at(i)));
            if (mTickLabels)
                tickLabels.append(mTickVectorLabels.at(i));
        }

        if (mSubTicks)
        {
            const int subTickCount = mSubTickVector.size();
            for (int i=0; i<subTickCount; ++i)
                subTickPositions.append(coordToPixel(mSubTickVector.at(i)));
        }
    }

    // transfer all properties of this axis to QCPAxisPainterPrivate which it needs to draw the axis.
    // Note that some axis painter properties are already set by direct feed-through with QCPAxis setters
    mAxisPainter->type = mAxisType;
    mAxisPainter->basePen = getBasePen();
    mAxisPainter->labelFont = getLabelFont();
    mAxisPainter->labelColor = getLabelColor();
    mAxisPainter->label = mLabel;
    mAxisPainter->substituteExponent = mNumberBeautifulPowers;
    mAxisPainter->tickPen = getTickPen();
    mAxisPainter->subTickPen = getSubTickPen();
    mAxisPainter->tickLabelFont = getTickLabelFont();
    mAxisPainter->tickLabelColor = getTickLabelColor();
    mAxisPainter->axisRect = mAxisRect->rect();
    mAxisPainter->viewportRect = mParentPlot->viewport();
    mAxisPainter->abbreviateDecimalPowers = mScaleType == stLogarithmic;
    mAxisPainter->reversedEndings = mRangeReversed;
    mAxisPainter->tickPositions = tickPositions;
    mAxisPainter->tickLabels = tickLabels;
    mAxisPainter->subTickPositions = subTickPositions;
    mAxisPainter->draw(painter);
}

/*! \internal
  
  Prepares the internal tick vector, sub tick vector and tick label vector. This is done by calling
  QCPAxisTicker::generate on the currently installed ticker.
  
  If a change in the label text/count is detected, the cached axis margin is invalidated to make
  sure the next margin calculation recalculates the label sizes and returns an up-to-date value.
*/
void QCPAxis::setupTickVectors()
{
    if (!mParentPlot) return;
    if ((!mTicks && !mTickLabels && !mGrid->visible()) || mRange.size() <= 0) return;

    QVector<QString> oldLabels = mTickVectorLabels;
    mTicker->generate(mRange, mParentPlot->locale(), mNumberFormatChar, mNumberPrecision, mTickVector, mSubTicks ? &mSubTickVector : nullptr, mTickLabels ? &mTickVectorLabels : nullptr);
    mCachedMarginValid &= mTickVectorLabels == oldLabels; // if labels have changed, margin might have changed, too
}

/*! \internal
  
  Returns the pen that is used to draw the axis base line. Depending on the selection state, this
  is either mSelectedBasePen or mBasePen.
*/
QPen QCPAxis::getBasePen() const
{
    return mSelectedParts.testFlag(spAxis) ? mSelectedBasePen : mBasePen;
}

/*! \internal
  
  Returns the pen that is used to draw the (major) ticks. Depending on the selection state, this
  is either mSelectedTickPen or mTickPen.
*/
QPen QCPAxis::getTickPen() const
{
    return mSelectedParts.testFlag(spAxis) ? mSelectedTickPen : mTickPen;
}

/*! \internal
  
  Returns the pen that is used to draw the subticks. Depending on the selection state, this
  is either mSelectedSubTickPen or mSubTickPen.
*/
QPen QCPAxis::getSubTickPen() const
{
    return mSelectedParts.testFlag(spAxis) ? mSelectedSubTickPen : mSubTickPen;
}

/*! \internal
  
  Returns the font that is used to draw the tick labels. Depending on the selection state, this
  is either mSelectedTickLabelFont or mTickLabelFont.
*/
QFont QCPAxis::getTickLabelFont() const
{
    return mSelectedParts.testFlag(spTickLabels) ? mSelectedTickLabelFont : mTickLabelFont;
}

/*! \internal
  
  Returns the font that is used to draw the axis label. Depending on the selection state, this
  is either mSelectedLabelFont or mLabelFont.
*/
QFont QCPAxis::getLabelFont() const
{
    return mSelectedParts.testFlag(spAxisLabel) ? mSelectedLabelFont : mLabelFont;
}

/*! \internal
  
  Returns the color that is used to draw the tick labels. Depending on the selection state, this
  is either mSelectedTickLabelColor or mTickLabelColor.
*/
QColor QCPAxis::getTickLabelColor() const
{
    return mSelectedParts.testFlag(spTickLabels) ? mSelectedTickLabelColor : mTickLabelColor;
}

/*! \internal
  
  Returns the color that is used to draw the axis label. Depending on the selection state, this
  is either mSelectedLabelColor or mLabelColor.
*/
QColor QCPAxis::getLabelColor() const
{
    return mSelectedParts.testFlag(spAxisLabel) ? mSelectedLabelColor : mLabelColor;
}

/*! \internal
  
  Returns the appropriate outward margin for this axis. It is needed if \ref
  QCPAxisRect::setAutoMargins is set to true on the parent axis rect. An axis with axis type \ref
  atLeft will return an appropriate left margin, \ref atBottom will return an appropriate bottom
  margin and so forth. For the calculation, this function goes through similar steps as \ref draw,
  so changing one function likely requires the modification of the other one as well.
  
  The margin consists of the outward tick length, tick label padding, tick label size, label
  padding, label size, and padding.
  
  The margin is cached internally, so repeated calls while leaving the axis range, fonts, etc.
  unchanged are very fast.
*/
int QCPAxis::calculateMargin()
{
    if (!mVisible) // if not visible, directly return 0, don't cache 0 because we can't react to setVisible in QCPAxis
        return 0;

    if (mCachedMarginValid)
        return mCachedMargin;

    // run through similar steps as QCPAxis::draw, and calculate margin needed to fit axis and its labels
    int margin = 0;

    QVector<double> tickPositions; // the final coordToPixel transformed vector passed to QCPAxisPainter
    QVector<QString> tickLabels; // the final vector passed to QCPAxisPainter
    tickPositions.reserve(mTickVector.size());
    tickLabels.reserve(mTickVector.size());

    if (mTicks)
    {
        for (int i=0; i<mTickVector.size(); ++i)
        {
            tickPositions.append(coordToPixel(mTickVector.at(i)));
            if (mTickLabels)
                tickLabels.append(mTickVectorLabels.at(i));
        }
    }
    // transfer all properties of this axis to QCPAxisPainterPrivate which it needs to calculate the size.
    // Note that some axis painter properties are already set by direct feed-through with QCPAxis setters
    mAxisPainter->type = mAxisType;
    mAxisPainter->labelFont = getLabelFont();
    mAxisPainter->label = mLabel;
    mAxisPainter->tickLabelFont = mTickLabelFont;
    mAxisPainter->axisRect = mAxisRect->rect();
    mAxisPainter->viewportRect = mParentPlot->viewport();
    mAxisPainter->tickPositions = tickPositions;
    mAxisPainter->tickLabels = tickLabels;
    margin += mAxisPainter->size();
    margin += mPadding;

    mCachedMargin = margin;
    mCachedMarginValid = true;
    return margin;
}

/* inherits documentation from base class */
QCP::Interaction QCPAxis::selectionCategory() const
{
    return QCP::iSelectAxes;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPAxisPainterPrivate
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPAxisPainterPrivate

  \internal
  \brief (Private)
  
  This is a private class and not part of the public PF_GraphicView interface.
  
  It is used by QCPAxis to do the low-level drawing of axis backbone, tick marks, tick labels and
  axis label. It also buffers the labels to reduce replot times. The parameters are configured by
  directly accessing the public member variables.
*/

/*!
  Constructs a QCPAxisPainterPrivate instance. Make sure to not create a new instance on every
  redraw, to utilize the caching mechanisms.
*/
QCPAxisPainterPrivate::QCPAxisPainterPrivate(PF_GraphicView *parentPlot) :
    type(QCPAxis::atLeft),
    basePen(QPen(Qt::black, 0, Qt::SolidLine, Qt::SquareCap)),
    lowerEnding(QCPLineEnding::esNone),
    upperEnding(QCPLineEnding::esNone),
    labelPadding(0),
    tickLabelPadding(0),
    tickLabelRotation(0),
    tickLabelSide(QCPAxis::lsOutside),
    substituteExponent(true),
    numberMultiplyCross(false),
    tickLengthIn(5),
    tickLengthOut(0),
    subTickLengthIn(2),
    subTickLengthOut(0),
    tickPen(QPen(Qt::black, 0, Qt::SolidLine, Qt::SquareCap)),
    subTickPen(QPen(Qt::black, 0, Qt::SolidLine, Qt::SquareCap)),
    offset(0),
    abbreviateDecimalPowers(false),
    reversedEndings(false),
    mParentPlot(parentPlot),
    mLabelCache(16) // cache at most 16 (tick) labels
{
}

QCPAxisPainterPrivate::~QCPAxisPainterPrivate()
{
}

/*! \internal
  
  Draws the axis with the specified \a painter.
  
  The selection boxes (mAxisSelectionBox, mTickLabelsSelectionBox, mLabelSelectionBox) are set
  here, too.
*/
void QCPAxisPainterPrivate::draw(QCPPainter *painter)
{
    QByteArray newHash = generateLabelParameterHash();
    if (newHash != mLabelParameterHash)
    {
        mLabelCache.clear();
        mLabelParameterHash = newHash;
    }

    QPoint origin;
    switch (type)
    {
    case QCPAxis::atLeft:   origin = axisRect.bottomLeft() +QPoint(-offset, 0); break;
    case QCPAxis::atRight:  origin = axisRect.bottomRight()+QPoint(+offset, 0); break;
    case QCPAxis::atTop:    origin = axisRect.topLeft()    +QPoint(0, -offset); break;
    case QCPAxis::atBottom: origin = axisRect.bottomLeft() +QPoint(0, +offset); break;
    }

    double xCor = 0, yCor = 0; // paint system correction, for pixel exact matches (affects baselines and ticks of top/right axes)
    switch (type)
    {
    case QCPAxis::atTop: yCor = -1; break;
    case QCPAxis::atRight: xCor = 1; break;
    default: break;
    }
    int margin = 0;
    // draw baseline:
    QLineF baseLine;
    painter->setPen(basePen);
    if (QCPAxis::orientation(type) == Qt::Horizontal)
        baseLine.setPoints(origin+QPointF(xCor, yCor), origin+QPointF(axisRect.width()+xCor, yCor));
    else
        baseLine.setPoints(origin+QPointF(xCor, yCor), origin+QPointF(xCor, -axisRect.height()+yCor));
    if (reversedEndings)
        baseLine = QLineF(baseLine.p2(), baseLine.p1()); // won't make a difference for line itself, but for line endings later
    painter->drawLine(baseLine);

    // draw ticks:
    if (!tickPositions.isEmpty())
    {
        painter->setPen(tickPen);
        int tickDir = (type == QCPAxis::atBottom || type == QCPAxis::atRight) ? -1 : 1; // direction of ticks ("inward" is right for left axis and left for right axis)
        if (QCPAxis::orientation(type) == Qt::Horizontal)
        {
            for (int i=0; i<tickPositions.size(); ++i)
                painter->drawLine(QLineF(tickPositions.at(i)+xCor, origin.y()-tickLengthOut*tickDir+yCor, tickPositions.at(i)+xCor, origin.y()+tickLengthIn*tickDir+yCor));
        } else
        {
            for (int i=0; i<tickPositions.size(); ++i)
                painter->drawLine(QLineF(origin.x()-tickLengthOut*tickDir+xCor, tickPositions.at(i)+yCor, origin.x()+tickLengthIn*tickDir+xCor, tickPositions.at(i)+yCor));
        }
    }

    // draw subticks:
    if (!subTickPositions.isEmpty())
    {
        painter->setPen(subTickPen);
        // direction of ticks ("inward" is right for left axis and left for right axis)
        int tickDir = (type == QCPAxis::atBottom || type == QCPAxis::atRight) ? -1 : 1;
        if (QCPAxis::orientation(type) == Qt::Horizontal)
        {
            for (int i=0; i<subTickPositions.size(); ++i)
                painter->drawLine(QLineF(subTickPositions.at(i)+xCor, origin.y()-subTickLengthOut*tickDir+yCor, subTickPositions.at(i)+xCor, origin.y()+subTickLengthIn*tickDir+yCor));
        } else
        {
            for (int i=0; i<subTickPositions.size(); ++i)
                painter->drawLine(QLineF(origin.x()-subTickLengthOut*tickDir+xCor, subTickPositions.at(i)+yCor, origin.x()+subTickLengthIn*tickDir+xCor, subTickPositions.at(i)+yCor));
        }
    }
    margin += qMax(0, qMax(tickLengthOut, subTickLengthOut));

    // draw axis base endings:
    bool antialiasingBackup = painter->antialiasing();
    painter->setAntialiasing(true); // always want endings to be antialiased, even if base and ticks themselves aren't
    painter->setBrush(QBrush(basePen.color()));
    QCPVector2D baseLineVector(baseLine.dx(), baseLine.dy());
    if (lowerEnding.style() != QCPLineEnding::esNone)
        lowerEnding.draw(painter, QCPVector2D(baseLine.p1())-baseLineVector.normalized()*lowerEnding.realLength()*(lowerEnding.inverted()?-1:1), -baseLineVector);
    if (upperEnding.style() != QCPLineEnding::esNone)
        upperEnding.draw(painter, QCPVector2D(baseLine.p2())+baseLineVector.normalized()*upperEnding.realLength()*(upperEnding.inverted()?-1:1), baseLineVector);
    painter->setAntialiasing(antialiasingBackup);

    // tick labels:
    QRect oldClipRect;
    if (tickLabelSide == QCPAxis::lsInside) // if using inside labels, clip them to the axis rect
    {
        oldClipRect = painter->clipRegion().boundingRect();
        painter->setClipRect(axisRect);
    }
    QSize tickLabelsSize(0, 0); // size of largest tick label, for offset calculation of axis label
    if (!tickLabels.isEmpty())
    {
        if (tickLabelSide == QCPAxis::lsOutside)
            margin += tickLabelPadding;
        painter->setFont(tickLabelFont);
        painter->setPen(QPen(tickLabelColor));
        const int maxLabelIndex = qMin(tickPositions.size(), tickLabels.size());
        int distanceToAxis = margin;
        if (tickLabelSide == QCPAxis::lsInside)
            distanceToAxis = -(qMax(tickLengthIn, subTickLengthIn)+tickLabelPadding);
        for (int i=0; i<maxLabelIndex; ++i)
            placeTickLabel(painter, tickPositions.at(i), distanceToAxis, tickLabels.at(i), &tickLabelsSize);
        if (tickLabelSide == QCPAxis::lsOutside)
            margin += (QCPAxis::orientation(type) == Qt::Horizontal) ? tickLabelsSize.height() : tickLabelsSize.width();
    }
    if (tickLabelSide == QCPAxis::lsInside)
        painter->setClipRect(oldClipRect);

    // axis label:
    QRect labelBounds;
    if (!label.isEmpty())
    {
        margin += labelPadding;
        painter->setFont(labelFont);
        painter->setPen(QPen(labelColor));
        labelBounds = painter->fontMetrics().boundingRect(0, 0, 0, 0, Qt::TextDontClip, label);
        if (type == QCPAxis::atLeft)
        {
            QTransform oldTransform = painter->transform();
            painter->translate((origin.x()-margin-labelBounds.height()), origin.y());
            painter->rotate(-90);
            painter->drawText(0, 0, axisRect.height(), labelBounds.height(), Qt::TextDontClip | Qt::AlignCenter, label);
            painter->setTransform(oldTransform);
        }
        else if (type == QCPAxis::atRight)
        {
            QTransform oldTransform = painter->transform();
            painter->translate((origin.x()+margin+labelBounds.height()), origin.y()-axisRect.height());
            painter->rotate(90);
            painter->drawText(0, 0, axisRect.height(), labelBounds.height(), Qt::TextDontClip | Qt::AlignCenter, label);
            painter->setTransform(oldTransform);
        }
        else if (type == QCPAxis::atTop)
            painter->drawText(origin.x(), origin.y()-margin-labelBounds.height(), axisRect.width(), labelBounds.height(), Qt::TextDontClip | Qt::AlignCenter, label);
        else if (type == QCPAxis::atBottom)
            painter->drawText(origin.x(), origin.y()+margin, axisRect.width(), labelBounds.height(), Qt::TextDontClip | Qt::AlignCenter, label);
    }

    // set selection boxes:
    int selectionTolerance = 0;
    if (mParentPlot)
        selectionTolerance = mParentPlot->selectionTolerance();
    else
        qDebug() << Q_FUNC_INFO << "mParentPlot is null";
    int selAxisOutSize = qMax(qMax(tickLengthOut, subTickLengthOut), selectionTolerance);
    int selAxisInSize = selectionTolerance;
    int selTickLabelSize;
    int selTickLabelOffset;
    if (tickLabelSide == QCPAxis::lsOutside)
    {
        selTickLabelSize = (QCPAxis::orientation(type) == Qt::Horizontal ? tickLabelsSize.height() : tickLabelsSize.width());
        selTickLabelOffset = qMax(tickLengthOut, subTickLengthOut)+tickLabelPadding;
    } else
    {
        selTickLabelSize = -(QCPAxis::orientation(type) == Qt::Horizontal ? tickLabelsSize.height() : tickLabelsSize.width());
        selTickLabelOffset = -(qMax(tickLengthIn, subTickLengthIn)+tickLabelPadding);
    }
    int selLabelSize = labelBounds.height();
    int selLabelOffset = qMax(tickLengthOut, subTickLengthOut)+(!tickLabels.isEmpty() && tickLabelSide == QCPAxis::lsOutside ? tickLabelPadding+selTickLabelSize : 0)+labelPadding;
    if (type == QCPAxis::atLeft)
    {
        mAxisSelectionBox.setCoords(origin.x()-selAxisOutSize, axisRect.top(), origin.x()+selAxisInSize, axisRect.bottom());
        mTickLabelsSelectionBox.setCoords(origin.x()-selTickLabelOffset-selTickLabelSize, axisRect.top(), origin.x()-selTickLabelOffset, axisRect.bottom());
        mLabelSelectionBox.setCoords(origin.x()-selLabelOffset-selLabelSize, axisRect.top(), origin.x()-selLabelOffset, axisRect.bottom());
    } else if (type == QCPAxis::atRight)
    {
        mAxisSelectionBox.setCoords(origin.x()-selAxisInSize, axisRect.top(), origin.x()+selAxisOutSize, axisRect.bottom());
        mTickLabelsSelectionBox.setCoords(origin.x()+selTickLabelOffset+selTickLabelSize, axisRect.top(), origin.x()+selTickLabelOffset, axisRect.bottom());
        mLabelSelectionBox.setCoords(origin.x()+selLabelOffset+selLabelSize, axisRect.top(), origin.x()+selLabelOffset, axisRect.bottom());
    } else if (type == QCPAxis::atTop)
    {
        mAxisSelectionBox.setCoords(axisRect.left(), origin.y()-selAxisOutSize, axisRect.right(), origin.y()+selAxisInSize);
        mTickLabelsSelectionBox.setCoords(axisRect.left(), origin.y()-selTickLabelOffset-selTickLabelSize, axisRect.right(), origin.y()-selTickLabelOffset);
        mLabelSelectionBox.setCoords(axisRect.left(), origin.y()-selLabelOffset-selLabelSize, axisRect.right(), origin.y()-selLabelOffset);
    } else if (type == QCPAxis::atBottom)
    {
        mAxisSelectionBox.setCoords(axisRect.left(), origin.y()-selAxisInSize, axisRect.right(), origin.y()+selAxisOutSize);
        mTickLabelsSelectionBox.setCoords(axisRect.left(), origin.y()+selTickLabelOffset+selTickLabelSize, axisRect.right(), origin.y()+selTickLabelOffset);
        mLabelSelectionBox.setCoords(axisRect.left(), origin.y()+selLabelOffset+selLabelSize, axisRect.right(), origin.y()+selLabelOffset);
    }
    mAxisSelectionBox = mAxisSelectionBox.normalized();
    mTickLabelsSelectionBox = mTickLabelsSelectionBox.normalized();
    mLabelSelectionBox = mLabelSelectionBox.normalized();
    // draw hitboxes for debug purposes:
    //painter->setBrush(Qt::NoBrush);
    //painter->drawRects(QVector<QRect>() << mAxisSelectionBox << mTickLabelsSelectionBox << mLabelSelectionBox);
}

/*! \internal
  
  Returns the size ("margin" in QCPAxisRect context, so measured perpendicular to the axis backbone
  direction) needed to fit the axis.
*/
int QCPAxisPainterPrivate::size() const
{
    int result = 0;

    // get length of tick marks pointing outwards:
    if (!tickPositions.isEmpty())
        result += qMax(0, qMax(tickLengthOut, subTickLengthOut));

    // calculate size of tick labels:
    if (tickLabelSide == QCPAxis::lsOutside)
    {
        QSize tickLabelsSize(0, 0);
        if (!tickLabels.isEmpty())
        {
            for (int i=0; i<tickLabels.size(); ++i)
                getMaxTickLabelSize(tickLabelFont, tickLabels.at(i), &tickLabelsSize);
            result += QCPAxis::orientation(type) == Qt::Horizontal ? tickLabelsSize.height() : tickLabelsSize.width();
            result += tickLabelPadding;
        }
    }

    // calculate size of axis label (only height needed, because left/right labels are rotated by 90 degrees):
    if (!label.isEmpty())
    {
        QFontMetrics fontMetrics(labelFont);
        QRect bounds;
        bounds = fontMetrics.boundingRect(0, 0, 0, 0, Qt::TextDontClip | Qt::AlignHCenter | Qt::AlignVCenter, label);
        result += bounds.height() + labelPadding;
    }

    return result;
}

/*! \internal
  
  Clears the internal label cache. Upon the next \ref draw, all labels will be created new. This
  method is called automatically in \ref draw, if any parameters have changed that invalidate the
  cached labels, such as font, color, etc.
*/
void QCPAxisPainterPrivate::clearCache()
{
    mLabelCache.clear();
}

/*! \internal
  
  Returns a hash that allows uniquely identifying whether the label parameters have changed such
  that the cached labels must be refreshed (\ref clearCache). It is used in \ref draw. If the
  return value of this method hasn't changed since the last redraw, the respective label parameters
  haven't changed and cached labels may be used.
*/
QByteArray QCPAxisPainterPrivate::generateLabelParameterHash() const
{
    QByteArray result;
    result.append(QByteArray::number(mParentPlot->bufferDevicePixelRatio()));
    result.append(QByteArray::number(tickLabelRotation));
    result.append(QByteArray::number((int)tickLabelSide));
    result.append(QByteArray::number((int)substituteExponent));
    result.append(QByteArray::number((int)numberMultiplyCross));
    result.append(tickLabelColor.name().toLatin1()+QByteArray::number(tickLabelColor.alpha(), 16));
    result.append(tickLabelFont.toString().toLatin1());
    return result;
}

/*! \internal
  
  Draws a single tick label with the provided \a painter, utilizing the internal label cache to
  significantly speed up drawing of labels that were drawn in previous calls. The tick label is
  always bound to an axis, the distance to the axis is controllable via \a distanceToAxis in
  pixels. The pixel position in the axis direction is passed in the \a position parameter. Hence
  for the bottom axis, \a position would indicate the horizontal pixel position (not coordinate),
  at which the label should be drawn.
  
  In order to later draw the axis label in a place that doesn't overlap with the tick labels, the
  largest tick label size is needed. This is acquired by passing a \a tickLabelsSize to the \ref
  drawTickLabel calls during the process of drawing all tick labels of one axis. In every call, \a
  tickLabelsSize is expanded, if the drawn label exceeds the value \a tickLabelsSize currently
  holds.
  
  The label is drawn with the font and pen that are currently set on the \a painter. To draw
  superscripted powers, the font is temporarily made smaller by a fixed factor (see \ref
  getTickLabelData).
*/
void QCPAxisPainterPrivate::placeTickLabel(QCPPainter *painter, double position, int distanceToAxis, const QString &text, QSize *tickLabelsSize)
{
    // warning: if you change anything here, also adapt getMaxTickLabelSize() accordingly!
    if (text.isEmpty()) return;
    QSize finalSize;
    QPointF labelAnchor;
    switch (type)
    {
    case QCPAxis::atLeft:   labelAnchor = QPointF(axisRect.left()-distanceToAxis-offset, position); break;
    case QCPAxis::atRight:  labelAnchor = QPointF(axisRect.right()+distanceToAxis+offset, position); break;
    case QCPAxis::atTop:    labelAnchor = QPointF(position, axisRect.top()-distanceToAxis-offset); break;
    case QCPAxis::atBottom: labelAnchor = QPointF(position, axisRect.bottom()+distanceToAxis+offset); break;
    }
    if (mParentPlot->plottingHints().testFlag(QCP::phCacheLabels) && !painter->modes().testFlag(QCPPainter::pmNoCaching)) // label caching enabled
    {
        CachedLabel *cachedLabel = mLabelCache.take(text); // attempt to get label from cache
        if (!cachedLabel)  // no cached label existed, create it
        {
            cachedLabel = new CachedLabel;
            TickLabelData labelData = getTickLabelData(painter->font(), text);
            cachedLabel->offset = getTickLabelDrawOffset(labelData)+labelData.rotatedTotalBounds.topLeft();
            if (!qFuzzyCompare(1.0, mParentPlot->bufferDevicePixelRatio()))
            {
                cachedLabel->pixmap = QPixmap(labelData.rotatedTotalBounds.size()*mParentPlot->bufferDevicePixelRatio());
#ifdef QCP_DEVICEPIXELRATIO_SUPPORTED
#  ifdef QCP_DEVICEPIXELRATIO_FLOAT
                cachedLabel->pixmap.setDevicePixelRatio(mParentPlot->devicePixelRatioF());
#  else
                cachedLabel->pixmap.setDevicePixelRatio(mParentPlot->devicePixelRatio());
#  endif
#endif
            } else
                cachedLabel->pixmap = QPixmap(labelData.rotatedTotalBounds.size());
            cachedLabel->pixmap.fill(Qt::transparent);
            QCPPainter cachePainter(&cachedLabel->pixmap);
            cachePainter.setPen(painter->pen());
            drawTickLabel(&cachePainter, -labelData.rotatedTotalBounds.topLeft().x(), -labelData.rotatedTotalBounds.topLeft().y(), labelData);
        }
        // if label would be partly clipped by widget border on sides, don't draw it (only for outside tick labels):
        bool labelClippedByBorder = false;
        if (tickLabelSide == QCPAxis::lsOutside)
        {
            if (QCPAxis::orientation(type) == Qt::Horizontal)
                labelClippedByBorder = labelAnchor.x()+cachedLabel->offset.x()+cachedLabel->pixmap.width()/mParentPlot->bufferDevicePixelRatio() > viewportRect.right() || labelAnchor.x()+cachedLabel->offset.x() < viewportRect.left();
            else
                labelClippedByBorder = labelAnchor.y()+cachedLabel->offset.y()+cachedLabel->pixmap.height()/mParentPlot->bufferDevicePixelRatio() > viewportRect.bottom() || labelAnchor.y()+cachedLabel->offset.y() < viewportRect.top();
        }
        if (!labelClippedByBorder)
        {
            painter->drawPixmap(labelAnchor+cachedLabel->offset, cachedLabel->pixmap);
            finalSize = cachedLabel->pixmap.size()/mParentPlot->bufferDevicePixelRatio();
        }
        mLabelCache.insert(text, cachedLabel); // return label to cache or insert for the first time if newly created
    } else // label caching disabled, draw text directly on surface:
    {
        TickLabelData labelData = getTickLabelData(painter->font(), text);
        QPointF finalPosition = labelAnchor + getTickLabelDrawOffset(labelData);
        // if label would be partly clipped by widget border on sides, don't draw it (only for outside tick labels):
        bool labelClippedByBorder = false;
        if (tickLabelSide == QCPAxis::lsOutside)
        {
            if (QCPAxis::orientation(type) == Qt::Horizontal)
                labelClippedByBorder = finalPosition.x()+(labelData.rotatedTotalBounds.width()+labelData.rotatedTotalBounds.left()) > viewportRect.right() || finalPosition.x()+labelData.rotatedTotalBounds.left() < viewportRect.left();
            else
                labelClippedByBorder = finalPosition.y()+(labelData.rotatedTotalBounds.height()+labelData.rotatedTotalBounds.top()) > viewportRect.bottom() || finalPosition.y()+labelData.rotatedTotalBounds.top() < viewportRect.top();
        }
        if (!labelClippedByBorder)
        {
            drawTickLabel(painter, finalPosition.x(), finalPosition.y(), labelData);
            finalSize = labelData.rotatedTotalBounds.size();
        }
    }

    // expand passed tickLabelsSize if current tick label is larger:
    if (finalSize.width() > tickLabelsSize->width())
        tickLabelsSize->setWidth(finalSize.width());
    if (finalSize.height() > tickLabelsSize->height())
        tickLabelsSize->setHeight(finalSize.height());
}

/*! \internal
  
  This is a \ref placeTickLabel helper function.
  
  Draws the tick label specified in \a labelData with \a painter at the pixel positions \a x and \a
  y. This function is used by \ref placeTickLabel to create new tick labels for the cache, or to
  directly draw the labels on the PF_GraphicView surface when label caching is disabled, i.e. when
  QCP::phCacheLabels plotting hint is not set.
*/
void QCPAxisPainterPrivate::drawTickLabel(QCPPainter *painter, double x, double y, const TickLabelData &labelData) const
{
    // backup painter settings that we're about to change:
    QTransform oldTransform = painter->transform();
    QFont oldFont = painter->font();

    // transform painter to position/rotation:
    painter->translate(x, y);
    if (!qFuzzyIsNull(tickLabelRotation))
        painter->rotate(tickLabelRotation);

    // draw text:
    if (!labelData.expPart.isEmpty()) // indicator that beautiful powers must be used
    {
        painter->setFont(labelData.baseFont);
        painter->drawText(0, 0, 0, 0, Qt::TextDontClip, labelData.basePart);
        if (!labelData.suffixPart.isEmpty())
            painter->drawText(labelData.baseBounds.width()+1+labelData.expBounds.width(), 0, 0, 0, Qt::TextDontClip, labelData.suffixPart);
        painter->setFont(labelData.expFont);
        painter->drawText(labelData.baseBounds.width()+1, 0, labelData.expBounds.width(), labelData.expBounds.height(), Qt::TextDontClip,  labelData.expPart);
    } else
    {
        painter->setFont(labelData.baseFont);
        painter->drawText(0, 0, labelData.totalBounds.width(), labelData.totalBounds.height(), Qt::TextDontClip | Qt::AlignHCenter, labelData.basePart);
    }

    // reset painter settings to what it was before:
    painter->setTransform(oldTransform);
    painter->setFont(oldFont);
}

/*! \internal
  
  This is a \ref placeTickLabel helper function.
  
  Transforms the passed \a text and \a font to a tickLabelData structure that can then be further
  processed by \ref getTickLabelDrawOffset and \ref drawTickLabel. It splits the text into base and
  exponent if necessary (member substituteExponent) and calculates appropriate bounding boxes.
*/
QCPAxisPainterPrivate::TickLabelData QCPAxisPainterPrivate::getTickLabelData(const QFont &font, const QString &text) const
{
    TickLabelData result;

    // determine whether beautiful decimal powers should be used
    bool useBeautifulPowers = false;
    int ePos = -1; // first index of exponent part, text before that will be basePart, text until eLast will be expPart
    int eLast = -1; // last index of exponent part, rest of text after this will be suffixPart
    if (substituteExponent)
    {
        ePos = text.indexOf(QLatin1Char('e'));
        if (ePos > 0 && text.at(ePos-1).isDigit())
        {
            eLast = ePos;
            while (eLast+1 < text.size() && (text.at(eLast+1) == QLatin1Char('+') || text.at(eLast+1) == QLatin1Char('-') || text.at(eLast+1).isDigit()))
                ++eLast;
            if (eLast > ePos) // only if also to right of 'e' is a digit/+/- interpret it as beautifiable power
                useBeautifulPowers = true;
        }
    }

    // calculate text bounding rects and do string preparation for beautiful decimal powers:
    result.baseFont = font;
    if (result.baseFont.pointSizeF() > 0) // might return -1 if specified with setPixelSize, in that case we can't do correction in next line
        result.baseFont.setPointSizeF(result.baseFont.pointSizeF()+0.05); // QFontMetrics.boundingRect has a bug for exact point sizes that make the results oscillate due to internal rounding
    if (useBeautifulPowers)
    {
        // split text into parts of number/symbol that will be drawn normally and part that will be drawn as exponent:
        result.basePart = text.left(ePos);
        result.suffixPart = text.mid(eLast+1); // also drawn normally but after exponent
        // in log scaling, we want to turn "1*10^n" into "10^n", else add multiplication sign and decimal base:
        if (abbreviateDecimalPowers && result.basePart == QLatin1String("1"))
            result.basePart = QLatin1String("10");
        else
            result.basePart += (numberMultiplyCross ? QString(QChar(215)) : QString(QChar(183))) + QLatin1String("10");
        result.expPart = text.mid(ePos+1, eLast-ePos);
        // clip "+" and leading zeros off expPart:
        while (result.expPart.length() > 2 && result.expPart.at(1) == QLatin1Char('0')) // length > 2 so we leave one zero when numberFormatChar is 'e'
            result.expPart.remove(1, 1);
        if (!result.expPart.isEmpty() && result.expPart.at(0) == QLatin1Char('+'))
            result.expPart.remove(0, 1);
        // prepare smaller font for exponent:
        result.expFont = font;
        if (result.expFont.pointSize() > 0)
            result.expFont.setPointSize(result.expFont.pointSize()*0.75);
        else
            result.expFont.setPixelSize(result.expFont.pixelSize()*0.75);
        // calculate bounding rects of base part(s), exponent part and total one:
        result.baseBounds = QFontMetrics(result.baseFont).boundingRect(0, 0, 0, 0, Qt::TextDontClip, result.basePart);
        result.expBounds = QFontMetrics(result.expFont).boundingRect(0, 0, 0, 0, Qt::TextDontClip, result.expPart);
        if (!result.suffixPart.isEmpty())
            result.suffixBounds = QFontMetrics(result.baseFont).boundingRect(0, 0, 0, 0, Qt::TextDontClip, result.suffixPart);
        result.totalBounds = result.baseBounds.adjusted(0, 0, result.expBounds.width()+result.suffixBounds.width()+2, 0); // +2 consists of the 1 pixel spacing between base and exponent (see drawTickLabel) and an extra pixel to include AA
    } else // useBeautifulPowers == false
    {
        result.basePart = text;
        result.totalBounds = QFontMetrics(result.baseFont).boundingRect(0, 0, 0, 0, Qt::TextDontClip | Qt::AlignHCenter, result.basePart);
    }
    result.totalBounds.moveTopLeft(QPoint(0, 0)); // want bounding box aligned top left at origin, independent of how it was created, to make further processing simpler

    // calculate possibly different bounding rect after rotation:
    result.rotatedTotalBounds = result.totalBounds;
    if (!qFuzzyIsNull(tickLabelRotation))
    {
        QTransform transform;
        transform.rotate(tickLabelRotation);
        result.rotatedTotalBounds = transform.mapRect(result.rotatedTotalBounds);
    }

    return result;
}

/*! \internal
  
  This is a \ref placeTickLabel helper function.
  
  Calculates the offset at which the top left corner of the specified tick label shall be drawn.
  The offset is relative to a point right next to the tick the label belongs to.
  
  This function is thus responsible for e.g. centering tick labels under ticks and positioning them
  appropriately when they are rotated.
*/
QPointF QCPAxisPainterPrivate::getTickLabelDrawOffset(const TickLabelData &labelData) const
{
    /*
    calculate label offset from base point at tick (non-trivial, for best visual appearance): short
    explanation for bottom axis: The anchor, i.e. the point in the label that is placed
    horizontally under the corresponding tick is always on the label side that is closer to the
    axis (e.g. the left side of the text when we're rotating clockwise). On that side, the height
    is halved and the resulting point is defined the anchor. This way, a 90 degree rotated text
    will be centered under the tick (i.e. displaced horizontally by half its height). At the same
    time, a 45 degree rotated text will "point toward" its tick, as is typical for rotated tick
    labels.
  */
    bool doRotation = !qFuzzyIsNull(tickLabelRotation);
    bool flip = qFuzzyCompare(qAbs(tickLabelRotation), 90.0); // perfect +/-90 degree flip. Indicates vertical label centering on vertical axes.
    double radians = tickLabelRotation/180.0*M_PI;
    int x=0, y=0;
    if ((type == QCPAxis::atLeft && tickLabelSide == QCPAxis::lsOutside) || (type == QCPAxis::atRight && tickLabelSide == QCPAxis::lsInside)) // Anchor at right side of tick label
    {
        if (doRotation)
        {
            if (tickLabelRotation > 0)
            {
                x = -qCos(radians)*labelData.totalBounds.width();
                y = flip ? -labelData.totalBounds.width()/2.0 : -qSin(radians)*labelData.totalBounds.width()-qCos(radians)*labelData.totalBounds.height()/2.0;
            } else
            {
                x = -qCos(-radians)*labelData.totalBounds.width()-qSin(-radians)*labelData.totalBounds.height();
                y = flip ? +labelData.totalBounds.width()/2.0 : +qSin(-radians)*labelData.totalBounds.width()-qCos(-radians)*labelData.totalBounds.height()/2.0;
            }
        } else
        {
            x = -labelData.totalBounds.width();
            y = -labelData.totalBounds.height()/2.0;
        }
    } else if ((type == QCPAxis::atRight && tickLabelSide == QCPAxis::lsOutside) || (type == QCPAxis::atLeft && tickLabelSide == QCPAxis::lsInside)) // Anchor at left side of tick label
    {
        if (doRotation)
        {
            if (tickLabelRotation > 0)
            {
                x = +qSin(radians)*labelData.totalBounds.height();
                y = flip ? -labelData.totalBounds.width()/2.0 : -qCos(radians)*labelData.totalBounds.height()/2.0;
            } else
            {
                x = 0;
                y = flip ? +labelData.totalBounds.width()/2.0 : -qCos(-radians)*labelData.totalBounds.height()/2.0;
            }
        } else
        {
            x = 0;
            y = -labelData.totalBounds.height()/2.0;
        }
    } else if ((type == QCPAxis::atTop && tickLabelSide == QCPAxis::lsOutside) || (type == QCPAxis::atBottom && tickLabelSide == QCPAxis::lsInside)) // Anchor at bottom side of tick label
    {
        if (doRotation)
        {
            if (tickLabelRotation > 0)
            {
                x = -qCos(radians)*labelData.totalBounds.width()+qSin(radians)*labelData.totalBounds.height()/2.0;
                y = -qSin(radians)*labelData.totalBounds.width()-qCos(radians)*labelData.totalBounds.height();
            } else
            {
                x = -qSin(-radians)*labelData.totalBounds.height()/2.0;
                y = -qCos(-radians)*labelData.totalBounds.height();
            }
        } else
        {
            x = -labelData.totalBounds.width()/2.0;
            y = -labelData.totalBounds.height();
        }
    } else if ((type == QCPAxis::atBottom && tickLabelSide == QCPAxis::lsOutside) || (type == QCPAxis::atTop && tickLabelSide == QCPAxis::lsInside)) // Anchor at top side of tick label
    {
        if (doRotation)
        {
            if (tickLabelRotation > 0)
            {
                x = +qSin(radians)*labelData.totalBounds.height()/2.0;
                y = 0;
            } else
            {
                x = -qCos(-radians)*labelData.totalBounds.width()-qSin(-radians)*labelData.totalBounds.height()/2.0;
                y = +qSin(-radians)*labelData.totalBounds.width();
            }
        } else
        {
            x = -labelData.totalBounds.width()/2.0;
            y = 0;
        }
    }

    return QPointF(x, y);
}

/*! \internal
  
  Simulates the steps done by \ref placeTickLabel by calculating bounding boxes of the text label
  to be drawn, depending on number format etc. Since only the largest tick label is wanted for the
  margin calculation, the passed \a tickLabelsSize is only expanded, if it's currently set to a
  smaller width/height.
*/
void QCPAxisPainterPrivate::getMaxTickLabelSize(const QFont &font, const QString &text,  QSize *tickLabelsSize) const
{
    // note: this function must return the same tick label sizes as the placeTickLabel function.
    QSize finalSize;
    if (mParentPlot->plottingHints().testFlag(QCP::phCacheLabels) && mLabelCache.contains(text)) // label caching enabled and have cached label
    {
        const CachedLabel *cachedLabel = mLabelCache.object(text);
        finalSize = cachedLabel->pixmap.size()/mParentPlot->bufferDevicePixelRatio();
    } else // label caching disabled or no label with this text cached:
    {
        TickLabelData labelData = getTickLabelData(font, text);
        finalSize = labelData.rotatedTotalBounds.size();
    }

    // expand passed tickLabelsSize if current tick label is larger:
    if (finalSize.width() > tickLabelsSize->width())
        tickLabelsSize->setWidth(finalSize.width());
    if (finalSize.height() > tickLabelsSize->height())
        tickLabelsSize->setHeight(finalSize.height());
}
/* end of 'src/axis/axis.cpp' */


/* including file 'src/scatterstyle.cpp', size 17450                         */
/* commit ce344b3f96a62e5f652585e55f1ae7c7883cd45b 2018-06-25 01:03:39 +0200 */

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPScatterStyle
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPScatterStyle
  \brief Represents the visual appearance of scatter points
  
  This class holds information about shape, color and size of scatter points. In plottables like
  QCPGraph it is used to store how scatter points shall be drawn. For example, \ref
  QCPGraph::setScatterStyle takes a QCPScatterStyle instance.
  
  A scatter style consists of a shape (\ref setShape), a line color (\ref setPen) and possibly a
  fill (\ref setBrush), if the shape provides a fillable area. Further, the size of the shape can
  be controlled with \ref setSize.

  \section QCPScatterStyle-defining Specifying a scatter style
  
  You can set all these configurations either by calling the respective functions on an instance:
  \snippet documentation/doc-code-snippets/mainwindow.cpp qcpscatterstyle-creation-1
  
  Or you can use one of the various constructors that take different parameter combinations, making
  it easy to specify a scatter style in a single call, like so:
  \snippet documentation/doc-code-snippets/mainwindow.cpp qcpscatterstyle-creation-2
  
  \section QCPScatterStyle-undefinedpen Leaving the color/pen up to the plottable
  
  There are two constructors which leave the pen undefined: \ref QCPScatterStyle() and \ref
  QCPScatterStyle(ScatterShape shape, double size). If those constructors are used, a call to \ref
  isPenDefined will return false. It leads to scatter points that inherit the pen from the
  plottable that uses the scatter style. Thus, if such a scatter style is passed to QCPGraph, the line
  color of the graph (\ref QCPGraph::setPen) will be used by the scatter points. This makes
  it very convenient to set up typical scatter settings:
  
  \snippet documentation/doc-code-snippets/mainwindow.cpp qcpscatterstyle-shortcreation

  Notice that it wasn't even necessary to explicitly call a QCPScatterStyle constructor. This works
  because QCPScatterStyle provides a constructor that can transform a \ref ScatterShape directly
  into a QCPScatterStyle instance (that's the \ref QCPScatterStyle(ScatterShape shape, double size)
  constructor with a default for \a size). In those cases, C++ allows directly supplying a \ref
  ScatterShape, where actually a QCPScatterStyle is expected.
  
  \section QCPScatterStyle-custompath-and-pixmap Custom shapes and pixmaps
  
  QCPScatterStyle supports drawing custom shapes and arbitrary pixmaps as scatter points.

  For custom shapes, you can provide a QPainterPath with the desired shape to the \ref
  setCustomPath function or call the constructor that takes a painter path. The scatter shape will
  automatically be set to \ref ssCustom.
  
  For pixmaps, you call \ref setPixmap with the desired QPixmap. Alternatively you can use the
  constructor that takes a QPixmap. The scatter shape will automatically be set to \ref ssPixmap.
  Note that \ref setSize does not influence the appearance of the pixmap.
*/

/* start documentation of inline functions */

/*! \fn bool QCPScatterStyle::isNone() const
  
  Returns whether the scatter shape is \ref ssNone.
  
  \see setShape
*/

/*! \fn bool QCPScatterStyle::isPenDefined() const
  
  Returns whether a pen has been defined for this scatter style.
  
  The pen is undefined if a constructor is called that does not carry \a pen as parameter. Those
  are \ref QCPScatterStyle() and \ref QCPScatterStyle(ScatterShape shape, double size). If the pen
  is undefined, the pen of the respective plottable will be used for drawing scatters.
  
  If a pen was defined for this scatter style instance, and you now wish to undefine the pen, call
  \ref undefinePen.
  
  \see setPen
*/

/* end documentation of inline functions */

/*!
  Creates a new QCPScatterStyle instance with size set to 6. No shape, pen or brush is defined.
  
  Since the pen is undefined (\ref isPenDefined returns false), the scatter color will be inherited
  from the plottable that uses this scatter style.
*/
QCPScatterStyle::QCPScatterStyle() :
    mSize(6),
    mShape(ssNone),
    mPen(Qt::NoPen),
    mBrush(Qt::NoBrush),
    mPenDefined(false)
{
}

/*!
  Creates a new QCPScatterStyle instance with shape set to \a shape and size to \a size. No pen or
  brush is defined.
  
  Since the pen is undefined (\ref isPenDefined returns false), the scatter color will be inherited
  from the plottable that uses this scatter style.
*/
QCPScatterStyle::QCPScatterStyle(ScatterShape shape, double size) :
    mSize(size),
    mShape(shape),
    mPen(Qt::NoPen),
    mBrush(Qt::NoBrush),
    mPenDefined(false)
{
}

/*!
  Creates a new QCPScatterStyle instance with shape set to \a shape, the pen color set to \a color,
  and size to \a size. No brush is defined, i.e. the scatter point will not be filled.
*/
QCPScatterStyle::QCPScatterStyle(ScatterShape shape, const QColor &color, double size) :
    mSize(size),
    mShape(shape),
    mPen(QPen(color)),
    mBrush(Qt::NoBrush),
    mPenDefined(true)
{
}

/*!
  Creates a new QCPScatterStyle instance with shape set to \a shape, the pen color set to \a color,
  the brush color to \a fill (with a solid pattern), and size to \a size.
*/
QCPScatterStyle::QCPScatterStyle(ScatterShape shape, const QColor &color, const QColor &fill, double size) :
    mSize(size),
    mShape(shape),
    mPen(QPen(color)),
    mBrush(QBrush(fill)),
    mPenDefined(true)
{
}

/*!
  Creates a new QCPScatterStyle instance with shape set to \a shape, the pen set to \a pen, the
  brush to \a brush, and size to \a size.
  
  \warning In some cases it might be tempting to directly use a pen style like <tt>Qt::NoPen</tt> as \a pen
  and a color like <tt>Qt::blue</tt> as \a brush. Notice however, that the corresponding call\n
  <tt>QCPScatterStyle(QCPScatterShape::ssCircle, Qt::NoPen, Qt::blue, 5)</tt>\n
  doesn't necessarily lead C++ to use this constructor in some cases, but might mistake
  <tt>Qt::NoPen</tt> for a QColor and use the
  \ref QCPScatterStyle(ScatterShape shape, const QColor &color, const QColor &fill, double size)
  constructor instead (which will lead to an unexpected look of the scatter points). To prevent
  this, be more explicit with the parameter types. For example, use <tt>QBrush(Qt::blue)</tt>
  instead of just <tt>Qt::blue</tt>, to clearly point out to the compiler that this constructor is
  wanted.
*/
QCPScatterStyle::QCPScatterStyle(ScatterShape shape, const QPen &pen, const QBrush &brush, double size) :
    mSize(size),
    mShape(shape),
    mPen(pen),
    mBrush(brush),
    mPenDefined(pen.style() != Qt::NoPen)
{
}

/*!
  Creates a new QCPScatterStyle instance which will show the specified \a pixmap. The scatter shape
  is set to \ref ssPixmap.
*/
QCPScatterStyle::QCPScatterStyle(const QPixmap &pixmap) :
    mSize(5),
    mShape(ssPixmap),
    mPen(Qt::NoPen),
    mBrush(Qt::NoBrush),
    mPixmap(pixmap),
    mPenDefined(false)
{
}

/*!
  Creates a new QCPScatterStyle instance with a custom shape that is defined via \a customPath. The
  scatter shape is set to \ref ssCustom.
  
  The custom shape line will be drawn with \a pen and filled with \a brush. The size has a slightly
  different meaning than for built-in scatter points: The custom path will be drawn scaled by a
  factor of \a size/6.0. Since the default \a size is 6, the custom path will appear in its
  original size by default. To for example double the size of the path, set \a size to 12.
*/
QCPScatterStyle::QCPScatterStyle(const QPainterPath &customPath, const QPen &pen, const QBrush &brush, double size) :
    mSize(size),
    mShape(ssCustom),
    mPen(pen),
    mBrush(brush),
    mCustomPath(customPath),
    mPenDefined(pen.style() != Qt::NoPen)
{
}

/*!
  Copies the specified \a properties from the \a other scatter style to this scatter style.
*/
void QCPScatterStyle::setFromOther(const QCPScatterStyle &other, ScatterProperties properties)
{
    if (properties.testFlag(spPen))
    {
        setPen(other.pen());
        if (!other.isPenDefined())
            undefinePen();
    }
    if (properties.testFlag(spBrush))
        setBrush(other.brush());
    if (properties.testFlag(spSize))
        setSize(other.size());
    if (properties.testFlag(spShape))
    {
        setShape(other.shape());
        if (other.shape() == ssPixmap)
            setPixmap(other.pixmap());
        else if (other.shape() == ssCustom)
            setCustomPath(other.customPath());
    }
}

/*!
  Sets the size (pixel diameter) of the drawn scatter points to \a size.
  
  \see setShape
*/
void QCPScatterStyle::setSize(double size)
{
    mSize = size;
}

/*!
  Sets the shape to \a shape.
  
  Note that the calls \ref setPixmap and \ref setCustomPath automatically set the shape to \ref
  ssPixmap and \ref ssCustom, respectively.
  
  \see setSize
*/
void QCPScatterStyle::setShape(QCPScatterStyle::ScatterShape shape)
{
    mShape = shape;
}

/*!
  Sets the pen that will be used to draw scatter points to \a pen.
  
  If the pen was previously undefined (see \ref isPenDefined), the pen is considered defined after
  a call to this function, even if \a pen is <tt>Qt::NoPen</tt>. If you have defined a pen
  previously by calling this function and now wish to undefine the pen, call \ref undefinePen.
  
  \see setBrush
*/
void QCPScatterStyle::setPen(const QPen &pen)
{
    mPenDefined = true;
    mPen = pen;
}

/*!
  Sets the brush that will be used to fill scatter points to \a brush. Note that not all scatter
  shapes have fillable areas. For example, \ref ssPlus does not while \ref ssCircle does.
  
  \see setPen
*/
void QCPScatterStyle::setBrush(const QBrush &brush)
{
    mBrush = brush;
}

/*!
  Sets the pixmap that will be drawn as scatter point to \a pixmap.
  
  Note that \ref setSize does not influence the appearance of the pixmap.
  
  The scatter shape is automatically set to \ref ssPixmap.
*/
void QCPScatterStyle::setPixmap(const QPixmap &pixmap)
{
    setShape(ssPixmap);
    mPixmap = pixmap;
}

/*!
  Sets the custom shape that will be drawn as scatter point to \a customPath.
  
  The scatter shape is automatically set to \ref ssCustom.
*/
void QCPScatterStyle::setCustomPath(const QPainterPath &customPath)
{
    setShape(ssCustom);
    mCustomPath = customPath;
}

/*!
  Sets this scatter style to have an undefined pen (see \ref isPenDefined for what an undefined pen
  implies).

  A call to \ref setPen will define a pen.
*/
void QCPScatterStyle::undefinePen()
{
    mPenDefined = false;
}

/*!
  Applies the pen and the brush of this scatter style to \a painter. If this scatter style has an
  undefined pen (\ref isPenDefined), sets the pen of \a painter to \a defaultPen instead.
  
  This function is used by plottables (or any class that wants to draw scatters) just before a
  number of scatters with this style shall be drawn with the \a painter.
  
  \see drawShape
*/
void QCPScatterStyle::applyTo(QCPPainter *painter, const QPen &defaultPen) const
{
    painter->setPen(mPenDefined ? mPen : defaultPen);
    painter->setBrush(mBrush);
}

/*!
  Draws the scatter shape with \a painter at position \a pos.
  
  This function does not modify the pen or the brush on the painter, as \ref applyTo is meant to be
  called before scatter points are drawn with \ref drawShape.
  
  \see applyTo
*/
void QCPScatterStyle::drawShape(QCPPainter *painter, const QPointF &pos) const
{
    drawShape(painter, pos.x(), pos.y());
}

/*! \overload
  Draws the scatter shape with \a painter at position \a x and \a y.
*/
void QCPScatterStyle::drawShape(QCPPainter *painter, double x, double y) const
{
    double w = mSize/2.0;
    switch (mShape)
    {
    case ssNone: break;
    case ssDot:
    {
        painter->drawLine(QPointF(x, y), QPointF(x+0.0001, y));
        break;
    }
    case ssCross:
    {
        painter->drawLine(QLineF(x-w, y-w, x+w, y+w));
        painter->drawLine(QLineF(x-w, y+w, x+w, y-w));
        break;
    }
    case ssPlus:
    {
        painter->drawLine(QLineF(x-w,   y, x+w,   y));
        painter->drawLine(QLineF(  x, y+w,   x, y-w));
        break;
    }
    case ssCircle:
    {
        painter->drawEllipse(QPointF(x , y), w, w);
        break;
    }
    case ssDisc:
    {
        QBrush b = painter->brush();
        painter->setBrush(painter->pen().color());
        painter->drawEllipse(QPointF(x , y), w, w);
        painter->setBrush(b);
        break;
    }
    case ssSquare:
    {
        painter->drawRect(QRectF(x-w, y-w, mSize, mSize));
        break;
    }
    case ssDiamond:
    {
        QPointF lineArray[4] = {QPointF(x-w,   y),
                                QPointF(  x, y-w),
                                QPointF(x+w,   y),
                                QPointF(  x, y+w)};
        painter->drawPolygon(lineArray, 4);
        break;
    }
    case ssStar:
    {
        painter->drawLine(QLineF(x-w,   y, x+w,   y));
        painter->drawLine(QLineF(  x, y+w,   x, y-w));
        painter->drawLine(QLineF(x-w*0.707, y-w*0.707, x+w*0.707, y+w*0.707));
        painter->drawLine(QLineF(x-w*0.707, y+w*0.707, x+w*0.707, y-w*0.707));
        break;
    }
    case ssTriangle:
    {
        QPointF lineArray[3] = {QPointF(x-w, y+0.755*w),
                                QPointF(x+w, y+0.755*w),
                                QPointF(  x, y-0.977*w)};
        painter->drawPolygon(lineArray, 3);
        break;
    }
    case ssTriangleInverted:
    {
        QPointF lineArray[3] = {QPointF(x-w, y-0.755*w),
                                QPointF(x+w, y-0.755*w),
                                QPointF(  x, y+0.977*w)};
        painter->drawPolygon(lineArray, 3);
        break;
    }
    case ssCrossSquare:
    {
        painter->drawRect(QRectF(x-w, y-w, mSize, mSize));
        painter->drawLine(QLineF(x-w, y-w, x+w*0.95, y+w*0.95));
        painter->drawLine(QLineF(x-w, y+w*0.95, x+w*0.95, y-w));
        break;
    }
    case ssPlusSquare:
    {
        painter->drawRect(QRectF(x-w, y-w, mSize, mSize));
        painter->drawLine(QLineF(x-w,   y, x+w*0.95,   y));
        painter->drawLine(QLineF(  x, y+w,        x, y-w));
        break;
    }
    case ssCrossCircle:
    {
        painter->drawEllipse(QPointF(x, y), w, w);
        painter->drawLine(QLineF(x-w*0.707, y-w*0.707, x+w*0.670, y+w*0.670));
        painter->drawLine(QLineF(x-w*0.707, y+w*0.670, x+w*0.670, y-w*0.707));
        break;
    }
    case ssPlusCircle:
    {
        painter->drawEllipse(QPointF(x, y), w, w);
        painter->drawLine(QLineF(x-w,   y, x+w,   y));
        painter->drawLine(QLineF(  x, y+w,   x, y-w));
        break;
    }
    case ssPeace:
    {
        painter->drawEllipse(QPointF(x, y), w, w);
        painter->drawLine(QLineF(x, y-w,         x,       y+w));
        painter->drawLine(QLineF(x,   y, x-w*0.707, y+w*0.707));
        painter->drawLine(QLineF(x,   y, x+w*0.707, y+w*0.707));
        break;
    }
    case ssPixmap:
    {
        const double widthHalf = mPixmap.width()*0.5;
        const double heightHalf = mPixmap.height()*0.5;
#if QT_VERSION < QT_VERSION_CHECK(4, 8, 0)
        const QRectF clipRect = painter->clipRegion().boundingRect().adjusted(-widthHalf, -heightHalf, widthHalf, heightHalf);
#else
        const QRectF clipRect = painter->clipBoundingRect().adjusted(-widthHalf, -heightHalf, widthHalf, heightHalf);
#endif
        if (clipRect.contains(x, y))
            painter->drawPixmap(x-widthHalf, y-heightHalf, mPixmap);
        break;
    }
    case ssCustom:
    {
        QTransform oldTransform = painter->transform();
        painter->translate(x, y);
        painter->scale(mSize/6.0, mSize/6.0);
        painter->drawPath(mCustomPath);
        painter->setTransform(oldTransform);
        break;
    }
    }
}
/* end of 'src/scatterstyle.cpp' */

//amalgamation: add datacontainer.cpp

/* including file 'src/plottable.cpp', size 38845                            */
/* commit ce344b3f96a62e5f652585e55f1ae7c7883cd45b 2018-06-25 01:03:39 +0200 */

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPSelectionDecorator
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPSelectionDecorator
  \brief Controls how a plottable's data selection is drawn
  
  Each \ref QCPAbstractPlottable instance has one \ref QCPSelectionDecorator (accessible via \ref
  QCPAbstractPlottable::selectionDecorator) and uses it when drawing selected segments of its data.
  
  The selection decorator controls both pen (\ref setPen) and brush (\ref setBrush), as well as the
  scatter style (\ref setScatterStyle) if the plottable draws scatters. Since a \ref
  QCPScatterStyle is itself composed of different properties such as color shape and size, the
  decorator allows specifying exactly which of those properties shall be used for the selected data
  point, via \ref setUsedScatterProperties.
  
  A \ref QCPSelectionDecorator subclass instance can be passed to a plottable via \ref
  QCPAbstractPlottable::setSelectionDecorator, allowing greater customizability of the appearance
  of selected segments.
  
  Use \ref copyFrom to easily transfer the settings of one decorator to another one. This is
  especially useful since plottables take ownership of the passed selection decorator, and thus the
  same decorator instance can not be passed to multiple plottables.
  
  Selection decorators can also themselves perform drawing operations by reimplementing \ref
  drawDecoration, which is called by the plottable's draw method. The base class \ref
  QCPSelectionDecorator does not make use of this however. For example, \ref
  QCPSelectionDecoratorBracket draws brackets around selected data segments.
*/

/*!
  Creates a new QCPSelectionDecorator instance with default values
*/
QCPSelectionDecorator::QCPSelectionDecorator() :
    mPen(QColor(80, 80, 255), 2.5),
    mBrush(Qt::NoBrush),
    mScatterStyle(),
    mUsedScatterProperties(QCPScatterStyle::spNone),
    mPlottable(nullptr)
{
}

QCPSelectionDecorator::~QCPSelectionDecorator()
{
}

/*!
  Sets the pen that will be used by the parent plottable to draw selected data segments.
*/
void QCPSelectionDecorator::setPen(const QPen &pen)
{
    mPen = pen;
}

/*!
  Sets the brush that will be used by the parent plottable to draw selected data segments.
*/
void QCPSelectionDecorator::setBrush(const QBrush &brush)
{
    mBrush = brush;
}

/*!
  Sets the scatter style that will be used by the parent plottable to draw scatters in selected
  data segments.
  
  \a usedProperties specifies which parts of the passed \a scatterStyle will be used by the
  plottable. The used properties can also be changed via \ref setUsedScatterProperties.
*/
void QCPSelectionDecorator::setScatterStyle(const QCPScatterStyle &scatterStyle, QCPScatterStyle::ScatterProperties usedProperties)
{
    mScatterStyle = scatterStyle;
    setUsedScatterProperties(usedProperties);
}

/*!
  Use this method to define which properties of the scatter style (set via \ref setScatterStyle)
  will be used for selected data segments. All properties of the scatter style that are not
  specified in \a properties will remain as specified in the plottable's original scatter style.
  
  \see QCPScatterStyle::ScatterProperty
*/
void QCPSelectionDecorator::setUsedScatterProperties(const QCPScatterStyle::ScatterProperties &properties)
{
    mUsedScatterProperties = properties;
}

/*!
  Sets the pen of \a painter to the pen of this selection decorator.
  
  \see applyBrush, getFinalScatterStyle
*/
void QCPSelectionDecorator::applyPen(QCPPainter *painter) const
{
    painter->setPen(mPen);
}

/*!
  Sets the brush of \a painter to the brush of this selection decorator.
  
  \see applyPen, getFinalScatterStyle
*/
void QCPSelectionDecorator::applyBrush(QCPPainter *painter) const
{
    painter->setBrush(mBrush);
}

/*!
  Returns the scatter style that the parent plottable shall use for selected scatter points. The
  plottable's original (unselected) scatter style must be passed as \a unselectedStyle. Depending
  on the setting of \ref setUsedScatterProperties, the returned scatter style is a mixture of this
  selecion decorator's scatter style (\ref setScatterStyle), and \a unselectedStyle.
  
  \see applyPen, applyBrush, setScatterStyle
*/
QCPScatterStyle QCPSelectionDecorator::getFinalScatterStyle(const QCPScatterStyle &unselectedStyle) const
{
    QCPScatterStyle result(unselectedStyle);
    result.setFromOther(mScatterStyle, mUsedScatterProperties);

    // if style shall inherit pen from plottable (has no own pen defined), give it the selected
    // plottable pen explicitly, so it doesn't use the unselected plottable pen when used in the
    // plottable:
    if (!result.isPenDefined())
        result.setPen(mPen);

    return result;
}

/*!
  Copies all properties (e.g. color, fill, scatter style) of the \a other selection decorator to
  this selection decorator.
*/
void QCPSelectionDecorator::copyFrom(const QCPSelectionDecorator *other)
{
    setPen(other->pen());
    setBrush(other->brush());
    setScatterStyle(other->scatterStyle(), other->usedScatterProperties());
}

/*!
  This method is called by all plottables' draw methods to allow custom selection decorations to be
  drawn. Use the passed \a painter to perform the drawing operations. \a selection carries the data
  selection for which the decoration shall be drawn.
  
  The default base class implementation of \ref QCPSelectionDecorator has no special decoration, so
  this method does nothing.
*/
void QCPSelectionDecorator::drawDecoration(QCPPainter *painter, QCPDataSelection selection)
{
    Q_UNUSED(painter)
    Q_UNUSED(selection)
}

/*! \internal
  
  This method is called as soon as a selection decorator is associated with a plottable, by a call
  to \ref QCPAbstractPlottable::setSelectionDecorator. This way the selection decorator can obtain a pointer to the plottable that uses it (e.g. to access
  data points via the \ref QCPAbstractPlottable::interface1D interface).
  
  If the selection decorator was already added to a different plottable before, this method aborts
  the registration and returns false.
*/
bool QCPSelectionDecorator::registerWithPlottable(QCPAbstractPlottable *plottable)
{
    if (!mPlottable)
    {
        mPlottable = plottable;
        return true;
    } else
    {
        qDebug() << Q_FUNC_INFO << "This selection decorator is already registered with plottable:" << reinterpret_cast<quintptr>(mPlottable);
        return false;
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPAbstractPlottable
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPAbstractPlottable
  \brief The abstract base class for all data representing objects in a plot.

  It defines a very basic interface like name, pen, brush, visibility etc. Since this class is
  abstract, it can't be instantiated. Use one of the subclasses or create a subclass yourself to
  create new ways of displaying data (see "Creating own plottables" below). Plottables that display
  one-dimensional data (i.e. data points have a single key dimension and one or multiple values at
  each key) are based off of the template subclass \ref QCPAbstractPlottable1D, see details
  there.
  
  All further specifics are in the subclasses, for example:
  \li A normal graph with possibly a line and/or scatter points \ref QCPGraph
  (typically created with \ref PF_GraphicView::addGraph)
  \li A parametric curve: \ref QCPCurve
  \li A bar chart: \ref QCPBars
  \li A statistical box plot: \ref QCPStatisticalBox
  \li A color encoded two-dimensional map: \ref QCPColorMap
  \li An OHLC/Candlestick chart: \ref QCPFinancial
  
  \section plottables-subclassing Creating own plottables
  
  Subclassing directly from QCPAbstractPlottable is only recommended if you wish to display
  two-dimensional data like \ref QCPColorMap, i.e. two logical key dimensions and one (or more)
  data dimensions. If you want to display data with only one logical key dimension, you should
  rather derive from \ref QCPAbstractPlottable1D.
  
  If subclassing QCPAbstractPlottable directly, these are the pure virtual functions you must
  implement:
  \li \ref selectTest
  \li \ref draw
  \li \ref drawLegendIcon
  \li \ref getKeyRange
  \li \ref getValueRange
  
  See the documentation of those functions for what they need to do.
  
  For drawing your plot, you can use the \ref coordsToPixels functions to translate a point in plot
  coordinates to pixel coordinates. This function is quite convenient, because it takes the
  orientation of the key and value axes into account for you (x and y are swapped when the key axis
  is vertical and the value axis horizontal). If you are worried about performance (i.e. you need
  to translate many points in a loop like QCPGraph), you can directly use \ref
  QCPAxis::coordToPixel. However, you must then take care about the orientation of the axis
  yourself.
  
  Here are some important members you inherit from QCPAbstractPlottable:
  <table>
  <tr>
    <td>PF_GraphicView *\b mParentPlot</td>
    <td>A pointer to the parent PF_GraphicView instance. The parent plot is inferred from the axes that are passed in the constructor.</td>
  </tr><tr>
    <td>QString \b mName</td>
    <td>The name of the plottable.</td>
  </tr><tr>
    <td>QPen \b mPen</td>
    <td>The generic pen of the plottable. You should use this pen for the most prominent data representing lines in the plottable
        (e.g QCPGraph uses this pen for its graph lines and scatters)</td>
  </tr><tr>
    <td>QBrush \b mBrush</td>
    <td>The generic brush of the plottable. You should use this brush for the most prominent fillable structures in the plottable
        (e.g. QCPGraph uses this brush to control filling under the graph)</td>
  </tr><tr>
    <td>QPointer<\ref QCPAxis> \b mKeyAxis, \b mValueAxis</td>
    <td>The key and value axes this plottable is attached to. Call their QCPAxis::coordToPixel functions to translate coordinates
        to pixels in either the key or value dimension. Make sure to check whether the pointer is null before using it. If one of
        the axes is null, don't draw the plottable.</td>
  </tr><tr>
    <td>\ref QCPSelectionDecorator \b mSelectionDecorator</td>
    <td>The currently set selection decorator which specifies how selected data of the plottable shall be drawn and decorated.
        When drawing your data, you must consult this decorator for the appropriate pen/brush before drawing unselected/selected data segments.
        Finally, you should call its \ref QCPSelectionDecorator::drawDecoration method at the end of your \ref draw implementation.</td>
  </tr><tr>
    <td>\ref QCP::SelectionType \b mSelectable</td>
    <td>In which composition, if at all, this plottable's data may be selected. Enforcing this setting on the data selection is done
        by QCPAbstractPlottable automatically.</td>
  </tr><tr>
    <td>\ref QCPDataSelection \b mSelection</td>
    <td>Holds the current selection state of the plottable's data, i.e. the selected data ranges (\ref QCPDataRange).</td>
  </tr>
  </table>
*/

/* start of documentation of inline functions */

/*! \fn QCPSelectionDecorator *QCPAbstractPlottable::selectionDecorator() const
  
  Provides access to the selection decorator of this plottable. The selection decorator controls
  how selected data ranges are drawn (e.g. their pen color and fill), see \ref
  QCPSelectionDecorator for details.
  
  If you wish to use an own \ref QCPSelectionDecorator subclass, pass an instance of it to \ref
  setSelectionDecorator.
*/

/*! \fn bool QCPAbstractPlottable::selected() const
  
  Returns true if there are any data points of the plottable currently selected. Use \ref selection
  to retrieve the current \ref QCPDataSelection.
*/

/*! \fn QCPDataSelection QCPAbstractPlottable::selection() const
  
  Returns a \ref QCPDataSelection encompassing all the data points that are currently selected on
  this plottable.
  
  \see selected, setSelection, setSelectable
*/

/*! \fn virtual QCPPlottableInterface1D *QCPAbstractPlottable::interface1D()
  
  If this plottable is a one-dimensional plottable, i.e. it implements the \ref
  QCPPlottableInterface1D, returns the \a this pointer with that type. Otherwise (e.g. in the case
  of a \ref QCPColorMap) returns zero.
  
  You can use this method to gain read access to data coordinates while holding a pointer to the
  abstract base class only.
*/

/* end of documentation of inline functions */
/* start of documentation of pure virtual functions */

/*! \fn void QCPAbstractPlottable::drawLegendIcon(QCPPainter *painter, const QRect &rect) const = 0
  \internal
  
  called by QCPLegend::draw (via QCPPlottableLegendItem::draw) to create a graphical representation
  of this plottable inside \a rect, next to the plottable name.
  
  The passed \a painter has its cliprect set to \a rect, so painting outside of \a rect won't
  appear outside the legend icon border.
*/

/*! \fn QCPRange QCPAbstractPlottable::getKeyRange(bool &foundRange, QCP::SignDomain inSignDomain) const = 0
  
  Returns the coordinate range that all data in this plottable span in the key axis dimension. For
  logarithmic plots, one can set \a inSignDomain to either \ref QCP::sdNegative or \ref
  QCP::sdPositive in order to restrict the returned range to that sign domain. E.g. when only
  negative range is wanted, set \a inSignDomain to \ref QCP::sdNegative and all positive points
  will be ignored for range calculation. For no restriction, just set \a inSignDomain to \ref
  QCP::sdBoth (default). \a foundRange is an output parameter that indicates whether a range could
  be found or not. If this is false, you shouldn't use the returned range (e.g. no points in data).

  Note that \a foundRange is not the same as \ref QCPRange::validRange, since the range returned by
  this function may have size zero (e.g. when there is only one data point). In this case \a
  foundRange would return true, but the returned range is not a valid range in terms of \ref
  QCPRange::validRange.
  
  \see rescaleAxes, getValueRange
*/

/*! \fn QCPRange QCPAbstractPlottable::getValueRange(bool &foundRange, QCP::SignDomain inSignDomain, const QCPRange &inKeyRange) const = 0
  
  Returns the coordinate range that the data points in the specified key range (\a inKeyRange) span
  in the value axis dimension. For logarithmic plots, one can set \a inSignDomain to either \ref
  QCP::sdNegative or \ref QCP::sdPositive in order to restrict the returned range to that sign
  domain. E.g. when only negative range is wanted, set \a inSignDomain to \ref QCP::sdNegative and
  all positive points will be ignored for range calculation. For no restriction, just set \a
  inSignDomain to \ref QCP::sdBoth (default). \a foundRange is an output parameter that indicates
  whether a range could be found or not. If this is false, you shouldn't use the returned range
  (e.g. no points in data).
  
  If \a inKeyRange has both lower and upper bound set to zero (is equal to <tt>QCPRange()</tt>),
  all data points are considered, without any restriction on the keys.

  Note that \a foundRange is not the same as \ref QCPRange::validRange, since the range returned by
  this function may have size zero (e.g. when there is only one data point). In this case \a
  foundRange would return true, but the returned range is not a valid range in terms of \ref
  QCPRange::validRange.
  
  \see rescaleAxes, getKeyRange
*/

/* end of documentation of pure virtual functions */
/* start of documentation of signals */

/*! \fn void QCPAbstractPlottable::selectionChanged(bool selected)
  
  This signal is emitted when the selection state of this plottable has changed, either by user
  interaction or by a direct call to \ref setSelection. The parameter \a selected indicates whether
  there are any points selected or not.
  
  \see selectionChanged(const QCPDataSelection &selection)
*/

/*! \fn void QCPAbstractPlottable::selectionChanged(const QCPDataSelection &selection)
  
  This signal is emitted when the selection state of this plottable has changed, either by user
  interaction or by a direct call to \ref setSelection. The parameter \a selection holds the
  currently selected data ranges.
  
  \see selectionChanged(bool selected)
*/

/*! \fn void QCPAbstractPlottable::selectableChanged(QCP::SelectionType selectable);
  
  This signal is emitted when the selectability of this plottable has changed.
  
  \see setSelectable
*/

/* end of documentation of signals */

/*!
  Constructs an abstract plottable which uses \a keyAxis as its key axis ("x") and \a valueAxis as
  its value axis ("y"). \a keyAxis and \a valueAxis must reside in the same PF_GraphicView instance
  and have perpendicular orientations. If either of these restrictions is violated, a corresponding
  message is printed to the debug output (qDebug), the construction is not aborted, though.
  
  Since QCPAbstractPlottable is an abstract class that defines the basic interface to plottables,
  it can't be directly instantiated.
  
  You probably want one of the subclasses like \ref QCPGraph or \ref QCPCurve instead.
*/
QCPAbstractPlottable::QCPAbstractPlottable(QCPAxis *keyAxis, QCPAxis *valueAxis) :
    QCPLayerable(keyAxis->parentPlot(), QString(), keyAxis->axisRect()),
    mName(),
    mAntialiasedFill(true),
    mAntialiasedScatters(true),
    mPen(Qt::black),
    mBrush(Qt::NoBrush),
    mKeyAxis(keyAxis),
    mValueAxis(valueAxis),
    mSelectable(QCP::stWhole),
    mSelectionDecorator(nullptr)
{
    if (keyAxis->parentPlot() != valueAxis->parentPlot())
        qDebug() << Q_FUNC_INFO << "Parent plot of keyAxis is not the same as that of valueAxis.";
    if (keyAxis->orientation() == valueAxis->orientation())
        qDebug() << Q_FUNC_INFO << "keyAxis and valueAxis must be orthogonal to each other.";

    mParentPlot->registerPlottable(this);
    setSelectionDecorator(new QCPSelectionDecorator);
}

QCPAbstractPlottable::~QCPAbstractPlottable()
{
    if (mSelectionDecorator)
    {
        delete mSelectionDecorator;
        mSelectionDecorator = nullptr;
    }
}

/*!
   The name is the textual representation of this plottable as it is displayed in the legend
   (\ref QCPLegend). It may contain any UTF-8 characters, including newlines.
*/
void QCPAbstractPlottable::setName(const QString &name)
{
    mName = name;
}

/*!
  Sets whether fills of this plottable are drawn antialiased or not.
  
  Note that this setting may be overridden by \ref PF_GraphicView::setAntialiasedElements and \ref
  PF_GraphicView::setNotAntialiasedElements.
*/
void QCPAbstractPlottable::setAntialiasedFill(bool enabled)
{
    mAntialiasedFill = enabled;
}

/*!
  Sets whether the scatter symbols of this plottable are drawn antialiased or not.
  
  Note that this setting may be overridden by \ref PF_GraphicView::setAntialiasedElements and \ref
  PF_GraphicView::setNotAntialiasedElements.
*/
void QCPAbstractPlottable::setAntialiasedScatters(bool enabled)
{
    mAntialiasedScatters = enabled;
}

/*!
  The pen is used to draw basic lines that make up the plottable representation in the
  plot.
  
  For example, the \ref QCPGraph subclass draws its graph lines with this pen.

  \see setBrush
*/
void QCPAbstractPlottable::setPen(const QPen &pen)
{
    mPen = pen;
}

/*!
  The brush is used to draw basic fills of the plottable representation in the
  plot. The Fill can be a color, gradient or texture, see the usage of QBrush.
  
  For example, the \ref QCPGraph subclass draws the fill under the graph with this brush, when
  it's not set to Qt::NoBrush.

  \see setPen
*/
void QCPAbstractPlottable::setBrush(const QBrush &brush)
{
    mBrush = brush;
}

/*!
  The key axis of a plottable can be set to any axis of a PF_GraphicView, as long as it is orthogonal
  to the plottable's value axis. This function performs no checks to make sure this is the case.
  The typical mathematical choice is to use the x-axis (PF_GraphicView::xAxis) as key axis and the
  y-axis (PF_GraphicView::yAxis) as value axis.
  
  Normally, the key and value axes are set in the constructor of the plottable (or \ref
  PF_GraphicView::addGraph when working with QCPGraphs through the dedicated graph interface).

  \see setValueAxis
*/
void QCPAbstractPlottable::setKeyAxis(QCPAxis *axis)
{
    mKeyAxis = axis;
}

/*!
  The value axis of a plottable can be set to any axis of a PF_GraphicView, as long as it is
  orthogonal to the plottable's key axis. This function performs no checks to make sure this is the
  case. The typical mathematical choice is to use the x-axis (PF_GraphicView::xAxis) as key axis and
  the y-axis (PF_GraphicView::yAxis) as value axis.

  Normally, the key and value axes are set in the constructor of the plottable (or \ref
  PF_GraphicView::addGraph when working with QCPGraphs through the dedicated graph interface).
  
  \see setKeyAxis
*/
void QCPAbstractPlottable::setValueAxis(QCPAxis *axis)
{
    mValueAxis = axis;
}


/*!
  Sets which data ranges of this plottable are selected. Selected data ranges are drawn differently
  (e.g. color) in the plot. This can be controlled via the selection decorator (see \ref
  selectionDecorator).
  
  The entire selection mechanism for plottables is handled automatically when \ref
  PF_GraphicView::setInteractions contains iSelectPlottables. You only need to call this function when
  you wish to change the selection state programmatically.
  
  Using \ref setSelectable you can further specify for each plottable whether and to which
  granularity it is selectable. If \a selection is not compatible with the current \ref
  QCP::SelectionType set via \ref setSelectable, the resulting selection will be adjusted
  accordingly (see \ref QCPDataSelection::enforceType).
  
  emits the \ref selectionChanged signal when \a selected is different from the previous selection state.
  
  \see setSelectable, selectTest
*/
void QCPAbstractPlottable::setSelection(QCPDataSelection selection)
{
    selection.enforceType(mSelectable);
    if (mSelection != selection)
    {
        mSelection = selection;
        emit selectionChanged(selected());
        emit selectionChanged(mSelection);
    }
}

/*!
  Use this method to set an own QCPSelectionDecorator (subclass) instance. This allows you to
  customize the visual representation of selected data ranges further than by using the default
  QCPSelectionDecorator.
  
  The plottable takes ownership of the \a decorator.
  
  The currently set decorator can be accessed via \ref selectionDecorator.
*/
void QCPAbstractPlottable::setSelectionDecorator(QCPSelectionDecorator *decorator)
{
    if (decorator)
    {
        if (decorator->registerWithPlottable(this))
        {
            if (mSelectionDecorator) // delete old decorator if necessary
                delete mSelectionDecorator;
            mSelectionDecorator = decorator;
        }
    } else if (mSelectionDecorator) // just clear decorator
    {
        delete mSelectionDecorator;
        mSelectionDecorator = nullptr;
    }
}

/*!
  Sets whether and to which granularity this plottable can be selected.

  A selection can happen by clicking on the PF_GraphicView surface (When \ref
  PF_GraphicView::setInteractions contains \ref QCP::iSelectPlottables), by dragging a selection rect
  (When \ref PF_GraphicView::setSelectionRectMode is \ref QCP::srmSelect), or programmatically by
  calling \ref setSelection.
  
  \see setSelection, QCP::SelectionType
*/
void QCPAbstractPlottable::setSelectable(QCP::SelectionType selectable)
{
    if (mSelectable != selectable)
    {
        mSelectable = selectable;
        QCPDataSelection oldSelection = mSelection;
        mSelection.enforceType(mSelectable);
        emit selectableChanged(mSelectable);
        if (mSelection != oldSelection)
        {
            emit selectionChanged(selected());
            emit selectionChanged(mSelection);
        }
    }
}


/*!
  Convenience function for transforming a key/value pair to pixels on the PF_GraphicView surface,
  taking the orientations of the axes associated with this plottable into account (e.g. whether key
  represents x or y).

  \a key and \a value are transformed to the coodinates in pixels and are written to \a x and \a y.

  \see pixelsToCoords, QCPAxis::coordToPixel
*/
void QCPAbstractPlottable::coordsToPixels(double key, double value, double &x, double &y) const
{
    QCPAxis *keyAxis = mKeyAxis.data();
    QCPAxis *valueAxis = mValueAxis.data();
    if (!keyAxis || !valueAxis) { qDebug() << Q_FUNC_INFO << "invalid key or value axis"; return; }

    if (keyAxis->orientation() == Qt::Horizontal)
    {
        x = keyAxis->coordToPixel(key);
        y = valueAxis->coordToPixel(value);
    } else
    {
        y = keyAxis->coordToPixel(key);
        x = valueAxis->coordToPixel(value);
    }
}

/*! \overload

  Transforms the given \a key and \a value to pixel coordinates and returns them in a QPointF.
*/
const QPointF QCPAbstractPlottable::coordsToPixels(double key, double value) const
{
    QCPAxis *keyAxis = mKeyAxis.data();
    QCPAxis *valueAxis = mValueAxis.data();
    if (!keyAxis || !valueAxis) { qDebug() << Q_FUNC_INFO << "invalid key or value axis"; return QPointF(); }

    if (keyAxis->orientation() == Qt::Horizontal)
        return QPointF(keyAxis->coordToPixel(key), valueAxis->coordToPixel(value));
    else
        return QPointF(valueAxis->coordToPixel(value), keyAxis->coordToPixel(key));
}

/*!
  Convenience function for transforming a x/y pixel pair on the PF_GraphicView surface to plot coordinates,
  taking the orientations of the axes associated with this plottable into account (e.g. whether key
  represents x or y).

  \a x and \a y are transformed to the plot coodinates and are written to \a key and \a value.

  \see coordsToPixels, QCPAxis::coordToPixel
*/
void QCPAbstractPlottable::pixelsToCoords(double x, double y, double &key, double &value) const
{
    QCPAxis *keyAxis = mKeyAxis.data();
    QCPAxis *valueAxis = mValueAxis.data();
    if (!keyAxis || !valueAxis) { qDebug() << Q_FUNC_INFO << "invalid key or value axis"; return; }

    if (keyAxis->orientation() == Qt::Horizontal)
    {
        key = keyAxis->pixelToCoord(x);
        value = valueAxis->pixelToCoord(y);
    } else
    {
        key = keyAxis->pixelToCoord(y);
        value = valueAxis->pixelToCoord(x);
    }
}

/*! \overload

  Returns the pixel input \a pixelPos as plot coordinates \a key and \a value.
*/
void QCPAbstractPlottable::pixelsToCoords(const QPointF &pixelPos, double &key, double &value) const
{
    pixelsToCoords(pixelPos.x(), pixelPos.y(), key, value);
}

/*!
  Rescales the key and value axes associated with this plottable to contain all displayed data, so
  the whole plottable is visible. If the scaling of an axis is logarithmic, rescaleAxes will make
  sure not to rescale to an illegal range i.e. a range containing different signs and/or zero.
  Instead it will stay in the current sign domain and ignore all parts of the plottable that lie
  outside of that domain.
  
  \a onlyEnlarge makes sure the ranges are only expanded, never reduced. So it's possible to show
  multiple plottables in their entirety by multiple calls to rescaleAxes where the first call has
  \a onlyEnlarge set to false (the default), and all subsequent set to true.
  
  \see rescaleKeyAxis, rescaleValueAxis, PF_GraphicView::rescaleAxes, QCPAxis::rescale
*/
void QCPAbstractPlottable::rescaleAxes(bool onlyEnlarge) const
{
    rescaleKeyAxis(onlyEnlarge);
    rescaleValueAxis(onlyEnlarge);
}

/*!
  Rescales the key axis of the plottable so the whole plottable is visible.
  
  See \ref rescaleAxes for detailed behaviour.
*/
void QCPAbstractPlottable::rescaleKeyAxis(bool onlyEnlarge) const
{
    QCPAxis *keyAxis = mKeyAxis.data();
    if (!keyAxis) { qDebug() << Q_FUNC_INFO << "invalid key axis"; return; }

    QCP::SignDomain signDomain = QCP::sdBoth;
    if (keyAxis->scaleType() == QCPAxis::stLogarithmic)
        signDomain = (keyAxis->range().upper < 0 ? QCP::sdNegative : QCP::sdPositive);

    bool foundRange;
    QCPRange newRange = getKeyRange(foundRange, signDomain);
    if (foundRange)
    {
        if (onlyEnlarge)
            newRange.expand(keyAxis->range());
        if (!QCPRange::validRange(newRange)) // likely due to range being zero (plottable has only constant data in this axis dimension), shift current range to at least center the plottable
        {
            double center = (newRange.lower+newRange.upper)*0.5; // upper and lower should be equal anyway, but just to make sure, incase validRange returned false for other reason
            if (keyAxis->scaleType() == QCPAxis::stLinear)
            {
                newRange.lower = center-keyAxis->range().size()/2.0;
                newRange.upper = center+keyAxis->range().size()/2.0;
            } else // scaleType() == stLogarithmic
            {
                newRange.lower = center/qSqrt(keyAxis->range().upper/keyAxis->range().lower);
                newRange.upper = center*qSqrt(keyAxis->range().upper/keyAxis->range().lower);
            }
        }
        keyAxis->setRange(newRange);
    }
}

/*!
  Rescales the value axis of the plottable so the whole plottable is visible. If \a inKeyRange is
  set to true, only the data points which are in the currently visible key axis range are
  considered.

  Returns true if the axis was actually scaled. This might not be the case if this plottable has an
  invalid range, e.g. because it has no data points.

  See \ref rescaleAxes for detailed behaviour.
*/
void QCPAbstractPlottable::rescaleValueAxis(bool onlyEnlarge, bool inKeyRange) const
{
    QCPAxis *keyAxis = mKeyAxis.data();
    QCPAxis *valueAxis = mValueAxis.data();
    if (!keyAxis || !valueAxis) { qDebug() << Q_FUNC_INFO << "invalid key or value axis"; return; }

    QCP::SignDomain signDomain = QCP::sdBoth;
    if (valueAxis->scaleType() == QCPAxis::stLogarithmic)
        signDomain = (valueAxis->range().upper < 0 ? QCP::sdNegative : QCP::sdPositive);

    bool foundRange;
    QCPRange newRange = getValueRange(foundRange, signDomain, inKeyRange ? keyAxis->range() : QCPRange());
    if (foundRange)
    {
        if (onlyEnlarge)
            newRange.expand(valueAxis->range());
        if (!QCPRange::validRange(newRange)) // likely due to range being zero (plottable has only constant data in this axis dimension), shift current range to at least center the plottable
        {
            double center = (newRange.lower+newRange.upper)*0.5; // upper and lower should be equal anyway, but just to make sure, incase validRange returned false for other reason
            if (valueAxis->scaleType() == QCPAxis::stLinear)
            {
                newRange.lower = center-valueAxis->range().size()/2.0;
                newRange.upper = center+valueAxis->range().size()/2.0;
            } else // scaleType() == stLogarithmic
            {
                newRange.lower = center/qSqrt(valueAxis->range().upper/valueAxis->range().lower);
                newRange.upper = center*qSqrt(valueAxis->range().upper/valueAxis->range().lower);
            }
        }
        valueAxis->setRange(newRange);
    }
}

/*! \overload

  Adds this plottable to the specified \a legend.

  Creates a QCPPlottableLegendItem which is inserted into the legend. Returns true on success, i.e.
  when the legend exists and a legend item associated with this plottable isn't already in the
  legend.

  If the plottable needs a more specialized representation in the legend, you can create a
  corresponding subclass of \ref QCPPlottableLegendItem and add it to the legend manually instead
  of calling this method.

  \see removeFromLegend, QCPLegend::addItem
*/
bool QCPAbstractPlottable::addToLegend(QCPLegend *legend)
{
    if (!legend)
    {
        qDebug() << Q_FUNC_INFO << "passed legend is null";
        return false;
    }
    if (legend->parentPlot() != mParentPlot)
    {
        qDebug() << Q_FUNC_INFO << "passed legend isn't in the same PF_GraphicView as this plottable";
        return false;
    }

    if (!legend->hasItemWithPlottable(this))
    {
        legend->addItem(new QCPPlottableLegendItem(legend, this));
        return true;
    } else
        return false;
}

/*! \overload

  Adds this plottable to the legend of the parent PF_GraphicView (\ref PF_GraphicView::legend).

  \see removeFromLegend
*/
bool QCPAbstractPlottable::addToLegend()
{
    if (!mParentPlot || !mParentPlot->legend)
        return false;
    else
        return addToLegend(mParentPlot->legend);
}

/*! \overload

  Removes the plottable from the specifed \a legend. This means the \ref QCPPlottableLegendItem
  that is associated with this plottable is removed.

  Returns true on success, i.e. if the legend exists and a legend item associated with this
  plottable was found and removed.

  \see addToLegend, QCPLegend::removeItem
*/
bool QCPAbstractPlottable::removeFromLegend(QCPLegend *legend) const
{
    if (!legend)
    {
        qDebug() << Q_FUNC_INFO << "passed legend is null";
        return false;
    }

    if (QCPPlottableLegendItem *lip = legend->itemWithPlottable(this))
        return legend->removeItem(lip);
    else
        return false;
}

/*! \overload

  Removes the plottable from the legend of the parent PF_GraphicView.

  \see addToLegend
*/
bool QCPAbstractPlottable::removeFromLegend() const
{
    if (!mParentPlot || !mParentPlot->legend)
        return false;
    else
        return removeFromLegend(mParentPlot->legend);
}

/* inherits documentation from base class */
QRect QCPAbstractPlottable::clipRect() const
{
    if (mKeyAxis && mValueAxis)
        return mKeyAxis.data()->axisRect()->rect() & mValueAxis.data()->axisRect()->rect();
    else
        return QRect();
}

/* inherits documentation from base class */
QCP::Interaction QCPAbstractPlottable::selectionCategory() const
{
    return QCP::iSelectPlottables;
}

/*! \internal

  A convenience function to easily set the QPainter::Antialiased hint on the provided \a painter
  before drawing plottable lines.

  This is the antialiasing state the painter passed to the \ref draw method is in by default.
  
  This function takes into account the local setting of the antialiasing flag as well as the
  overrides set with \ref PF_GraphicView::setAntialiasedElements and \ref
  PF_GraphicView::setNotAntialiasedElements.
  
  \seebaseclassmethod
  
  \see setAntialiased, applyFillAntialiasingHint, applyScattersAntialiasingHint
*/
void QCPAbstractPlottable::applyDefaultAntialiasingHint(QCPPainter *painter) const
{
    applyAntialiasingHint(painter, mAntialiased, QCP::aePlottables);
}

/*! \internal

  A convenience function to easily set the QPainter::Antialiased hint on the provided \a painter
  before drawing plottable fills.
  
  This function takes into account the local setting of the antialiasing flag as well as the
  overrides set with \ref PF_GraphicView::setAntialiasedElements and \ref
  PF_GraphicView::setNotAntialiasedElements.
  
  \see setAntialiased, applyDefaultAntialiasingHint, applyScattersAntialiasingHint
*/
void QCPAbstractPlottable::applyFillAntialiasingHint(QCPPainter *painter) const
{
    applyAntialiasingHint(painter, mAntialiasedFill, QCP::aeFills);
}

/*! \internal

  A convenience function to easily set the QPainter::Antialiased hint on the provided \a painter
  before drawing plottable scatter points.
  
  This function takes into account the local setting of the antialiasing flag as well as the
  overrides set with \ref PF_GraphicView::setAntialiasedElements and \ref
  PF_GraphicView::setNotAntialiasedElements.
  
  \see setAntialiased, applyFillAntialiasingHint, applyDefaultAntialiasingHint
*/
void QCPAbstractPlottable::applyScattersAntialiasingHint(QCPPainter *painter) const
{
    applyAntialiasingHint(painter, mAntialiasedScatters, QCP::aeScatters);
}

/* inherits documentation from base class */
void QCPAbstractPlottable::selectEvent(QMouseEvent *event, bool additive, const QVariant &details, bool *selectionStateChanged)
{
    Q_UNUSED(event)

    if (mSelectable != QCP::stNone)
    {
        QCPDataSelection newSelection = details.value<QCPDataSelection>();
        QCPDataSelection selectionBefore = mSelection;
        if (additive)
        {
            if (mSelectable == QCP::stWhole) // in whole selection mode, we toggle to no selection even if currently unselected point was hit
            {
                if (selected())
                    setSelection(QCPDataSelection());
                else
                    setSelection(newSelection);
            } else // in all other selection modes we toggle selections of homogeneously selected/unselected segments
            {
                if (mSelection.contains(newSelection)) // if entire newSelection is already selected, toggle selection
                    setSelection(mSelection-newSelection);
                else
                    setSelection(mSelection+newSelection);
            }
        } else
            setSelection(newSelection);
        if (selectionStateChanged)
            *selectionStateChanged = mSelection != selectionBefore;
    }
}

/* inherits documentation from base class */
void QCPAbstractPlottable::deselectEvent(bool *selectionStateChanged)
{
    if (mSelectable != QCP::stNone)
    {
        QCPDataSelection selectionBefore = mSelection;
        setSelection(QCPDataSelection());
        if (selectionStateChanged)
            *selectionStateChanged = mSelection != selectionBefore;
    }
}
/* end of 'src/plottable.cpp' */


/* including file 'src/item.cpp', size 49271                                 */
/* commit ce344b3f96a62e5f652585e55f1ae7c7883cd45b 2018-06-25 01:03:39 +0200 */

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPItemAnchor
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPItemAnchor
  \brief An anchor of an item to which positions can be attached to.
  
  An item (QCPAbstractItem) may have one or more anchors. Unlike QCPItemPosition, an anchor doesn't
  control anything on its item, but provides a way to tie other items via their positions to the
  anchor.

  For example, a QCPItemRect is defined by its positions \a topLeft and \a bottomRight.
  Additionally it has various anchors like \a top, \a topRight or \a bottomLeft etc. So you can
  attach the \a start (which is a QCPItemPosition) of a QCPItemLine to one of the anchors by
  calling QCPItemPosition::setParentAnchor on \a start, passing the wanted anchor of the
  QCPItemRect. This way the start of the line will now always follow the respective anchor location
  on the rect item.
  
  Note that QCPItemPosition derives from QCPItemAnchor, so every position can also serve as an
  anchor to other positions.
  
  To learn how to provide anchors in your own item subclasses, see the subclassing section of the
  QCPAbstractItem documentation.
*/

/* start documentation of inline functions */

/*! \fn virtual QCPItemPosition *QCPItemAnchor::toQCPItemPosition()
  
  Returns 0 if this instance is merely a QCPItemAnchor, and a valid pointer of type QCPItemPosition* if
  it actually is a QCPItemPosition (which is a subclass of QCPItemAnchor).
  
  This safe downcast functionality could also be achieved with a dynamic_cast. However, PF_GraphicView avoids
  dynamic_cast to work with projects that don't have RTTI support enabled (e.g. -fno-rtti flag with
  gcc compiler).
*/

/* end documentation of inline functions */

/*!
  Creates a new QCPItemAnchor. You shouldn't create QCPItemAnchor instances directly, even if
  you want to make a new item subclass. Use \ref QCPAbstractItem::createAnchor instead, as
  explained in the subclassing section of the QCPAbstractItem documentation.
*/
QCPItemAnchor::QCPItemAnchor(PF_GraphicView *parentPlot, QCPAbstractItem *parentItem, const QString &name, int anchorId) :
    mName(name),
    mParentPlot(parentPlot),
    mParentItem(parentItem),
    mAnchorId(anchorId)
{
}

QCPItemAnchor::~QCPItemAnchor()
{
    // unregister as parent at children:
    foreach (QCPItemPosition *child, mChildrenX.toList())
    {
        if (child->parentAnchorX() == this)
            child->setParentAnchorX(nullptr); // this acts back on this anchor and child removes itself from mChildrenX
    }
    foreach (QCPItemPosition *child, mChildrenY.toList())
    {
        if (child->parentAnchorY() == this)
            child->setParentAnchorY(nullptr); // this acts back on this anchor and child removes itself from mChildrenY
    }
}

/*!
  Returns the final absolute pixel position of the QCPItemAnchor on the PF_GraphicView surface.
  
  The pixel information is internally retrieved via QCPAbstractItem::anchorPixelPosition of the
  parent item, QCPItemAnchor is just an intermediary.
*/
QPointF QCPItemAnchor::pixelPosition() const
{
    if (mParentItem)
    {
        if (mAnchorId > -1)
        {
            return mParentItem->anchorPixelPosition(mAnchorId);
        } else
        {
            qDebug() << Q_FUNC_INFO << "no valid anchor id set:" << mAnchorId;
            return QPointF();
        }
    } else
    {
        qDebug() << Q_FUNC_INFO << "no parent item set";
        return QPointF();
    }
}

/*! \internal

  Adds \a pos to the childX list of this anchor, which keeps track of which children use this
  anchor as parent anchor for the respective coordinate. This is necessary to notify the children
  prior to destruction of the anchor.
  
  Note that this function does not change the parent setting in \a pos.
*/
void QCPItemAnchor::addChildX(QCPItemPosition *pos)
{
    if (!mChildrenX.contains(pos))
        mChildrenX.insert(pos);
    else
        qDebug() << Q_FUNC_INFO << "provided pos is child already" << reinterpret_cast<quintptr>(pos);
}

/*! \internal

  Removes \a pos from the childX list of this anchor.
  
  Note that this function does not change the parent setting in \a pos.
*/
void QCPItemAnchor::removeChildX(QCPItemPosition *pos)
{
    if (!mChildrenX.remove(pos))
        qDebug() << Q_FUNC_INFO << "provided pos isn't child" << reinterpret_cast<quintptr>(pos);
}

/*! \internal

  Adds \a pos to the childY list of this anchor, which keeps track of which children use this
  anchor as parent anchor for the respective coordinate. This is necessary to notify the children
  prior to destruction of the anchor.
  
  Note that this function does not change the parent setting in \a pos.
*/
void QCPItemAnchor::addChildY(QCPItemPosition *pos)
{
    if (!mChildrenY.contains(pos))
        mChildrenY.insert(pos);
    else
        qDebug() << Q_FUNC_INFO << "provided pos is child already" << reinterpret_cast<quintptr>(pos);
}

/*! \internal

  Removes \a pos from the childY list of this anchor.
  
  Note that this function does not change the parent setting in \a pos.
*/
void QCPItemAnchor::removeChildY(QCPItemPosition *pos)
{
    if (!mChildrenY.remove(pos))
        qDebug() << Q_FUNC_INFO << "provided pos isn't child" << reinterpret_cast<quintptr>(pos);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPItemPosition
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPItemPosition
  \brief Manages the position of an item.
  
  Every item has at least one public QCPItemPosition member pointer which provides ways to position the
  item on the PF_GraphicView surface. Some items have multiple positions, for example QCPItemRect has two:
  \a topLeft and \a bottomRight.

  QCPItemPosition has a type (\ref PositionType) that can be set with \ref setType. This type
  defines how coordinates passed to \ref setCoords are to be interpreted, e.g. as absolute pixel
  coordinates, as plot coordinates of certain axes, etc. For more advanced plots it is also
  possible to assign different types per X/Y coordinate of the position (see \ref setTypeX, \ref
  setTypeY). This way an item could be positioned at a fixed pixel distance from the top in the Y
  direction, while following a plot coordinate in the X direction.

  A QCPItemPosition may have a parent QCPItemAnchor, see \ref setParentAnchor. This way you can tie
  multiple items together. If the QCPItemPosition has a parent, its coordinates (\ref setCoords)
  are considered to be absolute pixels in the reference frame of the parent anchor, where (0, 0)
  means directly ontop of the parent anchor. For example, You could attach the \a start position of
  a QCPItemLine to the \a bottom anchor of a QCPItemText to make the starting point of the line
  always be centered under the text label, no matter where the text is moved to. For more advanced
  plots, it is possible to assign different parent anchors per X/Y coordinate of the position, see
  \ref setParentAnchorX, \ref setParentAnchorY. This way an item could follow another item in the X
  direction but stay at a fixed position in the Y direction. Or even follow item A in X, and item B
  in Y.

  Note that every QCPItemPosition inherits from QCPItemAnchor and thus can itself be used as parent
  anchor for other positions.

  To set the apparent pixel position on the PF_GraphicView surface directly, use \ref setPixelPosition. This
  works no matter what type this QCPItemPosition is or what parent-child situation it is in, as \ref
  setPixelPosition transforms the coordinates appropriately, to make the position appear at the specified
  pixel values.
*/

/* start documentation of inline functions */

/*! \fn QCPItemPosition::PositionType *QCPItemPosition::type() const
  
  Returns the current position type.
  
  If different types were set for X and Y (\ref setTypeX, \ref setTypeY), this method returns the
  type of the X coordinate. In that case rather use \a typeX() and \a typeY().
  
  \see setType
*/

/*! \fn QCPItemAnchor *QCPItemPosition::parentAnchor() const
  
  Returns the current parent anchor.
  
  If different parent anchors were set for X and Y (\ref setParentAnchorX, \ref setParentAnchorY),
  this method returns the parent anchor of the Y coordinate. In that case rather use \a
  parentAnchorX() and \a parentAnchorY().
  
  \see setParentAnchor
*/

/* end documentation of inline functions */

/*!
  Creates a new QCPItemPosition. You shouldn't create QCPItemPosition instances directly, even if
  you want to make a new item subclass. Use \ref QCPAbstractItem::createPosition instead, as
  explained in the subclassing section of the QCPAbstractItem documentation.
*/
QCPItemPosition::QCPItemPosition(PF_GraphicView *parentPlot, QCPAbstractItem *parentItem, const QString &name) :
    QCPItemAnchor(parentPlot, parentItem, name),
    mPositionTypeX(ptAbsolute),
    mPositionTypeY(ptAbsolute),
    mKey(0),
    mValue(0),
    mParentAnchorX(nullptr),
    mParentAnchorY(nullptr)
{
}

QCPItemPosition::~QCPItemPosition()
{
    // unregister as parent at children:
    // Note: this is done in ~QCPItemAnchor again, but it's important QCPItemPosition does it itself, because only then
    //       the setParentAnchor(0) call the correct QCPItemPosition::pixelPosition function instead of QCPItemAnchor::pixelPosition
    foreach (QCPItemPosition *child, mChildrenX.toList())
    {
        if (child->parentAnchorX() == this)
            child->setParentAnchorX(nullptr); // this acts back on this anchor and child removes itself from mChildrenX
    }
    foreach (QCPItemPosition *child, mChildrenY.toList())
    {
        if (child->parentAnchorY() == this)
            child->setParentAnchorY(nullptr); // this acts back on this anchor and child removes itself from mChildrenY
    }
    // unregister as child in parent:
    if (mParentAnchorX)
        mParentAnchorX->removeChildX(this);
    if (mParentAnchorY)
        mParentAnchorY->removeChildY(this);
}

/* can't make this a header inline function, because QPointer breaks with forward declared types, see QTBUG-29588 */
QCPAxisRect *QCPItemPosition::axisRect() const
{
    return mAxisRect.data();
}

/*!
  Sets the type of the position. The type defines how the coordinates passed to \ref setCoords
  should be handled and how the QCPItemPosition should behave in the plot.
  
  The possible values for \a type can be separated in two main categories:

  \li The position is regarded as a point in plot coordinates. This corresponds to \ref ptPlotCoords
  and requires two axes that define the plot coordinate system. They can be specified with \ref setAxes.
  By default, the PF_GraphicView's x- and yAxis are used.
  
  \li The position is fixed on the PF_GraphicView surface, i.e. independent of axis ranges. This
  corresponds to all other types, i.e. \ref ptAbsolute, \ref ptViewportRatio and \ref
  ptAxisRectRatio. They differ only in the way the absolute position is described, see the
  documentation of \ref PositionType for details. For \ref ptAxisRectRatio, note that you can specify
  the axis rect with \ref setAxisRect. By default this is set to the main axis rect.
  
  Note that the position type \ref ptPlotCoords is only available (and sensible) when the position
  has no parent anchor (\ref setParentAnchor).
  
  If the type is changed, the apparent pixel position on the plot is preserved. This means
  the coordinates as retrieved with coords() and set with \ref setCoords may change in the process.
  
  This method sets the type for both X and Y directions. It is also possible to set different types
  for X and Y, see \ref setTypeX, \ref setTypeY.
*/
void QCPItemPosition::setType(QCPItemPosition::PositionType type)
{
    setTypeX(type);
    setTypeY(type);
}

/*!
  This method sets the position type of the X coordinate to \a type.
  
  For a detailed description of what a position type is, see the documentation of \ref setType.
  
  \see setType, setTypeY
*/
void QCPItemPosition::setTypeX(QCPItemPosition::PositionType type)
{
    if (mPositionTypeX != type)
    {
        // if switching from or to coordinate type that isn't valid (e.g. because axes or axis rect
        // were deleted), don't try to recover the pixelPosition() because it would output a qDebug warning.
        bool retainPixelPosition = true;
        if ((mPositionTypeX == ptPlotCoords || type == ptPlotCoords) && (!mKeyAxis || !mValueAxis))
            retainPixelPosition = false;
        if ((mPositionTypeX == ptAxisRectRatio || type == ptAxisRectRatio) && (!mAxisRect))
            retainPixelPosition = false;

        QPointF pixel;
        if (retainPixelPosition)
            pixel = pixelPosition();

        mPositionTypeX = type;

        if (retainPixelPosition)
            setPixelPosition(pixel);
    }
}

/*!
  This method sets the position type of the Y coordinate to \a type.
  
  For a detailed description of what a position type is, see the documentation of \ref setType.
  
  \see setType, setTypeX
*/
void QCPItemPosition::setTypeY(QCPItemPosition::PositionType type)
{
    if (mPositionTypeY != type)
    {
        // if switching from or to coordinate type that isn't valid (e.g. because axes or axis rect
        // were deleted), don't try to recover the pixelPosition() because it would output a qDebug warning.
        bool retainPixelPosition = true;
        if ((mPositionTypeY == ptPlotCoords || type == ptPlotCoords) && (!mKeyAxis || !mValueAxis))
            retainPixelPosition = false;
        if ((mPositionTypeY == ptAxisRectRatio || type == ptAxisRectRatio) && (!mAxisRect))
            retainPixelPosition = false;

        QPointF pixel;
        if (retainPixelPosition)
            pixel = pixelPosition();

        mPositionTypeY = type;

        if (retainPixelPosition)
            setPixelPosition(pixel);
    }
}

/*!
  Sets the parent of this QCPItemPosition to \a parentAnchor. This means the position will now
  follow any position changes of the anchor. The local coordinate system of positions with a parent
  anchor always is absolute pixels, with (0, 0) being exactly on top of the parent anchor. (Hence
  the type shouldn't be set to \ref ptPlotCoords for positions with parent anchors.)
  
  if \a keepPixelPosition is true, the current pixel position of the QCPItemPosition is preserved
  during reparenting. If it's set to false, the coordinates are set to (0, 0), i.e. the position
  will be exactly on top of the parent anchor.
  
  To remove this QCPItemPosition from any parent anchor, set \a parentAnchor to 0.
  
  If the QCPItemPosition previously had no parent and the type is \ref ptPlotCoords, the type is
  set to \ref ptAbsolute, to keep the position in a valid state.
  
  This method sets the parent anchor for both X and Y directions. It is also possible to set
  different parents for X and Y, see \ref setParentAnchorX, \ref setParentAnchorY.
*/
bool QCPItemPosition::setParentAnchor(QCPItemAnchor *parentAnchor, bool keepPixelPosition)
{
    bool successX = setParentAnchorX(parentAnchor, keepPixelPosition);
    bool successY = setParentAnchorY(parentAnchor, keepPixelPosition);
    return successX && successY;
}

/*!
  This method sets the parent anchor of the X coordinate to \a parentAnchor.
  
  For a detailed description of what a parent anchor is, see the documentation of \ref setParentAnchor.
  
  \see setParentAnchor, setParentAnchorY
*/
bool QCPItemPosition::setParentAnchorX(QCPItemAnchor *parentAnchor, bool keepPixelPosition)
{
    // make sure self is not assigned as parent:
    if (parentAnchor == this)
    {
        qDebug() << Q_FUNC_INFO << "can't set self as parent anchor" << reinterpret_cast<quintptr>(parentAnchor);
        return false;
    }
    // make sure no recursive parent-child-relationships are created:
    QCPItemAnchor *currentParent = parentAnchor;
    while (currentParent)
    {
        if (QCPItemPosition *currentParentPos = currentParent->toQCPItemPosition())
        {
            // is a QCPItemPosition, might have further parent, so keep iterating
            if (currentParentPos == this)
            {
                qDebug() << Q_FUNC_INFO << "can't create recursive parent-child-relationship" << reinterpret_cast<quintptr>(parentAnchor);
                return false;
            }
            currentParent = currentParentPos->parentAnchorX();
        } else
        {
            // is a QCPItemAnchor, can't have further parent. Now make sure the parent items aren't the
            // same, to prevent a position being child of an anchor which itself depends on the position,
            // because they're both on the same item:
            if (currentParent->mParentItem == mParentItem)
            {
                qDebug() << Q_FUNC_INFO << "can't set parent to be an anchor which itself depends on this position" << reinterpret_cast<quintptr>(parentAnchor);
                return false;
            }
            break;
        }
    }

    // if previously no parent set and PosType is still ptPlotCoords, set to ptAbsolute:
    if (!mParentAnchorX && mPositionTypeX == ptPlotCoords)
        setTypeX(ptAbsolute);

    // save pixel position:
    QPointF pixelP;
    if (keepPixelPosition)
        pixelP = pixelPosition();
    // unregister at current parent anchor:
    if (mParentAnchorX)
        mParentAnchorX->removeChildX(this);
    // register at new parent anchor:
    if (parentAnchor)
        parentAnchor->addChildX(this);
    mParentAnchorX = parentAnchor;
    // restore pixel position under new parent:
    if (keepPixelPosition)
        setPixelPosition(pixelP);
    else
        setCoords(0, coords().y());
    return true;
}

/*!
  This method sets the parent anchor of the Y coordinate to \a parentAnchor.
  
  For a detailed description of what a parent anchor is, see the documentation of \ref setParentAnchor.
  
  \see setParentAnchor, setParentAnchorX
*/
bool QCPItemPosition::setParentAnchorY(QCPItemAnchor *parentAnchor, bool keepPixelPosition)
{
    // make sure self is not assigned as parent:
    if (parentAnchor == this)
    {
        qDebug() << Q_FUNC_INFO << "can't set self as parent anchor" << reinterpret_cast<quintptr>(parentAnchor);
        return false;
    }
    // make sure no recursive parent-child-relationships are created:
    QCPItemAnchor *currentParent = parentAnchor;
    while (currentParent)
    {
        if (QCPItemPosition *currentParentPos = currentParent->toQCPItemPosition())
        {
            // is a QCPItemPosition, might have further parent, so keep iterating
            if (currentParentPos == this)
            {
                qDebug() << Q_FUNC_INFO << "can't create recursive parent-child-relationship" << reinterpret_cast<quintptr>(parentAnchor);
                return false;
            }
            currentParent = currentParentPos->parentAnchorY();
        } else
        {
            // is a QCPItemAnchor, can't have further parent. Now make sure the parent items aren't the
            // same, to prevent a position being child of an anchor which itself depends on the position,
            // because they're both on the same item:
            if (currentParent->mParentItem == mParentItem)
            {
                qDebug() << Q_FUNC_INFO << "can't set parent to be an anchor which itself depends on this position" << reinterpret_cast<quintptr>(parentAnchor);
                return false;
            }
            break;
        }
    }

    // if previously no parent set and PosType is still ptPlotCoords, set to ptAbsolute:
    if (!mParentAnchorY && mPositionTypeY == ptPlotCoords)
        setTypeY(ptAbsolute);

    // save pixel position:
    QPointF pixelP;
    if (keepPixelPosition)
        pixelP = pixelPosition();
    // unregister at current parent anchor:
    if (mParentAnchorY)
        mParentAnchorY->removeChildY(this);
    // register at new parent anchor:
    if (parentAnchor)
        parentAnchor->addChildY(this);
    mParentAnchorY = parentAnchor;
    // restore pixel position under new parent:
    if (keepPixelPosition)
        setPixelPosition(pixelP);
    else
        setCoords(coords().x(), 0);
    return true;
}

/*!
  Sets the coordinates of this QCPItemPosition. What the coordinates mean, is defined by the type
  (\ref setType, \ref setTypeX, \ref setTypeY).
  
  For example, if the type is \ref ptAbsolute, \a key and \a value mean the x and y pixel position
  on the PF_GraphicView surface. In that case the origin (0, 0) is in the top left corner of the
  PF_GraphicView viewport. If the type is \ref ptPlotCoords, \a key and \a value mean a point in the
  plot coordinate system defined by the axes set by \ref setAxes. By default those are the
  PF_GraphicView's xAxis and yAxis. See the documentation of \ref setType for other available
  coordinate types and their meaning.
  
  If different types were configured for X and Y (\ref setTypeX, \ref setTypeY), \a key and \a
  value must also be provided in the different coordinate systems. Here, the X type refers to \a
  key, and the Y type refers to \a value.

  \see setPixelPosition
*/
void QCPItemPosition::setCoords(double key, double value)
{
    mKey = key;
    mValue = value;
}

/*! \overload

  Sets the coordinates as a QPointF \a pos where pos.x has the meaning of \a key and pos.y the
  meaning of \a value of the \ref setCoords(double key, double value) method.
*/
void QCPItemPosition::setCoords(const QPointF &pos)
{
    setCoords(pos.x(), pos.y());
}

/*!
  Returns the final absolute pixel position of the QCPItemPosition on the PF_GraphicView surface. It
  includes all effects of type (\ref setType) and possible parent anchors (\ref setParentAnchor).

  \see setPixelPosition
*/
QPointF QCPItemPosition::pixelPosition() const
{
    QPointF result;

    // determine X:
    switch (mPositionTypeX)
    {
    case ptAbsolute:
    {
        result.rx() = mKey;
        if (mParentAnchorX)
            result.rx() += mParentAnchorX->pixelPosition().x();
        break;
    }
    case ptViewportRatio:
    {
        result.rx() = mKey*mParentPlot->viewport().width();
        if (mParentAnchorX)
            result.rx() += mParentAnchorX->pixelPosition().x();
        else
            result.rx() += mParentPlot->viewport().left();
        break;
    }
    case ptAxisRectRatio:
    {
        if (mAxisRect)
        {
            result.rx() = mKey*mAxisRect.data()->width();
            if (mParentAnchorX)
                result.rx() += mParentAnchorX->pixelPosition().x();
            else
                result.rx() += mAxisRect.data()->left();
        } else
            qDebug() << Q_FUNC_INFO << "Item position type x is ptAxisRectRatio, but no axis rect was defined";
        break;
    }
    case ptPlotCoords:
    {
        if (mKeyAxis && mKeyAxis.data()->orientation() == Qt::Horizontal)
            result.rx() = mKeyAxis.data()->coordToPixel(mKey);
        else if (mValueAxis && mValueAxis.data()->orientation() == Qt::Horizontal)
            result.rx() = mValueAxis.data()->coordToPixel(mValue);
        else
            qDebug() << Q_FUNC_INFO << "Item position type x is ptPlotCoords, but no axes were defined";
        break;
    }
    }

    // determine Y:
    switch (mPositionTypeY)
    {
    case ptAbsolute:
    {
        result.ry() = mValue;
        if (mParentAnchorY)
            result.ry() += mParentAnchorY->pixelPosition().y();
        break;
    }
    case ptViewportRatio:
    {
        result.ry() = mValue*mParentPlot->viewport().height();
        if (mParentAnchorY)
            result.ry() += mParentAnchorY->pixelPosition().y();
        else
            result.ry() += mParentPlot->viewport().top();
        break;
    }
    case ptAxisRectRatio:
    {
        if (mAxisRect)
        {
            result.ry() = mValue*mAxisRect.data()->height();
            if (mParentAnchorY)
                result.ry() += mParentAnchorY->pixelPosition().y();
            else
                result.ry() += mAxisRect.data()->top();
        } else
            qDebug() << Q_FUNC_INFO << "Item position type y is ptAxisRectRatio, but no axis rect was defined";
        break;
    }
    case ptPlotCoords:
    {
        if (mKeyAxis && mKeyAxis.data()->orientation() == Qt::Vertical)
            result.ry() = mKeyAxis.data()->coordToPixel(mKey);
        else if (mValueAxis && mValueAxis.data()->orientation() == Qt::Vertical)
            result.ry() = mValueAxis.data()->coordToPixel(mValue);
        else
            qDebug() << Q_FUNC_INFO << "Item position type y is ptPlotCoords, but no axes were defined";
        break;
    }
    }

    return result;
}

/*!
  When \ref setType is \ref ptPlotCoords, this function may be used to specify the axes the
  coordinates set with \ref setCoords relate to. By default they are set to the initial xAxis and
  yAxis of the PF_GraphicView.
*/
void QCPItemPosition::setAxes(QCPAxis *keyAxis, QCPAxis *valueAxis)
{
    mKeyAxis = keyAxis;
    mValueAxis = valueAxis;
}

/*!
  When \ref setType is \ref ptAxisRectRatio, this function may be used to specify the axis rect the
  coordinates set with \ref setCoords relate to. By default this is set to the main axis rect of
  the PF_GraphicView.
*/
void QCPItemPosition::setAxisRect(QCPAxisRect *axisRect)
{
    mAxisRect = axisRect;
}

/*!
  Sets the apparent pixel position. This works no matter what type (\ref setType) this
  QCPItemPosition is or what parent-child situation it is in, as coordinates are transformed
  appropriately, to make the position finally appear at the specified pixel values.

  Only if the type is \ref ptAbsolute and no parent anchor is set, this function's effect is
  identical to that of \ref setCoords.

  \see pixelPosition, setCoords
*/
void QCPItemPosition::setPixelPosition(const QPointF &pixelPosition)
{
    double x = pixelPosition.x();
    double y = pixelPosition.y();

    switch (mPositionTypeX)
    {
    case ptAbsolute:
    {
        if (mParentAnchorX)
            x -= mParentAnchorX->pixelPosition().x();
        break;
    }
    case ptViewportRatio:
    {
        if (mParentAnchorX)
            x -= mParentAnchorX->pixelPosition().x();
        else
            x -= mParentPlot->viewport().left();
        x /= (double)mParentPlot->viewport().width();
        break;
    }
    case ptAxisRectRatio:
    {
        if (mAxisRect)
        {
            if (mParentAnchorX)
                x -= mParentAnchorX->pixelPosition().x();
            else
                x -= mAxisRect.data()->left();
            x /= (double)mAxisRect.data()->width();
        } else
            qDebug() << Q_FUNC_INFO << "Item position type x is ptAxisRectRatio, but no axis rect was defined";
        break;
    }
    case ptPlotCoords:
    {
        if (mKeyAxis && mKeyAxis.data()->orientation() == Qt::Horizontal)
            x = mKeyAxis.data()->pixelToCoord(x);
        else if (mValueAxis && mValueAxis.data()->orientation() == Qt::Horizontal)
            y = mValueAxis.data()->pixelToCoord(x);
        else
            qDebug() << Q_FUNC_INFO << "Item position type x is ptPlotCoords, but no axes were defined";
        break;
    }
    }

    switch (mPositionTypeY)
    {
    case ptAbsolute:
    {
        if (mParentAnchorY)
            y -= mParentAnchorY->pixelPosition().y();
        break;
    }
    case ptViewportRatio:
    {
        if (mParentAnchorY)
            y -= mParentAnchorY->pixelPosition().y();
        else
            y -= mParentPlot->viewport().top();
        y /= (double)mParentPlot->viewport().height();
        break;
    }
    case ptAxisRectRatio:
    {
        if (mAxisRect)
        {
            if (mParentAnchorY)
                y -= mParentAnchorY->pixelPosition().y();
            else
                y -= mAxisRect.data()->top();
            y /= (double)mAxisRect.data()->height();
        } else
            qDebug() << Q_FUNC_INFO << "Item position type y is ptAxisRectRatio, but no axis rect was defined";
        break;
    }
    case ptPlotCoords:
    {
        if (mKeyAxis && mKeyAxis.data()->orientation() == Qt::Vertical)
            x = mKeyAxis.data()->pixelToCoord(y);
        else if (mValueAxis && mValueAxis.data()->orientation() == Qt::Vertical)
            y = mValueAxis.data()->pixelToCoord(y);
        else
            qDebug() << Q_FUNC_INFO << "Item position type y is ptPlotCoords, but no axes were defined";
        break;
    }
    }

    setCoords(x, y);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPAbstractItem
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPAbstractItem
  \brief The abstract base class for all items in a plot.
  
  In PF_GraphicView, items are supplemental graphical elements that are neither plottables
  (QCPAbstractPlottable) nor axes (QCPAxis). While plottables are always tied to two axes and thus
  plot coordinates, items can also be placed in absolute coordinates independent of any axes. Each
  specific item has at least one QCPItemPosition member which controls the positioning. Some items
  are defined by more than one coordinate and thus have two or more QCPItemPosition members (For
  example, QCPItemRect has \a topLeft and \a bottomRight).
  
  This abstract base class defines a very basic interface like visibility and clipping. Since this
  class is abstract, it can't be instantiated. Use one of the subclasses or create a subclass
  yourself to create new items.
  
  The built-in items are:
  <table>
  <tr><td>QCPItemLine</td><td>A line defined by a start and an end point. May have different ending styles on each side (e.g. arrows).</td></tr>
  <tr><td>QCPItemStraightLine</td><td>A straight line defined by a start and a direction point. Unlike QCPItemLine, the straight line is infinitely long and has no endings.</td></tr>
  <tr><td>QCPItemCurve</td><td>A curve defined by start, end and two intermediate control points. May have different ending styles on each side (e.g. arrows).</td></tr>
  <tr><td>QCPItemRect</td><td>A rectangle</td></tr>
  <tr><td>QCPItemEllipse</td><td>An ellipse</td></tr>
  <tr><td>QCPItemPixmap</td><td>An arbitrary pixmap</td></tr>
  <tr><td>QCPItemText</td><td>A text label</td></tr>
  <tr><td>QCPItemBracket</td><td>A bracket which may be used to reference/highlight certain parts in the plot.</td></tr>
  <tr><td>QCPItemTracer</td><td>An item that can be attached to a QCPGraph and sticks to its data points, given a key coordinate.</td></tr>
  </table>
  
  \section items-clipping Clipping

  Items are by default clipped to the main axis rect (they are only visible inside the axis rect).
  To make an item visible outside that axis rect, disable clipping via \ref setClipToAxisRect
  "setClipToAxisRect(false)".

  On the other hand if you want the item to be clipped to a different axis rect, specify it via
  \ref setClipAxisRect. This clipAxisRect property of an item is only used for clipping behaviour, and
  in principle is independent of the coordinate axes the item might be tied to via its position
  members (\ref QCPItemPosition::setAxes). However, it is common that the axis rect for clipping
  also contains the axes used for the item positions.
  
  \section items-using Using items
  
  First you instantiate the item you want to use and add it to the plot:
  \snippet documentation/doc-code-snippets/mainwindow.cpp qcpitemline-creation-1
  by default, the positions of the item are bound to the x- and y-Axis of the plot. So we can just
  set the plot coordinates where the line should start/end:
  \snippet documentation/doc-code-snippets/mainwindow.cpp qcpitemline-creation-2
  If we don't want the line to be positioned in plot coordinates but a different coordinate system,
  e.g. absolute pixel positions on the PF_GraphicView surface, we need to change the position type like this:
  \snippet documentation/doc-code-snippets/mainwindow.cpp qcpitemline-creation-3
  Then we can set the coordinates, this time in pixels:
  \snippet documentation/doc-code-snippets/mainwindow.cpp qcpitemline-creation-4
  and make the line visible on the entire PF_GraphicView, by disabling clipping to the axis rect:
  \snippet documentation/doc-code-snippets/mainwindow.cpp qcpitemline-creation-5
  
  For more advanced plots, it is even possible to set different types and parent anchors per X/Y
  coordinate of an item position, using for example \ref QCPItemPosition::setTypeX or \ref
  QCPItemPosition::setParentAnchorX. For details, see the documentation of \ref QCPItemPosition.
  
  \section items-subclassing Creating own items
  
  To create an own item, you implement a subclass of QCPAbstractItem. These are the pure
  virtual functions, you must implement:
  \li \ref selectTest
  \li \ref draw
  
  See the documentation of those functions for what they need to do.
  
  \subsection items-positioning Allowing the item to be positioned
  
  As mentioned, item positions are represented by QCPItemPosition members. Let's assume the new item shall
  have only one point as its position (as opposed to two like a rect or multiple like a polygon). You then add
  a public member of type QCPItemPosition like so:
  
  \code QCPItemPosition * const myPosition;\endcode
  
  the const makes sure the pointer itself can't be modified from the user of your new item (the QCPItemPosition
  instance it points to, can be modified, of course).
  The initialization of this pointer is made easy with the \ref createPosition function. Just assign
  the return value of this function to each QCPItemPosition in the constructor of your item. \ref createPosition
  takes a string which is the name of the position, typically this is identical to the variable name.
  For example, the constructor of QCPItemExample could look like this:
  
  \code
  QCPItemExample::QCPItemExample(PF_GraphicView *parentPlot) :
    QCPAbstractItem(parentPlot),
    myPosition(createPosition("myPosition"))
  {
    // other constructor code
  }
  \endcode
  
  \subsection items-drawing The draw function
  
  To give your item a visual representation, reimplement the \ref draw function and use the passed
  QCPPainter to draw the item. You can retrieve the item position in pixel coordinates from the
  position member(s) via \ref QCPItemPosition::pixelPosition.

  To optimize performance you should calculate a bounding rect first (don't forget to take the pen
  width into account), check whether it intersects the \ref clipRect, and only draw the item at all
  if this is the case.
  
  \subsection items-selection The selectTest function
  
  Your implementation of the \ref selectTest function may use the helpers \ref
  QCPVector2D::distanceSquaredToLine and \ref rectDistance. With these, the implementation of the
  selection test becomes significantly simpler for most items. See the documentation of \ref
  selectTest for what the function parameters mean and what the function should return.
  
  \subsection anchors Providing anchors
  
  Providing anchors (QCPItemAnchor) starts off like adding a position. First you create a public
  member, e.g.
  
  \code QCPItemAnchor * const bottom;\endcode

  and create it in the constructor with the \ref createAnchor function, assigning it a name and an
  anchor id (an integer enumerating all anchors on the item, you may create an own enum for this).
  Since anchors can be placed anywhere, relative to the item's position(s), your item needs to
  provide the position of every anchor with the reimplementation of the \ref anchorPixelPosition(int
  anchorId) function.
  
  In essence the QCPItemAnchor is merely an intermediary that itself asks your item for the pixel
  position when anything attached to the anchor needs to know the coordinates.
*/

/* start of documentation of inline functions */

/*! \fn QList<QCPItemPosition*> QCPAbstractItem::positions() const
  
  Returns all positions of the item in a list.
  
  \see anchors, position
*/

/*! \fn QList<QCPItemAnchor*> QCPAbstractItem::anchors() const
  
  Returns all anchors of the item in a list. Note that since a position (QCPItemPosition) is always
  also an anchor, the list will also contain the positions of this item.
  
  \see positions, anchor
*/

/* end of documentation of inline functions */
/* start documentation of pure virtual functions */

/*! \fn void QCPAbstractItem::draw(QCPPainter *painter) = 0
  \internal
  
  Draws this item with the provided \a painter.
  
  The cliprect of the provided painter is set to the rect returned by \ref clipRect before this
  function is called. The clipRect depends on the clipping settings defined by \ref
  setClipToAxisRect and \ref setClipAxisRect.
*/

/* end documentation of pure virtual functions */
/* start documentation of signals */

/*! \fn void QCPAbstractItem::selectionChanged(bool selected)
  This signal is emitted when the selection state of this item has changed, either by user interaction
  or by a direct call to \ref setSelected.
*/

/* end documentation of signals */

/*!
  Base class constructor which initializes base class members.
*/
QCPAbstractItem::QCPAbstractItem(PF_GraphicView *parentPlot) :
    QCPLayerable(parentPlot),
    mClipToAxisRect(false),
    mSelectable(true),
    mSelected(false)
{
    parentPlot->registerItem(this);

    QList<QCPAxisRect*> rects = parentPlot->axisRects();
    if (rects.size() > 0)
    {
        setClipToAxisRect(true);
        setClipAxisRect(rects.first());
    }
}

QCPAbstractItem::~QCPAbstractItem()
{
    // don't delete mPositions because every position is also an anchor and thus in mAnchors
    qDeleteAll(mAnchors);
}

/* can't make this a header inline function, because QPointer breaks with forward declared types, see QTBUG-29588 */
QCPAxisRect *QCPAbstractItem::clipAxisRect() const
{
    return mClipAxisRect.data();
}

/*!
  Sets whether the item shall be clipped to an axis rect or whether it shall be visible on the
  entire PF_GraphicView. The axis rect can be set with \ref setClipAxisRect.
  
  \see setClipAxisRect
*/
void QCPAbstractItem::setClipToAxisRect(bool clip)
{
    mClipToAxisRect = clip;
    if (mClipToAxisRect)
        setParentLayerable(mClipAxisRect.data());
}

/*!
  Sets the clip axis rect. It defines the rect that will be used to clip the item when \ref
  setClipToAxisRect is set to true.
  
  \see setClipToAxisRect
*/
void QCPAbstractItem::setClipAxisRect(QCPAxisRect *rect)
{
    mClipAxisRect = rect;
    if (mClipToAxisRect)
        setParentLayerable(mClipAxisRect.data());
}

/*!
  Sets whether the user can (de-)select this item by clicking on the PF_GraphicView surface.
  (When \ref PF_GraphicView::setInteractions contains PF_GraphicView::iSelectItems.)
  
  However, even when \a selectable was set to false, it is possible to set the selection manually,
  by calling \ref setSelected.
  
  \see PF_GraphicView::setInteractions, setSelected
*/
void QCPAbstractItem::setSelectable(bool selectable)
{
    if (mSelectable != selectable)
    {
        mSelectable = selectable;
        emit selectableChanged(mSelectable);
    }
}

/*!
  Sets whether this item is selected or not. When selected, it might use a different visual
  appearance (e.g. pen and brush), this depends on the specific item though.

  The entire selection mechanism for items is handled automatically when \ref
  PF_GraphicView::setInteractions contains PF_GraphicView::iSelectItems. You only need to call this
  function when you wish to change the selection state manually.
  
  This function can change the selection state even when \ref setSelectable was set to false.
  
  emits the \ref selectionChanged signal when \a selected is different from the previous selection state.
  
  \see setSelectable, selectTest
*/
void QCPAbstractItem::setSelected(bool selected)
{
    if (mSelected != selected)
    {
        mSelected = selected;
        emit selectionChanged(mSelected);
    }
}

/*!
  Returns the QCPItemPosition with the specified \a name. If this item doesn't have a position by
  that name, returns 0.
  
  This function provides an alternative way to access item positions. Normally, you access
  positions direcly by their member pointers (which typically have the same variable name as \a
  name).
  
  \see positions, anchor
*/
QCPItemPosition *QCPAbstractItem::position(const QString &name) const
{
    for (int i=0; i<mPositions.size(); ++i)
    {
        if (mPositions.at(i)->name() == name)
            return mPositions.at(i);
    }
    qDebug() << Q_FUNC_INFO << "position with name not found:" << name;
    return nullptr;
}

/*!
  Returns the QCPItemAnchor with the specified \a name. If this item doesn't have an anchor by
  that name, returns 0.
  
  This function provides an alternative way to access item anchors. Normally, you access
  anchors direcly by their member pointers (which typically have the same variable name as \a
  name).
  
  \see anchors, position
*/
QCPItemAnchor *QCPAbstractItem::anchor(const QString &name) const
{
    for (int i=0; i<mAnchors.size(); ++i)
    {
        if (mAnchors.at(i)->name() == name)
            return mAnchors.at(i);
    }
    qDebug() << Q_FUNC_INFO << "anchor with name not found:" << name;
    return nullptr;
}

/*!
  Returns whether this item has an anchor with the specified \a name.
  
  Note that you can check for positions with this function, too. This is because every position is
  also an anchor (QCPItemPosition inherits from QCPItemAnchor).
  
  \see anchor, position
*/
bool QCPAbstractItem::hasAnchor(const QString &name) const
{
    for (int i=0; i<mAnchors.size(); ++i)
    {
        if (mAnchors.at(i)->name() == name)
            return true;
    }
    return false;
}

/*! \internal
  
  Returns the rect the visual representation of this item is clipped to. This depends on the
  current setting of \ref setClipToAxisRect as well as the axis rect set with \ref setClipAxisRect.
  
  If the item is not clipped to an axis rect, PF_GraphicView's viewport rect is returned.
  
  \see draw
*/
QRect QCPAbstractItem::clipRect() const
{
    if (mClipToAxisRect && mClipAxisRect)
        return mClipAxisRect.data()->rect();
    else
        return mParentPlot->viewport();
}

/*! \internal

  A convenience function to easily set the QPainter::Antialiased hint on the provided \a painter
  before drawing item lines.

  This is the antialiasing state the painter passed to the \ref draw method is in by default.
  
  This function takes into account the local setting of the antialiasing flag as well as the
  overrides set with \ref PF_GraphicView::setAntialiasedElements and \ref
  PF_GraphicView::setNotAntialiasedElements.
  
  \see setAntialiased
*/
void QCPAbstractItem::applyDefaultAntialiasingHint(QCPPainter *painter) const
{
    applyAntialiasingHint(painter, mAntialiased, QCP::aeItems);
}

/*! \internal

  A convenience function which returns the selectTest value for a specified \a rect and a specified
  click position \a pos. \a filledRect defines whether a click inside the rect should also be
  considered a hit or whether only the rect border is sensitive to hits.
  
  This function may be used to help with the implementation of the \ref selectTest function for
  specific items.
  
  For example, if your item consists of four rects, call this function four times, once for each
  rect, in your \ref selectTest reimplementation. Finally, return the minimum (non -1) of all four
  returned values.
*/
double QCPAbstractItem::rectDistance(const QRectF &rect, const QPointF &pos, bool filledRect) const
{
    double result = -1;

    // distance to border:
    QList<QLineF> lines;
    lines << QLineF(rect.topLeft(), rect.topRight()) << QLineF(rect.bottomLeft(), rect.bottomRight())
          << QLineF(rect.topLeft(), rect.bottomLeft()) << QLineF(rect.topRight(), rect.bottomRight());
    double minDistSqr = (std::numeric_limits<double>::max)();
    for (int i=0; i<lines.size(); ++i)
    {
        double distSqr = QCPVector2D(pos).distanceSquaredToLine(lines.at(i).p1(), lines.at(i).p2());
        if (distSqr < minDistSqr)
            minDistSqr = distSqr;
    }
    result = qSqrt(minDistSqr);

    // filled rect, allow click inside to count as hit:
    if (filledRect && result > mParentPlot->selectionTolerance()*0.99)
    {
        if (rect.contains(pos))
            result = mParentPlot->selectionTolerance()*0.99;
    }
    return result;
}

/*! \internal

  Returns the pixel position of the anchor with Id \a anchorId. This function must be reimplemented in
  item subclasses if they want to provide anchors (QCPItemAnchor).
  
  For example, if the item has two anchors with id 0 and 1, this function takes one of these anchor
  ids and returns the respective pixel points of the specified anchor.
  
  \see createAnchor
*/
QPointF QCPAbstractItem::anchorPixelPosition(int anchorId) const
{
    qDebug() << Q_FUNC_INFO << "called on item which shouldn't have any anchors (this method not reimplemented). anchorId" << anchorId;
    return QPointF();
}

/*! \internal

  Creates a QCPItemPosition, registers it with this item and returns a pointer to it. The specified
  \a name must be a unique string that is usually identical to the variable name of the position
  member (This is needed to provide the name-based \ref position access to positions).
  
  Don't delete positions created by this function manually, as the item will take care of it.
  
  Use this function in the constructor (initialization list) of the specific item subclass to
  create each position member. Don't create QCPItemPositions with \b new yourself, because they
  won't be registered with the item properly.
  
  \see createAnchor
*/
QCPItemPosition *QCPAbstractItem::createPosition(const QString &name)
{
    if (hasAnchor(name))
        qDebug() << Q_FUNC_INFO << "anchor/position with name exists already:" << name;
    QCPItemPosition *newPosition = new QCPItemPosition(mParentPlot, this, name);
    mPositions.append(newPosition);
    mAnchors.append(newPosition); // every position is also an anchor
    newPosition->setAxes(mParentPlot->xAxis, mParentPlot->yAxis);
    newPosition->setType(QCPItemPosition::ptPlotCoords);
    if (mParentPlot->axisRect())
        newPosition->setAxisRect(mParentPlot->axisRect());
    newPosition->setCoords(0, 0);
    return newPosition;
}

/*! \internal

  Creates a QCPItemAnchor, registers it with this item and returns a pointer to it. The specified
  \a name must be a unique string that is usually identical to the variable name of the anchor
  member (This is needed to provide the name based \ref anchor access to anchors).
  
  The \a anchorId must be a number identifying the created anchor. It is recommended to create an
  enum (e.g. "AnchorIndex") for this on each item that uses anchors. This id is used by the anchor
  to identify itself when it calls QCPAbstractItem::anchorPixelPosition. That function then returns
  the correct pixel coordinates for the passed anchor id.
  
  Don't delete anchors created by this function manually, as the item will take care of it.
  
  Use this function in the constructor (initialization list) of the specific item subclass to
  create each anchor member. Don't create QCPItemAnchors with \b new yourself, because then they
  won't be registered with the item properly.
  
  \see createPosition
*/
QCPItemAnchor *QCPAbstractItem::createAnchor(const QString &name, int anchorId)
{
    if (hasAnchor(name))
        qDebug() << Q_FUNC_INFO << "anchor/position with name exists already:" << name;
    QCPItemAnchor *newAnchor = new QCPItemAnchor(mParentPlot, this, name, anchorId);
    mAnchors.append(newAnchor);
    return newAnchor;
}

/* inherits documentation from base class */
void QCPAbstractItem::selectEvent(QMouseEvent *event, bool additive, const QVariant &details, bool *selectionStateChanged)
{
    Q_UNUSED(event)
    Q_UNUSED(details)
    if (mSelectable)
    {
        bool selBefore = mSelected;
        setSelected(additive ? !mSelected : true);
        if (selectionStateChanged)
            *selectionStateChanged = mSelected != selBefore;
    }
}

/* inherits documentation from base class */
void QCPAbstractItem::deselectEvent(bool *selectionStateChanged)
{
    if (mSelectable)
    {
        bool selBefore = mSelected;
        setSelected(false);
        if (selectionStateChanged)
            *selectionStateChanged = mSelected != selBefore;
    }
}

/* inherits documentation from base class */
QCP::Interaction QCPAbstractItem::selectionCategory() const
{
    return QCP::iSelectItems;
}
/* end of 'src/item.cpp' */


/* including file 'src/core.cpp', size 126207                                */
/* commit ce344b3f96a62e5f652585e55f1ae7c7883cd45b 2018-06-25 01:03:39 +0200 */

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// PF_GraphicView
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class PF_GraphicView
  
  \brief The central class of the library. This is the QWidget which displays the plot and
  interacts with the user.
  
  For tutorials on how to use PF_GraphicView, see the website\n
  http://www.PF_GraphicView.com/
*/

/* start of documentation of inline functions */

/*! \fn QCPSelectionRect *PF_GraphicView::selectionRect() const
  
  Allows access to the currently used QCPSelectionRect instance (or subclass thereof), that is used
  to handle and draw selection rect interactions (see \ref setSelectionRectMode).
  
  \see setSelectionRect
*/

/*! \fn QCPLayoutGrid *PF_GraphicView::plotLayout() const
  
  Returns the top level layout of this PF_GraphicView instance. It is a \ref QCPLayoutGrid, initially containing just
  one cell with the main QCPAxisRect inside.
*/

/* end of documentation of inline functions */
/* start of documentation of signals */

/*! \fn void PF_GraphicView::mouseDoubleClick(QMouseEvent *event)

  This signal is emitted when the PF_GraphicView receives a mouse double click event.
*/

/*! \fn void PF_GraphicView::mousePress(QMouseEvent *event)

  This signal is emitted when the PF_GraphicView receives a mouse press event.
  
  It is emitted before PF_GraphicView handles any other mechanism like range dragging. So a slot
  connected to this signal can still influence the behaviour e.g. with \ref QCPAxisRect::setRangeDrag or \ref
  QCPAxisRect::setRangeDragAxes.
*/

/*! \fn void PF_GraphicView::mouseMove(QMouseEvent *event)

  This signal is emitted when the PF_GraphicView receives a mouse move event.
  
  It is emitted before PF_GraphicView handles any other mechanism like range dragging. So a slot
  connected to this signal can still influence the behaviour e.g. with \ref QCPAxisRect::setRangeDrag or \ref
  QCPAxisRect::setRangeDragAxes.
  
  \warning It is discouraged to change the drag-axes with \ref QCPAxisRect::setRangeDragAxes here,
  because the dragging starting point was saved the moment the mouse was pressed. Thus it only has
  a meaning for the range drag axes that were set at that moment. If you want to change the drag
  axes, consider doing this in the \ref mousePress signal instead.
*/

/*! \fn void PF_GraphicView::mouseRelease(QMouseEvent *event)

  This signal is emitted when the PF_GraphicView receives a mouse release event.
  
  It is emitted before PF_GraphicView handles any other mechanisms like object selection. So a
  slot connected to this signal can still influence the behaviour e.g. with \ref setInteractions or
  \ref QCPAbstractPlottable::setSelectable.
*/

/*! \fn void PF_GraphicView::mouseWheel(QMouseEvent *event)

  This signal is emitted when the PF_GraphicView receives a mouse wheel event.
  
  It is emitted before PF_GraphicView handles any other mechanisms like range zooming. So a slot
  connected to this signal can still influence the behaviour e.g. with \ref QCPAxisRect::setRangeZoom, \ref
  QCPAxisRect::setRangeZoomAxes or \ref QCPAxisRect::setRangeZoomFactor.
*/

/*! \fn void PF_GraphicView::plottableClick(QCPAbstractPlottable *plottable, int dataIndex, QMouseEvent *event)

  This signal is emitted when a plottable is clicked.

  \a event is the mouse event that caused the click and \a plottable is the plottable that received
  the click. The parameter \a dataIndex indicates the data point that was closest to the click
  position.

  \see plottableDoubleClick
*/

/*! \fn void PF_GraphicView::plottableDoubleClick(QCPAbstractPlottable *plottable, int dataIndex, QMouseEvent *event)

  This signal is emitted when a plottable is double clicked.

  \a event is the mouse event that caused the click and \a plottable is the plottable that received
  the click. The parameter \a dataIndex indicates the data point that was closest to the click
  position.

  \see plottableClick
*/

/*! \fn void PF_GraphicView::itemClick(QCPAbstractItem *item, QMouseEvent *event)
  
  This signal is emitted when an item is clicked.

  \a event is the mouse event that caused the click and \a item is the item that received the
  click.
  
  \see itemDoubleClick
*/

/*! \fn void PF_GraphicView::itemDoubleClick(QCPAbstractItem *item, QMouseEvent *event)
  
  This signal is emitted when an item is double clicked.
  
  \a event is the mouse event that caused the click and \a item is the item that received the
  click.
  
  \see itemClick
*/

/*! \fn void PF_GraphicView::axisClick(QCPAxis *axis, QCPAxis::SelectablePart part, QMouseEvent *event)
  
  This signal is emitted when an axis is clicked.
  
  \a event is the mouse event that caused the click, \a axis is the axis that received the click and
  \a part indicates the part of the axis that was clicked.
  
  \see axisDoubleClick
*/

/*! \fn void PF_GraphicView::axisDoubleClick(QCPAxis *axis, QCPAxis::SelectablePart part, QMouseEvent *event)

  This signal is emitted when an axis is double clicked.
  
  \a event is the mouse event that caused the click, \a axis is the axis that received the click and
  \a part indicates the part of the axis that was clicked.
  
  \see axisClick
*/

/*! \fn void PF_GraphicView::legendClick(QCPLegend *legend, QCPAbstractLegendItem *item, QMouseEvent *event)

  This signal is emitted when a legend (item) is clicked.
  
  \a event is the mouse event that caused the click, \a legend is the legend that received the
  click and \a item is the legend item that received the click. If only the legend and no item is
  clicked, \a item is 0. This happens for a click inside the legend padding or the space between
  two items.
  
  \see legendDoubleClick
*/

/*! \fn void PF_GraphicView::legendDoubleClick(QCPLegend *legend,  QCPAbstractLegendItem *item, QMouseEvent *event)

  This signal is emitted when a legend (item) is double clicked.
  
  \a event is the mouse event that caused the click, \a legend is the legend that received the
  click and \a item is the legend item that received the click. If only the legend and no item is
  clicked, \a item is 0. This happens for a click inside the legend padding or the space between
  two items.
  
  \see legendClick
*/

/*! \fn void PF_GraphicView::selectionChangedByUser()
  
  This signal is emitted after the user has changed the selection in the PF_GraphicView, e.g. by
  clicking. It is not emitted when the selection state of an object has changed programmatically by
  a direct call to <tt>setSelected()</tt>/<tt>setSelection()</tt> on an object or by calling \ref
  deselectAll.
  
  In addition to this signal, selectable objects also provide individual signals, for example \ref
  QCPAxis::selectionChanged or \ref QCPAbstractPlottable::selectionChanged. Note that those signals
  are emitted even if the selection state is changed programmatically.
  
  See the documentation of \ref setInteractions for details about the selection mechanism.
  
  \see selectedPlottables, selectedGraphs, selectedItems, selectedAxes, selectedLegends
*/

/*! \fn void PF_GraphicView::beforeReplot()
  
  This signal is emitted immediately before a replot takes place (caused by a call to the slot \ref
  replot).
  
  It is safe to mutually connect the replot slot with this signal on two PF_GraphicViews to make them
  replot synchronously, it won't cause an infinite recursion.
  
  \see replot, afterReplot
*/

/*! \fn void PF_GraphicView::afterReplot()
  
  This signal is emitted immediately after a replot has taken place (caused by a call to the slot \ref
  replot).
  
  It is safe to mutually connect the replot slot with this signal on two PF_GraphicViews to make them
  replot synchronously, it won't cause an infinite recursion.
  
  \see replot, beforeReplot
*/

/* end of documentation of signals */
/* start of documentation of public members */


/* end of 'src/core.cpp' */

//amalgamation: add plottable1d.cpp

/* including file 'src/colorgradient.cpp', size 25342                        */
/* commit ce344b3f96a62e5f652585e55f1ae7c7883cd45b 2018-06-25 01:03:39 +0200 */


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPColorGradient
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPColorGradient
  \brief Defines a color gradient for use with e.g. \ref QCPColorMap
  
  This class describes a color gradient which can be used to encode data with color. For example,
  QCPColorMap and QCPColorScale have \ref QCPColorMap::setGradient "setGradient" methods which
  take an instance of this class. Colors are set with \ref setColorStopAt(double position, const QColor &color)
  with a \a position from 0 to 1. In between these defined color positions, the
  color will be interpolated linearly either in RGB or HSV space, see \ref setColorInterpolation.

  Alternatively, load one of the preset color gradients shown in the image below, with \ref
  loadPreset, or by directly specifying the preset in the constructor.
  
  Apart from red, green and blue components, the gradient also interpolates the alpha values of the
  configured color stops. This allows to display some portions of the data range as transparent in
  the plot.
  
  \image html QCPColorGradient.png
  
  The \ref QCPColorGradient(GradientPreset preset) constructor allows directly converting a \ref
  GradientPreset to a QCPColorGradient. This means that you can directly pass \ref GradientPreset
  to all the \a setGradient methods, e.g.:
  \snippet documentation/doc-code-snippets/mainwindow.cpp qcpcolorgradient-setgradient
  
  The total number of levels used in the gradient can be set with \ref setLevelCount. Whether the
  color gradient shall be applied periodically (wrapping around) to data values that lie outside
  the data range specified on the plottable instance can be controlled with \ref setPeriodic.
*/

/*!
  Constructs a new, empty QCPColorGradient with no predefined color stops. You can add own color
  stops with \ref setColorStopAt.

  The color level count is initialized to 350.
*/
QCPColorGradient::QCPColorGradient() :
    mLevelCount(350),
    mColorInterpolation(ciRGB),
    mPeriodic(false),
    mColorBufferInvalidated(true)
{
    mColorBuffer.fill(qRgb(0, 0, 0), mLevelCount);
}

/*!
  Constructs a new QCPColorGradient initialized with the colors and color interpolation according
  to \a preset.

  The color level count is initialized to 350.
*/
QCPColorGradient::QCPColorGradient(GradientPreset preset) :
    mLevelCount(350),
    mColorInterpolation(ciRGB),
    mPeriodic(false),
    mColorBufferInvalidated(true)
{
    mColorBuffer.fill(qRgb(0, 0, 0), mLevelCount);
    loadPreset(preset);
}

/* undocumented operator */
bool QCPColorGradient::operator==(const QCPColorGradient &other) const
{
    return ((other.mLevelCount == this->mLevelCount) &&
            (other.mColorInterpolation == this->mColorInterpolation) &&
            (other.mPeriodic == this->mPeriodic) &&
            (other.mColorStops == this->mColorStops));
}

/*!
  Sets the number of discretization levels of the color gradient to \a n. The default is 350 which
  is typically enough to create a smooth appearance. The minimum number of levels is 2.

  \image html QCPColorGradient-levelcount.png
*/
void QCPColorGradient::setLevelCount(int n)
{
    if (n < 2)
    {
        qDebug() << Q_FUNC_INFO << "n must be greater or equal 2 but was" << n;
        n = 2;
    }
    if (n != mLevelCount)
    {
        mLevelCount = n;
        mColorBufferInvalidated = true;
    }
}

/*!
  Sets at which positions from 0 to 1 which color shall occur. The positions are the keys, the
  colors are the values of the passed QMap \a colorStops. In between these color stops, the color
  is interpolated according to \ref setColorInterpolation.
  
  A more convenient way to create a custom gradient may be to clear all color stops with \ref
  clearColorStops (or creating a new, empty QCPColorGradient) and then adding them one by one with
  \ref setColorStopAt.
  
  \see clearColorStops
*/
void QCPColorGradient::setColorStops(const QMap<double, QColor> &colorStops)
{
    mColorStops = colorStops;
    mColorBufferInvalidated = true;
}

/*!
  Sets the \a color the gradient will have at the specified \a position (from 0 to 1). In between
  these color stops, the color is interpolated according to \ref setColorInterpolation.
  
  \see setColorStops, clearColorStops
*/
void QCPColorGradient::setColorStopAt(double position, const QColor &color)
{
    mColorStops.insert(position, color);
    mColorBufferInvalidated = true;
}

/*!
  Sets whether the colors in between the configured color stops (see \ref setColorStopAt) shall be
  interpolated linearly in RGB or in HSV color space.
  
  For example, a sweep in RGB space from red to green will have a muddy brown intermediate color,
  whereas in HSV space the intermediate color is yellow.
*/
void QCPColorGradient::setColorInterpolation(QCPColorGradient::ColorInterpolation interpolation)
{
    if (interpolation != mColorInterpolation)
    {
        mColorInterpolation = interpolation;
        mColorBufferInvalidated = true;
    }
}

/*!
  Sets whether data points that are outside the configured data range (e.g. \ref
  QCPColorMap::setDataRange) are colored by periodically repeating the color gradient or whether
  they all have the same color, corresponding to the respective gradient boundary color.
  
  \image html QCPColorGradient-periodic.png
  
  As shown in the image above, gradients that have the same start and end color are especially
  suitable for a periodic gradient mapping, since they produce smooth color transitions throughout
  the color map. A preset that has this property is \ref gpHues.
  
  In practice, using periodic color gradients makes sense when the data corresponds to a periodic
  dimension, such as an angle or a phase. If this is not the case, the color encoding might become
  ambiguous, because multiple different data values are shown as the same color.
*/
void QCPColorGradient::setPeriodic(bool enabled)
{
    mPeriodic = enabled;
}

/*! \overload
  
  This method is used to quickly convert a \a data array to colors. The colors will be output in
  the array \a scanLine. Both \a data and \a scanLine must have the length \a n when passed to this
  function. The data range that shall be used for mapping the data value to the gradient is passed
  in \a range. \a logarithmic indicates whether the data values shall be mapped to colors
  logarithmically.

  if \a data actually contains 2D-data linearized via <tt>[row*columnCount + column]</tt>, you can
  set \a dataIndexFactor to <tt>columnCount</tt> to convert a column instead of a row of the data
  array, in \a scanLine. \a scanLine will remain a regular (1D) array. This works because \a data
  is addressed <tt>data[i*dataIndexFactor]</tt>.
  
  Use the overloaded method to additionally provide alpha map data.

  The QRgb values that are placed in \a scanLine have their r, g and b components premultiplied
  with alpha (see QImage::Format_ARGB32_Premultiplied).
*/
void QCPColorGradient::colorize(const double *data, const QCPRange &range, QRgb *scanLine, int n, int dataIndexFactor, bool logarithmic)
{
    // If you change something here, make sure to also adapt color() and the other colorize() overload
    if (!data)
    {
        qDebug() << Q_FUNC_INFO << "null pointer given as data";
        return;
    }
    if (!scanLine)
    {
        qDebug() << Q_FUNC_INFO << "null pointer given as scanLine";
        return;
    }
    if (mColorBufferInvalidated)
        updateColorBuffer();

    if (!logarithmic)
    {
        const double posToIndexFactor = (mLevelCount-1)/range.size();
        if (mPeriodic)
        {
            for (int i=0; i<n; ++i)
            {
                int index = (int)((data[dataIndexFactor*i]-range.lower)*posToIndexFactor) % mLevelCount;
                if (index < 0)
                    index += mLevelCount;
                scanLine[i] = mColorBuffer.at(index);
            }
        } else
        {
            for (int i=0; i<n; ++i)
            {
                int index = (data[dataIndexFactor*i]-range.lower)*posToIndexFactor;
                if (index < 0)
                    index = 0;
                else if (index >= mLevelCount)
                    index = mLevelCount-1;
                scanLine[i] = mColorBuffer.at(index);
            }
        }
    } else // logarithmic == true
    {
        if (mPeriodic)
        {
            for (int i=0; i<n; ++i)
            {
                int index = (int)(qLn(data[dataIndexFactor*i]/range.lower)/qLn(range.upper/range.lower)*(mLevelCount-1)) % mLevelCount;
                if (index < 0)
                    index += mLevelCount;
                scanLine[i] = mColorBuffer.at(index);
            }
        } else
        {
            for (int i=0; i<n; ++i)
            {
                int index = qLn(data[dataIndexFactor*i]/range.lower)/qLn(range.upper/range.lower)*(mLevelCount-1);
                if (index < 0)
                    index = 0;
                else if (index >= mLevelCount)
                    index = mLevelCount-1;
                scanLine[i] = mColorBuffer.at(index);
            }
        }
    }
}

/*! \overload

  Additionally to the other overload of \ref colorize, this method takes the array \a alpha, which
  has the same size and structure as \a data and encodes the alpha information per data point.

  The QRgb values that are placed in \a scanLine have their r, g and b components premultiplied
  with alpha (see QImage::Format_ARGB32_Premultiplied).
*/
void QCPColorGradient::colorize(const double *data, const unsigned char *alpha, const QCPRange &range, QRgb *scanLine, int n, int dataIndexFactor, bool logarithmic)
{
    // If you change something here, make sure to also adapt color() and the other colorize() overload
    if (!data)
    {
        qDebug() << Q_FUNC_INFO << "null pointer given as data";
        return;
    }
    if (!alpha)
    {
        qDebug() << Q_FUNC_INFO << "null pointer given as alpha";
        return;
    }
    if (!scanLine)
    {
        qDebug() << Q_FUNC_INFO << "null pointer given as scanLine";
        return;
    }
    if (mColorBufferInvalidated)
        updateColorBuffer();

    if (!logarithmic)
    {
        const double posToIndexFactor = (mLevelCount-1)/range.size();
        if (mPeriodic)
        {
            for (int i=0; i<n; ++i)
            {
                int index = (int)((data[dataIndexFactor*i]-range.lower)*posToIndexFactor) % mLevelCount;
                if (index < 0)
                    index += mLevelCount;
                if (alpha[dataIndexFactor*i] == 255)
                {
                    scanLine[i] = mColorBuffer.at(index);
                } else
                {
                    const QRgb rgb = mColorBuffer.at(index);
                    const float alphaF = alpha[dataIndexFactor*i]/255.0f;
                    scanLine[i] = qRgba(qRed(rgb)*alphaF, qGreen(rgb)*alphaF, qBlue(rgb)*alphaF, qAlpha(rgb)*alphaF);
                }
            }
        } else
        {
            for (int i=0; i<n; ++i)
            {
                int index = (data[dataIndexFactor*i]-range.lower)*posToIndexFactor;
                if (index < 0)
                    index = 0;
                else if (index >= mLevelCount)
                    index = mLevelCount-1;
                if (alpha[dataIndexFactor*i] == 255)
                {
                    scanLine[i] = mColorBuffer.at(index);
                } else
                {
                    const QRgb rgb = mColorBuffer.at(index);
                    const float alphaF = alpha[dataIndexFactor*i]/255.0f;
                    scanLine[i] = qRgba(qRed(rgb)*alphaF, qGreen(rgb)*alphaF, qBlue(rgb)*alphaF, qAlpha(rgb)*alphaF);
                }
            }
        }
    } else // logarithmic == true
    {
        if (mPeriodic)
        {
            for (int i=0; i<n; ++i)
            {
                int index = (int)(qLn(data[dataIndexFactor*i]/range.lower)/qLn(range.upper/range.lower)*(mLevelCount-1)) % mLevelCount;
                if (index < 0)
                    index += mLevelCount;
                if (alpha[dataIndexFactor*i] == 255)
                {
                    scanLine[i] = mColorBuffer.at(index);
                } else
                {
                    const QRgb rgb = mColorBuffer.at(index);
                    const float alphaF = alpha[dataIndexFactor*i]/255.0f;
                    scanLine[i] = qRgba(qRed(rgb)*alphaF, qGreen(rgb)*alphaF, qBlue(rgb)*alphaF, qAlpha(rgb)*alphaF);
                }
            }
        } else
        {
            for (int i=0; i<n; ++i)
            {
                int index = qLn(data[dataIndexFactor*i]/range.lower)/qLn(range.upper/range.lower)*(mLevelCount-1);
                if (index < 0)
                    index = 0;
                else if (index >= mLevelCount)
                    index = mLevelCount-1;
                if (alpha[dataIndexFactor*i] == 255)
                {
                    scanLine[i] = mColorBuffer.at(index);
                } else
                {
                    const QRgb rgb = mColorBuffer.at(index);
                    const float alphaF = alpha[dataIndexFactor*i]/255.0f;
                    scanLine[i] = qRgba(qRed(rgb)*alphaF, qGreen(rgb)*alphaF, qBlue(rgb)*alphaF, qAlpha(rgb)*alphaF);
                }
            }
        }
    }
}

/*! \internal

  This method is used to colorize a single data value given in \a position, to colors. The data
  range that shall be used for mapping the data value to the gradient is passed in \a range. \a
  logarithmic indicates whether the data value shall be mapped to a color logarithmically.

  If an entire array of data values shall be converted, rather use \ref colorize, for better
  performance.

  The returned QRgb has its r, g and b components premultiplied with alpha (see
  QImage::Format_ARGB32_Premultiplied).
*/
QRgb QCPColorGradient::color(double position, const QCPRange &range, bool logarithmic)
{
    // If you change something here, make sure to also adapt ::colorize()
    if (mColorBufferInvalidated)
        updateColorBuffer();
    int index = 0;
    if (!logarithmic)
        index = (position-range.lower)*(mLevelCount-1)/range.size();
    else
        index = qLn(position/range.lower)/qLn(range.upper/range.lower)*(mLevelCount-1);
    if (mPeriodic)
    {
        index = index % mLevelCount;
        if (index < 0)
            index += mLevelCount;
    } else
    {
        if (index < 0)
            index = 0;
        else if (index >= mLevelCount)
            index = mLevelCount-1;
    }
    return mColorBuffer.at(index);
}

/*!
  Clears the current color stops and loads the specified \a preset. A preset consists of predefined
  color stops and the corresponding color interpolation method.
  
  The available presets are:
  \image html QCPColorGradient.png
*/
void QCPColorGradient::loadPreset(GradientPreset preset)
{
    clearColorStops();
    switch (preset)
    {
    case gpGrayscale:
        setColorInterpolation(ciRGB);
        setColorStopAt(0, Qt::black);
        setColorStopAt(1, Qt::white);
        break;
    case gpHot:
        setColorInterpolation(ciRGB);
        setColorStopAt(0, QColor(50, 0, 0));
        setColorStopAt(0.2, QColor(180, 10, 0));
        setColorStopAt(0.4, QColor(245, 50, 0));
        setColorStopAt(0.6, QColor(255, 150, 10));
        setColorStopAt(0.8, QColor(255, 255, 50));
        setColorStopAt(1, QColor(255, 255, 255));
        break;
    case gpCold:
        setColorInterpolation(ciRGB);
        setColorStopAt(0, QColor(0, 0, 50));
        setColorStopAt(0.2, QColor(0, 10, 180));
        setColorStopAt(0.4, QColor(0, 50, 245));
        setColorStopAt(0.6, QColor(10, 150, 255));
        setColorStopAt(0.8, QColor(50, 255, 255));
        setColorStopAt(1, QColor(255, 255, 255));
        break;
    case gpNight:
        setColorInterpolation(ciHSV);
        setColorStopAt(0, QColor(10, 20, 30));
        setColorStopAt(1, QColor(250, 255, 250));
        break;
    case gpCandy:
        setColorInterpolation(ciHSV);
        setColorStopAt(0, QColor(0, 0, 255));
        setColorStopAt(1, QColor(255, 250, 250));
        break;
    case gpGeography:
        setColorInterpolation(ciRGB);
        setColorStopAt(0, QColor(70, 170, 210));
        setColorStopAt(0.20, QColor(90, 160, 180));
        setColorStopAt(0.25, QColor(45, 130, 175));
        setColorStopAt(0.30, QColor(100, 140, 125));
        setColorStopAt(0.5, QColor(100, 140, 100));
        setColorStopAt(0.6, QColor(130, 145, 120));
        setColorStopAt(0.7, QColor(140, 130, 120));
        setColorStopAt(0.9, QColor(180, 190, 190));
        setColorStopAt(1, QColor(210, 210, 230));
        break;
    case gpIon:
        setColorInterpolation(ciHSV);
        setColorStopAt(0, QColor(50, 10, 10));
        setColorStopAt(0.45, QColor(0, 0, 255));
        setColorStopAt(0.8, QColor(0, 255, 255));
        setColorStopAt(1, QColor(0, 255, 0));
        break;
    case gpThermal:
        setColorInterpolation(ciRGB);
        setColorStopAt(0, QColor(0, 0, 50));
        setColorStopAt(0.15, QColor(20, 0, 120));
        setColorStopAt(0.33, QColor(200, 30, 140));
        setColorStopAt(0.6, QColor(255, 100, 0));
        setColorStopAt(0.85, QColor(255, 255, 40));
        setColorStopAt(1, QColor(255, 255, 255));
        break;
    case gpPolar:
        setColorInterpolation(ciRGB);
        setColorStopAt(0, QColor(50, 255, 255));
        setColorStopAt(0.18, QColor(10, 70, 255));
        setColorStopAt(0.28, QColor(10, 10, 190));
        setColorStopAt(0.5, QColor(0, 0, 0));
        setColorStopAt(0.72, QColor(190, 10, 10));
        setColorStopAt(0.82, QColor(255, 70, 10));
        setColorStopAt(1, QColor(255, 255, 50));
        break;
    case gpSpectrum:
        setColorInterpolation(ciHSV);
        setColorStopAt(0, QColor(50, 0, 50));
        setColorStopAt(0.15, QColor(0, 0, 255));
        setColorStopAt(0.35, QColor(0, 255, 255));
        setColorStopAt(0.6, QColor(255, 255, 0));
        setColorStopAt(0.75, QColor(255, 30, 0));
        setColorStopAt(1, QColor(50, 0, 0));
        break;
    case gpJet:
        setColorInterpolation(ciRGB);
        setColorStopAt(0, QColor(0, 0, 100));
        setColorStopAt(0.15, QColor(0, 50, 255));
        setColorStopAt(0.35, QColor(0, 255, 255));
        setColorStopAt(0.65, QColor(255, 255, 0));
        setColorStopAt(0.85, QColor(255, 30, 0));
        setColorStopAt(1, QColor(100, 0, 0));
        break;
    case gpHues:
        setColorInterpolation(ciHSV);
        setColorStopAt(0, QColor(255, 0, 0));
        setColorStopAt(1.0/3.0, QColor(0, 0, 255));
        setColorStopAt(2.0/3.0, QColor(0, 255, 0));
        setColorStopAt(1, QColor(255, 0, 0));
        break;
    }
}

/*!
  Clears all color stops.
  
  \see setColorStops, setColorStopAt
*/
void QCPColorGradient::clearColorStops()
{
    mColorStops.clear();
    mColorBufferInvalidated = true;
}

/*!
  Returns an inverted gradient. The inverted gradient has all properties as this \ref
  QCPColorGradient, but the order of the color stops is inverted.
  
  \see setColorStops, setColorStopAt
*/
QCPColorGradient QCPColorGradient::inverted() const
{
    QCPColorGradient result(*this);
    result.clearColorStops();
    for (QMap<double, QColor>::const_iterator it=mColorStops.constBegin(); it!=mColorStops.constEnd(); ++it)
        result.setColorStopAt(1.0-it.key(), it.value());
    return result;
}

/*! \internal
  
  Returns true if the color gradient uses transparency, i.e. if any of the configured color stops
  has an alpha value below 255.
*/
bool QCPColorGradient::stopsUseAlpha() const
{
    for (QMap<double, QColor>::const_iterator it=mColorStops.constBegin(); it!=mColorStops.constEnd(); ++it)
    {
        if (it.value().alpha() < 255)
            return true;
    }
    return false;
}

/*! \internal
  
  Updates the internal color buffer which will be used by \ref colorize and \ref color, to quickly
  convert positions to colors. This is where the interpolation between color stops is calculated.
*/
void QCPColorGradient::updateColorBuffer()
{
    if (mColorBuffer.size() != mLevelCount)
        mColorBuffer.resize(mLevelCount);
    if (mColorStops.size() > 1)
    {
        double indexToPosFactor = 1.0/(double)(mLevelCount-1);
        const bool useAlpha = stopsUseAlpha();
        for (int i=0; i<mLevelCount; ++i)
        {
            double position = i*indexToPosFactor;
            QMap<double, QColor>::const_iterator it = mColorStops.lowerBound(position);
            if (it == mColorStops.constEnd()) // position is on or after last stop, use color of last stop
            {
                if (useAlpha)
                {
                    const QColor col = (it-1).value();
                    const float alphaPremultiplier = col.alpha()/255.0f; // since we use QImage::Format_ARGB32_Premultiplied
                    mColorBuffer[i] = qRgba(col.red()*alphaPremultiplier, col.green()*alphaPremultiplier, col.blue()*alphaPremultiplier, col.alpha());
                } else
                    mColorBuffer[i] = (it-1).value().rgba();
            } else if (it == mColorStops.constBegin()) // position is on or before first stop, use color of first stop
            {
                if (useAlpha)
                {
                    const QColor col = it.value();
                    const float alphaPremultiplier = col.alpha()/255.0f; // since we use QImage::Format_ARGB32_Premultiplied
                    mColorBuffer[i] = qRgba(col.red()*alphaPremultiplier, col.green()*alphaPremultiplier, col.blue()*alphaPremultiplier, col.alpha());
                } else
                    mColorBuffer[i] = it.value().rgba();
            } else // position is in between stops (or on an intermediate stop), interpolate color
            {
                QMap<double, QColor>::const_iterator high = it;
                QMap<double, QColor>::const_iterator low = it-1;
                double t = (position-low.key())/(high.key()-low.key()); // interpolation factor 0..1
                switch (mColorInterpolation)
                {
                case ciRGB:
                {
                    if (useAlpha)
                    {
                        const int alpha = (1-t)*low.value().alpha() + t*high.value().alpha();
                        const float alphaPremultiplier = alpha/255.0f; // since we use QImage::Format_ARGB32_Premultiplied
                        mColorBuffer[i] = qRgba(((1-t)*low.value().red() + t*high.value().red())*alphaPremultiplier,
                                                ((1-t)*low.value().green() + t*high.value().green())*alphaPremultiplier,
                                                ((1-t)*low.value().blue() + t*high.value().blue())*alphaPremultiplier,
                                                alpha);
                    } else
                    {
                        mColorBuffer[i] = qRgb(((1-t)*low.value().red() + t*high.value().red()),
                                               ((1-t)*low.value().green() + t*high.value().green()),
                                               ((1-t)*low.value().blue() + t*high.value().blue()));
                    }
                    break;
                }
                case ciHSV:
                {
                    QColor lowHsv = low.value().toHsv();
                    QColor highHsv = high.value().toHsv();
                    double hue = 0;
                    double hueDiff = highHsv.hueF()-lowHsv.hueF();
                    if (hueDiff > 0.5)
                        hue = lowHsv.hueF() - t*(1.0-hueDiff);
                    else if (hueDiff < -0.5)
                        hue = lowHsv.hueF() + t*(1.0+hueDiff);
                    else
                        hue = lowHsv.hueF() + t*hueDiff;
                    if (hue < 0) hue += 1.0;
                    else if (hue >= 1.0) hue -= 1.0;
                    if (useAlpha)
                    {
                        const QRgb rgb = QColor::fromHsvF(hue,
                                                          (1-t)*lowHsv.saturationF() + t*highHsv.saturationF(),
                                                          (1-t)*lowHsv.valueF() + t*highHsv.valueF()).rgb();
                        const float alpha = (1-t)*lowHsv.alphaF() + t*highHsv.alphaF();
                        mColorBuffer[i] = qRgba(qRed(rgb)*alpha, qGreen(rgb)*alpha, qBlue(rgb)*alpha, 255*alpha);
                    }
                    else
                    {
                        mColorBuffer[i] = QColor::fromHsvF(hue,
                                                           (1-t)*lowHsv.saturationF() + t*highHsv.saturationF(),
                                                           (1-t)*lowHsv.valueF() + t*highHsv.valueF()).rgb();
                    }
                    break;
                }
                }
            }
        }
    } else if (mColorStops.size() == 1)
    {
        const QRgb rgb = mColorStops.constBegin().value().rgb();
        const float alpha = mColorStops.constBegin().value().alphaF();
        mColorBuffer.fill(qRgba(qRed(rgb)*alpha, qGreen(rgb)*alpha, qBlue(rgb)*alpha, 255*alpha));
    } else // mColorStops is empty, fill color buffer with black
    {
        mColorBuffer.fill(qRgb(0, 0, 0));
    }
    mColorBufferInvalidated = false;
}
/* end of 'src/colorgradient.cpp' */



/* including file 'src/layoutelements/layoutelement-axisrect.cpp', size 47584 */
/* commit ce344b3f96a62e5f652585e55f1ae7c7883cd45b 2018-06-25 01:03:39 +0200  */


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPAxisRect
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPAxisRect
  \brief Holds multiple axes and arranges them in a rectangular shape.
  
  This class represents an axis rect, a rectangular area that is bounded on all sides with an
  arbitrary number of axes.
  
  Initially PF_GraphicView has one axis rect, accessible via PF_GraphicView::axisRect(). However, the
  layout system allows to have multiple axis rects, e.g. arranged in a grid layout
  (PF_GraphicView::plotLayout).
  
  By default, QCPAxisRect comes with four axes, at bottom, top, left and right. They can be
  accessed via \ref axis by providing the respective axis type (\ref QCPAxis::AxisType) and index.
  If you need all axes in the axis rect, use \ref axes. The top and right axes are set to be
  invisible initially (QCPAxis::setVisible). To add more axes to a side, use \ref addAxis or \ref
  addAxes. To remove an axis, use \ref removeAxis.
  
  The axis rect layerable itself only draws a background pixmap or color, if specified (\ref
  setBackground). It is placed on the "background" layer initially (see \ref QCPLayer for an
  explanation of the PF_GraphicView layer system). The axes that are held by the axis rect can be
  placed on other layers, independently of the axis rect.
  
  Every axis rect has a child layout of type \ref QCPLayoutInset. It is accessible via \ref
  insetLayout and can be used to have other layout elements (or even other layouts with multiple
  elements) hovering inside the axis rect.
  
  If an axis rect is clicked and dragged, it processes this by moving certain axis ranges. The
  behaviour can be controlled with \ref setRangeDrag and \ref setRangeDragAxes. If the mouse wheel
  is scrolled while the cursor is on the axis rect, certain axes are scaled. This is controllable
  via \ref setRangeZoom, \ref setRangeZoomAxes and \ref setRangeZoomFactor. These interactions are
  only enabled if \ref PF_GraphicView::setInteractions contains \ref QCP::iRangeDrag and \ref
  QCP::iRangeZoom.
  
  \image html AxisRectSpacingOverview.png
  <center>Overview of the spacings and paddings that define the geometry of an axis. The dashed
  line on the far left indicates the viewport/widget border.</center>
*/

/* start documentation of inline functions */

/*! \fn QCPLayoutInset *QCPAxisRect::insetLayout() const
  
  Returns the inset layout of this axis rect. It can be used to place other layout elements (or
  even layouts with multiple other elements) inside/on top of an axis rect.
  
  \see QCPLayoutInset
*/

/*! \fn int QCPAxisRect::left() const
  
  Returns the pixel position of the left border of this axis rect. Margins are not taken into
  account here, so the returned value is with respect to the inner \ref rect.
*/

/*! \fn int QCPAxisRect::right() const
  
  Returns the pixel position of the right border of this axis rect. Margins are not taken into
  account here, so the returned value is with respect to the inner \ref rect.
*/

/*! \fn int QCPAxisRect::top() const
  
  Returns the pixel position of the top border of this axis rect. Margins are not taken into
  account here, so the returned value is with respect to the inner \ref rect.
*/

/*! \fn int QCPAxisRect::bottom() const
  
  Returns the pixel position of the bottom border of this axis rect. Margins are not taken into
  account here, so the returned value is with respect to the inner \ref rect.
*/

/*! \fn int QCPAxisRect::width() const
  
  Returns the pixel width of this axis rect. Margins are not taken into account here, so the
  returned value is with respect to the inner \ref rect.
*/

/*! \fn int QCPAxisRect::height() const
  
  Returns the pixel height of this axis rect. Margins are not taken into account here, so the
  returned value is with respect to the inner \ref rect.
*/

/*! \fn QSize QCPAxisRect::size() const
  
  Returns the pixel size of this axis rect. Margins are not taken into account here, so the
  returned value is with respect to the inner \ref rect.
*/

/*! \fn QPoint QCPAxisRect::topLeft() const
  
  Returns the top left corner of this axis rect in pixels. Margins are not taken into account here,
  so the returned value is with respect to the inner \ref rect.
*/

/*! \fn QPoint QCPAxisRect::topRight() const
  
  Returns the top right corner of this axis rect in pixels. Margins are not taken into account
  here, so the returned value is with respect to the inner \ref rect.
*/

/*! \fn QPoint QCPAxisRect::bottomLeft() const
  
  Returns the bottom left corner of this axis rect in pixels. Margins are not taken into account
  here, so the returned value is with respect to the inner \ref rect.
*/

/*! \fn QPoint QCPAxisRect::bottomRight() const
  
  Returns the bottom right corner of this axis rect in pixels. Margins are not taken into account
  here, so the returned value is with respect to the inner \ref rect.
*/

/*! \fn QPoint QCPAxisRect::center() const
  
  Returns the center of this axis rect in pixels. Margins are not taken into account here, so the
  returned value is with respect to the inner \ref rect.
*/

/* end documentation of inline functions */

/*!
  Creates a QCPAxisRect instance and sets default values. An axis is added for each of the four
  sides, the top and right axes are set invisible initially.
*/
QCPAxisRect::QCPAxisRect(PF_GraphicView *parentPlot, bool setupDefaultAxes) :
    QCPLayoutElement(parentPlot),
    mBackgroundBrush(Qt::NoBrush),
    mBackgroundScaled(true),
    mBackgroundScaledMode(Qt::KeepAspectRatioByExpanding),
    mInsetLayout(new QCPLayoutInset),
    mRangeDrag(Qt::Horizontal|Qt::Vertical),
    mRangeZoom(Qt::Horizontal|Qt::Vertical),
    mRangeZoomFactorHorz(0.85),
    mRangeZoomFactorVert(0.85),
    mDragging(false),
    isSyncAxis(false)
{
    mInsetLayout->initializeParentPlot(mParentPlot);
    mInsetLayout->setParentLayerable(this);
    mInsetLayout->setParent(this);

    setMinimumSize(50, 50);
    setMinimumMargins(QMargins(15, 15, 15, 15));
    mAxes.insert(QCPAxis::atLeft, QList<QCPAxis*>());
    mAxes.insert(QCPAxis::atRight, QList<QCPAxis*>());
    mAxes.insert(QCPAxis::atTop, QList<QCPAxis*>());
    mAxes.insert(QCPAxis::atBottom, QList<QCPAxis*>());

    if (setupDefaultAxes)
    {
        QCPAxis *xAxis = addAxis(QCPAxis::atBottom);
        QCPAxis *yAxis = addAxis(QCPAxis::atLeft);
        QCPAxis *xAxis2 = addAxis(QCPAxis::atTop);
        QCPAxis *yAxis2 = addAxis(QCPAxis::atRight);
        setRangeDragAxes(xAxis, yAxis);
        setRangeZoomAxes(xAxis, yAxis);
        xAxis2->setVisible(false);
        yAxis2->setVisible(false);
        xAxis->grid()->setVisible(true);
        yAxis->grid()->setVisible(true);
        xAxis2->grid()->setVisible(false);
        yAxis2->grid()->setVisible(false);
        xAxis2->grid()->setZeroLinePen(Qt::NoPen);
        yAxis2->grid()->setZeroLinePen(Qt::NoPen);
        xAxis2->grid()->setVisible(false);
        yAxis2->grid()->setVisible(false);
    }
}

QCPAxisRect::~QCPAxisRect()
{
    delete mInsetLayout;
    mInsetLayout = nullptr;

    QList<QCPAxis*> axesList = axes();
    for (int i=0; i<axesList.size(); ++i)
        removeAxis(axesList.at(i));
}

/*!
  Returns the number of axes on the axis rect side specified with \a type.
  
  \see axis
*/
int QCPAxisRect::axisCount(QCPAxis::AxisType type) const
{
    return mAxes.value(type).size();
}

/*!
  Returns the axis with the given \a index on the axis rect side specified with \a type.
  
  \see axisCount, axes
*/
QCPAxis *QCPAxisRect::axis(QCPAxis::AxisType type, int index) const
{
    QList<QCPAxis*> ax(mAxes.value(type));
    if (index >= 0 && index < ax.size())
    {
        return ax.at(index);
    } else
    {
        qDebug() << Q_FUNC_INFO << "Axis index out of bounds:" << index;
        return nullptr;
    }
}

/*!
  Returns all axes on the axis rect sides specified with \a types.
  
  \a types may be a single \ref QCPAxis::AxisType or an <tt>or</tt>-combination, to get the axes of
  multiple sides.
  
  \see axis
*/
QList<QCPAxis*> QCPAxisRect::axes(QCPAxis::AxisTypes types) const
{
    QList<QCPAxis*> result;
    if (types.testFlag(QCPAxis::atLeft))
        result << mAxes.value(QCPAxis::atLeft);
    if (types.testFlag(QCPAxis::atRight))
        result << mAxes.value(QCPAxis::atRight);
    if (types.testFlag(QCPAxis::atTop))
        result << mAxes.value(QCPAxis::atTop);
    if (types.testFlag(QCPAxis::atBottom))
        result << mAxes.value(QCPAxis::atBottom);
    return result;
}

/*! \overload
  
  Returns all axes of this axis rect.
*/
QList<QCPAxis*> QCPAxisRect::axes() const
{
    QList<QCPAxis*> result;
    QHashIterator<QCPAxis::AxisType, QList<QCPAxis*> > it(mAxes);
    while (it.hasNext())
    {
        it.next();
        result << it.value();
    }
    return result;
}

/*!
  Adds a new axis to the axis rect side specified with \a type, and returns it. If \a axis is 0, a
  new QCPAxis instance is created internally. PF_GraphicView owns the returned axis, so if you want to
  remove an axis, use \ref removeAxis instead of deleting it manually.

  You may inject QCPAxis instances (or subclasses of QCPAxis) by setting \a axis to an axis that was
  previously created outside PF_GraphicView. It is important to note that PF_GraphicView takes ownership
  of the axis, so you may not delete it afterwards. Further, the \a axis must have been created
  with this axis rect as parent and with the same axis type as specified in \a type. If this is not
  the case, a debug output is generated, the axis is not added, and the method returns 0.

  This method can not be used to move \a axis between axis rects. The same \a axis instance must
  not be added multiple times to the same or different axis rects.

  If an axis rect side already contains one or more axes, the lower and upper endings of the new
  axis (\ref QCPAxis::setLowerEnding, \ref QCPAxis::setUpperEnding) are set to \ref
  QCPLineEnding::esHalfBar.

  \see addAxes, setupFullAxesBox
*/
QCPAxis *QCPAxisRect::addAxis(QCPAxis::AxisType type, QCPAxis *axis)
{
    QCPAxis *newAxis = axis;
    if (!newAxis)
    {
        newAxis = new QCPAxis(this, type);
    } else // user provided existing axis instance, do some sanity checks
    {
        if (newAxis->axisType() != type)
        {
            qDebug() << Q_FUNC_INFO << "passed axis has different axis type than specified in type parameter";
            return nullptr;
        }
        if (newAxis->axisRect() != this)
        {
            qDebug() << Q_FUNC_INFO << "passed axis doesn't have this axis rect as parent axis rect";
            return nullptr;
        }
        if (axes().contains(newAxis))
        {
            qDebug() << Q_FUNC_INFO << "passed axis is already owned by this axis rect";
            return nullptr;
        }
    }
    if (mAxes[type].size() > 0) // multiple axes on one side, add half-bar axis ending to additional axes with offset
    {
        bool invert = (type == QCPAxis::atRight) || (type == QCPAxis::atBottom);
        newAxis->setLowerEnding(QCPLineEnding(QCPLineEnding::esHalfBar, 6, 10, !invert));
        newAxis->setUpperEnding(QCPLineEnding(QCPLineEnding::esHalfBar, 6, 10, invert));
    }
    mAxes[type].append(newAxis);

    // reset convenience axis pointers on parent PF_GraphicView if they are unset:
    if (mParentPlot && mParentPlot->axisRectCount() > 0 && mParentPlot->axisRect(0) == this)
    {
        switch (type)
        {
        case QCPAxis::atBottom: { if (!mParentPlot->xAxis) mParentPlot->xAxis = newAxis; break; }
        case QCPAxis::atLeft: { if (!mParentPlot->yAxis) mParentPlot->yAxis = newAxis; break; }
        case QCPAxis::atTop: { if (!mParentPlot->xAxis2) mParentPlot->xAxis2 = newAxis; break; }
        case QCPAxis::atRight: { if (!mParentPlot->yAxis2) mParentPlot->yAxis2 = newAxis; break; }
        }
    }

    return newAxis;
}

/*!
  Adds a new axis with \ref addAxis to each axis rect side specified in \a types. This may be an
  <tt>or</tt>-combination of QCPAxis::AxisType, so axes can be added to multiple sides at once.
  
  Returns a list of the added axes.
  
  \see addAxis, setupFullAxesBox
*/
QList<QCPAxis*> QCPAxisRect::addAxes(QCPAxis::AxisTypes types)
{
    QList<QCPAxis*> result;
    if (types.testFlag(QCPAxis::atLeft))
        result << addAxis(QCPAxis::atLeft);
    if (types.testFlag(QCPAxis::atRight))
        result << addAxis(QCPAxis::atRight);
    if (types.testFlag(QCPAxis::atTop))
        result << addAxis(QCPAxis::atTop);
    if (types.testFlag(QCPAxis::atBottom))
        result << addAxis(QCPAxis::atBottom);
    return result;
}

/*!
  Removes the specified \a axis from the axis rect and deletes it.
  
  Returns true on success, i.e. if \a axis was a valid axis in this axis rect.
  
  \see addAxis
*/
bool QCPAxisRect::removeAxis(QCPAxis *axis)
{
    // don't access axis->axisType() to provide safety when axis is an invalid pointer, rather go through all axis containers:
    QHashIterator<QCPAxis::AxisType, QList<QCPAxis*> > it(mAxes);
    while (it.hasNext())
    {
        it.next();
        if (it.value().contains(axis))
        {
            if (it.value().first() == axis && it.value().size() > 1) // if removing first axis, transfer axis offset to the new first axis (which at this point is the second axis, if it exists)
                it.value()[1]->setOffset(axis->offset());
            mAxes[it.key()].removeOne(axis);
            if (qobject_cast<PF_GraphicView*>(parentPlot())) // make sure this isn't called from QObject dtor when PF_GraphicView is already destructed (happens when the axis rect is not in any layout and thus QObject-child of PF_GraphicView)
                parentPlot()->axisRemoved(axis);
            delete axis;
            return true;
        }
    }
    qDebug() << Q_FUNC_INFO << "Axis isn't in axis rect:" << reinterpret_cast<quintptr>(axis);
    return false;
}

/*!
  Zooms in (or out) to the passed rectangular region \a pixelRect, given in pixel coordinates.

  All axes of this axis rect will have their range zoomed accordingly. If you only wish to zoom
  specific axes, use the overloaded version of this method.
  
  \see PF_GraphicView::setSelectionRectMode
*/
void QCPAxisRect::zoom(const QRectF &pixelRect)
{
    zoom(pixelRect, axes());
}

/*! \overload
  
  Zooms in (or out) to the passed rectangular region \a pixelRect, given in pixel coordinates.
  
  Only the axes passed in \a affectedAxes will have their ranges zoomed accordingly.
  
  \see PF_GraphicView::setSelectionRectMode
*/
void QCPAxisRect::zoom(const QRectF &pixelRect, const QList<QCPAxis*> &affectedAxes)
{
    foreach (QCPAxis *axis, affectedAxes)
    {
        if (!axis)
        {
            qDebug() << Q_FUNC_INFO << "a passed axis was zero";
            continue;
        }
        QCPRange pixelRange;
        if (axis->orientation() == Qt::Horizontal)
            pixelRange = QCPRange(pixelRect.left(), pixelRect.right());
        else
            pixelRange = QCPRange(pixelRect.top(), pixelRect.bottom());
        axis->setRange(axis->pixelToCoord(pixelRange.lower), axis->pixelToCoord(pixelRange.upper));
    }
}

/*!
  Convenience function to create an axis on each side that doesn't have any axes yet and set their
  visibility to true. Further, the top/right axes are assigned the following properties of the
  bottom/left axes:

  \li range (\ref QCPAxis::setRange)
  \li range reversed (\ref QCPAxis::setRangeReversed)
  \li scale type (\ref QCPAxis::setScaleType)
  \li tick visibility (\ref QCPAxis::setTicks)
  \li number format (\ref QCPAxis::setNumberFormat)
  \li number precision (\ref QCPAxis::setNumberPrecision)
  \li tick count of ticker (\ref QCPAxisTicker::setTickCount)
  \li tick origin of ticker (\ref QCPAxisTicker::setTickOrigin)

  Tick label visibility (\ref QCPAxis::setTickLabels) of the right and top axes are set to false.

  If \a connectRanges is true, the \ref QCPAxis::rangeChanged "rangeChanged" signals of the bottom
  and left axes are connected to the \ref QCPAxis::setRange slots of the top and right axes.
*/
void QCPAxisRect::setupFullAxesBox(bool connectRanges)
{
    QCPAxis *xAxis, *yAxis, *xAxis2, *yAxis2;
    if (axisCount(QCPAxis::atBottom) == 0)
        xAxis = addAxis(QCPAxis::atBottom);
    else
        xAxis = axis(QCPAxis::atBottom);

    if (axisCount(QCPAxis::atLeft) == 0)
        yAxis = addAxis(QCPAxis::atLeft);
    else
        yAxis = axis(QCPAxis::atLeft);

    if (axisCount(QCPAxis::atTop) == 0)
        xAxis2 = addAxis(QCPAxis::atTop);
    else
        xAxis2 = axis(QCPAxis::atTop);

    if (axisCount(QCPAxis::atRight) == 0)
        yAxis2 = addAxis(QCPAxis::atRight);
    else
        yAxis2 = axis(QCPAxis::atRight);

    xAxis->setVisible(true);
    yAxis->setVisible(true);
    xAxis2->setVisible(true);
    yAxis2->setVisible(true);
    xAxis2->setTickLabels(false);
    yAxis2->setTickLabels(false);

    xAxis2->setRange(xAxis->range());
    xAxis2->setRangeReversed(xAxis->rangeReversed());
    xAxis2->setScaleType(xAxis->scaleType());
    xAxis2->setTicks(xAxis->ticks());
    xAxis2->setNumberFormat(xAxis->numberFormat());
    xAxis2->setNumberPrecision(xAxis->numberPrecision());
    xAxis2->ticker()->setTickCount(xAxis->ticker()->tickCount());
    xAxis2->ticker()->setTickOrigin(xAxis->ticker()->tickOrigin());

    yAxis2->setRange(yAxis->range());
    yAxis2->setRangeReversed(yAxis->rangeReversed());
    yAxis2->setScaleType(yAxis->scaleType());
    yAxis2->setTicks(yAxis->ticks());
    yAxis2->setNumberFormat(yAxis->numberFormat());
    yAxis2->setNumberPrecision(yAxis->numberPrecision());
    yAxis2->ticker()->setTickCount(yAxis->ticker()->tickCount());
    yAxis2->ticker()->setTickOrigin(yAxis->ticker()->tickOrigin());

    if (connectRanges)
    {
        connect(xAxis, SIGNAL(rangeChanged(QCPRange)), xAxis2, SLOT(setRange(QCPRange)));
        connect(yAxis, SIGNAL(rangeChanged(QCPRange)), yAxis2, SLOT(setRange(QCPRange)));
    }
}

/*!
  Returns a list of all the plottables that are associated with this axis rect.
  
  A plottable is considered associated with an axis rect if its key or value axis (or both) is in
  this axis rect.
  
  \see graphs, items
*/
QList<QCPAbstractPlottable*> QCPAxisRect::plottables() const
{
    // Note: don't append all QCPAxis::plottables() into a list, because we might get duplicate entries
    QList<QCPAbstractPlottable*> result;
    for (int i=0; i<mParentPlot->mPlottables.size(); ++i)
    {
        if (mParentPlot->mPlottables.at(i)->keyAxis()->axisRect() == this || mParentPlot->mPlottables.at(i)->valueAxis()->axisRect() == this)
            result.append(mParentPlot->mPlottables.at(i));
    }
    return result;
}

/*!
  Returns a list of all the graphs that are associated with this axis rect.
  
  A graph is considered associated with an axis rect if its key or value axis (or both) is in
  this axis rect.
  
  \see plottables, items
*/
//QList<QCPGraph*> QCPAxisRect::graphs() const
//{
//    // Note: don't append all QCPAxis::graphs() into a list, because we might get duplicate entries
//    QList<QCPGraph*> result;
//    for (int i=0; i<mParentPlot->mGraphs.size(); ++i)
//    {
//        if (mParentPlot->mGraphs.at(i)->keyAxis()->axisRect() == this || mParentPlot->mGraphs.at(i)->valueAxis()->axisRect() == this)
//            result.append(mParentPlot->mGraphs.at(i));
//    }
//    return result;
//}

/*!
  Returns a list of all the items that are associated with this axis rect.
  
  An item is considered associated with an axis rect if any of its positions has key or value axis
  set to an axis that is in this axis rect, or if any of its positions has \ref
  QCPItemPosition::setAxisRect set to the axis rect, or if the clip axis rect (\ref
  QCPAbstractItem::setClipAxisRect) is set to this axis rect.
  
  \see plottables, graphs
*/
QList<QCPAbstractItem *> QCPAxisRect::items() const
{
    // Note: don't just append all QCPAxis::items() into a list, because we might get duplicate entries
    //       and miss those items that have this axis rect as clipAxisRect.
    QList<QCPAbstractItem*> result;
    for (int itemId=0; itemId<mParentPlot->mItems.size(); ++itemId)
    {
        if (mParentPlot->mItems.at(itemId)->clipAxisRect() == this)
        {
            result.append(mParentPlot->mItems.at(itemId));
            continue;
        }
        QList<QCPItemPosition*> positions = mParentPlot->mItems.at(itemId)->positions();
        for (int posId=0; posId<positions.size(); ++posId)
        {
            if (positions.at(posId)->axisRect() == this ||
                    positions.at(posId)->keyAxis()->axisRect() == this ||
                    positions.at(posId)->valueAxis()->axisRect() == this)
            {
                result.append(mParentPlot->mItems.at(itemId));
                break;
            }
        }
    }
    return result;
}

/*!
  This method is called automatically upon replot and doesn't need to be called by users of
  QCPAxisRect.
  
  Calls the base class implementation to update the margins (see \ref QCPLayoutElement::update),
  and finally passes the \ref rect to the inset layout (\ref insetLayout) and calls its
  QCPInsetLayout::update function.
  
  \seebaseclassmethod
*/
void QCPAxisRect::update(UpdatePhase phase)
{
    QCPLayoutElement::update(phase);

    switch (phase)
    {
    case upPreparation:
    {
        QList<QCPAxis*> allAxes = axes();
        for (int i=0; i<allAxes.size(); ++i)
            allAxes.at(i)->setupTickVectors();
        break;
    }
    case upLayout:
    {
        mInsetLayout->setOuterRect(rect());
        break;
    }
    default: break;
    }

    // pass update call on to inset layout (doesn't happen automatically, because QCPAxisRect doesn't derive from QCPLayout):
    mInsetLayout->update(phase);
}

/* inherits documentation from base class */
QList<QCPLayoutElement*> QCPAxisRect::elements(bool recursive) const
{
    QList<QCPLayoutElement*> result;
    if (mInsetLayout)
    {
        result << mInsetLayout;
        if (recursive)
            result << mInsetLayout->elements(recursive);
    }
    return result;
}

/* inherits documentation from base class */
void QCPAxisRect::applyDefaultAntialiasingHint(QCPPainter *painter) const
{
    painter->setAntialiasing(false);
}

/* inherits documentation from base class */
void QCPAxisRect::draw(QCPPainter *painter)
{
    drawBackground(painter);
}

/*!
  Sets \a pm as the axis background pixmap. The axis background pixmap will be drawn inside the
  axis rect. Since axis rects place themselves on the "background" layer by default, the axis rect
  backgrounds are usually drawn below everything else.

  For cases where the provided pixmap doesn't have the same size as the axis rect, scaling can be
  enabled with \ref setBackgroundScaled and the scaling mode (i.e. whether and how the aspect ratio
  is preserved) can be set with \ref setBackgroundScaledMode. To set all these options in one call,
  consider using the overloaded version of this function.

  Below the pixmap, the axis rect may be optionally filled with a brush, if specified with \ref
  setBackground(const QBrush &brush).
  
  \see setBackgroundScaled, setBackgroundScaledMode, setBackground(const QBrush &brush)
*/
void QCPAxisRect::setBackground(const QPixmap &pm)
{
    mBackgroundPixmap = pm;
    mScaledBackgroundPixmap = QPixmap();
}

/*! \overload
  
  Sets \a brush as the background brush. The axis rect background will be filled with this brush.
  Since axis rects place themselves on the "background" layer by default, the axis rect backgrounds
  are usually drawn below everything else.

  The brush will be drawn before (under) any background pixmap, which may be specified with \ref
  setBackground(const QPixmap &pm).

  To disable drawing of a background brush, set \a brush to Qt::NoBrush.
  
  \see setBackground(const QPixmap &pm)
*/
void QCPAxisRect::setBackground(const QBrush &brush)
{
    mBackgroundBrush = brush;
}

/*! \overload
  
  Allows setting the background pixmap of the axis rect, whether it shall be scaled and how it
  shall be scaled in one call.

  \see setBackground(const QPixmap &pm), setBackgroundScaled, setBackgroundScaledMode
*/
void QCPAxisRect::setBackground(const QPixmap &pm, bool scaled, Qt::AspectRatioMode mode)
{
    mBackgroundPixmap = pm;
    mScaledBackgroundPixmap = QPixmap();
    mBackgroundScaled = scaled;
    mBackgroundScaledMode = mode;
}

/*!
  Sets whether the axis background pixmap shall be scaled to fit the axis rect or not. If \a scaled
  is set to true, you may control whether and how the aspect ratio of the original pixmap is
  preserved with \ref setBackgroundScaledMode.
  
  Note that the scaled version of the original pixmap is buffered, so there is no performance
  penalty on replots. (Except when the axis rect dimensions are changed continuously.)
  
  \see setBackground, setBackgroundScaledMode
*/
void QCPAxisRect::setBackgroundScaled(bool scaled)
{
    mBackgroundScaled = scaled;
}

/*!
  If scaling of the axis background pixmap is enabled (\ref setBackgroundScaled), use this function to
  define whether and how the aspect ratio of the original pixmap passed to \ref setBackground is preserved.
  \see setBackground, setBackgroundScaled
*/
void QCPAxisRect::setBackgroundScaledMode(Qt::AspectRatioMode mode)
{
    mBackgroundScaledMode = mode;
}

/*!
  Returns the range drag axis of the \a orientation provided. If multiple axes were set, returns
  the first one (use \ref rangeDragAxes to retrieve a list with all set axes).

  \see setRangeDragAxes
*/
QCPAxis *QCPAxisRect::rangeDragAxis(Qt::Orientation orientation)
{
    if (orientation == Qt::Horizontal)
        return mRangeDragHorzAxis.isEmpty() ? nullptr : mRangeDragHorzAxis.first().data();
    else
        return mRangeDragVertAxis.isEmpty() ? nullptr : mRangeDragVertAxis.first().data();
}

/*!
  Returns the range zoom axis of the \a orientation provided. If multiple axes were set, returns
  the first one (use \ref rangeZoomAxes to retrieve a list with all set axes).

  \see setRangeZoomAxes
*/
QCPAxis *QCPAxisRect::rangeZoomAxis(Qt::Orientation orientation)
{
    if (orientation == Qt::Horizontal)
        return mRangeZoomHorzAxis.isEmpty() ? nullptr : mRangeZoomHorzAxis.first().data();
    else
        return mRangeZoomVertAxis.isEmpty() ? nullptr : mRangeZoomVertAxis.first().data();
}

/*!
  Returns all range drag axes of the \a orientation provided.

  \see rangeZoomAxis, setRangeZoomAxes
*/
QList<QCPAxis*> QCPAxisRect::rangeDragAxes(Qt::Orientation orientation)
{
    QList<QCPAxis*> result;
    if (orientation == Qt::Horizontal)
    {
        for (int i=0; i<mRangeDragHorzAxis.size(); ++i)
        {
            if (!mRangeDragHorzAxis.at(i).isNull())
                result.append(mRangeDragHorzAxis.at(i).data());
        }
    } else
    {
        for (int i=0; i<mRangeDragVertAxis.size(); ++i)
        {
            if (!mRangeDragVertAxis.at(i).isNull())
                result.append(mRangeDragVertAxis.at(i).data());
        }
    }
    return result;
}

/*!
  Returns all range zoom axes of the \a orientation provided.

  \see rangeDragAxis, setRangeDragAxes
*/
QList<QCPAxis*> QCPAxisRect::rangeZoomAxes(Qt::Orientation orientation)
{
    QList<QCPAxis*> result;
    if (orientation == Qt::Horizontal)
    {
        for (int i=0; i<mRangeZoomHorzAxis.size(); ++i)
        {
            if (!mRangeZoomHorzAxis.at(i).isNull())
                result.append(mRangeZoomHorzAxis.at(i).data());
        }
    } else
    {
        for (int i=0; i<mRangeZoomVertAxis.size(); ++i)
        {
            if (!mRangeZoomVertAxis.at(i).isNull())
                result.append(mRangeZoomVertAxis.at(i).data());
        }
    }
    return result;
}

/*!
  Returns the range zoom factor of the \a orientation provided.
  
  \see setRangeZoomFactor
*/
double QCPAxisRect::rangeZoomFactor(Qt::Orientation orientation)
{
    return (orientation == Qt::Horizontal ? mRangeZoomFactorHorz : mRangeZoomFactorVert);
}

/*!
  Sets which axis orientation may be range dragged by the user with mouse interaction.
  What orientation corresponds to which specific axis can be set with
  \ref setRangeDragAxes(QCPAxis *horizontal, QCPAxis *vertical). By
  default, the horizontal axis is the bottom axis (xAxis) and the vertical axis
  is the left axis (yAxis).
  
  To disable range dragging entirely, pass 0 as \a orientations or remove \ref QCP::iRangeDrag from \ref
  PF_GraphicView::setInteractions. To enable range dragging for both directions, pass <tt>Qt::Horizontal |
  Qt::Vertical</tt> as \a orientations.
  
  In addition to setting \a orientations to a non-zero value, make sure \ref PF_GraphicView::setInteractions
  contains \ref QCP::iRangeDrag to enable the range dragging interaction.
  
  \see setRangeZoom, setRangeDragAxes, PF_GraphicView::setNoAntialiasingOnDrag
*/
void QCPAxisRect::setRangeDrag(Qt::Orientations orientations)
{
    mRangeDrag = orientations;
}

/*!
  Sets which axis orientation may be zoomed by the user with the mouse wheel. What orientation
  corresponds to which specific axis can be set with \ref setRangeZoomAxes(QCPAxis *horizontal,
  QCPAxis *vertical). By default, the horizontal axis is the bottom axis (xAxis) and the vertical
  axis is the left axis (yAxis).

  To disable range zooming entirely, pass 0 as \a orientations or remove \ref QCP::iRangeZoom from \ref
  PF_GraphicView::setInteractions. To enable range zooming for both directions, pass <tt>Qt::Horizontal |
  Qt::Vertical</tt> as \a orientations.
  
  In addition to setting \a orientations to a non-zero value, make sure \ref PF_GraphicView::setInteractions
  contains \ref QCP::iRangeZoom to enable the range zooming interaction.
  
  \see setRangeZoomFactor, setRangeZoomAxes, setRangeDrag
*/
void QCPAxisRect::setRangeZoom(Qt::Orientations orientations)
{
    mRangeZoom = orientations;
}

/*! \overload
  
  Sets the axes whose range will be dragged when \ref setRangeDrag enables mouse range dragging on
  the PF_GraphicView widget. Pass 0 if no axis shall be dragged in the respective orientation.

  Use the overload taking a list of axes, if multiple axes (more than one per orientation) shall
  react to dragging interactions.

  \see setRangeZoomAxes
*/
void QCPAxisRect::setRangeDragAxes(QCPAxis *horizontal, QCPAxis *vertical)
{
    QList<QCPAxis*> horz, vert;
    if (horizontal)
        horz.append(horizontal);
    if (vertical)
        vert.append(vertical);
    setRangeDragAxes(horz, vert);
}

/*! \overload

  This method allows to set up multiple axes to react to horizontal and vertical dragging. The drag
  orientation that the respective axis will react to is deduced from its orientation (\ref
  QCPAxis::orientation).

  In the unusual case that you wish to e.g. drag a vertically oriented axis with a horizontal drag
  motion, use the overload taking two separate lists for horizontal and vertical dragging.
*/
void QCPAxisRect::setRangeDragAxes(QList<QCPAxis*> axes)
{
    QList<QCPAxis*> horz, vert;
    foreach (QCPAxis *ax, axes)
    {
        if (ax->orientation() == Qt::Horizontal)
            horz.append(ax);
        else
            vert.append(ax);
    }
    setRangeDragAxes(horz, vert);
}

/*! \overload

  This method allows to set multiple axes up to react to horizontal and vertical dragging, and
  define specifically which axis reacts to which drag orientation (irrespective of the axis
  orientation).
*/
void QCPAxisRect::setRangeDragAxes(QList<QCPAxis*> horizontal, QList<QCPAxis*> vertical)
{
    mRangeDragHorzAxis.clear();
    foreach (QCPAxis *ax, horizontal)
    {
        QPointer<QCPAxis> axPointer(ax);
        if (!axPointer.isNull())
            mRangeDragHorzAxis.append(axPointer);
        else
            qDebug() << Q_FUNC_INFO << "invalid axis passed in horizontal list:" << reinterpret_cast<quintptr>(ax);
    }
    mRangeDragVertAxis.clear();
    foreach (QCPAxis *ax, vertical)
    {
        QPointer<QCPAxis> axPointer(ax);
        if (!axPointer.isNull())
            mRangeDragVertAxis.append(axPointer);
        else
            qDebug() << Q_FUNC_INFO << "invalid axis passed in vertical list:" << reinterpret_cast<quintptr>(ax);
    }
}

/*!
  Sets the axes whose range will be zoomed when \ref setRangeZoom enables mouse wheel zooming on
  the PF_GraphicView widget. Pass 0 if no axis shall be zoomed in the respective orientation.

  The two axes can be zoomed with different strengths, when different factors are passed to \ref
  setRangeZoomFactor(double horizontalFactor, double verticalFactor).

  Use the overload taking a list of axes, if multiple axes (more than one per orientation) shall
  react to zooming interactions.

  \see setRangeDragAxes
*/
void QCPAxisRect::setRangeZoomAxes(QCPAxis *horizontal, QCPAxis *vertical)
{
    QList<QCPAxis*> horz, vert;
    if (horizontal)
        horz.append(horizontal);
    if (vertical)
        vert.append(vertical);
    setRangeZoomAxes(horz, vert);
}

/*! \overload

  This method allows to set up multiple axes to react to horizontal and vertical range zooming. The
  zoom orientation that the respective axis will react to is deduced from its orientation (\ref
  QCPAxis::orientation).

  In the unusual case that you wish to e.g. zoom a vertically oriented axis with a horizontal zoom
  interaction, use the overload taking two separate lists for horizontal and vertical zooming.
*/
void QCPAxisRect::setRangeZoomAxes(QList<QCPAxis*> axes)
{
    QList<QCPAxis*> horz, vert;
    foreach (QCPAxis *ax, axes)
    {
        if (ax->orientation() == Qt::Horizontal)
            horz.append(ax);
        else
            vert.append(ax);
    }
    setRangeZoomAxes(horz, vert);
}

/*! \overload

  This method allows to set multiple axes up to react to horizontal and vertical zooming, and
  define specifically which axis reacts to which zoom orientation (irrespective of the axis
  orientation).
*/
void QCPAxisRect::setRangeZoomAxes(QList<QCPAxis*> horizontal, QList<QCPAxis*> vertical)
{
    mRangeZoomHorzAxis.clear();
    foreach (QCPAxis *ax, horizontal)
    {
        QPointer<QCPAxis> axPointer(ax);
        if (!axPointer.isNull())
            mRangeZoomHorzAxis.append(axPointer);
        else
            qDebug() << Q_FUNC_INFO << "invalid axis passed in horizontal list:" << reinterpret_cast<quintptr>(ax);
    }
    mRangeZoomVertAxis.clear();
    foreach (QCPAxis *ax, vertical)
    {
        QPointer<QCPAxis> axPointer(ax);
        if (!axPointer.isNull())
            mRangeZoomVertAxis.append(axPointer);
        else
            qDebug() << Q_FUNC_INFO << "invalid axis passed in vertical list:" << reinterpret_cast<quintptr>(ax);
    }
}

/*!
  Sets how strong one rotation step of the mouse wheel zooms, when range zoom was activated with
  \ref setRangeZoom. The two parameters \a horizontalFactor and \a verticalFactor provide a way to
  let the horizontal axis zoom at different rates than the vertical axis. Which axis is horizontal
  and which is vertical, can be set with \ref setRangeZoomAxes.

  When the zoom factor is greater than one, scrolling the mouse wheel backwards (towards the user)
  will zoom in (make the currently visible range smaller). For zoom factors smaller than one, the
  same scrolling direction will zoom out.
*/
void QCPAxisRect::setRangeZoomFactor(double horizontalFactor, double verticalFactor)
{
    mRangeZoomFactorHorz = horizontalFactor;
    mRangeZoomFactorVert = verticalFactor;
}

/*! \overload
  
  Sets both the horizontal and vertical zoom \a factor.
*/
void QCPAxisRect::setRangeZoomFactor(double factor)
{
    mRangeZoomFactorHorz = factor;
    mRangeZoomFactorVert = factor;
}

/*! \internal
  
  Draws the background of this axis rect. It may consist of a background fill (a QBrush) and a
  pixmap.
  
  If a brush was given via \ref setBackground(const QBrush &brush), this function first draws an
  according filling inside the axis rect with the provided \a painter.
  
  Then, if a pixmap was provided via \ref setBackground, this function buffers the scaled version
  depending on \ref setBackgroundScaled and \ref setBackgroundScaledMode and then draws it inside
  the axis rect with the provided \a painter. The scaled version is buffered in
  mScaledBackgroundPixmap to prevent expensive rescaling at every redraw. It is only updated, when
  the axis rect has changed in a way that requires a rescale of the background pixmap (this is
  dependent on the \ref setBackgroundScaledMode), or when a differend axis background pixmap was
  set.
  
  \see setBackground, setBackgroundScaled, setBackgroundScaledMode
*/
void QCPAxisRect::drawBackground(QCPPainter *painter)
{
    // draw background fill:
    if (mBackgroundBrush != Qt::NoBrush)
        painter->fillRect(mRect, mBackgroundBrush);

    // draw background pixmap (on top of fill, if brush specified):
    if (!mBackgroundPixmap.isNull())
    {
        if (mBackgroundScaled)
        {
            // check whether mScaledBackground needs to be updated:
            QSize scaledSize(mBackgroundPixmap.size());
            scaledSize.scale(mRect.size(), mBackgroundScaledMode);
            if (mScaledBackgroundPixmap.size() != scaledSize)
                mScaledBackgroundPixmap = mBackgroundPixmap.scaled(mRect.size(), mBackgroundScaledMode, Qt::SmoothTransformation);
            painter->drawPixmap(mRect.topLeft()+QPoint(0, -1), mScaledBackgroundPixmap, QRect(0, 0, mRect.width(), mRect.height()) & mScaledBackgroundPixmap.rect());
        } else
        {
            painter->drawPixmap(mRect.topLeft()+QPoint(0, -1), mBackgroundPixmap, QRect(0, 0, mRect.width(), mRect.height()));
        }
    }
}

/*! \internal
  
  This function makes sure multiple axes on the side specified with \a type don't collide, but are
  distributed according to their respective space requirement (QCPAxis::calculateMargin).
  
  It does this by setting an appropriate offset (\ref QCPAxis::setOffset) on all axes except the
  one with index zero.
  
  This function is called by \ref calculateAutoMargin.
*/
void QCPAxisRect::updateAxesOffset(QCPAxis::AxisType type)
{
    const QList<QCPAxis*> axesList = mAxes.value(type);
    if (axesList.isEmpty())
        return;

    bool isFirstVisible = !axesList.first()->visible(); // if the first axis is visible, the second axis (which is where the loop starts) isn't the first visible axis, so initialize with false
    for (int i=1; i<axesList.size(); ++i)
    {
        int offset = axesList.at(i-1)->offset() + axesList.at(i-1)->calculateMargin();
        if (axesList.at(i)->visible()) // only add inner tick length to offset if this axis is visible and it's not the first visible one (might happen if true first axis is invisible)
        {
            if (!isFirstVisible)
                offset += axesList.at(i)->tickLengthIn();
            isFirstVisible = false;
        }
        axesList.at(i)->setOffset(offset);
    }
}

/* inherits documentation from base class */
int QCPAxisRect::calculateAutoMargin(QCP::MarginSide side)
{
    if (!mAutoMargins.testFlag(side))
        qDebug() << Q_FUNC_INFO << "Called with side that isn't specified as auto margin";

    updateAxesOffset(QCPAxis::marginSideToAxisType(side));

    // note: only need to look at the last (outer most) axis to determine the total margin, due to updateAxisOffset call
    const QList<QCPAxis*> axesList = mAxes.value(QCPAxis::marginSideToAxisType(side));
    if (axesList.size() > 0)
        return axesList.last()->offset() + axesList.last()->calculateMargin();
    else
        return 0;
}

/*! \internal
  
  Reacts to a change in layout to potentially set the convenience axis pointers \ref
  PF_GraphicView::xAxis, \ref PF_GraphicView::yAxis, etc. of the parent PF_GraphicView to the respective
  axes of this axis rect. This is only done if the respective convenience pointer is currently zero
  and if there is no QCPAxisRect at position (0, 0) of the plot layout.
  
  This automation makes it simpler to replace the main axis rect with a newly created one, without
  the need to manually reset the convenience pointers.
*/
void QCPAxisRect::layoutChanged()
{
    if (mParentPlot && mParentPlot->axisRectCount() > 0 && mParentPlot->axisRect(0) == this)
    {
        if (axisCount(QCPAxis::atBottom) > 0 && !mParentPlot->xAxis)
            mParentPlot->xAxis = axis(QCPAxis::atBottom);
        if (axisCount(QCPAxis::atLeft) > 0 && !mParentPlot->yAxis)
            mParentPlot->yAxis = axis(QCPAxis::atLeft);
        if (axisCount(QCPAxis::atTop) > 0 && !mParentPlot->xAxis2)
            mParentPlot->xAxis2 = axis(QCPAxis::atTop);
        if (axisCount(QCPAxis::atRight) > 0 && !mParentPlot->yAxis2)
            mParentPlot->yAxis2 = axis(QCPAxis::atRight);
    }
}

/*! \internal
  
  Event handler for when a mouse button is pressed on the axis rect. If the left mouse button is
  pressed, the range dragging interaction is initialized (the actual range manipulation happens in
  the \ref mouseMoveEvent).

  The mDragging flag is set to true and some anchor points are set that are needed to determine the
  distance the mouse was dragged in the mouse move/release events later.
  
  \see mouseMoveEvent, mouseReleaseEvent
*/
void QCPAxisRect::mousePressEvent(QMouseEvent *event, const QVariant &details)
{
    Q_UNUSED(details)
    if (event->buttons() & Qt::LeftButton)
    {
        mDragging = true;
        // initialize antialiasing backup in case we start dragging:
        if (mParentPlot->noAntialiasingOnDrag())
        {
            mAADragBackup = mParentPlot->antialiasedElements();
            mNotAADragBackup = mParentPlot->notAntialiasedElements();
        }
        // Mouse range dragging interaction:
        if (mParentPlot->interactions().testFlag(QCP::iRangeDrag))
        {
            mDragStartHorzRange.clear();
            for (int i=0; i<mRangeDragHorzAxis.size(); ++i)
                mDragStartHorzRange.append(mRangeDragHorzAxis.at(i).isNull() ? QCPRange() : mRangeDragHorzAxis.at(i)->range());
            mDragStartVertRange.clear();
            for (int i=0; i<mRangeDragVertAxis.size(); ++i)
                mDragStartVertRange.append(mRangeDragVertAxis.at(i).isNull() ? QCPRange() : mRangeDragVertAxis.at(i)->range());
        }
    }
}

/*! \internal
  
  Event handler for when the mouse is moved on the axis rect. If range dragging was activated in a
  preceding \ref mousePressEvent, the range is moved accordingly.
  
  \see mousePressEvent, mouseReleaseEvent
*/
void QCPAxisRect::mouseMoveEvent(QMouseEvent *event, const QPointF &startPos)
{
    Q_UNUSED(startPos)
    // Mouse range dragging interaction:
    if (mDragging && mParentPlot->interactions().testFlag(QCP::iRangeDrag))
    {

        if (mRangeDrag.testFlag(Qt::Horizontal))
        {
            for (int i=0; i<mRangeDragHorzAxis.size(); ++i)
            {
                QCPAxis *ax = mRangeDragHorzAxis.at(i).data();
                if (!ax)
                    continue;
                if (i >= mDragStartHorzRange.size())
                    break;
                if (ax->mScaleType == QCPAxis::stLinear)
                {
                    double diff = ax->pixelToCoord(startPos.x()) - ax->pixelToCoord(event->pos().x());
                    ax->setRange(mDragStartHorzRange.at(i).lower+diff, mDragStartHorzRange.at(i).upper+diff);
                } else if (ax->mScaleType == QCPAxis::stLogarithmic)
                {
                    double diff = ax->pixelToCoord(startPos.x()) / ax->pixelToCoord(event->pos().x());
                    ax->setRange(mDragStartHorzRange.at(i).lower*diff, mDragStartHorzRange.at(i).upper*diff);
                }
            }
        }

        if (mRangeDrag.testFlag(Qt::Vertical))
        {
            for (int i=0; i<mRangeDragVertAxis.size(); ++i)
            {
                QCPAxis *ax = mRangeDragVertAxis.at(i).data();
                if (!ax)
                    continue;
                if (i >= mDragStartVertRange.size())
                    break;
                if (ax->mScaleType == QCPAxis::stLinear)
                {
                    double diff = ax->pixelToCoord(startPos.y()) - ax->pixelToCoord(event->pos().y());
                    ax->setRange(mDragStartVertRange.at(i).lower+diff, mDragStartVertRange.at(i).upper+diff);
                } else if (ax->mScaleType == QCPAxis::stLogarithmic)
                {
                    double diff = ax->pixelToCoord(startPos.y()) / ax->pixelToCoord(event->pos().y());
                    ax->setRange(mDragStartVertRange.at(i).lower*diff, mDragStartVertRange.at(i).upper*diff);
                }
            }
        }

        if (mRangeDrag != 0) // if either vertical or horizontal drag was enabled, do a replot
        {
            if (mParentPlot->noAntialiasingOnDrag())
                mParentPlot->setNotAntialiasedElements(QCP::aeAll);
            mParentPlot->replot(PF_GraphicView::rpQueuedReplot);
        }

    }
}

/* inherits documentation from base class */
void QCPAxisRect::mouseReleaseEvent(QMouseEvent *event, const QPointF &startPos)
{
    Q_UNUSED(event)
    Q_UNUSED(startPos)
    mDragging = false;
    if (mParentPlot->noAntialiasingOnDrag())
    {
        mParentPlot->setAntialiasedElements(mAADragBackup);
        mParentPlot->setNotAntialiasedElements(mNotAADragBackup);
    }
}

/*! \internal
  
  Event handler for mouse wheel events. If rangeZoom is Qt::Horizontal, Qt::Vertical or both, the
  ranges of the axes defined as rangeZoomHorzAxis and rangeZoomVertAxis are scaled. The center of
  the scaling operation is the current cursor position inside the axis rect. The scaling factor is
  dependent on the mouse wheel delta (which direction the wheel was rotated) to provide a natural
  zooming feel. The Strength of the zoom can be controlled via \ref setRangeZoomFactor.
  
  Note, that event->delta() is usually +/-120 for single rotation steps. However, if the mouse
  wheel is turned rapidly, many steps may bunch up to one event, so the event->delta() may then be
  multiples of 120. This is taken into account here, by calculating \a wheelSteps and using it as
  exponent of the range zoom factor. This takes care of the wheel direction automatically, by
  inverting the factor, when the wheel step is negative (f^-1 = 1/f).
*/
void QCPAxisRect::wheelEvent(QWheelEvent *event)
{
    // Mouse range zooming interaction:
    if (mParentPlot->interactions().testFlag(QCP::iRangeZoom))
    {
        if (mRangeZoom != 0)
        {
            double factor;
            double wheelSteps = event->delta()/120.0; // a single step delta is +/-120 usually
            if (mRangeZoom.testFlag(Qt::Horizontal))
            {
                factor = qPow(mRangeZoomFactorHorz, wheelSteps);
                for (int i=0; i<mRangeZoomHorzAxis.size(); ++i)
                {
                    if (!mRangeZoomHorzAxis.at(i).isNull())
                        mRangeZoomHorzAxis.at(i)->scaleRange(factor, mRangeZoomHorzAxis.at(i)->pixelToCoord(event->pos().x()));
                }
            }
            if (mRangeZoom.testFlag(Qt::Vertical))
            {
                factor = qPow(mRangeZoomFactorVert, wheelSteps);
                for (int i=0; i<mRangeZoomVertAxis.size(); ++i)
                {
                    if (!mRangeZoomVertAxis.at(i).isNull())
                        mRangeZoomVertAxis.at(i)->scaleRange(factor, mRangeZoomVertAxis.at(i)->pixelToCoord(event->pos().y()));
                }
            }
            mParentPlot->replot();
        }
    }
}
/* end of 'src/layoutelements/layoutelement-axisrect.cpp' */


/* including file 'src/layoutelements/layoutelement-legend.cpp', size 31153  */
/* commit ce344b3f96a62e5f652585e55f1ae7c7883cd45b 2018-06-25 01:03:39 +0200 */

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPAbstractLegendItem
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPAbstractLegendItem
  \brief The abstract base class for all entries in a QCPLegend.
  
  It defines a very basic interface for entries in a QCPLegend. For representing plottables in the
  legend, the subclass \ref QCPPlottableLegendItem is more suitable.
  
  Only derive directly from this class when you need absolute freedom (e.g. a custom legend entry
  that's not even associated with a plottable).

  You must implement the following pure virtual functions:
  \li \ref draw (from QCPLayerable)
  
  You inherit the following members you may use:
  <table>
    <tr>
      <td>QCPLegend *\b mParentLegend</td>
      <td>A pointer to the parent QCPLegend.</td>
    </tr><tr>
      <td>QFont \b mFont</td>
      <td>The generic font of the item. You should use this font for all or at least the most prominent text of the item.</td>
    </tr>
  </table>
*/

/* start of documentation of signals */

/*! \fn void QCPAbstractLegendItem::selectionChanged(bool selected)
  
  This signal is emitted when the selection state of this legend item has changed, either by user
  interaction or by a direct call to \ref setSelected.
*/

/* end of documentation of signals */

/*!
  Constructs a QCPAbstractLegendItem and associates it with the QCPLegend \a parent. This does not
  cause the item to be added to \a parent, so \ref QCPLegend::addItem must be called separately.
*/
QCPAbstractLegendItem::QCPAbstractLegendItem(QCPLegend *parent) :
    QCPLayoutElement(parent->parentPlot()),
    mParentLegend(parent),
    mFont(parent->font()),
    mTextColor(parent->textColor()),
    mSelectedFont(parent->selectedFont()),
    mSelectedTextColor(parent->selectedTextColor()),
    mSelectable(true),
    mSelected(false)
{
    setLayer(QLatin1String("legend"));
    setMargins(QMargins(0, 0, 0, 0));
}

/*!
  Sets the default font of this specific legend item to \a font.
  
  \see setTextColor, QCPLegend::setFont
*/
void QCPAbstractLegendItem::setFont(const QFont &font)
{
    mFont = font;
}

/*!
  Sets the default text color of this specific legend item to \a color.
  
  \see setFont, QCPLegend::setTextColor
*/
void QCPAbstractLegendItem::setTextColor(const QColor &color)
{
    mTextColor = color;
}

/*!
  When this legend item is selected, \a font is used to draw generic text, instead of the normal
  font set with \ref setFont.
  
  \see setFont, QCPLegend::setSelectedFont
*/
void QCPAbstractLegendItem::setSelectedFont(const QFont &font)
{
    mSelectedFont = font;
}

/*!
  When this legend item is selected, \a color is used to draw generic text, instead of the normal
  color set with \ref setTextColor.
  
  \see setTextColor, QCPLegend::setSelectedTextColor
*/
void QCPAbstractLegendItem::setSelectedTextColor(const QColor &color)
{
    mSelectedTextColor = color;
}

/*!
  Sets whether this specific legend item is selectable.
  
  \see setSelectedParts, PF_GraphicView::setInteractions
*/
void QCPAbstractLegendItem::setSelectable(bool selectable)
{
    if (mSelectable != selectable)
    {
        mSelectable = selectable;
        emit selectableChanged(mSelectable);
    }
}

/*!
  Sets whether this specific legend item is selected.
  
  It is possible to set the selection state of this item by calling this function directly, even if
  setSelectable is set to false.
  
  \see setSelectableParts, PF_GraphicView::setInteractions
*/
void QCPAbstractLegendItem::setSelected(bool selected)
{
    if (mSelected != selected)
    {
        mSelected = selected;
        emit selectionChanged(mSelected);
    }
}

/* inherits documentation from base class */
double QCPAbstractLegendItem::selectTest(const QPointF &pos, bool onlySelectable, QVariant *details) const
{
    Q_UNUSED(details)
    if (!mParentPlot) return -1;
    if (onlySelectable && (!mSelectable || !mParentLegend->selectableParts().testFlag(QCPLegend::spItems)))
        return -1;

    if (mRect.contains(pos.toPoint()))
        return mParentPlot->selectionTolerance()*0.99;
    else
        return -1;
}

/* inherits documentation from base class */
void QCPAbstractLegendItem::applyDefaultAntialiasingHint(QCPPainter *painter) const
{
    applyAntialiasingHint(painter, mAntialiased, QCP::aeLegendItems);
}

/* inherits documentation from base class */
QRect QCPAbstractLegendItem::clipRect() const
{
    return mOuterRect;
}

/* inherits documentation from base class */
void QCPAbstractLegendItem::selectEvent(QMouseEvent *event, bool additive, const QVariant &details, bool *selectionStateChanged)
{
    Q_UNUSED(event)
    Q_UNUSED(details)
    if (mSelectable && mParentLegend->selectableParts().testFlag(QCPLegend::spItems))
    {
        bool selBefore = mSelected;
        setSelected(additive ? !mSelected : true);
        if (selectionStateChanged)
            *selectionStateChanged = mSelected != selBefore;
    }
}

/* inherits documentation from base class */
void QCPAbstractLegendItem::deselectEvent(bool *selectionStateChanged)
{
    if (mSelectable && mParentLegend->selectableParts().testFlag(QCPLegend::spItems))
    {
        bool selBefore = mSelected;
        setSelected(false);
        if (selectionStateChanged)
            *selectionStateChanged = mSelected != selBefore;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPPlottableLegendItem
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPPlottableLegendItem
  \brief A legend item representing a plottable with an icon and the plottable name.
  
  This is the standard legend item for plottables. It displays an icon of the plottable next to the
  plottable name. The icon is drawn by the respective plottable itself (\ref
  QCPAbstractPlottable::drawLegendIcon), and tries to give an intuitive symbol for the plottable.
  For example, the QCPGraph draws a centered horizontal line and/or a single scatter point in the
  middle.
  
  Legend items of this type are always associated with one plottable (retrievable via the
  plottable() function and settable with the constructor). You may change the font of the plottable
  name with \ref setFont. Icon padding and border pen is taken from the parent QCPLegend, see \ref
  QCPLegend::setIconBorderPen and \ref QCPLegend::setIconTextPadding.

  The function \ref QCPAbstractPlottable::addToLegend/\ref QCPAbstractPlottable::removeFromLegend
  creates/removes legend items of this type.
  
  Since QCPLegend is based on QCPLayoutGrid, a legend item itself is just a subclass of
  QCPLayoutElement. While it could be added to a legend (or any other layout) via the normal layout
  interface, QCPLegend has specialized functions for handling legend items conveniently, see the
  documentation of \ref QCPLegend.
*/

/*!
  Creates a new legend item associated with \a plottable.
  
  Once it's created, it can be added to the legend via \ref QCPLegend::addItem.
  
  A more convenient way of adding/removing a plottable to/from the legend is via the functions \ref
  QCPAbstractPlottable::addToLegend and \ref QCPAbstractPlottable::removeFromLegend.
*/
QCPPlottableLegendItem::QCPPlottableLegendItem(QCPLegend *parent, QCPAbstractPlottable *plottable) :
    QCPAbstractLegendItem(parent),
    mPlottable(plottable)
{
    setAntialiased(false);
}

/*! \internal
  
  Returns the pen that shall be used to draw the icon border, taking into account the selection
  state of this item.
*/
QPen QCPPlottableLegendItem::getIconBorderPen() const
{
    return mSelected ? mParentLegend->selectedIconBorderPen() : mParentLegend->iconBorderPen();
}

/*! \internal
  
  Returns the text color that shall be used to draw text, taking into account the selection state
  of this item.
*/
QColor QCPPlottableLegendItem::getTextColor() const
{
    return mSelected ? mSelectedTextColor : mTextColor;
}

/*! \internal
  
  Returns the font that shall be used to draw text, taking into account the selection state of this
  item.
*/
QFont QCPPlottableLegendItem::getFont() const
{
    return mSelected ? mSelectedFont : mFont;
}

/*! \internal
  
  Draws the item with \a painter. The size and position of the drawn legend item is defined by the
  parent layout (typically a \ref QCPLegend) and the \ref minimumOuterSizeHint and \ref
  maximumOuterSizeHint of this legend item.
*/
void QCPPlottableLegendItem::draw(QCPPainter *painter)
{
    if (!mPlottable) return;
    painter->setFont(getFont());
    painter->setPen(QPen(getTextColor()));
    QSizeF iconSize = mParentLegend->iconSize();
    QRectF textRect = painter->fontMetrics().boundingRect(0, 0, 0, iconSize.height(), Qt::TextDontClip, mPlottable->name());
    QRectF iconRect(mRect.topLeft(), iconSize);
    int textHeight = qMax(textRect.height(), iconSize.height());  // if text has smaller height than icon, center text vertically in icon height, else align tops
    painter->drawText(mRect.x()+iconSize.width()+mParentLegend->iconTextPadding(), mRect.y(), textRect.width(), textHeight, Qt::TextDontClip, mPlottable->name());
    // draw icon:
    painter->save();
    painter->setClipRect(iconRect, Qt::IntersectClip);
    mPlottable->drawLegendIcon(painter, iconRect);
    painter->restore();
    // draw icon border:
    if (getIconBorderPen().style() != Qt::NoPen)
    {
        painter->setPen(getIconBorderPen());
        painter->setBrush(Qt::NoBrush);
        int halfPen = qCeil(painter->pen().widthF()*0.5)+1;
        painter->setClipRect(mOuterRect.adjusted(-halfPen, -halfPen, halfPen, halfPen)); // extend default clip rect so thicker pens (especially during selection) are not clipped
        painter->drawRect(iconRect);
    }
}

/*! \internal
  
  Calculates and returns the size of this item. This includes the icon, the text and the padding in
  between.
  
  \seebaseclassmethod
*/
QSize QCPPlottableLegendItem::minimumOuterSizeHint() const
{
    if (!mPlottable) return QSize();
    QSize result(0, 0);
    QRect textRect;
    QFontMetrics fontMetrics(getFont());
    QSize iconSize = mParentLegend->iconSize();
    textRect = fontMetrics.boundingRect(0, 0, 0, iconSize.height(), Qt::TextDontClip, mPlottable->name());
    result.setWidth(iconSize.width() + mParentLegend->iconTextPadding() + textRect.width());
    result.setHeight(qMax(textRect.height(), iconSize.height()));
    result.rwidth() += mMargins.left()+mMargins.right();
    result.rheight() += mMargins.top()+mMargins.bottom();
    return result;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPLegend
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPLegend
  \brief Manages a legend inside a PF_GraphicView.

  A legend is a small box somewhere in the plot which lists plottables with their name and icon.

  A legend is populated with legend items by calling \ref QCPAbstractPlottable::addToLegend on the
  plottable, for which a legend item shall be created. In the case of the main legend (\ref
  PF_GraphicView::legend), simply adding plottables to the plot while \ref
  PF_GraphicView::setAutoAddPlottableToLegend is set to true (the default) creates corresponding
  legend items. The legend item associated with a certain plottable can be removed with \ref
  QCPAbstractPlottable::removeFromLegend. However, QCPLegend also offers an interface to add and
  manipulate legend items directly: \ref item, \ref itemWithPlottable, \ref itemCount, \ref
  addItem, \ref removeItem, etc.

  Since \ref QCPLegend derives from \ref QCPLayoutGrid, it can be placed in any position a \ref
  QCPLayoutElement may be positioned. The legend items are themselves \ref QCPLayoutElement
  "QCPLayoutElements" which are placed in the grid layout of the legend. \ref QCPLegend only adds
  an interface specialized for handling child elements of type \ref QCPAbstractLegendItem, as
  mentioned above. In principle, any other layout elements may also be added to a legend via the
  normal \ref QCPLayoutGrid interface. See the special page about \link thelayoutsystem The Layout
  System\endlink for examples on how to add other elements to the legend and move it outside the axis
  rect.

  Use the methods \ref setFillOrder and \ref setWrap inherited from \ref QCPLayoutGrid to control
  in which order (column first or row first) the legend is filled up when calling \ref addItem, and
  at which column or row wrapping occurs. The default fill order for legends is \ref foRowsFirst.

  By default, every PF_GraphicView has one legend (\ref PF_GraphicView::legend) which is placed in the
  inset layout of the main axis rect (\ref QCPAxisRect::insetLayout). To move the legend to another
  position inside the axis rect, use the methods of the \ref QCPLayoutInset. To move the legend
  outside of the axis rect, place it anywhere else with the \ref QCPLayout/\ref QCPLayoutElement
  interface.
*/

/* start of documentation of signals */

/*! \fn void QCPLegend::selectionChanged(QCPLegend::SelectableParts selection);

  This signal is emitted when the selection state of this legend has changed.
  
  \see setSelectedParts, setSelectableParts
*/

/* end of documentation of signals */

/*!
  Constructs a new QCPLegend instance with default values.
  
  Note that by default, PF_GraphicView already contains a legend ready to be used as \ref
  PF_GraphicView::legend
*/
QCPLegend::QCPLegend()
{
    setFillOrder(QCPLayoutGrid::foRowsFirst);
    setWrap(0);

    setRowSpacing(3);
    setColumnSpacing(8);
    setMargins(QMargins(7, 5, 7, 4));
    setAntialiased(false);
    setIconSize(32, 18);

    setIconTextPadding(7);

    setSelectableParts(spLegendBox | spItems);
    setSelectedParts(spNone);

    setBorderPen(QPen(Qt::black, 0));
    setSelectedBorderPen(QPen(Qt::blue, 2));
    setIconBorderPen(Qt::NoPen);
    setSelectedIconBorderPen(QPen(Qt::blue, 2));
    setBrush(Qt::white);
    setSelectedBrush(Qt::white);
    setTextColor(Qt::black);
    setSelectedTextColor(Qt::blue);
}

QCPLegend::~QCPLegend()
{
    clearItems();
    if (qobject_cast<PF_GraphicView*>(mParentPlot)) // make sure this isn't called from QObject dtor when PF_GraphicView is already destructed (happens when the legend is not in any layout and thus QObject-child of PF_GraphicView)
        mParentPlot->legendRemoved(this);
}

/* no doc for getter, see setSelectedParts */
QCPLegend::SelectableParts QCPLegend::selectedParts() const
{
    // check whether any legend elements selected, if yes, add spItems to return value
    bool hasSelectedItems = false;
    for (int i=0; i<itemCount(); ++i)
    {
        if (item(i) && item(i)->selected())
        {
            hasSelectedItems = true;
            break;
        }
    }
    if (hasSelectedItems)
        return mSelectedParts | spItems;
    else
        return mSelectedParts & ~spItems;
}

/*!
  Sets the pen, the border of the entire legend is drawn with.
*/
void QCPLegend::setBorderPen(const QPen &pen)
{
    mBorderPen = pen;
}

/*!
  Sets the brush of the legend background.
*/
void QCPLegend::setBrush(const QBrush &brush)
{
    mBrush = brush;
}

/*!
  Sets the default font of legend text. Legend items that draw text (e.g. the name of a graph) will
  use this font by default. However, a different font can be specified on a per-item-basis by
  accessing the specific legend item.
  
  This function will also set \a font on all already existing legend items.
  
  \see QCPAbstractLegendItem::setFont
*/
void QCPLegend::setFont(const QFont &font)
{
    mFont = font;
    for (int i=0; i<itemCount(); ++i)
    {
        if (item(i))
            item(i)->setFont(mFont);
    }
}

/*!
  Sets the default color of legend text. Legend items that draw text (e.g. the name of a graph)
  will use this color by default. However, a different colors can be specified on a per-item-basis
  by accessing the specific legend item.
  
  This function will also set \a color on all already existing legend items.
  
  \see QCPAbstractLegendItem::setTextColor
*/
void QCPLegend::setTextColor(const QColor &color)
{
    mTextColor = color;
    for (int i=0; i<itemCount(); ++i)
    {
        if (item(i))
            item(i)->setTextColor(color);
    }
}

/*!
  Sets the size of legend icons. Legend items that draw an icon (e.g. a visual
  representation of the graph) will use this size by default.
*/
void QCPLegend::setIconSize(const QSize &size)
{
    mIconSize = size;
}

/*! \overload
*/
void QCPLegend::setIconSize(int width, int height)
{
    mIconSize.setWidth(width);
    mIconSize.setHeight(height);
}

/*!
  Sets the horizontal space in pixels between the legend icon and the text next to it.
  Legend items that draw an icon (e.g. a visual representation of the graph) and text (e.g. the
  name of the graph) will use this space by default.
*/
void QCPLegend::setIconTextPadding(int padding)
{
    mIconTextPadding = padding;
}

/*!
  Sets the pen used to draw a border around each legend icon. Legend items that draw an
  icon (e.g. a visual representation of the graph) will use this pen by default.
  
  If no border is wanted, set this to \a Qt::NoPen.
*/
void QCPLegend::setIconBorderPen(const QPen &pen)
{
    mIconBorderPen = pen;
}

/*!
  Sets whether the user can (de-)select the parts in \a selectable by clicking on the PF_GraphicView surface.
  (When \ref PF_GraphicView::setInteractions contains \ref QCP::iSelectLegend.)
  
  However, even when \a selectable is set to a value not allowing the selection of a specific part,
  it is still possible to set the selection of this part manually, by calling \ref setSelectedParts
  directly.
  
  \see SelectablePart, setSelectedParts
*/
void QCPLegend::setSelectableParts(const SelectableParts &selectable)
{
    if (mSelectableParts != selectable)
    {
        mSelectableParts = selectable;
        emit selectableChanged(mSelectableParts);
    }
}

/*!
  Sets the selected state of the respective legend parts described by \ref SelectablePart. When a part
  is selected, it uses a different pen/font and brush. If some legend items are selected and \a selected
  doesn't contain \ref spItems, those items become deselected.
  
  The entire selection mechanism is handled automatically when \ref PF_GraphicView::setInteractions
  contains iSelectLegend. You only need to call this function when you wish to change the selection
  state manually.
  
  This function can change the selection state of a part even when \ref setSelectableParts was set to a
  value that actually excludes the part.
  
  emits the \ref selectionChanged signal when \a selected is different from the previous selection state.
  
  Note that it doesn't make sense to set the selected state \ref spItems here when it wasn't set
  before, because there's no way to specify which exact items to newly select. Do this by calling
  \ref QCPAbstractLegendItem::setSelected directly on the legend item you wish to select.
  
  \see SelectablePart, setSelectableParts, selectTest, setSelectedBorderPen, setSelectedIconBorderPen, setSelectedBrush,
  setSelectedFont
*/
void QCPLegend::setSelectedParts(const SelectableParts &selected)
{
    SelectableParts newSelected = selected;
    mSelectedParts = this->selectedParts(); // update mSelectedParts in case item selection changed

    if (mSelectedParts != newSelected)
    {
        if (!mSelectedParts.testFlag(spItems) && newSelected.testFlag(spItems)) // attempt to set spItems flag (can't do that)
        {
            qDebug() << Q_FUNC_INFO << "spItems flag can not be set, it can only be unset with this function";
            newSelected &= ~spItems;
        }
        if (mSelectedParts.testFlag(spItems) && !newSelected.testFlag(spItems)) // spItems flag was unset, so clear item selection
        {
            for (int i=0; i<itemCount(); ++i)
            {
                if (item(i))
                    item(i)->setSelected(false);
            }
        }
        mSelectedParts = newSelected;
        emit selectionChanged(mSelectedParts);
    }
}

/*!
  When the legend box is selected, this pen is used to draw the border instead of the normal pen
  set via \ref setBorderPen.

  \see setSelectedParts, setSelectableParts, setSelectedBrush
*/
void QCPLegend::setSelectedBorderPen(const QPen &pen)
{
    mSelectedBorderPen = pen;
}

/*!
  Sets the pen legend items will use to draw their icon borders, when they are selected.

  \see setSelectedParts, setSelectableParts, setSelectedFont
*/
void QCPLegend::setSelectedIconBorderPen(const QPen &pen)
{
    mSelectedIconBorderPen = pen;
}

/*!
  When the legend box is selected, this brush is used to draw the legend background instead of the normal brush
  set via \ref setBrush.

  \see setSelectedParts, setSelectableParts, setSelectedBorderPen
*/
void QCPLegend::setSelectedBrush(const QBrush &brush)
{
    mSelectedBrush = brush;
}

/*!
  Sets the default font that is used by legend items when they are selected.
  
  This function will also set \a font on all already existing legend items.

  \see setFont, QCPAbstractLegendItem::setSelectedFont
*/
void QCPLegend::setSelectedFont(const QFont &font)
{
    mSelectedFont = font;
    for (int i=0; i<itemCount(); ++i)
    {
        if (item(i))
            item(i)->setSelectedFont(font);
    }
}

/*!
  Sets the default text color that is used by legend items when they are selected.
  
  This function will also set \a color on all already existing legend items.

  \see setTextColor, QCPAbstractLegendItem::setSelectedTextColor
*/
void QCPLegend::setSelectedTextColor(const QColor &color)
{
    mSelectedTextColor = color;
    for (int i=0; i<itemCount(); ++i)
    {
        if (item(i))
            item(i)->setSelectedTextColor(color);
    }
}

/*!
  Returns the item with index \a i.

  Note that the linear index depends on the current fill order (\ref setFillOrder).

  \see itemCount, addItem, itemWithPlottable
*/
QCPAbstractLegendItem *QCPLegend::item(int index) const
{
    return qobject_cast<QCPAbstractLegendItem*>(elementAt(index));
}

/*!
  Returns the QCPPlottableLegendItem which is associated with \a plottable (e.g. a \ref QCPGraph*).
  If such an item isn't in the legend, returns 0.
  
  \see hasItemWithPlottable
*/
QCPPlottableLegendItem *QCPLegend::itemWithPlottable(const QCPAbstractPlottable *plottable) const
{
    for (int i=0; i<itemCount(); ++i)
    {
        if (QCPPlottableLegendItem *pli = qobject_cast<QCPPlottableLegendItem*>(item(i)))
        {
            if (pli->plottable() == plottable)
                return pli;
        }
    }
    return nullptr;
}

/*!
  Returns the number of items currently in the legend.

  Note that if empty cells are in the legend (e.g. by calling methods of the \ref QCPLayoutGrid
  base class which allows creating empty cells), they are included in the returned count.

  \see item
*/
int QCPLegend::itemCount() const
{
    return elementCount();
}

/*!
  Returns whether the legend contains \a item.
  
  \see hasItemWithPlottable
*/
bool QCPLegend::hasItem(QCPAbstractLegendItem *item) const
{
    for (int i=0; i<itemCount(); ++i)
    {
        if (item == this->item(i))
            return true;
    }
    return false;
}

/*!
  Returns whether the legend contains a QCPPlottableLegendItem which is associated with \a plottable (e.g. a \ref QCPGraph*).
  If such an item isn't in the legend, returns false.
  
  \see itemWithPlottable
*/
bool QCPLegend::hasItemWithPlottable(const QCPAbstractPlottable *plottable) const
{
    return itemWithPlottable(plottable);
}

/*!
  Adds \a item to the legend, if it's not present already. The element is arranged according to the
  current fill order (\ref setFillOrder) and wrapping (\ref setWrap).

  Returns true on sucess, i.e. if the item wasn't in the list already and has been successfuly added.

  The legend takes ownership of the item.

  \see removeItem, item, hasItem
*/
bool QCPLegend::addItem(QCPAbstractLegendItem *item)
{
    return addElement(item);
}

/*! \overload

  Removes the item with the specified \a index from the legend and deletes it.

  After successful removal, the legend is reordered according to the current fill order (\ref
  setFillOrder) and wrapping (\ref setWrap), so no empty cell remains where the removed \a item
  was. If you don't want this, rather use the raw element interface of \ref QCPLayoutGrid.

  Returns true, if successful. Unlike \ref QCPLayoutGrid::removeAt, this method only removes
  elements derived from \ref QCPAbstractLegendItem.

  \see itemCount, clearItems
*/
bool QCPLegend::removeItem(int index)
{
    if (QCPAbstractLegendItem *ali = item(index))
    {
        bool success = remove(ali);
        if (success)
            setFillOrder(fillOrder(), true); // gets rid of empty cell by reordering
        return success;
    } else
        return false;
}

/*! \overload

  Removes \a item from the legend and deletes it.

  After successful removal, the legend is reordered according to the current fill order (\ref
  setFillOrder) and wrapping (\ref setWrap), so no empty cell remains where the removed \a item
  was. If you don't want this, rather use the raw element interface of \ref QCPLayoutGrid.

  Returns true, if successful.

  \see clearItems
*/
bool QCPLegend::removeItem(QCPAbstractLegendItem *item)
{
    bool success = remove(item);
    if (success)
        setFillOrder(fillOrder(), true); // gets rid of empty cell by reordering
    return success;
}

/*!
  Removes all items from the legend.
*/
void QCPLegend::clearItems()
{
    for (int i=itemCount()-1; i>=0; --i)
        removeItem(i);
}

/*!
  Returns the legend items that are currently selected. If no items are selected,
  the list is empty.
  
  \see QCPAbstractLegendItem::setSelected, setSelectable
*/
QList<QCPAbstractLegendItem *> QCPLegend::selectedItems() const
{
    QList<QCPAbstractLegendItem*> result;
    for (int i=0; i<itemCount(); ++i)
    {
        if (QCPAbstractLegendItem *ali = item(i))
        {
            if (ali->selected())
                result.append(ali);
        }
    }
    return result;
}

/*! \internal

  A convenience function to easily set the QPainter::Antialiased hint on the provided \a painter
  before drawing main legend elements.

  This is the antialiasing state the painter passed to the \ref draw method is in by default.
  
  This function takes into account the local setting of the antialiasing flag as well as the
  overrides set with \ref PF_GraphicView::setAntialiasedElements and \ref
  PF_GraphicView::setNotAntialiasedElements.
  
  \seebaseclassmethod
  
  \see setAntialiased
*/
void QCPLegend::applyDefaultAntialiasingHint(QCPPainter *painter) const
{
    applyAntialiasingHint(painter, mAntialiased, QCP::aeLegend);
}

/*! \internal
  
  Returns the pen used to paint the border of the legend, taking into account the selection state
  of the legend box.
*/
QPen QCPLegend::getBorderPen() const
{
    return mSelectedParts.testFlag(spLegendBox) ? mSelectedBorderPen : mBorderPen;
}

/*! \internal
  
  Returns the brush used to paint the background of the legend, taking into account the selection
  state of the legend box.
*/
QBrush QCPLegend::getBrush() const
{
    return mSelectedParts.testFlag(spLegendBox) ? mSelectedBrush : mBrush;
}

/*! \internal
  
  Draws the legend box with the provided \a painter. The individual legend items are layerables
  themselves, thus are drawn independently.
*/
void QCPLegend::draw(QCPPainter *painter)
{
    // draw background rect:
    painter->setBrush(getBrush());
    painter->setPen(getBorderPen());
    painter->drawRect(mOuterRect);
}

/* inherits documentation from base class */
double QCPLegend::selectTest(const QPointF &pos, bool onlySelectable, QVariant *details) const
{
    if (!mParentPlot) return -1;
    if (onlySelectable && !mSelectableParts.testFlag(spLegendBox))
        return -1;

    if (mOuterRect.contains(pos.toPoint()))
    {
        if (details) details->setValue(spLegendBox);
        return mParentPlot->selectionTolerance()*0.99;
    }
    return -1;
}

/* inherits documentation from base class */
void QCPLegend::selectEvent(QMouseEvent *event, bool additive, const QVariant &details, bool *selectionStateChanged)
{
    Q_UNUSED(event)
    mSelectedParts = selectedParts(); // in case item selection has changed
    if (details.value<SelectablePart>() == spLegendBox && mSelectableParts.testFlag(spLegendBox))
    {
        SelectableParts selBefore = mSelectedParts;
        setSelectedParts(additive ? mSelectedParts^spLegendBox : mSelectedParts|spLegendBox); // no need to unset spItems in !additive case, because they will be deselected by PF_GraphicView (they're normal QCPLayerables with own deselectEvent)
        if (selectionStateChanged)
            *selectionStateChanged = mSelectedParts != selBefore;
    }
}

/* inherits documentation from base class */
void QCPLegend::deselectEvent(bool *selectionStateChanged)
{
    mSelectedParts = selectedParts(); // in case item selection has changed
    if (mSelectableParts.testFlag(spLegendBox))
    {
        SelectableParts selBefore = mSelectedParts;
        setSelectedParts(selectedParts() & ~spLegendBox);
        if (selectionStateChanged)
            *selectionStateChanged = mSelectedParts != selBefore;
    }
}

/* inherits documentation from base class */
QCP::Interaction QCPLegend::selectionCategory() const
{
    return QCP::iSelectLegend;
}

/* inherits documentation from base class */
QCP::Interaction QCPAbstractLegendItem::selectionCategory() const
{
    return QCP::iSelectLegend;
}

/* inherits documentation from base class */
void QCPLegend::parentPlotInitialized(PF_GraphicView *parentPlot)
{
    if (parentPlot && !parentPlot->legend)
        parentPlot->legend = this;
}
/* end of 'src/layoutelements/layoutelement-legend.cpp' */


/* including file 'src/layoutelements/layoutelement-textelement.cpp', size 12761 */
/* commit ce344b3f96a62e5f652585e55f1ae7c7883cd45b 2018-06-25 01:03:39 +0200     */

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPTextElement
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPTextElement
  \brief A layout element displaying a text

  The text may be specified with \ref setText, the formatting can be controlled with \ref setFont,
  \ref setTextColor, and \ref setTextFlags.

  A text element can be added as follows:
  \snippet documentation/doc-code-snippets/mainwindow.cpp qcptextelement-creation
*/

/* start documentation of signals */

/*! \fn void QCPTextElement::selectionChanged(bool selected)
  
  This signal is emitted when the selection state has changed to \a selected, either by user
  interaction or by a direct call to \ref setSelected.
  
  \see setSelected, setSelectable
*/

/*! \fn void QCPTextElement::clicked(QMouseEvent *event)

  This signal is emitted when the text element is clicked.

  \see doubleClicked, selectTest
*/

/*! \fn void QCPTextElement::doubleClicked(QMouseEvent *event)

  This signal is emitted when the text element is double clicked.

  \see clicked, selectTest
*/

/* end documentation of signals */

/*! \overload
  
  Creates a new QCPTextElement instance and sets default values. The initial text is empty (\ref
  setText).
*/
QCPTextElement::QCPTextElement(PF_GraphicView *parentPlot) :
    QCPLayoutElement(parentPlot),
    mText(),
    mTextFlags(Qt::AlignCenter|Qt::TextWordWrap),
    mFont(QFont(QLatin1String("sans serif"), 12)), // will be taken from parentPlot if available, see below
    mTextColor(Qt::black),
    mSelectedFont(QFont(QLatin1String("sans serif"), 12)), // will be taken from parentPlot if available, see below
    mSelectedTextColor(Qt::blue),
    mSelectable(false),
    mSelected(false)
{
    if (parentPlot)
    {
        mFont = parentPlot->font();
        mSelectedFont = parentPlot->font();
    }
    setMargins(QMargins(2, 2, 2, 2));
}

/*! \overload
  
  Creates a new QCPTextElement instance and sets default values.

  The initial text is set to \a text.
*/
QCPTextElement::QCPTextElement(PF_GraphicView *parentPlot, const QString &text) :
    QCPLayoutElement(parentPlot),
    mText(text),
    mTextFlags(Qt::AlignCenter|Qt::TextWordWrap),
    mFont(QFont(QLatin1String("sans serif"), 12)), // will be taken from parentPlot if available, see below
    mTextColor(Qt::black),
    mSelectedFont(QFont(QLatin1String("sans serif"), 12)), // will be taken from parentPlot if available, see below
    mSelectedTextColor(Qt::blue),
    mSelectable(false),
    mSelected(false)
{
    if (parentPlot)
    {
        mFont = parentPlot->font();
        mSelectedFont = parentPlot->font();
    }
    setMargins(QMargins(2, 2, 2, 2));
}

/*! \overload
  
  Creates a new QCPTextElement instance and sets default values.

  The initial text is set to \a text with \a pointSize.
*/
QCPTextElement::QCPTextElement(PF_GraphicView *parentPlot, const QString &text, double pointSize) :
    QCPLayoutElement(parentPlot),
    mText(text),
    mTextFlags(Qt::AlignCenter|Qt::TextWordWrap),
    mFont(QFont(QLatin1String("sans serif"), pointSize)), // will be taken from parentPlot if available, see below
    mTextColor(Qt::black),
    mSelectedFont(QFont(QLatin1String("sans serif"), pointSize)), // will be taken from parentPlot if available, see below
    mSelectedTextColor(Qt::blue),
    mSelectable(false),
    mSelected(false)
{
    if (parentPlot)
    {
        mFont = parentPlot->font();
        mFont.setPointSizeF(pointSize);
        mSelectedFont = parentPlot->font();
        mSelectedFont.setPointSizeF(pointSize);
    }
    setMargins(QMargins(2, 2, 2, 2));
}

/*! \overload
  
  Creates a new QCPTextElement instance and sets default values.

  The initial text is set to \a text with \a pointSize and the specified \a fontFamily.
*/
QCPTextElement::QCPTextElement(PF_GraphicView *parentPlot, const QString &text, const QString &fontFamily, double pointSize) :
    QCPLayoutElement(parentPlot),
    mText(text),
    mTextFlags(Qt::AlignCenter|Qt::TextWordWrap),
    mFont(QFont(fontFamily, pointSize)),
    mTextColor(Qt::black),
    mSelectedFont(QFont(fontFamily, pointSize)),
    mSelectedTextColor(Qt::blue),
    mSelectable(false),
    mSelected(false)
{
    setMargins(QMargins(2, 2, 2, 2));
}

/*! \overload
  
  Creates a new QCPTextElement instance and sets default values.

  The initial text is set to \a text with the specified \a font.
*/
QCPTextElement::QCPTextElement(PF_GraphicView *parentPlot, const QString &text, const QFont &font) :
    QCPLayoutElement(parentPlot),
    mText(text),
    mTextFlags(Qt::AlignCenter|Qt::TextWordWrap),
    mFont(font),
    mTextColor(Qt::black),
    mSelectedFont(font),
    mSelectedTextColor(Qt::blue),
    mSelectable(false),
    mSelected(false)
{
    setMargins(QMargins(2, 2, 2, 2));
}

/*!
  Sets the text that will be displayed to \a text. Multiple lines can be created by insertion of "\n".
  
  \see setFont, setTextColor, setTextFlags
*/
void QCPTextElement::setText(const QString &text)
{
    mText = text;
}

/*!
  Sets options for text alignment and wrapping behaviour. \a flags is a bitwise OR-combination of
  \c Qt::AlignmentFlag and \c Qt::TextFlag enums.
  
  Possible enums are:
  - Qt::AlignLeft
  - Qt::AlignRight
  - Qt::AlignHCenter
  - Qt::AlignJustify
  - Qt::AlignTop
  - Qt::AlignBottom
  - Qt::AlignVCenter
  - Qt::AlignCenter
  - Qt::TextDontClip
  - Qt::TextSingleLine
  - Qt::TextExpandTabs
  - Qt::TextShowMnemonic
  - Qt::TextWordWrap
  - Qt::TextIncludeTrailingSpaces
*/
void QCPTextElement::setTextFlags(int flags)
{
    mTextFlags = flags;
}

/*!
  Sets the \a font of the text.
  
  \see setTextColor, setSelectedFont
*/
void QCPTextElement::setFont(const QFont &font)
{
    mFont = font;
}

/*!
  Sets the \a color of the text.
  
  \see setFont, setSelectedTextColor
*/
void QCPTextElement::setTextColor(const QColor &color)
{
    mTextColor = color;
}

/*!
  Sets the \a font of the text that will be used if the text element is selected (\ref setSelected).
  
  \see setFont
*/
void QCPTextElement::setSelectedFont(const QFont &font)
{
    mSelectedFont = font;
}

/*!
  Sets the \a color of the text that will be used if the text element is selected (\ref setSelected).
  
  \see setTextColor
*/
void QCPTextElement::setSelectedTextColor(const QColor &color)
{
    mSelectedTextColor = color;
}

/*!
  Sets whether the user may select this text element.

  Note that even when \a selectable is set to <tt>false</tt>, the selection state may be changed
  programmatically via \ref setSelected.
*/
void QCPTextElement::setSelectable(bool selectable)
{
    if (mSelectable != selectable)
    {
        mSelectable = selectable;
        emit selectableChanged(mSelectable);
    }
}

/*!
  Sets the selection state of this text element to \a selected. If the selection has changed, \ref
  selectionChanged is emitted.
  
  Note that this function can change the selection state independently of the current \ref
  setSelectable state.
*/
void QCPTextElement::setSelected(bool selected)
{
    if (mSelected != selected)
    {
        mSelected = selected;
        emit selectionChanged(mSelected);
    }
}

/* inherits documentation from base class */
void QCPTextElement::applyDefaultAntialiasingHint(QCPPainter *painter) const
{
    applyAntialiasingHint(painter, mAntialiased, QCP::aeOther);
}

/* inherits documentation from base class */
void QCPTextElement::draw(QCPPainter *painter)
{
    painter->setFont(mainFont());
    painter->setPen(QPen(mainTextColor()));
    painter->drawText(mRect, Qt::AlignCenter, mText, &mTextBoundingRect);
}

/* inherits documentation from base class */
QSize QCPTextElement::minimumOuterSizeHint() const
{
    QFontMetrics metrics(mFont);
    QSize result(metrics.boundingRect(0, 0, 0, 0, Qt::AlignCenter, mText).size());
    result.rwidth() += mMargins.left()+mMargins.right();
    result.rheight() += mMargins.top()+mMargins.bottom();
    return result;
}

/* inherits documentation from base class */
QSize QCPTextElement::maximumOuterSizeHint() const
{
    QFontMetrics metrics(mFont);
    QSize result(metrics.boundingRect(0, 0, 0, 0, Qt::AlignCenter, mText).size());
    result.setWidth(QWIDGETSIZE_MAX);
    result.rheight() += mMargins.top()+mMargins.bottom();
    return result;
}

/* inherits documentation from base class */
void QCPTextElement::selectEvent(QMouseEvent *event, bool additive, const QVariant &details, bool *selectionStateChanged)
{
    Q_UNUSED(event)
    Q_UNUSED(details)
    if (mSelectable)
    {
        bool selBefore = mSelected;
        setSelected(additive ? !mSelected : true);
        if (selectionStateChanged)
            *selectionStateChanged = mSelected != selBefore;
    }
}

/* inherits documentation from base class */
void QCPTextElement::deselectEvent(bool *selectionStateChanged)
{
    if (mSelectable)
    {
        bool selBefore = mSelected;
        setSelected(false);
        if (selectionStateChanged)
            *selectionStateChanged = mSelected != selBefore;
    }
}

/*!
  Returns 0.99*selectionTolerance (see \ref PF_GraphicView::setSelectionTolerance) when \a pos is
  within the bounding box of the text element's text. Note that this bounding box is updated in the
  draw call.

  If \a pos is outside the text's bounding box or if \a onlySelectable is true and this text
  element is not selectable (\ref setSelectable), returns -1.

  \seebaseclassmethod
*/
double QCPTextElement::selectTest(const QPointF &pos, bool onlySelectable, QVariant *details) const
{
    Q_UNUSED(details)
    if (onlySelectable && !mSelectable)
        return -1;

    if (mTextBoundingRect.contains(pos.toPoint()))
        return mParentPlot->selectionTolerance()*0.99;
    else
        return -1;
}

/*!
  Accepts the mouse event in order to emit the according click signal in the \ref
  mouseReleaseEvent.

  \seebaseclassmethod
*/
void QCPTextElement::mousePressEvent(QMouseEvent *event, const QVariant &details)
{
    Q_UNUSED(details)
    event->accept();
}

/*!
  Emits the \ref clicked signal if the cursor hasn't moved by more than a few pixels since the \ref
  mousePressEvent.

  \seebaseclassmethod
*/
void QCPTextElement::mouseReleaseEvent(QMouseEvent *event, const QPointF &startPos)
{
    if ((QPointF(event->pos())-startPos).manhattanLength() <= 3)
        emit clicked(event);
}

/*!
  Emits the \ref doubleClicked signal.

  \seebaseclassmethod
*/
void QCPTextElement::mouseDoubleClickEvent(QMouseEvent *event, const QVariant &details)
{
    Q_UNUSED(details)
    emit doubleClicked(event);
}

/*! \internal
  
  Returns the main font to be used. This is mSelectedFont if \ref setSelected is set to
  <tt>true</tt>, else mFont is returned.
*/
QFont QCPTextElement::mainFont() const
{
    return mSelected ? mSelectedFont : mFont;
}

/*! \internal
  
  Returns the main color to be used. This is mSelectedTextColor if \ref setSelected is set to
  <tt>true</tt>, else mTextColor is returned.
*/
QColor QCPTextElement::mainTextColor() const
{
    return mSelected ? mSelectedTextColor : mTextColor;
}
/* end of 'src/layoutelements/layoutelement-textelement.cpp' */


/* including file 'src/layoutelements/layoutelement-colorscale.cpp', size 26246 */
/* commit ce344b3f96a62e5f652585e55f1ae7c7883cd45b 2018-06-25 01:03:39 +0200    */


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPColorScale
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPColorScale
  \brief A color scale for use with color coding data such as QCPColorMap
  
  This layout element can be placed on the plot to correlate a color gradient with data values. It
  is usually used in combination with one or multiple \ref QCPColorMap "QCPColorMaps".

  \image html QCPColorScale.png
  
  The color scale can be either horizontal or vertical, as shown in the image above. The
  orientation and the side where the numbers appear is controlled with \ref setType.
  
  Use \ref QCPColorMap::setColorScale to connect a color map with a color scale. Once they are
  connected, they share their gradient, data range and data scale type (\ref setGradient, \ref
  setDataRange, \ref setDataScaleType). Multiple color maps may be associated with a single color
  scale, to make them all synchronize these properties.
  
  To have finer control over the number display and axis behaviour, you can directly access the
  \ref axis. See the documentation of QCPAxis for details about configuring axes. For example, if
  you want to change the number of automatically generated ticks, call
  \snippet documentation/doc-code-snippets/mainwindow.cpp qcpcolorscale-tickcount
  
  Placing a color scale next to the main axis rect works like with any other layout element:
  \snippet documentation/doc-code-snippets/mainwindow.cpp qcpcolorscale-creation
  In this case we have placed it to the right of the default axis rect, so it wasn't necessary to
  call \ref setType, since \ref QCPAxis::atRight is already the default. The text next to the color
  scale can be set with \ref setLabel.
  
  For optimum appearance (like in the image above), it may be desirable to line up the axis rect and
  the borders of the color scale. Use a \ref QCPMarginGroup to achieve this:
  \snippet documentation/doc-code-snippets/mainwindow.cpp qcpcolorscale-margingroup
  
  Color scales are initialized with a non-zero minimum top and bottom margin (\ref
  setMinimumMargins), because vertical color scales are most common and the minimum top/bottom
  margin makes sure it keeps some distance to the top/bottom widget border. So if you change to a
  horizontal color scale by setting \ref setType to \ref QCPAxis::atBottom or \ref QCPAxis::atTop, you
  might want to also change the minimum margins accordingly, e.g. <tt>setMinimumMargins(QMargins(6, 0, 6, 0))</tt>.
*/

/* start documentation of inline functions */

/*! \fn QCPAxis *QCPColorScale::axis() const
  
  Returns the internal \ref QCPAxis instance of this color scale. You can access it to alter the
  appearance and behaviour of the axis. \ref QCPColorScale duplicates some properties in its
  interface for convenience. Those are \ref setDataRange (\ref QCPAxis::setRange), \ref
  setDataScaleType (\ref QCPAxis::setScaleType), and the method \ref setLabel (\ref
  QCPAxis::setLabel). As they each are connected, it does not matter whether you use the method on
  the QCPColorScale or on its QCPAxis.
  
  If the type of the color scale is changed with \ref setType, the axis returned by this method
  will change, too, to either the left, right, bottom or top axis, depending on which type was set.
*/

/* end documentation of signals */
/* start documentation of signals */

/*! \fn void QCPColorScale::dataRangeChanged(const QCPRange &newRange);
  
  This signal is emitted when the data range changes.
  
  \see setDataRange
*/

/*! \fn void QCPColorScale::dataScaleTypeChanged(QCPAxis::ScaleType scaleType);
  
  This signal is emitted when the data scale type changes.
  
  \see setDataScaleType
*/

/*! \fn void QCPColorScale::gradientChanged(const QCPColorGradient &newGradient);
  
  This signal is emitted when the gradient changes.
  
  \see setGradient
*/

/* end documentation of signals */

/*!
  Constructs a new QCPColorScale.
*/
QCPColorScale::QCPColorScale(PF_GraphicView *parentPlot) :
    QCPLayoutElement(parentPlot),
    mType(QCPAxis::atTop), // set to atTop such that setType(QCPAxis::atRight) below doesn't skip work because it thinks it's already atRight
    mDataScaleType(QCPAxis::stLinear),
    mBarWidth(20),
    mAxisRect(new QCPColorScaleAxisRectPrivate(this))
{
    setMinimumMargins(QMargins(0, 6, 0, 6)); // for default right color scale types, keep some room at bottom and top (important if no margin group is used)
    setType(QCPAxis::atRight);
    setDataRange(QCPRange(0, 6));
}

QCPColorScale::~QCPColorScale()
{
    delete mAxisRect;
}

/* undocumented getter */
QString QCPColorScale::label() const
{
    if (!mColorAxis)
    {
        qDebug() << Q_FUNC_INFO << "internal color axis undefined";
        return QString();
    }

    return mColorAxis.data()->label();
}

/* undocumented getter */
bool QCPColorScale::rangeDrag() const
{
    if (!mAxisRect)
    {
        qDebug() << Q_FUNC_INFO << "internal axis rect was deleted";
        return false;
    }

    return mAxisRect.data()->rangeDrag().testFlag(QCPAxis::orientation(mType)) &&
            mAxisRect.data()->rangeDragAxis(QCPAxis::orientation(mType)) &&
            mAxisRect.data()->rangeDragAxis(QCPAxis::orientation(mType))->orientation() == QCPAxis::orientation(mType);
}

/* undocumented getter */
bool QCPColorScale::rangeZoom() const
{
    if (!mAxisRect)
    {
        qDebug() << Q_FUNC_INFO << "internal axis rect was deleted";
        return false;
    }

    return mAxisRect.data()->rangeZoom().testFlag(QCPAxis::orientation(mType)) &&
            mAxisRect.data()->rangeZoomAxis(QCPAxis::orientation(mType)) &&
            mAxisRect.data()->rangeZoomAxis(QCPAxis::orientation(mType))->orientation() == QCPAxis::orientation(mType);
}

/*!
  Sets at which side of the color scale the axis is placed, and thus also its orientation.
  
  Note that after setting \a type to a different value, the axis returned by \ref axis() will
  be a different one. The new axis will adopt the following properties from the previous axis: The
  range, scale type, label and ticker (the latter will be shared and not copied).
*/
void QCPColorScale::setType(QCPAxis::AxisType type)
{
    if (!mAxisRect)
    {
        qDebug() << Q_FUNC_INFO << "internal axis rect was deleted";
        return;
    }
    if (mType != type)
    {
        mType = type;
        QCPRange rangeTransfer(0, 6);
        QString labelTransfer;
        QSharedPointer<QCPAxisTicker> tickerTransfer;
        // transfer/revert some settings on old axis if it exists:
        bool doTransfer = (bool)mColorAxis;
        if (doTransfer)
        {
            rangeTransfer = mColorAxis.data()->range();
            labelTransfer = mColorAxis.data()->label();
            tickerTransfer = mColorAxis.data()->ticker();
            mColorAxis.data()->setLabel(QString());
            disconnect(mColorAxis.data(), SIGNAL(rangeChanged(QCPRange)), this, SLOT(setDataRange(QCPRange)));
            disconnect(mColorAxis.data(), SIGNAL(scaleTypeChanged(QCPAxis::ScaleType)), this, SLOT(setDataScaleType(QCPAxis::ScaleType)));
        }
        QList<QCPAxis::AxisType> allAxisTypes = QList<QCPAxis::AxisType>() << QCPAxis::atLeft << QCPAxis::atRight << QCPAxis::atBottom << QCPAxis::atTop;
        foreach (QCPAxis::AxisType atype, allAxisTypes)
        {
            mAxisRect.data()->axis(atype)->setTicks(atype == mType);
            mAxisRect.data()->axis(atype)->setTickLabels(atype== mType);
        }
        // set new mColorAxis pointer:
        mColorAxis = mAxisRect.data()->axis(mType);
        // transfer settings to new axis:
        if (doTransfer)
        {
            mColorAxis.data()->setRange(rangeTransfer); // range transfer necessary if axis changes from vertical to horizontal or vice versa (axes with same orientation are synchronized via signals)
            mColorAxis.data()->setLabel(labelTransfer);
            mColorAxis.data()->setTicker(tickerTransfer);
        }
        connect(mColorAxis.data(), SIGNAL(rangeChanged(QCPRange)), this, SLOT(setDataRange(QCPRange)));
        connect(mColorAxis.data(), SIGNAL(scaleTypeChanged(QCPAxis::ScaleType)), this, SLOT(setDataScaleType(QCPAxis::ScaleType)));
        mAxisRect.data()->setRangeDragAxes(QList<QCPAxis*>() << mColorAxis.data());
    }
}

/*!
  Sets the range spanned by the color gradient and that is shown by the axis in the color scale.
  
  It is equivalent to calling QCPColorMap::setDataRange on any of the connected color maps. It is
  also equivalent to directly accessing the \ref axis and setting its range with \ref
  QCPAxis::setRange.
  
  \see setDataScaleType, setGradient, rescaleDataRange
*/
void QCPColorScale::setDataRange(const QCPRange &dataRange)
{
    if (mDataRange.lower != dataRange.lower || mDataRange.upper != dataRange.upper)
    {
        mDataRange = dataRange;
        if (mColorAxis)
            mColorAxis.data()->setRange(mDataRange);
        emit dataRangeChanged(mDataRange);
    }
}

/*!
  Sets the scale type of the color scale, i.e. whether values are associated with colors linearly
  or logarithmically.
  
  It is equivalent to calling QCPColorMap::setDataScaleType on any of the connected color maps. It is
  also equivalent to directly accessing the \ref axis and setting its scale type with \ref
  QCPAxis::setScaleType.
  
  Note that this method controls the coordinate transformation. For logarithmic scales, you will
  likely also want to use a logarithmic tick spacing and labeling, which can be achieved by setting
  the color scale's \ref axis ticker to an instance of \ref QCPAxisTickerLog :
  
  \snippet documentation/doc-code-snippets/mainwindow.cpp qcpaxisticker-log-colorscale
  
  See the documentation of \ref QCPAxisTickerLog about the details of logarithmic axis tick
  creation.
  
  \see setDataRange, setGradient
*/
void QCPColorScale::setDataScaleType(QCPAxis::ScaleType scaleType)
{
    if (mDataScaleType != scaleType)
    {
        mDataScaleType = scaleType;
        if (mColorAxis)
            mColorAxis.data()->setScaleType(mDataScaleType);
        if (mDataScaleType == QCPAxis::stLogarithmic)
            setDataRange(mDataRange.sanitizedForLogScale());
        emit dataScaleTypeChanged(mDataScaleType);
    }
}

/*!
  Sets the color gradient that will be used to represent data values.
  
  It is equivalent to calling QCPColorMap::setGradient on any of the connected color maps.
  
  \see setDataRange, setDataScaleType
*/
void QCPColorScale::setGradient(const QCPColorGradient &gradient)
{
    if (mGradient != gradient)
    {
        mGradient = gradient;
        if (mAxisRect)
            mAxisRect.data()->mGradientImageInvalidated = true;
        emit gradientChanged(mGradient);
    }
}

/*!
  Sets the axis label of the color scale. This is equivalent to calling \ref QCPAxis::setLabel on
  the internal \ref axis.
*/
void QCPColorScale::setLabel(const QString &str)
{
    if (!mColorAxis)
    {
        qDebug() << Q_FUNC_INFO << "internal color axis undefined";
        return;
    }

    mColorAxis.data()->setLabel(str);
}

/*!
  Sets the width (or height, for horizontal color scales) the bar where the gradient is displayed
  will have.
*/
void QCPColorScale::setBarWidth(int width)
{
    mBarWidth = width;
}

/*!
  Sets whether the user can drag the data range (\ref setDataRange).
  
  Note that \ref QCP::iRangeDrag must be in the PF_GraphicView's interactions (\ref
  PF_GraphicView::setInteractions) to allow range dragging.
*/
void QCPColorScale::setRangeDrag(bool enabled)
{
    if (!mAxisRect)
    {
        qDebug() << Q_FUNC_INFO << "internal axis rect was deleted";
        return;
    }

    if (enabled)
        mAxisRect.data()->setRangeDrag(QCPAxis::orientation(mType));
    else
        mAxisRect.data()->setRangeDrag(nullptr);
}

/*!
  Sets whether the user can zoom the data range (\ref setDataRange) by scrolling the mouse wheel.
  
  Note that \ref QCP::iRangeZoom must be in the PF_GraphicView's interactions (\ref
  PF_GraphicView::setInteractions) to allow range dragging.
*/
void QCPColorScale::setRangeZoom(bool enabled)
{
    if (!mAxisRect)
    {
        qDebug() << Q_FUNC_INFO << "internal axis rect was deleted";
        return;
    }

    if (enabled)
        mAxisRect.data()->setRangeZoom(QCPAxis::orientation(mType));
    else
        mAxisRect.data()->setRangeZoom(nullptr);
}

/*!
  Returns a list of all the color maps associated with this color scale.
*/
QList<QCPColorMap*> QCPColorScale::colorMaps() const
{
    QList<QCPColorMap*> result;
//    for (int i=0; i<mParentPlot->plottableCount(); ++i)
//    {
//        if (QCPColorMap *cm = qobject_cast<QCPColorMap*>(mParentPlot->plottable(i)))
//            if (cm->colorScale() == this)
//                result.append(cm);
//    }
    return result;
}

/*!
  Changes the data range such that all color maps associated with this color scale are fully mapped
  to the gradient in the data dimension.
  
  \see setDataRange
*/
void QCPColorScale::rescaleDataRange(bool onlyVisibleMaps)
{
    QList<QCPColorMap*> maps = colorMaps();
    QCPRange newRange;
    bool haveRange = false;
    QCP::SignDomain sign = QCP::sdBoth;
    if (mDataScaleType == QCPAxis::stLogarithmic)
        sign = (mDataRange.upper < 0 ? QCP::sdNegative : QCP::sdPositive);
    for (int i=0; i<maps.size(); ++i)
    {
        if (!maps.at(i)->realVisibility() && onlyVisibleMaps)
            continue;
        QCPRange mapRange;
        if (maps.at(i)->colorScale() == this)
        {
            bool currentFoundRange = true;
            mapRange = maps.at(i)->data()->dataBounds();
            if (sign == QCP::sdPositive)
            {
                if (mapRange.lower <= 0 && mapRange.upper > 0)
                    mapRange.lower = mapRange.upper*1e-3;
                else if (mapRange.lower <= 0 && mapRange.upper <= 0)
                    currentFoundRange = false;
            } else if (sign == QCP::sdNegative)
            {
                if (mapRange.upper >= 0 && mapRange.lower < 0)
                    mapRange.upper = mapRange.lower*1e-3;
                else if (mapRange.upper >= 0 && mapRange.lower >= 0)
                    currentFoundRange = false;
            }
            if (currentFoundRange)
            {
                if (!haveRange)
                    newRange = mapRange;
                else
                    newRange.expand(mapRange);
                haveRange = true;
            }
        }
    }
    if (haveRange)
    {
        if (!QCPRange::validRange(newRange)) // likely due to range being zero (plottable has only constant data in this dimension), shift current range to at least center the data
        {
            double center = (newRange.lower+newRange.upper)*0.5; // upper and lower should be equal anyway, but just to make sure, incase validRange returned false for other reason
            if (mDataScaleType == QCPAxis::stLinear)
            {
                newRange.lower = center-mDataRange.size()/2.0;
                newRange.upper = center+mDataRange.size()/2.0;
            } else // mScaleType == stLogarithmic
            {
                newRange.lower = center/qSqrt(mDataRange.upper/mDataRange.lower);
                newRange.upper = center*qSqrt(mDataRange.upper/mDataRange.lower);
            }
        }
        setDataRange(newRange);
    }
}

/* inherits documentation from base class */
void QCPColorScale::update(UpdatePhase phase)
{
    QCPLayoutElement::update(phase);
    if (!mAxisRect)
    {
        qDebug() << Q_FUNC_INFO << "internal axis rect was deleted";
        return;
    }

    mAxisRect.data()->update(phase);

    switch (phase)
    {
    case upMargins:
    {
        if (mType == QCPAxis::atBottom || mType == QCPAxis::atTop)
        {
            setMaximumSize(QWIDGETSIZE_MAX, mBarWidth+mAxisRect.data()->margins().top()+mAxisRect.data()->margins().bottom());
            setMinimumSize(0,               mBarWidth+mAxisRect.data()->margins().top()+mAxisRect.data()->margins().bottom());
        } else
        {
            setMaximumSize(mBarWidth+mAxisRect.data()->margins().left()+mAxisRect.data()->margins().right(), QWIDGETSIZE_MAX);
            setMinimumSize(mBarWidth+mAxisRect.data()->margins().left()+mAxisRect.data()->margins().right(), 0);
        }
        break;
    }
    case upLayout:
    {
        mAxisRect.data()->setOuterRect(rect());
        break;
    }
    default: break;
    }
}

/* inherits documentation from base class */
void QCPColorScale::applyDefaultAntialiasingHint(QCPPainter *painter) const
{
    painter->setAntialiasing(false);
}

/* inherits documentation from base class */
void QCPColorScale::mousePressEvent(QMouseEvent *event, const QVariant &details)
{
    if (!mAxisRect)
    {
        qDebug() << Q_FUNC_INFO << "internal axis rect was deleted";
        return;
    }
    mAxisRect.data()->mousePressEvent(event, details);
}

/* inherits documentation from base class */
void QCPColorScale::mouseMoveEvent(QMouseEvent *event, const QPointF &startPos)
{
    if (!mAxisRect)
    {
        qDebug() << Q_FUNC_INFO << "internal axis rect was deleted";
        return;
    }
    mAxisRect.data()->mouseMoveEvent(event, startPos);
}

/* inherits documentation from base class */
void QCPColorScale::mouseReleaseEvent(QMouseEvent *event, const QPointF &startPos)
{
    if (!mAxisRect)
    {
        qDebug() << Q_FUNC_INFO << "internal axis rect was deleted";
        return;
    }
    mAxisRect.data()->mouseReleaseEvent(event, startPos);
}

/* inherits documentation from base class */
void QCPColorScale::wheelEvent(QWheelEvent *event)
{
    if (!mAxisRect)
    {
        qDebug() << Q_FUNC_INFO << "internal axis rect was deleted";
        return;
    }
    mAxisRect.data()->wheelEvent(event);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPColorScaleAxisRectPrivate
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPColorScaleAxisRectPrivate

  \internal
  \brief An axis rect subclass for use in a QCPColorScale
  
  This is a private class and not part of the public PF_GraphicView interface.
  
  It provides the axis rect functionality for the QCPColorScale class.
*/


/*!
  Creates a new instance, as a child of \a parentColorScale.
*/
QCPColorScaleAxisRectPrivate::QCPColorScaleAxisRectPrivate(QCPColorScale *parentColorScale) :
    QCPAxisRect(parentColorScale->parentPlot(), true),
    mParentColorScale(parentColorScale),
    mGradientImageInvalidated(true)
{
    setParentLayerable(parentColorScale);
    setMinimumMargins(QMargins(0, 0, 0, 0));
    QList<QCPAxis::AxisType> allAxisTypes = QList<QCPAxis::AxisType>() << QCPAxis::atBottom << QCPAxis::atTop << QCPAxis::atLeft << QCPAxis::atRight;
    foreach (QCPAxis::AxisType type, allAxisTypes)
    {
        axis(type)->setVisible(true);
        axis(type)->grid()->setVisible(false);
        axis(type)->setPadding(0);
        connect(axis(type), SIGNAL(selectionChanged(QCPAxis::SelectableParts)), this, SLOT(axisSelectionChanged(QCPAxis::SelectableParts)));
        connect(axis(type), SIGNAL(selectableChanged(QCPAxis::SelectableParts)), this, SLOT(axisSelectableChanged(QCPAxis::SelectableParts)));
    }

    connect(axis(QCPAxis::atLeft), SIGNAL(rangeChanged(QCPRange)), axis(QCPAxis::atRight), SLOT(setRange(QCPRange)));
    connect(axis(QCPAxis::atRight), SIGNAL(rangeChanged(QCPRange)), axis(QCPAxis::atLeft), SLOT(setRange(QCPRange)));
    connect(axis(QCPAxis::atBottom), SIGNAL(rangeChanged(QCPRange)), axis(QCPAxis::atTop), SLOT(setRange(QCPRange)));
    connect(axis(QCPAxis::atTop), SIGNAL(rangeChanged(QCPRange)), axis(QCPAxis::atBottom), SLOT(setRange(QCPRange)));
    connect(axis(QCPAxis::atLeft), SIGNAL(scaleTypeChanged(QCPAxis::ScaleType)), axis(QCPAxis::atRight), SLOT(setScaleType(QCPAxis::ScaleType)));
    connect(axis(QCPAxis::atRight), SIGNAL(scaleTypeChanged(QCPAxis::ScaleType)), axis(QCPAxis::atLeft), SLOT(setScaleType(QCPAxis::ScaleType)));
    connect(axis(QCPAxis::atBottom), SIGNAL(scaleTypeChanged(QCPAxis::ScaleType)), axis(QCPAxis::atTop), SLOT(setScaleType(QCPAxis::ScaleType)));
    connect(axis(QCPAxis::atTop), SIGNAL(scaleTypeChanged(QCPAxis::ScaleType)), axis(QCPAxis::atBottom), SLOT(setScaleType(QCPAxis::ScaleType)));

    // make layer transfers of color scale transfer to axis rect and axes
    // the axes must be set after axis rect, such that they appear above color gradient drawn by axis rect:
    connect(parentColorScale, SIGNAL(layerChanged(QCPLayer*)), this, SLOT(setLayer(QCPLayer*)));
    foreach (QCPAxis::AxisType type, allAxisTypes)
        connect(parentColorScale, SIGNAL(layerChanged(QCPLayer*)), axis(type), SLOT(setLayer(QCPLayer*)));
}

/*! \internal
  
  Updates the color gradient image if necessary, by calling \ref updateGradientImage, then draws
  it. Then the axes are drawn by calling the \ref QCPAxisRect::draw base class implementation.
  
  \seebaseclassmethod
*/
void QCPColorScaleAxisRectPrivate::draw(QCPPainter *painter)
{
    if (mGradientImageInvalidated)
        updateGradientImage();

    bool mirrorHorz = false;
    bool mirrorVert = false;
    if (mParentColorScale->mColorAxis)
    {
        mirrorHorz = mParentColorScale->mColorAxis.data()->rangeReversed() && (mParentColorScale->type() == QCPAxis::atBottom || mParentColorScale->type() == QCPAxis::atTop);
        mirrorVert = mParentColorScale->mColorAxis.data()->rangeReversed() && (mParentColorScale->type() == QCPAxis::atLeft || mParentColorScale->type() == QCPAxis::atRight);
    }

    painter->drawImage(rect().adjusted(0, -1, 0, -1), mGradientImage.mirrored(mirrorHorz, mirrorVert));
    QCPAxisRect::draw(painter);
}

/*! \internal

  Uses the current gradient of the parent \ref QCPColorScale (specified in the constructor) to
  generate a gradient image. This gradient image will be used in the \ref draw method.
*/
void QCPColorScaleAxisRectPrivate::updateGradientImage()
{
    if (rect().isEmpty())
        return;

    const QImage::Format format = QImage::Format_ARGB32_Premultiplied;
    int n = mParentColorScale->mGradient.levelCount();
    int w, h;
    QVector<double> data(n);
    for (int i=0; i<n; ++i)
        data[i] = i;
    if (mParentColorScale->mType == QCPAxis::atBottom || mParentColorScale->mType == QCPAxis::atTop)
    {
        w = n;
        h = rect().height();
        mGradientImage = QImage(w, h, format);
        QVector<QRgb*> pixels;
        for (int y=0; y<h; ++y)
            pixels.append(reinterpret_cast<QRgb*>(mGradientImage.scanLine(y)));
        mParentColorScale->mGradient.colorize(data.constData(), QCPRange(0, n-1), pixels.first(), n);
        for (int y=1; y<h; ++y)
            memcpy(pixels.at(y), pixels.first(), n*sizeof(QRgb));
    } else
    {
        w = rect().width();
        h = n;
        mGradientImage = QImage(w, h, format);
        for (int y=0; y<h; ++y)
        {
            QRgb *pixels = reinterpret_cast<QRgb*>(mGradientImage.scanLine(y));
            const QRgb lineColor = mParentColorScale->mGradient.color(data[h-1-y], QCPRange(0, n-1));
            for (int x=0; x<w; ++x)
                pixels[x] = lineColor;
        }
    }
    mGradientImageInvalidated = false;
}

/*! \internal

  This slot is connected to the selectionChanged signals of the four axes in the constructor. It
  synchronizes the selection state of the axes.
*/
void QCPColorScaleAxisRectPrivate::axisSelectionChanged(QCPAxis::SelectableParts selectedParts)
{
    // axis bases of four axes shall always (de-)selected synchronously:
    QList<QCPAxis::AxisType> allAxisTypes = QList<QCPAxis::AxisType>() << QCPAxis::atBottom << QCPAxis::atTop << QCPAxis::atLeft << QCPAxis::atRight;
    foreach (QCPAxis::AxisType type, allAxisTypes)
    {
        if (QCPAxis *senderAxis = qobject_cast<QCPAxis*>(sender()))
            if (senderAxis->axisType() == type)
                continue;

        if (axis(type)->selectableParts().testFlag(QCPAxis::spAxis))
        {
            if (selectedParts.testFlag(QCPAxis::spAxis))
                axis(type)->setSelectedParts(axis(type)->selectedParts() | QCPAxis::spAxis);
            else
                axis(type)->setSelectedParts(axis(type)->selectedParts() & ~QCPAxis::spAxis);
        }
    }
}

/*! \internal

  This slot is connected to the selectableChanged signals of the four axes in the constructor. It
  synchronizes the selectability of the axes.
*/
void QCPColorScaleAxisRectPrivate::axisSelectableChanged(QCPAxis::SelectableParts selectableParts)
{
    // synchronize axis base selectability:
    QList<QCPAxis::AxisType> allAxisTypes = QList<QCPAxis::AxisType>() << QCPAxis::atBottom << QCPAxis::atTop << QCPAxis::atLeft << QCPAxis::atRight;
    foreach (QCPAxis::AxisType type, allAxisTypes)
    {
        if (QCPAxis *senderAxis = qobject_cast<QCPAxis*>(sender()))
            if (senderAxis->axisType() == type)
                continue;

        if (axis(type)->selectableParts().testFlag(QCPAxis::spAxis))
        {
            if (selectableParts.testFlag(QCPAxis::spAxis))
                axis(type)->setSelectableParts(axis(type)->selectableParts() | QCPAxis::spAxis);
            else
                axis(type)->setSelectableParts(axis(type)->selectableParts() & ~QCPAxis::spAxis);
        }
    }
}
/* end of 'src/layoutelements/layoutelement-colorscale.cpp' */




/* including file 'src/plottables/plottable-colormap.cpp', size 47881        */
/* commit ce344b3f96a62e5f652585e55f1ae7c7883cd45b 2018-06-25 01:03:39 +0200 */

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPColorMapData
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPColorMapData
  \brief Holds the two-dimensional data of a QCPColorMap plottable.
  
  This class is a data storage for \ref QCPColorMap. It holds a two-dimensional array, which \ref
  QCPColorMap then displays as a 2D image in the plot, where the array values are represented by a
  color, depending on the value.
  
  The size of the array can be controlled via \ref setSize (or \ref setKeySize, \ref setValueSize).
  Which plot coordinates these cells correspond to can be configured with \ref setRange (or \ref
  setKeyRange, \ref setValueRange).
  
  The data cells can be accessed in two ways: They can be directly addressed by an integer index
  with \ref setCell. This is the fastest method. Alternatively, they can be addressed by their plot
  coordinate with \ref setData. plot coordinate to cell index transformations and vice versa are
  provided by the functions \ref coordToCell and \ref cellToCoord.
  
  A \ref QCPColorMapData also holds an on-demand two-dimensional array of alpha values which (if
  allocated) has the same size as the data map. It can be accessed via \ref setAlpha, \ref
  fillAlpha and \ref clearAlpha. The memory for the alpha map is only allocated if needed, i.e. on
  the first call of \ref setAlpha. \ref clearAlpha restores full opacity and frees the alpha map.
  
  This class also buffers the minimum and maximum values that are in the data set, to provide
  QCPColorMap::rescaleDataRange with the necessary information quickly. Setting a cell to a value
  that is greater than the current maximum increases this maximum to the new value. However,
  setting the cell that currently holds the maximum value to a smaller value doesn't decrease the
  maximum again, because finding the true new maximum would require going through the entire data
  array, which might be time consuming. The same holds for the data minimum. This functionality is
  given by \ref recalculateDataBounds, such that you can decide when it is sensible to find the
  true current minimum and maximum. The method QCPColorMap::rescaleDataRange offers a convenience
  parameter \a recalculateDataBounds which may be set to true to automatically call \ref
  recalculateDataBounds internally.
*/

/* start of documentation of inline functions */

/*! \fn bool QCPColorMapData::isEmpty() const
  
  Returns whether this instance carries no data. This is equivalent to having a size where at least
  one of the dimensions is 0 (see \ref setSize).
*/

/* end of documentation of inline functions */

/*!
  Constructs a new QCPColorMapData instance. The instance has \a keySize cells in the key direction
  and \a valueSize cells in the value direction. These cells will be displayed by the \ref QCPColorMap
  at the coordinates \a keyRange and \a valueRange.
  
  \see setSize, setKeySize, setValueSize, setRange, setKeyRange, setValueRange
*/
QCPColorMapData::QCPColorMapData(int keySize, int valueSize, const QCPRange &keyRange, const QCPRange &valueRange) :
    mKeySize(0),
    mValueSize(0),
    mKeyRange(keyRange),
    mValueRange(valueRange),
    mIsEmpty(true),
    mData(nullptr),
    mAlpha(nullptr),
    mDataModified(true)
{
    setSize(keySize, valueSize);
    fill(0);
}

QCPColorMapData::~QCPColorMapData()
{
    if (mData)
        delete[] mData;
    if (mAlpha)
        delete[] mAlpha;
}

/*!
  Constructs a new QCPColorMapData instance copying the data and range of \a other.
*/
QCPColorMapData::QCPColorMapData(const QCPColorMapData &other) :
    mKeySize(0),
    mValueSize(0),
    mIsEmpty(true),
    mData(nullptr),
    mAlpha(nullptr),
    mDataModified(true)
{
    *this = other;
}

/*!
  Overwrites this color map data instance with the data stored in \a other. The alpha map state is
  transferred, too.
*/
QCPColorMapData &QCPColorMapData::operator=(const QCPColorMapData &other)
{
    if (&other != this)
    {
        const int keySize = other.keySize();
        const int valueSize = other.valueSize();
        if (!other.mAlpha && mAlpha)
            clearAlpha();
        setSize(keySize, valueSize);
        if (other.mAlpha && !mAlpha)
            createAlpha(false);
        setRange(other.keyRange(), other.valueRange());
        if (!isEmpty())
        {
            memcpy(mData, other.mData, sizeof(mData[0])*keySize*valueSize);
            if (mAlpha)
                memcpy(mAlpha, other.mAlpha, sizeof(mAlpha[0])*keySize*valueSize);
        }
        mDataBounds = other.mDataBounds;
        mDataModified = true;
    }
    return *this;
}

/* undocumented getter */
double QCPColorMapData::data(double key, double value)
{
    int keyCell = (key-mKeyRange.lower)/(mKeyRange.upper-mKeyRange.lower)*(mKeySize-1)+0.5;
    int valueCell = (value-mValueRange.lower)/(mValueRange.upper-mValueRange.lower)*(mValueSize-1)+0.5;
    if (keyCell >= 0 && keyCell < mKeySize && valueCell >= 0 && valueCell < mValueSize)
        return mData[valueCell*mKeySize + keyCell];
    else
        return 0;
}

/* undocumented getter */
double QCPColorMapData::cell(int keyIndex, int valueIndex)
{
    if (keyIndex >= 0 && keyIndex < mKeySize && valueIndex >= 0 && valueIndex < mValueSize)
        return mData[valueIndex*mKeySize + keyIndex];
    else
        return 0;
}

/*!
  Returns the alpha map value of the cell with the indices \a keyIndex and \a valueIndex.

  If this color map data doesn't have an alpha map (because \ref setAlpha was never called after
  creation or after a call to \ref clearAlpha), returns 255, which corresponds to full opacity.

  \see setAlpha
*/
unsigned char QCPColorMapData::alpha(int keyIndex, int valueIndex)
{
    if (mAlpha && keyIndex >= 0 && keyIndex < mKeySize && valueIndex >= 0 && valueIndex < mValueSize)
        return mAlpha[valueIndex*mKeySize + keyIndex];
    else
        return 255;
}

/*!
  Resizes the data array to have \a keySize cells in the key dimension and \a valueSize cells in
  the value dimension.

  The current data is discarded and the map cells are set to 0, unless the map had already the
  requested size.
  
  Setting at least one of \a keySize or \a valueSize to zero frees the internal data array and \ref
  isEmpty returns true.

  \see setRange, setKeySize, setValueSize
*/
void QCPColorMapData::setSize(int keySize, int valueSize)
{
    if (keySize != mKeySize || valueSize != mValueSize)
    {
        mKeySize = keySize;
        mValueSize = valueSize;
        if (mData)
            delete[] mData;
        mIsEmpty = mKeySize == 0 || mValueSize == 0;
        if (!mIsEmpty)
        {
#ifdef __EXCEPTIONS
            try { // 2D arrays get memory intensive fast. So if the allocation fails, at least output debug message
#endif
                mData = new double[mKeySize*mValueSize];
#ifdef __EXCEPTIONS
            } catch (...) { mData = nullptr; }
#endif
            if (mData)
                fill(0);
            else
                qDebug() << Q_FUNC_INFO << "out of memory for data dimensions "<< mKeySize << "*" << mValueSize;
        } else
            mData = nullptr;

        if (mAlpha) // if we had an alpha map, recreate it with new size
            createAlpha();

        mDataModified = true;
    }
}

/*!
  Resizes the data array to have \a keySize cells in the key dimension.

  The current data is discarded and the map cells are set to 0, unless the map had already the
  requested size.
  
  Setting \a keySize to zero frees the internal data array and \ref isEmpty returns true.

  \see setKeyRange, setSize, setValueSize
*/
void QCPColorMapData::setKeySize(int keySize)
{
    setSize(keySize, mValueSize);
}

/*!
  Resizes the data array to have \a valueSize cells in the value dimension.

  The current data is discarded and the map cells are set to 0, unless the map had already the
  requested size.
  
  Setting \a valueSize to zero frees the internal data array and \ref isEmpty returns true.

  \see setValueRange, setSize, setKeySize
*/
void QCPColorMapData::setValueSize(int valueSize)
{
    setSize(mKeySize, valueSize);
}

/*!
  Sets the coordinate ranges the data shall be distributed over. This defines the rectangular area
  covered by the color map in plot coordinates.
  
  The outer cells will be centered on the range boundaries given to this function. For example, if
  the key size (\ref setKeySize) is 3 and \a keyRange is set to <tt>QCPRange(2, 3)</tt> there will
  be cells centered on the key coordinates 2, 2.5 and 3.

  \see setSize
*/
void QCPColorMapData::setRange(const QCPRange &keyRange, const QCPRange &valueRange)
{
    setKeyRange(keyRange);
    setValueRange(valueRange);
}

/*!
  Sets the coordinate range the data shall be distributed over in the key dimension. Together with
  the value range, This defines the rectangular area covered by the color map in plot coordinates.
  
  The outer cells will be centered on the range boundaries given to this function. For example, if
  the key size (\ref setKeySize) is 3 and \a keyRange is set to <tt>QCPRange(2, 3)</tt> there will
  be cells centered on the key coordinates 2, 2.5 and 3.

  \see setRange, setValueRange, setSize
*/
void QCPColorMapData::setKeyRange(const QCPRange &keyRange)
{
    mKeyRange = keyRange;
}

/*!
  Sets the coordinate range the data shall be distributed over in the value dimension. Together with
  the key range, This defines the rectangular area covered by the color map in plot coordinates.
  
  The outer cells will be centered on the range boundaries given to this function. For example, if
  the value size (\ref setValueSize) is 3 and \a valueRange is set to <tt>QCPRange(2, 3)</tt> there
  will be cells centered on the value coordinates 2, 2.5 and 3.

  \see setRange, setKeyRange, setSize
*/
void QCPColorMapData::setValueRange(const QCPRange &valueRange)
{
    mValueRange = valueRange;
}

/*!
  Sets the data of the cell, which lies at the plot coordinates given by \a key and \a value, to \a
  z.
  
  \note The QCPColorMap always displays the data at equal key/value intervals, even if the key or
  value axis is set to a logarithmic scaling. If you want to use QCPColorMap with logarithmic axes,
  you shouldn't use the \ref QCPColorMapData::setData method as it uses a linear transformation to
  determine the cell index. Rather directly access the cell index with \ref
  QCPColorMapData::setCell.

  \see setCell, setRange
*/
void QCPColorMapData::setData(double key, double value, double z)
{
    int keyCell = (key-mKeyRange.lower)/(mKeyRange.upper-mKeyRange.lower)*(mKeySize-1)+0.5;
    int valueCell = (value-mValueRange.lower)/(mValueRange.upper-mValueRange.lower)*(mValueSize-1)+0.5;
    if (keyCell >= 0 && keyCell < mKeySize && valueCell >= 0 && valueCell < mValueSize)
    {
        mData[valueCell*mKeySize + keyCell] = z;
        if (z < mDataBounds.lower)
            mDataBounds.lower = z;
        if (z > mDataBounds.upper)
            mDataBounds.upper = z;
        mDataModified = true;
    }
}

/*!
  Sets the data of the cell with indices \a keyIndex and \a valueIndex to \a z. The indices
  enumerate the cells starting from zero, up to the map's size-1 in the respective dimension (see
  \ref setSize).
  
  In the standard plot configuration (horizontal key axis and vertical value axis, both not
  range-reversed), the cell with indices (0, 0) is in the bottom left corner and the cell with
  indices (keySize-1, valueSize-1) is in the top right corner of the color map.
  
  \see setData, setSize
*/
void QCPColorMapData::setCell(int keyIndex, int valueIndex, double z)
{
    if (keyIndex >= 0 && keyIndex < mKeySize && valueIndex >= 0 && valueIndex < mValueSize)
    {
        mData[valueIndex*mKeySize + keyIndex] = z;
        if (z < mDataBounds.lower)
            mDataBounds.lower = z;
        if (z > mDataBounds.upper)
            mDataBounds.upper = z;
        mDataModified = true;
    } else
        qDebug() << Q_FUNC_INFO << "index out of bounds:" << keyIndex << valueIndex;
}

/*!
  Sets the alpha of the color map cell given by \a keyIndex and \a valueIndex to \a alpha. A value
  of 0 for \a alpha results in a fully transparent cell, and a value of 255 results in a fully
  opaque cell.

  If an alpha map doesn't exist yet for this color map data, it will be created here. If you wish
  to restore full opacity and free any allocated memory of the alpha map, call \ref clearAlpha.

  Note that the cell-wise alpha which can be configured here is independent of any alpha configured
  in the color map's gradient (\ref QCPColorGradient). If a cell is affected both by the cell-wise
  and gradient alpha, the alpha values will be blended accordingly during rendering of the color
  map.

  \see fillAlpha, clearAlpha
*/
void QCPColorMapData::setAlpha(int keyIndex, int valueIndex, unsigned char alpha)
{
    if (keyIndex >= 0 && keyIndex < mKeySize && valueIndex >= 0 && valueIndex < mValueSize)
    {
        if (mAlpha || createAlpha())
        {
            mAlpha[valueIndex*mKeySize + keyIndex] = alpha;
            mDataModified = true;
        }
    } else
        qDebug() << Q_FUNC_INFO << "index out of bounds:" << keyIndex << valueIndex;
}

/*!
  Goes through the data and updates the buffered minimum and maximum data values.
  
  Calling this method is only advised if you are about to call \ref QCPColorMap::rescaleDataRange
  and can not guarantee that the cells holding the maximum or minimum data haven't been overwritten
  with a smaller or larger value respectively, since the buffered maximum/minimum values have been
  updated the last time. Why this is the case is explained in the class description (\ref
  QCPColorMapData).
  
  Note that the method \ref QCPColorMap::rescaleDataRange provides a parameter \a
  recalculateDataBounds for convenience. Setting this to true will call this method for you, before
  doing the rescale.
*/
void QCPColorMapData::recalculateDataBounds()
{
    if (mKeySize > 0 && mValueSize > 0)
    {
        double minHeight = mData[0];
        double maxHeight = mData[0];
        const int dataCount = mValueSize*mKeySize;
        for (int i=0; i<dataCount; ++i)
        {
            if (mData[i] > maxHeight)
                maxHeight = mData[i];
            if (mData[i] < minHeight)
                minHeight = mData[i];
        }
        mDataBounds.lower = minHeight;
        mDataBounds.upper = maxHeight;
    }
}

/*!
  Frees the internal data memory.
  
  This is equivalent to calling \ref setSize "setSize(0, 0)".
*/
void QCPColorMapData::clear()
{
    setSize(0, 0);
}

/*!
  Frees the internal alpha map. The color map will have full opacity again.
*/
void QCPColorMapData::clearAlpha()
{
    if (mAlpha)
    {
        delete[] mAlpha;
        mAlpha = nullptr;
        mDataModified = true;
    }
}

/*!
  Sets all cells to the value \a z.
*/
void QCPColorMapData::fill(double z)
{
    const int dataCount = mValueSize*mKeySize;
    for (int i=0; i<dataCount; ++i)
        mData[i] = z;
    mDataBounds = QCPRange(z, z);
    mDataModified = true;
}

/*!
  Sets the opacity of all color map cells to \a alpha. A value of 0 for \a alpha results in a fully
  transparent color map, and a value of 255 results in a fully opaque color map.

  If you wish to restore opacity to 100% and free any used memory for the alpha map, rather use
  \ref clearAlpha.

  \see setAlpha
*/
void QCPColorMapData::fillAlpha(unsigned char alpha)
{
    if (mAlpha || createAlpha(false))
    {
        const int dataCount = mValueSize*mKeySize;
        for (int i=0; i<dataCount; ++i)
            mAlpha[i] = alpha;
        mDataModified = true;
    }
}

/*!
  Transforms plot coordinates given by \a key and \a value to cell indices of this QCPColorMapData
  instance. The resulting cell indices are returned via the output parameters \a keyIndex and \a
  valueIndex.
  
  The retrieved key/value cell indices can then be used for example with \ref setCell.
  
  If you are only interested in a key or value index, you may pass 0 as \a valueIndex or \a
  keyIndex.
  
  \note The QCPColorMap always displays the data at equal key/value intervals, even if the key or
  value axis is set to a logarithmic scaling. If you want to use QCPColorMap with logarithmic axes,
  you shouldn't use the \ref QCPColorMapData::coordToCell method as it uses a linear transformation to
  determine the cell index.
  
  \see cellToCoord, QCPAxis::coordToPixel
*/
void QCPColorMapData::coordToCell(double key, double value, int *keyIndex, int *valueIndex) const
{
    if (keyIndex)
        *keyIndex = (key-mKeyRange.lower)/(mKeyRange.upper-mKeyRange.lower)*(mKeySize-1)+0.5;
    if (valueIndex)
        *valueIndex = (value-mValueRange.lower)/(mValueRange.upper-mValueRange.lower)*(mValueSize-1)+0.5;
}

/*!
  Transforms cell indices given by \a keyIndex and \a valueIndex to cell indices of this QCPColorMapData
  instance. The resulting coordinates are returned via the output parameters \a key and \a
  value.
  
  If you are only interested in a key or value coordinate, you may pass 0 as \a key or \a
  value.
  
  \note The QCPColorMap always displays the data at equal key/value intervals, even if the key or
  value axis is set to a logarithmic scaling. If you want to use QCPColorMap with logarithmic axes,
  you shouldn't use the \ref QCPColorMapData::cellToCoord method as it uses a linear transformation to
  determine the cell index.
  
  \see coordToCell, QCPAxis::pixelToCoord
*/
void QCPColorMapData::cellToCoord(int keyIndex, int valueIndex, double *key, double *value) const
{
    if (key)
        *key = keyIndex/(double)(mKeySize-1)*(mKeyRange.upper-mKeyRange.lower)+mKeyRange.lower;
    if (value)
        *value = valueIndex/(double)(mValueSize-1)*(mValueRange.upper-mValueRange.lower)+mValueRange.lower;
}

/*! \internal

  Allocates the internal alpha map with the current data map key/value size and, if \a
  initializeOpaque is true, initializes all values to 255. If \a initializeOpaque is false, the
  values are not initialized at all. In this case, the alpha map should be initialized manually,
  e.g. with \ref fillAlpha.

  If an alpha map exists already, it is deleted first. If this color map is empty (has either key
  or value size zero, see \ref isEmpty), the alpha map is cleared.

  The return value indicates the existence of the alpha map after the call. So this method returns
  true if the data map isn't empty and an alpha map was successfully allocated.
*/
bool QCPColorMapData::createAlpha(bool initializeOpaque)
{
    clearAlpha();
    if (isEmpty())
        return false;

#ifdef __EXCEPTIONS
    try { // 2D arrays get memory intensive fast. So if the allocation fails, at least output debug message
#endif
        mAlpha = new unsigned char[mKeySize*mValueSize];
#ifdef __EXCEPTIONS
    } catch (...) { mAlpha = nullptr; }
#endif
    if (mAlpha)
    {
        if (initializeOpaque)
            fillAlpha(255);
        return true;
    } else
    {
        qDebug() << Q_FUNC_INFO << "out of memory for data dimensions "<< mKeySize << "*" << mValueSize;
        return false;
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPColorMap
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPColorMap
  \brief A plottable representing a two-dimensional color map in a plot.

  \image html QCPColorMap.png
  
  The data is stored in the class \ref QCPColorMapData, which can be accessed via the data()
  method.
  
  A color map has three dimensions to represent a data point: The \a key dimension, the \a value
  dimension and the \a data dimension. As with other plottables such as graphs, \a key and \a value
  correspond to two orthogonal axes on the PF_GraphicView surface that you specify in the QCPColorMap
  constructor. The \a data dimension however is encoded as the color of the point at (\a key, \a
  value).

  Set the number of points (or \a cells) in the key/value dimension via \ref
  QCPColorMapData::setSize. The plot coordinate range over which these points will be displayed is
  specified via \ref QCPColorMapData::setRange. The first cell will be centered on the lower range
  boundary and the last cell will be centered on the upper range boundary. The data can be set by
  either accessing the cells directly with QCPColorMapData::setCell or by addressing the cells via
  their plot coordinates with \ref QCPColorMapData::setData. If possible, you should prefer
  setCell, since it doesn't need to do any coordinate transformation and thus performs a bit
  better.
  
  The cell with index (0, 0) is at the bottom left, if the color map uses normal (i.e. not reversed)
  key and value axes.
  
  To show the user which colors correspond to which \a data values, a \ref QCPColorScale is
  typically placed to the right of the axis rect. See the documentation there for details on how to
  add and use a color scale.
  
  \section qcpcolormap-appearance Changing the appearance
  
  The central part of the appearance is the color gradient, which can be specified via \ref
  setGradient. See the documentation of \ref QCPColorGradient for details on configuring a color
  gradient.
  
  The \a data range that is mapped to the colors of the gradient can be specified with \ref
  setDataRange. To make the data range encompass the whole data set minimum to maximum, call \ref
  rescaleDataRange.
  
  \section qcpcolormap-transparency Transparency
  
  Transparency in color maps can be achieved by two mechanisms. On one hand, you can specify alpha
  values for color stops of the \ref QCPColorGradient, via the regular QColor interface. This will
  cause the color map data which gets mapped to colors around those color stops to appear with the
  accordingly interpolated transparency.
  
  On the other hand you can also directly apply an alpha value to each cell independent of its
  data, by using the alpha map feature of \ref QCPColorMapData. The relevant methods are \ref
  QCPColorMapData::setAlpha, QCPColorMapData::fillAlpha and \ref QCPColorMapData::clearAlpha().
  
  The two transparencies will be joined together in the plot and otherwise not interfere with each
  other. They are mixed in a multiplicative matter, so an alpha of e.g. 50% (128/255) in both modes
  simultaneously, will result in a total transparency of 25% (64/255).
  
  \section qcpcolormap-usage Usage
  
  Like all data representing objects in PF_GraphicView, the QCPColorMap is a plottable
  (QCPAbstractPlottable). So the plottable-interface of PF_GraphicView applies
  (PF_GraphicView::plottable, PF_GraphicView::removePlottable, etc.)
  
  Usually, you first create an instance:
  \snippet documentation/doc-code-snippets/mainwindow.cpp qcpcolormap-creation-1
  which registers it with the PF_GraphicView instance of the passed axes. Note that this PF_GraphicView instance takes
  ownership of the plottable, so do not delete it manually but use PF_GraphicView::removePlottable() instead.
  The newly created plottable can be modified, e.g.:
  \snippet documentation/doc-code-snippets/mainwindow.cpp qcpcolormap-creation-2
  
  \note The QCPColorMap always displays the data at equal key/value intervals, even if the key or
  value axis is set to a logarithmic scaling. If you want to use QCPColorMap with logarithmic axes,
  you shouldn't use the \ref QCPColorMapData::setData method as it uses a linear transformation to
  determine the cell index. Rather directly access the cell index with \ref
  QCPColorMapData::setCell.
*/

/* start documentation of inline functions */

/*! \fn QCPColorMapData *QCPColorMap::data() const
  
  Returns a pointer to the internal data storage of type \ref QCPColorMapData. Access this to
  modify data points (cells) and the color map key/value range.
  
  \see setData
*/

/* end documentation of inline functions */

/* start documentation of signals */

/*! \fn void QCPColorMap::dataRangeChanged(const QCPRange &newRange);
  
  This signal is emitted when the data range changes.
  
  \see setDataRange
*/

/*! \fn void QCPColorMap::dataScaleTypeChanged(QCPAxis::ScaleType scaleType);
  
  This signal is emitted when the data scale type changes.
  
  \see setDataScaleType
*/

/*! \fn void QCPColorMap::gradientChanged(const QCPColorGradient &newGradient);
  
  This signal is emitted when the gradient changes.
  
  \see setGradient
*/

/* end documentation of signals */

/*!
  Constructs a color map with the specified \a keyAxis and \a valueAxis.
  
  The created QCPColorMap is automatically registered with the PF_GraphicView instance inferred from
  \a keyAxis. This PF_GraphicView instance takes ownership of the QCPColorMap, so do not delete it
  manually but use PF_GraphicView::removePlottable() instead.
*/
QCPColorMap::QCPColorMap(QCPAxis *keyAxis, QCPAxis *valueAxis) :
    QCPAbstractPlottable(keyAxis, valueAxis),
    mDataScaleType(QCPAxis::stLinear),
    mMapData(new QCPColorMapData(10, 10, QCPRange(0, 5), QCPRange(0, 5))),
    mGradient(QCPColorGradient::gpCold),
    mInterpolate(true),
    mTightBoundary(false),
    mMapImageInvalidated(true)
{
}

QCPColorMap::~QCPColorMap()
{
    delete mMapData;
}

/*!
  Replaces the current \ref data with the provided \a data.
  
  If \a copy is set to true, the \a data object will only be copied. if false, the color map
  takes ownership of the passed data and replaces the internal data pointer with it. This is
  significantly faster than copying for large datasets.
*/
void QCPColorMap::setData(QCPColorMapData *data, bool copy)
{
    if (mMapData == data)
    {
        qDebug() << Q_FUNC_INFO << "The data pointer is already in (and owned by) this plottable" << reinterpret_cast<quintptr>(data);
        return;
    }
    if (copy)
    {
        *mMapData = *data;
    } else
    {
        delete mMapData;
        mMapData = data;
    }
    mMapImageInvalidated = true;
}

/*!
  Sets the data range of this color map to \a dataRange. The data range defines which data values
  are mapped to the color gradient.
  
  To make the data range span the full range of the data set, use \ref rescaleDataRange.
  
  \see QCPColorScale::setDataRange
*/
void QCPColorMap::setDataRange(const QCPRange &dataRange)
{
    if (!QCPRange::validRange(dataRange)) return;
    if (mDataRange.lower != dataRange.lower || mDataRange.upper != dataRange.upper)
    {
        if (mDataScaleType == QCPAxis::stLogarithmic)
            mDataRange = dataRange.sanitizedForLogScale();
        else
            mDataRange = dataRange.sanitizedForLinScale();
        mMapImageInvalidated = true;
        emit dataRangeChanged(mDataRange);
    }
}

/*!
  Sets whether the data is correlated with the color gradient linearly or logarithmically.
  
  \see QCPColorScale::setDataScaleType
*/
void QCPColorMap::setDataScaleType(QCPAxis::ScaleType scaleType)
{
    if (mDataScaleType != scaleType)
    {
        mDataScaleType = scaleType;
        mMapImageInvalidated = true;
        emit dataScaleTypeChanged(mDataScaleType);
        if (mDataScaleType == QCPAxis::stLogarithmic)
            setDataRange(mDataRange.sanitizedForLogScale());
    }
}

/*!
  Sets the color gradient that is used to represent the data. For more details on how to create an
  own gradient or use one of the preset gradients, see \ref QCPColorGradient.
  
  The colors defined by the gradient will be used to represent data values in the currently set
  data range, see \ref setDataRange. Data points that are outside this data range will either be
  colored uniformly with the respective gradient boundary color, or the gradient will repeat,
  depending on \ref QCPColorGradient::setPeriodic.
  
  \see QCPColorScale::setGradient
*/
void QCPColorMap::setGradient(const QCPColorGradient &gradient)
{
    if (mGradient != gradient)
    {
        mGradient = gradient;
        mMapImageInvalidated = true;
        emit gradientChanged(mGradient);
    }
}

/*!
  Sets whether the color map image shall use bicubic interpolation when displaying the color map
  shrinked or expanded, and not at a 1:1 pixel-to-data scale.
  
  \image html QCPColorMap-interpolate.png "A 10*10 color map, with interpolation and without interpolation enabled"
*/
void QCPColorMap::setInterpolate(bool enabled)
{
    mInterpolate = enabled;
    mMapImageInvalidated = true; // because oversampling factors might need to change
}

/*!
  Sets whether the outer most data rows and columns are clipped to the specified key and value
  range (see \ref QCPColorMapData::setKeyRange, \ref QCPColorMapData::setValueRange).
  
  if \a enabled is set to false, the data points at the border of the color map are drawn with the
  same width and height as all other data points. Since the data points are represented by
  rectangles of one color centered on the data coordinate, this means that the shown color map
  extends by half a data point over the specified key/value range in each direction.
  
  \image html QCPColorMap-tightboundary.png "A color map, with tight boundary enabled and disabled"
*/
void QCPColorMap::setTightBoundary(bool enabled)
{
    mTightBoundary = enabled;
}

/*!
  Associates the color scale \a colorScale with this color map.
  
  This means that both the color scale and the color map synchronize their gradient, data range and
  data scale type (\ref setGradient, \ref setDataRange, \ref setDataScaleType). Multiple color maps
  can be associated with one single color scale. This causes the color maps to also synchronize
  those properties, via the mutual color scale.
  
  This function causes the color map to adopt the current color gradient, data range and data scale
  type of \a colorScale. After this call, you may change these properties at either the color map
  or the color scale, and the setting will be applied to both.
  
  Pass 0 as \a colorScale to disconnect the color scale from this color map again.
*/
void QCPColorMap::setColorScale(QCPColorScale *colorScale)
{
    if (mColorScale) // unconnect signals from old color scale
    {
        disconnect(this, SIGNAL(dataRangeChanged(QCPRange)), mColorScale.data(), SLOT(setDataRange(QCPRange)));
        disconnect(this, SIGNAL(dataScaleTypeChanged(QCPAxis::ScaleType)), mColorScale.data(), SLOT(setDataScaleType(QCPAxis::ScaleType)));
        disconnect(this, SIGNAL(gradientChanged(QCPColorGradient)), mColorScale.data(), SLOT(setGradient(QCPColorGradient)));
        disconnect(mColorScale.data(), SIGNAL(dataRangeChanged(QCPRange)), this, SLOT(setDataRange(QCPRange)));
        disconnect(mColorScale.data(), SIGNAL(gradientChanged(QCPColorGradient)), this, SLOT(setGradient(QCPColorGradient)));
        disconnect(mColorScale.data(), SIGNAL(dataScaleTypeChanged(QCPAxis::ScaleType)), this, SLOT(setDataScaleType(QCPAxis::ScaleType)));
    }
    mColorScale = colorScale;
    if (mColorScale) // connect signals to new color scale
    {
        setGradient(mColorScale.data()->gradient());
        setDataRange(mColorScale.data()->dataRange());
        setDataScaleType(mColorScale.data()->dataScaleType());
        connect(this, SIGNAL(dataRangeChanged(QCPRange)), mColorScale.data(), SLOT(setDataRange(QCPRange)));
        connect(this, SIGNAL(dataScaleTypeChanged(QCPAxis::ScaleType)), mColorScale.data(), SLOT(setDataScaleType(QCPAxis::ScaleType)));
        connect(this, SIGNAL(gradientChanged(QCPColorGradient)), mColorScale.data(), SLOT(setGradient(QCPColorGradient)));
        connect(mColorScale.data(), SIGNAL(dataRangeChanged(QCPRange)), this, SLOT(setDataRange(QCPRange)));
        connect(mColorScale.data(), SIGNAL(gradientChanged(QCPColorGradient)), this, SLOT(setGradient(QCPColorGradient)));
        connect(mColorScale.data(), SIGNAL(dataScaleTypeChanged(QCPAxis::ScaleType)), this, SLOT(setDataScaleType(QCPAxis::ScaleType)));
    }
}

/*!
  Sets the data range (\ref setDataRange) to span the minimum and maximum values that occur in the
  current data set. This corresponds to the \ref rescaleKeyAxis or \ref rescaleValueAxis methods,
  only for the third data dimension of the color map.
  
  The minimum and maximum values of the data set are buffered in the internal QCPColorMapData
  instance (\ref data). As data is updated via its \ref QCPColorMapData::setCell or \ref
  QCPColorMapData::setData, the buffered minimum and maximum values are updated, too. For
  performance reasons, however, they are only updated in an expanding fashion. So the buffered
  maximum can only increase and the buffered minimum can only decrease. In consequence, changes to
  the data that actually lower the maximum of the data set (by overwriting the cell holding the
  current maximum with a smaller value), aren't recognized and the buffered maximum overestimates
  the true maximum of the data set. The same happens for the buffered minimum. To recalculate the
  true minimum and maximum by explicitly looking at each cell, the method
  QCPColorMapData::recalculateDataBounds can be used. For convenience, setting the parameter \a
  recalculateDataBounds calls this method before setting the data range to the buffered minimum and
  maximum.
  
  \see setDataRange
*/
void QCPColorMap::rescaleDataRange(bool recalculateDataBounds)
{
    if (recalculateDataBounds)
        mMapData->recalculateDataBounds();
    setDataRange(mMapData->dataBounds());
}

/*!
  Takes the current appearance of the color map and updates the legend icon, which is used to
  represent this color map in the legend (see \ref QCPLegend).
  
  The \a transformMode specifies whether the rescaling is done by a faster, low quality image
  scaling algorithm (Qt::FastTransformation) or by a slower, higher quality algorithm
  (Qt::SmoothTransformation).
  
  The current color map appearance is scaled down to \a thumbSize. Ideally, this should be equal to
  the size of the legend icon (see \ref QCPLegend::setIconSize). If it isn't exactly the configured
  legend icon size, the thumb will be rescaled during drawing of the legend item.
  
  \see setDataRange
*/
void QCPColorMap::updateLegendIcon(Qt::TransformationMode transformMode, const QSize &thumbSize)
{
    if (mMapImage.isNull() && !data()->isEmpty())
        updateMapImage(); // try to update map image if it's null (happens if no draw has happened yet)

    if (!mMapImage.isNull()) // might still be null, e.g. if data is empty, so check here again
    {
        bool mirrorX = (keyAxis()->orientation() == Qt::Horizontal ? keyAxis() : valueAxis())->rangeReversed();
        bool mirrorY = (valueAxis()->orientation() == Qt::Vertical ? valueAxis() : keyAxis())->rangeReversed();
        mLegendIcon = QPixmap::fromImage(mMapImage.mirrored(mirrorX, mirrorY)).scaled(thumbSize, Qt::KeepAspectRatio, transformMode);
    }
}

/* inherits documentation from base class */
double QCPColorMap::selectTest(const QPointF &pos, bool onlySelectable, QVariant *details) const
{
    Q_UNUSED(details)
    if ((onlySelectable && mSelectable == QCP::stNone) || mMapData->isEmpty())
        return -1;
    if (!mKeyAxis || !mValueAxis)
        return -1;

    if (mKeyAxis.data()->axisRect()->rect().contains(pos.toPoint()))
    {
        double posKey, posValue;
        pixelsToCoords(pos, posKey, posValue);
        if (mMapData->keyRange().contains(posKey) && mMapData->valueRange().contains(posValue))
        {
            if (details)
                details->setValue(QCPDataSelection(QCPDataRange(0, 1))); // temporary solution, to facilitate whole-plottable selection. Replace in future version with segmented 2D selection.
            return mParentPlot->selectionTolerance()*0.99;
        }
    }
    return -1;
}

/* inherits documentation from base class */
QCPRange QCPColorMap::getKeyRange(bool &foundRange, QCP::SignDomain inSignDomain) const
{
    foundRange = true;
    QCPRange result = mMapData->keyRange();
    result.normalize();
    if (inSignDomain == QCP::sdPositive)
    {
        if (result.lower <= 0 && result.upper > 0)
            result.lower = result.upper*1e-3;
        else if (result.lower <= 0 && result.upper <= 0)
            foundRange = false;
    } else if (inSignDomain == QCP::sdNegative)
    {
        if (result.upper >= 0 && result.lower < 0)
            result.upper = result.lower*1e-3;
        else if (result.upper >= 0 && result.lower >= 0)
            foundRange = false;
    }
    return result;
}

/* inherits documentation from base class */
QCPRange QCPColorMap::getValueRange(bool &foundRange, QCP::SignDomain inSignDomain, const QCPRange &inKeyRange) const
{
    if (inKeyRange != QCPRange())
    {
        if (mMapData->keyRange().upper < inKeyRange.lower || mMapData->keyRange().lower > inKeyRange.upper)
        {
            foundRange = false;
            return QCPRange();
        }
    }

    foundRange = true;
    QCPRange result = mMapData->valueRange();
    result.normalize();
    if (inSignDomain == QCP::sdPositive)
    {
        if (result.lower <= 0 && result.upper > 0)
            result.lower = result.upper*1e-3;
        else if (result.lower <= 0 && result.upper <= 0)
            foundRange = false;
    } else if (inSignDomain == QCP::sdNegative)
    {
        if (result.upper >= 0 && result.lower < 0)
            result.upper = result.lower*1e-3;
        else if (result.upper >= 0 && result.lower >= 0)
            foundRange = false;
    }
    return result;
}

/*! \internal
  
  Updates the internal map image buffer by going through the internal \ref QCPColorMapData and
  turning the data values into color pixels with \ref QCPColorGradient::colorize.
  
  This method is called by \ref QCPColorMap::draw if either the data has been modified or the map image
  has been invalidated for a different reason (e.g. a change of the data range with \ref
  setDataRange).
  
  If the map cell count is low, the image created will be oversampled in order to avoid a
  QPainter::drawImage bug which makes inner pixel boundaries jitter when stretch-drawing images
  without smooth transform enabled. Accordingly, oversampling isn't performed if \ref
  setInterpolate is true.
*/
void QCPColorMap::updateMapImage()
{
    QCPAxis *keyAxis = mKeyAxis.data();
    if (!keyAxis) return;
    if (mMapData->isEmpty()) return;

    const QImage::Format format = QImage::Format_ARGB32_Premultiplied;
    const int keySize = mMapData->keySize();
    const int valueSize = mMapData->valueSize();
    int keyOversamplingFactor = mInterpolate ? 1 : (int)(1.0+100.0/(double)keySize); // make mMapImage have at least size 100, factor becomes 1 if size > 200 or interpolation is on
    int valueOversamplingFactor = mInterpolate ? 1 : (int)(1.0+100.0/(double)valueSize); // make mMapImage have at least size 100, factor becomes 1 if size > 200 or interpolation is on

    // resize mMapImage to correct dimensions including possible oversampling factors, according to key/value axes orientation:
    if (keyAxis->orientation() == Qt::Horizontal && (mMapImage.width() != keySize*keyOversamplingFactor || mMapImage.height() != valueSize*valueOversamplingFactor))
        mMapImage = QImage(QSize(keySize*keyOversamplingFactor, valueSize*valueOversamplingFactor), format);
    else if (keyAxis->orientation() == Qt::Vertical && (mMapImage.width() != valueSize*valueOversamplingFactor || mMapImage.height() != keySize*keyOversamplingFactor))
        mMapImage = QImage(QSize(valueSize*valueOversamplingFactor, keySize*keyOversamplingFactor), format);

    if (mMapImage.isNull())
    {
        qDebug() << Q_FUNC_INFO << "Couldn't create map image (possibly too large for memory)";
        mMapImage = QImage(QSize(10, 10), format);
        mMapImage.fill(Qt::black);
    } else
    {
        QImage *localMapImage = &mMapImage; // this is the image on which the colorization operates. Either the final mMapImage, or if we need oversampling, mUndersampledMapImage
        if (keyOversamplingFactor > 1 || valueOversamplingFactor > 1)
        {
            // resize undersampled map image to actual key/value cell sizes:
            if (keyAxis->orientation() == Qt::Horizontal && (mUndersampledMapImage.width() != keySize || mUndersampledMapImage.height() != valueSize))
                mUndersampledMapImage = QImage(QSize(keySize, valueSize), format);
            else if (keyAxis->orientation() == Qt::Vertical && (mUndersampledMapImage.width() != valueSize || mUndersampledMapImage.height() != keySize))
                mUndersampledMapImage = QImage(QSize(valueSize, keySize), format);
            localMapImage = &mUndersampledMapImage; // make the colorization run on the undersampled image
        } else if (!mUndersampledMapImage.isNull())
            mUndersampledMapImage = QImage(); // don't need oversampling mechanism anymore (map size has changed) but mUndersampledMapImage still has nonzero size, free it

        const double *rawData = mMapData->mData;
        const unsigned char *rawAlpha = mMapData->mAlpha;
        if (keyAxis->orientation() == Qt::Horizontal)
        {
            const int lineCount = valueSize;
            const int rowCount = keySize;
            for (int line=0; line<lineCount; ++line)
            {
                QRgb* pixels = reinterpret_cast<QRgb*>(localMapImage->scanLine(lineCount-1-line)); // invert scanline index because QImage counts scanlines from top, but our vertical index counts from bottom (mathematical coordinate system)
                if (rawAlpha)
                    mGradient.colorize(rawData+line*rowCount, rawAlpha+line*rowCount, mDataRange, pixels, rowCount, 1, mDataScaleType==QCPAxis::stLogarithmic);
                else
                    mGradient.colorize(rawData+line*rowCount, mDataRange, pixels, rowCount, 1, mDataScaleType==QCPAxis::stLogarithmic);
            }
        } else // keyAxis->orientation() == Qt::Vertical
        {
            const int lineCount = keySize;
            const int rowCount = valueSize;
            for (int line=0; line<lineCount; ++line)
            {
                QRgb* pixels = reinterpret_cast<QRgb*>(localMapImage->scanLine(lineCount-1-line)); // invert scanline index because QImage counts scanlines from top, but our vertical index counts from bottom (mathematical coordinate system)
                if (rawAlpha)
                    mGradient.colorize(rawData+line, rawAlpha+line, mDataRange, pixels, rowCount, lineCount, mDataScaleType==QCPAxis::stLogarithmic);
                else
                    mGradient.colorize(rawData+line, mDataRange, pixels, rowCount, lineCount, mDataScaleType==QCPAxis::stLogarithmic);
            }
        }

        if (keyOversamplingFactor > 1 || valueOversamplingFactor > 1)
        {
            if (keyAxis->orientation() == Qt::Horizontal)
                mMapImage = mUndersampledMapImage.scaled(keySize*keyOversamplingFactor, valueSize*valueOversamplingFactor, Qt::IgnoreAspectRatio, Qt::FastTransformation);
            else
                mMapImage = mUndersampledMapImage.scaled(valueSize*valueOversamplingFactor, keySize*keyOversamplingFactor, Qt::IgnoreAspectRatio, Qt::FastTransformation);
        }
    }
    mMapData->mDataModified = false;
    mMapImageInvalidated = false;
}

/* inherits documentation from base class */
void QCPColorMap::draw(QCPPainter *painter)
{
    if (mMapData->isEmpty()) return;
    if (!mKeyAxis || !mValueAxis) return;
    applyDefaultAntialiasingHint(painter);

    if (mMapData->mDataModified || mMapImageInvalidated)
        updateMapImage();

    // use buffer if painting vectorized (PDF):
    const bool useBuffer = painter->modes().testFlag(QCPPainter::pmVectorized);
    QCPPainter *localPainter = painter; // will be redirected to paint on mapBuffer if painting vectorized
    QRectF mapBufferTarget; // the rect in absolute widget coordinates where the visible map portion/buffer will end up in
    QPixmap mapBuffer;
    if (useBuffer)
    {
        const double mapBufferPixelRatio = 3; // factor by which DPI is increased in embedded bitmaps
        mapBufferTarget = painter->clipRegion().boundingRect();
        mapBuffer = QPixmap((mapBufferTarget.size()*mapBufferPixelRatio).toSize());
        mapBuffer.fill(Qt::transparent);
        localPainter = new QCPPainter(&mapBuffer);
        localPainter->scale(mapBufferPixelRatio, mapBufferPixelRatio);
        localPainter->translate(-mapBufferTarget.topLeft());
    }

    QRectF imageRect = QRectF(coordsToPixels(mMapData->keyRange().lower, mMapData->valueRange().lower),
                              coordsToPixels(mMapData->keyRange().upper, mMapData->valueRange().upper)).normalized();
    // extend imageRect to contain outer halves/quarters of bordering/cornering pixels (cells are centered on map range boundary):
    double halfCellWidth = 0; // in pixels
    double halfCellHeight = 0; // in pixels
    if (keyAxis()->orientation() == Qt::Horizontal)
    {
        if (mMapData->keySize() > 1)
            halfCellWidth = 0.5*imageRect.width()/(double)(mMapData->keySize()-1);
        if (mMapData->valueSize() > 1)
            halfCellHeight = 0.5*imageRect.height()/(double)(mMapData->valueSize()-1);
    } else // keyAxis orientation is Qt::Vertical
    {
        if (mMapData->keySize() > 1)
            halfCellHeight = 0.5*imageRect.height()/(double)(mMapData->keySize()-1);
        if (mMapData->valueSize() > 1)
            halfCellWidth = 0.5*imageRect.width()/(double)(mMapData->valueSize()-1);
    }
    imageRect.adjust(-halfCellWidth, -halfCellHeight, halfCellWidth, halfCellHeight);
    const bool mirrorX = (keyAxis()->orientation() == Qt::Horizontal ? keyAxis() : valueAxis())->rangeReversed();
    const bool mirrorY = (valueAxis()->orientation() == Qt::Vertical ? valueAxis() : keyAxis())->rangeReversed();
    const bool smoothBackup = localPainter->renderHints().testFlag(QPainter::SmoothPixmapTransform);
    localPainter->setRenderHint(QPainter::SmoothPixmapTransform, mInterpolate);
    QRegion clipBackup;
    if (mTightBoundary)
    {
        clipBackup = localPainter->clipRegion();
        QRectF tightClipRect = QRectF(coordsToPixels(mMapData->keyRange().lower, mMapData->valueRange().lower),
                                      coordsToPixels(mMapData->keyRange().upper, mMapData->valueRange().upper)).normalized();
        localPainter->setClipRect(tightClipRect, Qt::IntersectClip);
    }
    localPainter->drawImage(imageRect, mMapImage.mirrored(mirrorX, mirrorY));
    if (mTightBoundary)
        localPainter->setClipRegion(clipBackup);
    localPainter->setRenderHint(QPainter::SmoothPixmapTransform, smoothBackup);

    if (useBuffer) // localPainter painted to mapBuffer, so now draw buffer with original painter
    {
        delete localPainter;
        painter->drawPixmap(mapBufferTarget.toRect(), mapBuffer);
    }
}

/* inherits documentation from base class */
void QCPColorMap::drawLegendIcon(QCPPainter *painter, const QRectF &rect) const
{
    applyDefaultAntialiasingHint(painter);
    // draw map thumbnail:
    if (!mLegendIcon.isNull())
    {
        QPixmap scaledIcon = mLegendIcon.scaled(rect.size().toSize(), Qt::KeepAspectRatio, Qt::FastTransformation);
        QRectF iconRect = QRectF(0, 0, scaledIcon.width(), scaledIcon.height());
        iconRect.moveCenter(rect.center());
        painter->drawPixmap(iconRect.topLeft(), scaledIcon);
    }
    /*
  // draw frame:
  painter->setBrush(Qt::NoBrush);
  painter->setPen(Qt::black);
  painter->drawRect(rect.adjusted(1, 1, 0, 0));
  */
}
/* end of 'src/plottables/plottable-colormap.cpp' */



/* including file 'src/items/item-line.cpp', size 8498                       */
/* commit ce344b3f96a62e5f652585e55f1ae7c7883cd45b 2018-06-25 01:03:39 +0200 */

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPItemLine
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPItemLine
  \brief A line from one point to another

  \image html QCPItemLine.png "Line example. Blue dotted circles are anchors, solid blue discs are positions."

  It has two positions, \a start and \a end, which define the end points of the line.
  
  With \ref setHead and \ref setTail you may set different line ending styles, e.g. to create an arrow.
*/

/*!
  Creates a line item and sets default values.
  
  The created item is automatically registered with \a parentPlot. This PF_GraphicView instance takes
  ownership of the item, so do not delete it manually but use PF_GraphicView::removeItem() instead.
*/
QCPItemLine::QCPItemLine(PF_GraphicView *parentPlot) :
    QCPAbstractItem(parentPlot),
    start(createPosition(QLatin1String("start"))),
    end(createPosition(QLatin1String("end")))
{
    start->setCoords(0, 0);
    end->setCoords(1, 1);

    setPen(QPen(Qt::black));
    setSelectedPen(QPen(Qt::blue,2));
}

QCPItemLine::~QCPItemLine()
{
}

/*!
  Sets the pen that will be used to draw the line
  
  \see setSelectedPen
*/
void QCPItemLine::setPen(const QPen &pen)
{
    mPen = pen;
}

/*!
  Sets the pen that will be used to draw the line when selected
  
  \see setPen, setSelected
*/
void QCPItemLine::setSelectedPen(const QPen &pen)
{
    mSelectedPen = pen;
}

/*!
  Sets the line ending style of the head. The head corresponds to the \a end position.
  
  Note that due to the overloaded QCPLineEnding constructor, you may directly specify
  a QCPLineEnding::EndingStyle here, e.g. \code setHead(QCPLineEnding::esSpikeArrow) \endcode
  
  \see setTail
*/
void QCPItemLine::setHead(const QCPLineEnding &head)
{
    mHead = head;
}

/*!
  Sets the line ending style of the tail. The tail corresponds to the \a start position.
  
  Note that due to the overloaded QCPLineEnding constructor, you may directly specify
  a QCPLineEnding::EndingStyle here, e.g. \code setTail(QCPLineEnding::esSpikeArrow) \endcode
  
  \see setHead
*/
void QCPItemLine::setTail(const QCPLineEnding &tail)
{
    mTail = tail;
}

/* inherits documentation from base class */
double QCPItemLine::selectTest(const QPointF &pos, bool onlySelectable, QVariant *details) const
{
    Q_UNUSED(details)
    if (onlySelectable && !mSelectable)
        return -1;

    return qSqrt(QCPVector2D(pos).distanceSquaredToLine(start->pixelPosition(), end->pixelPosition()));
}

/* inherits documentation from base class */
void QCPItemLine::draw(QCPPainter *painter)
{
    QCPVector2D startVec(start->pixelPosition());
    QCPVector2D endVec(end->pixelPosition());
    if (qFuzzyIsNull((startVec-endVec).lengthSquared()))
        return;
    // get visible segment of straight line inside clipRect:
    double clipPad = qMax(mHead.boundingDistance(), mTail.boundingDistance());
    clipPad = qMax(clipPad, (double)mainPen().widthF());
    QLineF line = getRectClippedLine(startVec, endVec, clipRect().adjusted(-clipPad, -clipPad, clipPad, clipPad));
    // paint visible segment, if existent:
    if (!line.isNull())
    {
        painter->setPen(mainPen());
        painter->drawLine(line);
        painter->setBrush(Qt::SolidPattern);
        if (mTail.style() != QCPLineEnding::esNone)
            mTail.draw(painter, startVec, startVec-endVec);
        if (mHead.style() != QCPLineEnding::esNone)
            mHead.draw(painter, endVec, endVec-startVec);
    }
}

/*! \internal

  Returns the section of the line defined by \a start and \a end, that is visible in the specified
  \a rect.
  
  This is a helper function for \ref draw.
*/
QLineF QCPItemLine::getRectClippedLine(const QCPVector2D &start, const QCPVector2D &end, const QRect &rect) const
{
    bool containsStart = rect.contains(start.x(), start.y());
    bool containsEnd = rect.contains(end.x(), end.y());
    if (containsStart && containsEnd)
        return QLineF(start.toPointF(), end.toPointF());

    QCPVector2D base = start;
    QCPVector2D vec = end-start;
    double bx, by;
    double gamma, mu;
    QLineF result;
    QList<QCPVector2D> pointVectors;

    if (!qFuzzyIsNull(vec.y())) // line is not horizontal
    {
        // check top of rect:
        bx = rect.left();
        by = rect.top();
        mu = (by-base.y())/vec.y();
        if (mu >= 0 && mu <= 1)
        {
            gamma = base.x()-bx + mu*vec.x();
            if (gamma >= 0 && gamma <= rect.width())
                pointVectors.append(QCPVector2D(bx+gamma, by));
        }
        // check bottom of rect:
        bx = rect.left();
        by = rect.bottom();
        mu = (by-base.y())/vec.y();
        if (mu >= 0 && mu <= 1)
        {
            gamma = base.x()-bx + mu*vec.x();
            if (gamma >= 0 && gamma <= rect.width())
                pointVectors.append(QCPVector2D(bx+gamma, by));
        }
    }
    if (!qFuzzyIsNull(vec.x())) // line is not vertical
    {
        // check left of rect:
        bx = rect.left();
        by = rect.top();
        mu = (bx-base.x())/vec.x();
        if (mu >= 0 && mu <= 1)
        {
            gamma = base.y()-by + mu*vec.y();
            if (gamma >= 0 && gamma <= rect.height())
                pointVectors.append(QCPVector2D(bx, by+gamma));
        }
        // check right of rect:
        bx = rect.right();
        by = rect.top();
        mu = (bx-base.x())/vec.x();
        if (mu >= 0 && mu <= 1)
        {
            gamma = base.y()-by + mu*vec.y();
            if (gamma >= 0 && gamma <= rect.height())
                pointVectors.append(QCPVector2D(bx, by+gamma));
        }
    }

    if (containsStart)
        pointVectors.append(start);
    if (containsEnd)
        pointVectors.append(end);

    // evaluate points:
    if (pointVectors.size() == 2)
    {
        result.setPoints(pointVectors.at(0).toPointF(), pointVectors.at(1).toPointF());
    } else if (pointVectors.size() > 2)
    {
        // line probably goes through corner of rect, and we got two points there. single out the point pair with greatest distance:
        double distSqrMax = 0;
        QCPVector2D pv1, pv2;
        for (int i=0; i<pointVectors.size()-1; ++i)
        {
            for (int k=i+1; k<pointVectors.size(); ++k)
            {
                double distSqr = (pointVectors.at(i)-pointVectors.at(k)).lengthSquared();
                if (distSqr > distSqrMax)
                {
                    pv1 = pointVectors.at(i);
                    pv2 = pointVectors.at(k);
                    distSqrMax = distSqr;
                }
            }
        }
        result.setPoints(pv1.toPointF(), pv2.toPointF());
    }
    return result;
}

/*! \internal

  Returns the pen that should be used for drawing lines. Returns mPen when the
  item is not selected and mSelectedPen when it is.
*/
QPen QCPItemLine::mainPen() const
{
    return mSelected ? mSelectedPen : mPen;
}
/* end of 'src/items/item-line.cpp' */


/* including file 'src/items/item-curve.cpp', size 7248                      */
/* commit ce344b3f96a62e5f652585e55f1ae7c7883cd45b 2018-06-25 01:03:39 +0200 */

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPItemCurve
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPItemCurve
  \brief A curved line from one point to another

  \image html QCPItemCurve.png "Curve example. Blue dotted circles are anchors, solid blue discs are positions."

  It has four positions, \a start and \a end, which define the end points of the line, and two
  control points which define the direction the line exits from the start and the direction from
  which it approaches the end: \a startDir and \a endDir.
  
  With \ref setHead and \ref setTail you may set different line ending styles, e.g. to create an
  arrow.
  
  Often it is desirable for the control points to stay at fixed relative positions to the start/end
  point. This can be achieved by setting the parent anchor e.g. of \a startDir simply to \a start,
  and then specify the desired pixel offset with QCPItemPosition::setCoords on \a startDir.
*/

/*!
  Creates a curve item and sets default values.
  
  The created item is automatically registered with \a parentPlot. This PF_GraphicView instance takes
  ownership of the item, so do not delete it manually but use PF_GraphicView::removeItem() instead.
*/
QCPItemCurve::QCPItemCurve(PF_GraphicView *parentPlot) :
    QCPAbstractItem(parentPlot),
    start(createPosition(QLatin1String("start"))),
    startDir(createPosition(QLatin1String("startDir"))),
    endDir(createPosition(QLatin1String("endDir"))),
    end(createPosition(QLatin1String("end")))
{
    start->setCoords(0, 0);
    startDir->setCoords(0.5, 0);
    endDir->setCoords(0, 0.5);
    end->setCoords(1, 1);

    setPen(QPen(Qt::black));
    setSelectedPen(QPen(Qt::blue,2));
}

QCPItemCurve::~QCPItemCurve()
{
}

/*!
  Sets the pen that will be used to draw the line
  
  \see setSelectedPen
*/
void QCPItemCurve::setPen(const QPen &pen)
{
    mPen = pen;
}

/*!
  Sets the pen that will be used to draw the line when selected
  
  \see setPen, setSelected
*/
void QCPItemCurve::setSelectedPen(const QPen &pen)
{
    mSelectedPen = pen;
}

/*!
  Sets the line ending style of the head. The head corresponds to the \a end position.
  
  Note that due to the overloaded QCPLineEnding constructor, you may directly specify
  a QCPLineEnding::EndingStyle here, e.g. \code setHead(QCPLineEnding::esSpikeArrow) \endcode
  
  \see setTail
*/
void QCPItemCurve::setHead(const QCPLineEnding &head)
{
    mHead = head;
}

/*!
  Sets the line ending style of the tail. The tail corresponds to the \a start position.
  
  Note that due to the overloaded QCPLineEnding constructor, you may directly specify
  a QCPLineEnding::EndingStyle here, e.g. \code setTail(QCPLineEnding::esSpikeArrow) \endcode
  
  \see setHead
*/
void QCPItemCurve::setTail(const QCPLineEnding &tail)
{
    mTail = tail;
}

/* inherits documentation from base class */
double QCPItemCurve::selectTest(const QPointF &pos, bool onlySelectable, QVariant *details) const
{
    Q_UNUSED(details)
    if (onlySelectable && !mSelectable)
        return -1;

    QPointF startVec(start->pixelPosition());
    QPointF startDirVec(startDir->pixelPosition());
    QPointF endDirVec(endDir->pixelPosition());
    QPointF endVec(end->pixelPosition());

    QPainterPath cubicPath(startVec);
    cubicPath.cubicTo(startDirVec, endDirVec, endVec);

    QList<QPolygonF> polygons = cubicPath.toSubpathPolygons();
    if (polygons.isEmpty())
        return -1;
    const QPolygonF polygon = polygons.first();
    QCPVector2D p(pos);
    double minDistSqr = (std::numeric_limits<double>::max)();
    for (int i=1; i<polygon.size(); ++i)
    {
        double distSqr = p.distanceSquaredToLine(polygon.at(i-1), polygon.at(i));
        if (distSqr < minDistSqr)
            minDistSqr = distSqr;
    }
    return qSqrt(minDistSqr);
}

/* inherits documentation from base class */
void QCPItemCurve::draw(QCPPainter *painter)
{
    QCPVector2D startVec(start->pixelPosition());
    QCPVector2D startDirVec(startDir->pixelPosition());
    QCPVector2D endDirVec(endDir->pixelPosition());
    QCPVector2D endVec(end->pixelPosition());
    if ((endVec-startVec).length() > 1e10) // too large curves cause crash
        return;

    QPainterPath cubicPath(startVec.toPointF());
    cubicPath.cubicTo(startDirVec.toPointF(), endDirVec.toPointF(), endVec.toPointF());

    // paint visible segment, if existent:
    QRect clip = clipRect().adjusted(-mainPen().widthF(), -mainPen().widthF(), mainPen().widthF(), mainPen().widthF());
    QRect cubicRect = cubicPath.controlPointRect().toRect();
    if (cubicRect.isEmpty()) // may happen when start and end exactly on same x or y position
        cubicRect.adjust(0, 0, 1, 1);
    if (clip.intersects(cubicRect))
    {
        painter->setPen(mainPen());
        painter->drawPath(cubicPath);
        painter->setBrush(Qt::SolidPattern);
        if (mTail.style() != QCPLineEnding::esNone)
            mTail.draw(painter, startVec, M_PI-cubicPath.angleAtPercent(0)/180.0*M_PI);
        if (mHead.style() != QCPLineEnding::esNone)
            mHead.draw(painter, endVec, -cubicPath.angleAtPercent(1)/180.0*M_PI);
    }
}

/*! \internal

  Returns the pen that should be used for drawing lines. Returns mPen when the
  item is not selected and mSelectedPen when it is.
*/
QPen QCPItemCurve::mainPen() const
{
    return mSelected ? mSelectedPen : mPen;
}
/* end of 'src/items/item-curve.cpp' */


/* including file 'src/items/item-rect.cpp', size 6479                       */
/* commit ce344b3f96a62e5f652585e55f1ae7c7883cd45b 2018-06-25 01:03:39 +0200 */

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPItemRect
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPItemRect
  \brief A rectangle

  \image html QCPItemRect.png "Rectangle example. Blue dotted circles are anchors, solid blue discs are positions."

  It has two positions, \a topLeft and \a bottomRight, which define the rectangle.
*/

/*!
  Creates a rectangle item and sets default values.
  
  The created item is automatically registered with \a parentPlot. This PF_GraphicView instance takes
  ownership of the item, so do not delete it manually but use PF_GraphicView::removeItem() instead.
*/
QCPItemRect::QCPItemRect(PF_GraphicView *parentPlot) :
    QCPAbstractItem(parentPlot),
    topLeft(createPosition(QLatin1String("topLeft"))),
    bottomRight(createPosition(QLatin1String("bottomRight"))),
    top(createAnchor(QLatin1String("top"), aiTop)),
    topRight(createAnchor(QLatin1String("topRight"), aiTopRight)),
    right(createAnchor(QLatin1String("right"), aiRight)),
    bottom(createAnchor(QLatin1String("bottom"), aiBottom)),
    bottomLeft(createAnchor(QLatin1String("bottomLeft"), aiBottomLeft)),
    left(createAnchor(QLatin1String("left"), aiLeft))
{
    topLeft->setCoords(0, 1);
    bottomRight->setCoords(1, 0);

    setPen(QPen(Qt::black));
    setSelectedPen(QPen(Qt::blue,2));
    setBrush(Qt::NoBrush);
    setSelectedBrush(Qt::NoBrush);
}

QCPItemRect::~QCPItemRect()
{
}

/*!
  Sets the pen that will be used to draw the line of the rectangle
  
  \see setSelectedPen, setBrush
*/
void QCPItemRect::setPen(const QPen &pen)
{
    mPen = pen;
}

/*!
  Sets the pen that will be used to draw the line of the rectangle when selected
  
  \see setPen, setSelected
*/
void QCPItemRect::setSelectedPen(const QPen &pen)
{
    mSelectedPen = pen;
}

/*!
  Sets the brush that will be used to fill the rectangle. To disable filling, set \a brush to
  Qt::NoBrush.
  
  \see setSelectedBrush, setPen
*/
void QCPItemRect::setBrush(const QBrush &brush)
{
    mBrush = brush;
}

/*!
  Sets the brush that will be used to fill the rectangle when selected. To disable filling, set \a
  brush to Qt::NoBrush.
  
  \see setBrush
*/
void QCPItemRect::setSelectedBrush(const QBrush &brush)
{
    mSelectedBrush = brush;
}

/* inherits documentation from base class */
double QCPItemRect::selectTest(const QPointF &pos, bool onlySelectable, QVariant *details) const
{
    Q_UNUSED(details)
    if (onlySelectable && !mSelectable)
        return -1;

    QRectF rect = QRectF(topLeft->pixelPosition(), bottomRight->pixelPosition()).normalized();
    bool filledRect = mBrush.style() != Qt::NoBrush && mBrush.color().alpha() != 0;
    return rectDistance(rect, pos, filledRect);
}

/* inherits documentation from base class */
void QCPItemRect::draw(QCPPainter *painter)
{
    QPointF p1 = topLeft->pixelPosition();
    QPointF p2 = bottomRight->pixelPosition();
    if (p1.toPoint() == p2.toPoint())
        return;
    QRectF rect = QRectF(p1, p2).normalized();
    double clipPad = mainPen().widthF();
    QRectF boundingRect = rect.adjusted(-clipPad, -clipPad, clipPad, clipPad);
    if (boundingRect.intersects(clipRect())) // only draw if bounding rect of rect item is visible in cliprect
    {
        painter->setPen(mainPen());
        painter->setBrush(mainBrush());
        painter->drawRect(rect);
    }
}

/* inherits documentation from base class */
QPointF QCPItemRect::anchorPixelPosition(int anchorId) const
{
    QRectF rect = QRectF(topLeft->pixelPosition(), bottomRight->pixelPosition());
    switch (anchorId)
    {
    case aiTop:         return (rect.topLeft()+rect.topRight())*0.5;
    case aiTopRight:    return rect.topRight();
    case aiRight:       return (rect.topRight()+rect.bottomRight())*0.5;
    case aiBottom:      return (rect.bottomLeft()+rect.bottomRight())*0.5;
    case aiBottomLeft:  return rect.bottomLeft();
    case aiLeft:        return (rect.topLeft()+rect.bottomLeft())*0.5;
    }

    qDebug() << Q_FUNC_INFO << "invalid anchorId" << anchorId;
    return QPointF();
}

/*! \internal

  Returns the pen that should be used for drawing lines. Returns mPen when the item is not selected
  and mSelectedPen when it is.
*/
QPen QCPItemRect::mainPen() const
{
    return mSelected ? mSelectedPen : mPen;
}

/*! \internal

  Returns the brush that should be used for drawing fills of the item. Returns mBrush when the item
  is not selected and mSelectedBrush when it is.
*/
QBrush QCPItemRect::mainBrush() const
{
    return mSelected ? mSelectedBrush : mBrush;
}
/* end of 'src/items/item-rect.cpp' */


/* including file 'src/items/item-text.cpp', size 13338                      */
/* commit ce344b3f96a62e5f652585e55f1ae7c7883cd45b 2018-06-25 01:03:39 +0200 */

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPItemText
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPItemText
  \brief A text label

  \image html QCPItemText.png "Text example. Blue dotted circles are anchors, solid blue discs are positions."

  Its position is defined by the member \a position and the setting of \ref setPositionAlignment.
  The latter controls which part of the text rect shall be aligned with \a position.
  
  The text alignment itself (i.e. left, center, right) can be controlled with \ref
  setTextAlignment.
  
  The text may be rotated around the \a position point with \ref setRotation.
*/

/*!
  Creates a text item and sets default values.
  
  The created item is automatically registered with \a parentPlot. This PF_GraphicView instance takes
  ownership of the item, so do not delete it manually but use PF_GraphicView::removeItem() instead.
*/
QCPItemText::QCPItemText(PF_GraphicView *parentPlot) :
    QCPAbstractItem(parentPlot),
    position(createPosition(QLatin1String("position"))),
    topLeft(createAnchor(QLatin1String("topLeft"), aiTopLeft)),
    top(createAnchor(QLatin1String("top"), aiTop)),
    topRight(createAnchor(QLatin1String("topRight"), aiTopRight)),
    right(createAnchor(QLatin1String("right"), aiRight)),
    bottomRight(createAnchor(QLatin1String("bottomRight"), aiBottomRight)),
    bottom(createAnchor(QLatin1String("bottom"), aiBottom)),
    bottomLeft(createAnchor(QLatin1String("bottomLeft"), aiBottomLeft)),
    left(createAnchor(QLatin1String("left"), aiLeft)),
    mText(QLatin1String("text")),
    mPositionAlignment(Qt::AlignCenter),
    mTextAlignment(Qt::AlignTop|Qt::AlignHCenter),
    mRotation(0)
{
    position->setCoords(0, 0);

    setPen(Qt::NoPen);
    setSelectedPen(Qt::NoPen);
    setBrush(Qt::NoBrush);
    setSelectedBrush(Qt::NoBrush);
    setColor(Qt::black);
    setSelectedColor(Qt::blue);
}

QCPItemText::~QCPItemText()
{
}

/*!
  Sets the color of the text.
*/
void QCPItemText::setColor(const QColor &color)
{
    mColor = color;
}

/*!
  Sets the color of the text that will be used when the item is selected.
*/
void QCPItemText::setSelectedColor(const QColor &color)
{
    mSelectedColor = color;
}

/*!
  Sets the pen that will be used do draw a rectangular border around the text. To disable the
  border, set \a pen to Qt::NoPen.
  
  \see setSelectedPen, setBrush, setPadding
*/
void QCPItemText::setPen(const QPen &pen)
{
    mPen = pen;
}

/*!
  Sets the pen that will be used do draw a rectangular border around the text, when the item is
  selected. To disable the border, set \a pen to Qt::NoPen.
  
  \see setPen
*/
void QCPItemText::setSelectedPen(const QPen &pen)
{
    mSelectedPen = pen;
}

/*!
  Sets the brush that will be used do fill the background of the text. To disable the
  background, set \a brush to Qt::NoBrush.
  
  \see setSelectedBrush, setPen, setPadding
*/
void QCPItemText::setBrush(const QBrush &brush)
{
    mBrush = brush;
}

/*!
  Sets the brush that will be used do fill the background of the text, when the item is selected. To disable the
  background, set \a brush to Qt::NoBrush.
  
  \see setBrush
*/
void QCPItemText::setSelectedBrush(const QBrush &brush)
{
    mSelectedBrush = brush;
}

/*!
  Sets the font of the text.
  
  \see setSelectedFont, setColor
*/
void QCPItemText::setFont(const QFont &font)
{
    mFont = font;
}

/*!
  Sets the font of the text that will be used when the item is selected.
  
  \see setFont
*/
void QCPItemText::setSelectedFont(const QFont &font)
{
    mSelectedFont = font;
}

/*!
  Sets the text that will be displayed. Multi-line texts are supported by inserting a line break
  character, e.g. '\n'.
  
  \see setFont, setColor, setTextAlignment
*/
void QCPItemText::setText(const QString &text)
{
    mText = text;
}

/*!
  Sets which point of the text rect shall be aligned with \a position.
  
  Examples:
  \li If \a alignment is <tt>Qt::AlignHCenter | Qt::AlignTop</tt>, the text will be positioned such
  that the top of the text rect will be horizontally centered on \a position.
  \li If \a alignment is <tt>Qt::AlignLeft | Qt::AlignBottom</tt>, \a position will indicate the
  bottom left corner of the text rect.
  
  If you want to control the alignment of (multi-lined) text within the text rect, use \ref
  setTextAlignment.
*/
void QCPItemText::setPositionAlignment(Qt::Alignment alignment)
{
    mPositionAlignment = alignment;
}

/*!
  Controls how (multi-lined) text is aligned inside the text rect (typically Qt::AlignLeft, Qt::AlignCenter or Qt::AlignRight).
*/
void QCPItemText::setTextAlignment(Qt::Alignment alignment)
{
    mTextAlignment = alignment;
}

/*!
  Sets the angle in degrees by which the text (and the text rectangle, if visible) will be rotated
  around \a position.
*/
void QCPItemText::setRotation(double degrees)
{
    mRotation = degrees;
}

/*!
  Sets the distance between the border of the text rectangle and the text. The appearance (and
  visibility) of the text rectangle can be controlled with \ref setPen and \ref setBrush.
*/
void QCPItemText::setPadding(const QMargins &padding)
{
    mPadding = padding;
}

/* inherits documentation from base class */
double QCPItemText::selectTest(const QPointF &pos, bool onlySelectable, QVariant *details) const
{
    Q_UNUSED(details)
    if (onlySelectable && !mSelectable)
        return -1;

    // The rect may be rotated, so we transform the actual clicked pos to the rotated
    // coordinate system, so we can use the normal rectDistance function for non-rotated rects:
    QPointF positionPixels(position->pixelPosition());
    QTransform inputTransform;
    inputTransform.translate(positionPixels.x(), positionPixels.y());
    inputTransform.rotate(-mRotation);
    inputTransform.translate(-positionPixels.x(), -positionPixels.y());
    QPointF rotatedPos = inputTransform.map(pos);
    QFontMetrics fontMetrics(mFont);
    QRect textRect = fontMetrics.boundingRect(0, 0, 0, 0, Qt::TextDontClip|mTextAlignment, mText);
    QRect textBoxRect = textRect.adjusted(-mPadding.left(), -mPadding.top(), mPadding.right(), mPadding.bottom());
    QPointF textPos = getTextDrawPoint(positionPixels, textBoxRect, mPositionAlignment);
    textBoxRect.moveTopLeft(textPos.toPoint());

    return rectDistance(textBoxRect, rotatedPos, true);
}

/* inherits documentation from base class */
void QCPItemText::draw(QCPPainter *painter)
{
    QPointF pos(position->pixelPosition());
    QTransform transform = painter->transform();
    transform.translate(pos.x(), pos.y());
    if (!qFuzzyIsNull(mRotation))
        transform.rotate(mRotation);
    painter->setFont(mainFont());
    QRect textRect = painter->fontMetrics().boundingRect(0, 0, 0, 0, Qt::TextDontClip|mTextAlignment, mText);
    QRect textBoxRect = textRect.adjusted(-mPadding.left(), -mPadding.top(), mPadding.right(), mPadding.bottom());
    QPointF textPos = getTextDrawPoint(QPointF(0, 0), textBoxRect, mPositionAlignment); // 0, 0 because the transform does the translation
    textRect.moveTopLeft(textPos.toPoint()+QPoint(mPadding.left(), mPadding.top()));
    textBoxRect.moveTopLeft(textPos.toPoint());
    double clipPad = mainPen().widthF();
    QRect boundingRect = textBoxRect.adjusted(-clipPad, -clipPad, clipPad, clipPad);
    if (transform.mapRect(boundingRect).intersects(painter->transform().mapRect(clipRect())))
    {
        painter->setTransform(transform);
        if ((mainBrush().style() != Qt::NoBrush && mainBrush().color().alpha() != 0) ||
                (mainPen().style() != Qt::NoPen && mainPen().color().alpha() != 0))
        {
            painter->setPen(mainPen());
            painter->setBrush(mainBrush());
            painter->drawRect(textBoxRect);
        }
        painter->setBrush(Qt::NoBrush);
        painter->setPen(QPen(mainColor()));
        painter->drawText(textRect, Qt::TextDontClip|mTextAlignment, mText);
    }
}

/* inherits documentation from base class */
QPointF QCPItemText::anchorPixelPosition(int anchorId) const
{
    // get actual rect points (pretty much copied from draw function):
    QPointF pos(position->pixelPosition());
    QTransform transform;
    transform.translate(pos.x(), pos.y());
    if (!qFuzzyIsNull(mRotation))
        transform.rotate(mRotation);
    QFontMetrics fontMetrics(mainFont());
    QRect textRect = fontMetrics.boundingRect(0, 0, 0, 0, Qt::TextDontClip|mTextAlignment, mText);
    QRectF textBoxRect = textRect.adjusted(-mPadding.left(), -mPadding.top(), mPadding.right(), mPadding.bottom());
    QPointF textPos = getTextDrawPoint(QPointF(0, 0), textBoxRect, mPositionAlignment); // 0, 0 because the transform does the translation
    textBoxRect.moveTopLeft(textPos.toPoint());
    QPolygonF rectPoly = transform.map(QPolygonF(textBoxRect));

    switch (anchorId)
    {
    case aiTopLeft:     return rectPoly.at(0);
    case aiTop:         return (rectPoly.at(0)+rectPoly.at(1))*0.5;
    case aiTopRight:    return rectPoly.at(1);
    case aiRight:       return (rectPoly.at(1)+rectPoly.at(2))*0.5;
    case aiBottomRight: return rectPoly.at(2);
    case aiBottom:      return (rectPoly.at(2)+rectPoly.at(3))*0.5;
    case aiBottomLeft:  return rectPoly.at(3);
    case aiLeft:        return (rectPoly.at(3)+rectPoly.at(0))*0.5;
    }

    qDebug() << Q_FUNC_INFO << "invalid anchorId" << anchorId;
    return QPointF();
}

/*! \internal
  
  Returns the point that must be given to the QPainter::drawText function (which expects the top
  left point of the text rect), according to the position \a pos, the text bounding box \a rect and
  the requested \a positionAlignment.
  
  For example, if \a positionAlignment is <tt>Qt::AlignLeft | Qt::AlignBottom</tt> the returned point
  will be shifted upward by the height of \a rect, starting from \a pos. So if the text is finally
  drawn at that point, the lower left corner of the resulting text rect is at \a pos.
*/
QPointF QCPItemText::getTextDrawPoint(const QPointF &pos, const QRectF &rect, Qt::Alignment positionAlignment) const
{
    if (positionAlignment == 0 || positionAlignment == (Qt::AlignLeft|Qt::AlignTop))
        return pos;

    QPointF result = pos; // start at top left
    if (positionAlignment.testFlag(Qt::AlignHCenter))
        result.rx() -= rect.width()/2.0;
    else if (positionAlignment.testFlag(Qt::AlignRight))
        result.rx() -= rect.width();
    if (positionAlignment.testFlag(Qt::AlignVCenter))
        result.ry() -= rect.height()/2.0;
    else if (positionAlignment.testFlag(Qt::AlignBottom))
        result.ry() -= rect.height();
    return result;
}

/*! \internal

  Returns the font that should be used for drawing text. Returns mFont when the item is not selected
  and mSelectedFont when it is.
*/
QFont QCPItemText::mainFont() const
{
    return mSelected ? mSelectedFont : mFont;
}

/*! \internal

  Returns the color that should be used for drawing text. Returns mColor when the item is not
  selected and mSelectedColor when it is.
*/
QColor QCPItemText::mainColor() const
{
    return mSelected ? mSelectedColor : mColor;
}

/*! \internal

  Returns the pen that should be used for drawing lines. Returns mPen when the item is not selected
  and mSelectedPen when it is.
*/
QPen QCPItemText::mainPen() const
{
    return mSelected ? mSelectedPen : mPen;
}

/*! \internal

  Returns the brush that should be used for drawing fills of the item. Returns mBrush when the item
  is not selected and mSelectedBrush when it is.
*/
QBrush QCPItemText::mainBrush() const
{
    return mSelected ? mSelectedBrush : mBrush;
}
/* end of 'src/items/item-text.cpp' */


/* including file 'src/items/item-ellipse.cpp', size 7863                    */
/* commit ce344b3f96a62e5f652585e55f1ae7c7883cd45b 2018-06-25 01:03:39 +0200 */

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPItemEllipse
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPItemEllipse
  \brief An ellipse

  \image html QCPItemEllipse.png "Ellipse example. Blue dotted circles are anchors, solid blue discs are positions."

  It has two positions, \a topLeft and \a bottomRight, which define the rect the ellipse will be drawn in.
*/

/*!
  Creates an ellipse item and sets default values.
  
  The created item is automatically registered with \a parentPlot. This PF_GraphicView instance takes
  ownership of the item, so do not delete it manually but use PF_GraphicView::removeItem() instead.
*/
QCPItemEllipse::QCPItemEllipse(PF_GraphicView *parentPlot) :
    QCPAbstractItem(parentPlot),
    topLeft(createPosition(QLatin1String("topLeft"))),
    bottomRight(createPosition(QLatin1String("bottomRight"))),
    topLeftRim(createAnchor(QLatin1String("topLeftRim"), aiTopLeftRim)),
    top(createAnchor(QLatin1String("top"), aiTop)),
    topRightRim(createAnchor(QLatin1String("topRightRim"), aiTopRightRim)),
    right(createAnchor(QLatin1String("right"), aiRight)),
    bottomRightRim(createAnchor(QLatin1String("bottomRightRim"), aiBottomRightRim)),
    bottom(createAnchor(QLatin1String("bottom"), aiBottom)),
    bottomLeftRim(createAnchor(QLatin1String("bottomLeftRim"), aiBottomLeftRim)),
    left(createAnchor(QLatin1String("left"), aiLeft)),
    center(createAnchor(QLatin1String("center"), aiCenter))
{
    topLeft->setCoords(0, 1);
    bottomRight->setCoords(1, 0);

    setPen(QPen(Qt::black));
    setSelectedPen(QPen(Qt::blue, 2));
    setBrush(Qt::NoBrush);
    setSelectedBrush(Qt::NoBrush);
}

QCPItemEllipse::~QCPItemEllipse()
{
}

/*!
  Sets the pen that will be used to draw the line of the ellipse
  
  \see setSelectedPen, setBrush
*/
void QCPItemEllipse::setPen(const QPen &pen)
{
    mPen = pen;
}

/*!
  Sets the pen that will be used to draw the line of the ellipse when selected
  
  \see setPen, setSelected
*/
void QCPItemEllipse::setSelectedPen(const QPen &pen)
{
    mSelectedPen = pen;
}

/*!
  Sets the brush that will be used to fill the ellipse. To disable filling, set \a brush to
  Qt::NoBrush.
  
  \see setSelectedBrush, setPen
*/
void QCPItemEllipse::setBrush(const QBrush &brush)
{
    mBrush = brush;
}

/*!
  Sets the brush that will be used to fill the ellipse when selected. To disable filling, set \a
  brush to Qt::NoBrush.
  
  \see setBrush
*/
void QCPItemEllipse::setSelectedBrush(const QBrush &brush)
{
    mSelectedBrush = brush;
}

/* inherits documentation from base class */
double QCPItemEllipse::selectTest(const QPointF &pos, bool onlySelectable, QVariant *details) const
{
    Q_UNUSED(details)
    if (onlySelectable && !mSelectable)
        return -1;

    QPointF p1 = topLeft->pixelPosition();
    QPointF p2 = bottomRight->pixelPosition();
    QPointF center((p1+p2)/2.0);
    double a = qAbs(p1.x()-p2.x())/2.0;
    double b = qAbs(p1.y()-p2.y())/2.0;
    double x = pos.x()-center.x();
    double y = pos.y()-center.y();

    // distance to border:
    double c = 1.0/qSqrt(x*x/(a*a)+y*y/(b*b));
    double result = qAbs(c-1)*qSqrt(x*x+y*y);
    // filled ellipse, allow click inside to count as hit:
    if (result > mParentPlot->selectionTolerance()*0.99 && mBrush.style() != Qt::NoBrush && mBrush.color().alpha() != 0)
    {
        if (x*x/(a*a) + y*y/(b*b) <= 1)
            result = mParentPlot->selectionTolerance()*0.99;
    }
    return result;
}

/* inherits documentation from base class */
void QCPItemEllipse::draw(QCPPainter *painter)
{
    QPointF p1 = topLeft->pixelPosition();
    QPointF p2 = bottomRight->pixelPosition();
    if (p1.toPoint() == p2.toPoint())
        return;
    QRectF ellipseRect = QRectF(p1, p2).normalized();
    QRect clip = clipRect().adjusted(-mainPen().widthF(), -mainPen().widthF(), mainPen().widthF(), mainPen().widthF());
    if (ellipseRect.intersects(clip)) // only draw if bounding rect of ellipse is visible in cliprect
    {
        painter->setPen(mainPen());
        painter->setBrush(mainBrush());
#ifdef __EXCEPTIONS
        try // drawEllipse sometimes throws exceptions if ellipse is too big
        {
#endif
            painter->drawEllipse(ellipseRect);
#ifdef __EXCEPTIONS
        } catch (...)
        {
            qDebug() << Q_FUNC_INFO << "Item too large for memory, setting invisible";
            setVisible(false);
        }
#endif
    }
}

/* inherits documentation from base class */
QPointF QCPItemEllipse::anchorPixelPosition(int anchorId) const
{
    QRectF rect = QRectF(topLeft->pixelPosition(), bottomRight->pixelPosition());
    switch (anchorId)
    {
    case aiTopLeftRim:     return rect.center()+(rect.topLeft()-rect.center())*1/qSqrt(2);
    case aiTop:            return (rect.topLeft()+rect.topRight())*0.5;
    case aiTopRightRim:    return rect.center()+(rect.topRight()-rect.center())*1/qSqrt(2);
    case aiRight:          return (rect.topRight()+rect.bottomRight())*0.5;
    case aiBottomRightRim: return rect.center()+(rect.bottomRight()-rect.center())*1/qSqrt(2);
    case aiBottom:         return (rect.bottomLeft()+rect.bottomRight())*0.5;
    case aiBottomLeftRim:  return rect.center()+(rect.bottomLeft()-rect.center())*1/qSqrt(2);
    case aiLeft:           return (rect.topLeft()+rect.bottomLeft())*0.5;
    case aiCenter:         return (rect.topLeft()+rect.bottomRight())*0.5;
    }

    qDebug() << Q_FUNC_INFO << "invalid anchorId" << anchorId;
    return QPointF();
}

/*! \internal

  Returns the pen that should be used for drawing lines. Returns mPen when the item is not selected
  and mSelectedPen when it is.
*/
QPen QCPItemEllipse::mainPen() const
{
    return mSelected ? mSelectedPen : mPen;
}

/*! \internal

  Returns the brush that should be used for drawing fills of the item. Returns mBrush when the item
  is not selected and mSelectedBrush when it is.
*/
QBrush QCPItemEllipse::mainBrush() const
{
    return mSelected ? mSelectedBrush : mBrush;
}
/* end of 'src/items/item-ellipse.cpp' */




