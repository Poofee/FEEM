/****************************************************************************
**
** Qtitan Library by Developer Machines (Microsoft-Ribbon implementation for Qt.C++)
** 
** Copyright (c) 2009-2014 Developer Machines (http://www.devmachines.com)
**           ALL RIGHTS RESERVED
** 
**  The entire contents of this file is protected by copyright law and
**  international treaties. Unauthorized reproduction, reverse-engineering
**  and distribution of all or any portion of the code contained in this
**  file is strictly prohibited and may result in severe civil and 
**  criminal penalties and will be prosecuted to the maximum extent 
**  possible under the law.
**
**  RESTRICTIONS
**
**  THE SOURCE CODE CONTAINED WITHIN THIS FILE AND ALL RELATED
**  FILES OR ANY PORTION OF ITS CONTENTS SHALL AT NO TIME BE
**  COPIED, TRANSFERRED, SOLD, DISTRIBUTED, OR OTHERWISE MADE
**  AVAILABLE TO OTHER INDIVIDUALS WITHOUT WRITTEN CONSENT
**  AND PERMISSION FROM DEVELOPER MACHINES
**
**  CONSULT THE END USER LICENSE AGREEMENT FOR INFORMATION ON
**  ADDITIONAL RESTRICTIONS.
**
****************************************************************************/
#include <QApplication>
#include <QStyleOption>
#include <QResizeEvent>
#include <QDesktopWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QPainter>
#include <QLayout>
#include <QEvent>
#include <QLabel>

#include "QtnRibbonPrivate.h"
#include "QtnStyleHelpers.h"
#include "QtnRibbonGroupPrivate.h"
#include "QtnRibbonGroup.h"
#include "QtnRibbonButton.h"
#include "QtnRibbonStyle.h"
#include "QtnRibbonPage.h"

using namespace Qtitan;

static const int margin = 4;

static int keyTipEventNumber = -1;
static int showKeyTipEventNumber = -1;
static int hideKeyTipEventNumber = -1;
static int minimizedEventNumber = -1;

namespace Qtitan
{
    /* ExWidgetWrapperPrivate */
    class ExWidgetWrapperPrivate : public QObject
    {
    public:
        QTN_DECLARE_PUBLIC(ExWidgetWrapper)
    public:
        class IconLabel : public QLabel
        {
        public:
            IconLabel() { m_height = 0; }
            void setIcon(const QIcon& icon) 
            { 
                m_icon = icon; 
                updateIcon(16);
            }
        protected:
            void updateIcon(int height)
            {
                if (!m_icon.isNull())
                {
                    if (m_height != height)
                    {
                        QPixmap px = m_icon.pixmap(QSize(height, height));
                        if (!px.isNull())
                            setPixmap(px);
                        m_height = height;
                    }
                }
            }
            virtual void resizeEvent(QResizeEvent* event)
            {
                updateIcon(event->size().height());
                QLabel::resizeEvent(event);
            }
        protected:
            QIcon m_icon;
            int m_height;
        };

        /* HBoxLayout */
        class HBoxLayout : public QHBoxLayout
        {
        public:
            explicit HBoxLayout() : QHBoxLayout() { m_widthFix = -1; }
            void setWidthFix(int width) { m_widthFix = width; }
        public:
            virtual QSize sizeHint() const 
            {
                QSize sz = QHBoxLayout::sizeHint();
                if (m_widthFix != -1)
                    sz.setWidth(m_widthFix);
                return sz;
            }
        protected:
            int m_widthFix;
        };
    public:
        explicit ExWidgetWrapperPrivate();

    public:
        void init(QWidget* widget);

    public:
        QWidget* m_buddy;
        QLabel* m_textLabel;
        IconLabel* m_iconLabel;
        HBoxLayout* m_iconTextlayout;
        QHBoxLayout* m_layout;
        bool m_align;
    };
}; //namespace Qtitan

/* ExWidgetWrapperPrivate */
ExWidgetWrapperPrivate::ExWidgetWrapperPrivate()
    : m_buddy(Q_NULL)
    , m_textLabel(Q_NULL)
    , m_iconLabel(Q_NULL)
    , m_iconTextlayout(Q_NULL)
    , m_layout(Q_NULL)
    , m_align(false)
{
}

void ExWidgetWrapperPrivate::init(QWidget* widget)
{
    Q_ASSERT(widget != Q_NULL);
    QTN_P(ExWidgetWrapper);

    m_buddy = widget;
    m_layout = new QHBoxLayout();
    m_layout->setMargin(0);
    m_layout->setSpacing(margin);

    m_iconTextlayout = new HBoxLayout();
    m_iconTextlayout->setMargin(0);
    m_iconTextlayout->setSpacing(margin);
    m_layout->addLayout(m_iconTextlayout);

    m_buddy->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_layout->addWidget(m_buddy/*, 0, Qt::AlignRight|Qt::AlignVCenter*/);
    p.setLayout(m_layout);

    p.setFocusPolicy(m_buddy->focusPolicy());
    p.setAttribute(Qt::WA_InputMethodEnabled);
//    p.setSizePolicy(m_buddy->sizePolicy());
    p.setMouseTracking(true);
    p.setAcceptDrops(true);
    p.setAttribute(Qt::WA_MacShowFocusRect, true);

    m_buddy->setFocusProxy(&p);
    m_buddy->setAttribute(Qt::WA_MacShowFocusRect, false);

    p.setToolTip(m_buddy->toolTip());
    m_buddy->setToolTip("");
}

/*!
\class Qtitan::ExWidgetWrapper
\internal
*/
ExWidgetWrapper::ExWidgetWrapper(QWidget* parent, QWidget* widget)
    : QWidget(parent)
{
    QTN_INIT_PRIVATE(ExWidgetWrapper);
    QTN_D(ExWidgetWrapper);
    d.init(widget);
}

ExWidgetWrapper::~ExWidgetWrapper()
{
    QTN_FINI_PRIVATE();
}

QString ExWidgetWrapper::text() const
{
    QTN_D(const ExWidgetWrapper);
    return d.m_textLabel ? d.m_textLabel->text() : QString();
}

void ExWidgetWrapper::setText(const QString& text)
{
    QTN_D(ExWidgetWrapper);

    if (!text.isEmpty())
    {
        d.m_textLabel = new QLabel(text);
        d.m_textLabel->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
        d.m_textLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        d.m_iconTextlayout->addWidget(d.m_textLabel, 1);
    }
    else
    {
        if (d.m_textLabel)
            d.m_iconTextlayout->removeWidget(d.m_textLabel);
        delete d.m_textLabel;
        d.m_textLabel= Q_NULL;
    }
}

void ExWidgetWrapper::setIcon(const QIcon& icon)
{
    QTN_D(ExWidgetWrapper);
    if (!icon.isNull())
    {
        d.m_iconLabel = new ExWidgetWrapperPrivate::IconLabel();
        d.m_iconLabel->setIcon(icon);
        d.m_iconLabel->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
        d.m_iconLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        d.m_iconTextlayout->insertWidget(0, d.m_iconLabel);
    }
    else
    {
        if (d.m_iconLabel)
            d.m_iconTextlayout->removeWidget(d.m_iconLabel);
        delete d.m_iconLabel;
        d.m_iconLabel = Q_NULL;
    }
}

QIcon ExWidgetWrapper::icon() const
{
    QTN_D(const ExWidgetWrapper);
    return d.m_iconLabel ? QIcon(*d.m_iconLabel->pixmap()) : QIcon();
}

QPixmap ExWidgetWrapper::pixmap() const
{
    QTN_D(const ExWidgetWrapper);
    return d.m_iconLabel ? *d.m_iconLabel->pixmap() : QPixmap();
}

void ExWidgetWrapper::setAlignWidget(bool align)
{
    QTN_D(ExWidgetWrapper);
    d.m_align = align;
}

bool ExWidgetWrapper::alignWidget() const
{
    QTN_D(const ExWidgetWrapper);
    return d.m_align;
}

void ExWidgetWrapper::setLengthLabel(int lengthTotal)
{
    QTN_D(ExWidgetWrapper);
    d.m_iconTextlayout->setWidthFix(lengthTotal);
}

int ExWidgetWrapper::sizeHintLabels() const
{
    QTN_D(const ExWidgetWrapper);
    return d.m_iconTextlayout->QHBoxLayout::sizeHint().width();
}

QWidget* ExWidgetWrapper::buddy() const
{
    QTN_D(const ExWidgetWrapper);
    return d.m_buddy;
}

bool ExWidgetWrapper::isReadOnly() const
{
    QTN_D(const ExWidgetWrapper);

    bool readOnly = false;
    if (QLineEdit* pEdit = qobject_cast<QLineEdit*>(d.m_buddy))
        readOnly = pEdit->isReadOnly();
    else if (QComboBox* pComboBox = qobject_cast<QComboBox*>(d.m_buddy))
        readOnly = pComboBox->isEditable();
    else if (QAbstractSpinBox* pSpinBox = qobject_cast<QAbstractSpinBox*>(d.m_buddy))
        readOnly = pSpinBox->isReadOnly();

    return readOnly;
}

void ExWidgetWrapper::focusInEvent(QFocusEvent* event)
{
    QTN_D(ExWidgetWrapper);
    QCoreApplication::sendEvent(d.m_buddy, event);
    QWidget::focusInEvent(event);
}

void ExWidgetWrapper::focusOutEvent(QFocusEvent* event)
{
    QTN_D(ExWidgetWrapper);
    QCoreApplication::sendEvent(d.m_buddy, event);
    QWidget::focusOutEvent(event);
}

bool ExWidgetWrapper::event(QEvent* event)
{
    QTN_D(ExWidgetWrapper);

    if (!d.m_buddy)
        return false;

    switch(event->type()) 
    {
        case QEvent::ShortcutOverride:
        case QEvent::KeyPress :
            {
                class QtnWidget : public QWidget { friend class Qtitan::ExWidgetWrapper; };
                return ((QtnWidget*)d.m_buddy)->event(event);
            }
        default:
            break;
    }
    return QWidget::event(event);
}

QVariant ExWidgetWrapper::inputMethodQuery(Qt::InputMethodQuery property) const
{
    QTN_D(const ExWidgetWrapper);
    return d.m_buddy->inputMethodQuery(property);
}

void ExWidgetWrapper::inputMethodEvent(QInputMethodEvent* event)
{
    QTN_D(const ExWidgetWrapper);
    QCoreApplication::sendEvent(d.m_buddy, (QEvent*)event);
}



/*!
\class Qtitan::RibbonDefaultGroupButton
\internal
*/
RibbonDefaultGroupButton::RibbonDefaultGroupButton(QWidget* parent, RibbonGroup* group)
    : QToolButton(parent)
{
    m_ribbonGroup = group;
    m_eventLoop = Q_NULL;
    m_hasPopup = false;

    m_ribbonGroup->adjustSize();

    ensurePolished();

    setAttribute(Qt::WA_LayoutUsesWidgetRect);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
    setPopupMode(QToolButton::MenuButtonPopup);
    setText(m_ribbonGroup->title());
}

RibbonDefaultGroupButton::~RibbonDefaultGroupButton()
{
    if (m_eventLoop)
        m_eventLoop->exit();
}

bool RibbonDefaultGroupButton::isShowPopup() const
{
    return m_hasPopup;
}

void RibbonDefaultGroupButton::setVisible(bool visible)
{
    QToolButton::setVisible(visible);
}

void RibbonDefaultGroupButton::resetReducedGroup()
{
    if (m_ribbonGroup->qtn_d().m_reduced)
    {
        m_ribbonGroup->qtn_d().m_reduced = false;
        m_ribbonGroup->show();
    }
}

void RibbonDefaultGroupButton::resetPopopGroup()
{
    if (m_eventLoop)
        m_eventLoop->exit(); 
//    m_hasPopup = false;
    repaint();
}

QSize RibbonDefaultGroupButton::sizeHint() const
{
    QStyleOptionToolButton opt;
    initStyleOption(&opt);
    return QSize(style()->pixelMetric((QStyle::PixelMetric)RibbonStyle::PM_RibbonReducedGroupWidth, &opt, this), m_ribbonGroup->sizeHint().height());
}

void RibbonDefaultGroupButton::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter p(this );
    QStyleOptionToolButton opt;
    initStyleOption(&opt);
    if (m_hasPopup)
        opt.features |= QStyleOptionToolButton::HasMenu;
    QRect rcGroup = opt.rect;
    rcGroup.setBottom(rcGroup.bottom() - 1);
    opt.rect = rcGroup;
    style()->drawControl((QStyle::ControlElement)RibbonStyle::CE_ReducedGroup, &opt, &p, this);
}

void RibbonDefaultGroupButton::mousePressEvent(QMouseEvent* event)
{
    if (!m_ribbonGroup)
        return;

    if (m_ribbonGroup->qtn_d().m_reduced /*&& !m_hasPopup*/ )
    {
        QStyleOptionToolButton opt;
        initStyleOption(&opt);
        if (event->button() == Qt::LeftButton && popupMode() == QToolButton::MenuButtonPopup) 
        {
            QRect popupr = style()->subControlRect(QStyle::CC_ToolButton, &opt, QStyle::SC_ToolButtonMenu, this);
            if (popupr.isValid() && popupr.contains(event->pos())) 
            {
                QPoint pnt(0, 0);
                QRect rc = rect();

                QRect screen = QApplication::desktop()->availableGeometry(this);
                int h = rc.height();
                if (this->mapToGlobal(QPoint(0, rc.bottom())).y() + h <= screen.height()) 
                    pnt = this->mapToGlobal(rc.bottomLeft());
                else 
                    pnt = this->mapToGlobal(rc.topLeft() - QPoint(0, h));

                QSize size = m_ribbonGroup->sizeHint();
                const int desktopFrame = style()->pixelMetric(QStyle::PM_MenuDesktopFrameWidth, 0, this);

                if (pnt.x() + size.width() - 1 > screen.right() - desktopFrame)
                    pnt.setX(screen.right() - desktopFrame - size.width() + 1);
                if (pnt.x() < screen.left() + desktopFrame)
                    pnt.setX(screen.left() + desktopFrame);

                m_hasPopup = true;
                m_ribbonGroup->setGeometry(QRect(pnt, size));
                m_ribbonGroup->show();
                repaint();
    
                QObject::connect(m_ribbonGroup, SIGNAL(hidePopup()), this, SLOT(resetPopopGroup()));
                QEventLoop eventLoop;
                m_eventLoop = &eventLoop;
                QPointer<QObject> guard = m_ribbonGroup;
                (void) eventLoop.exec();
                if (guard.isNull())
                    return;
                
                m_hasPopup = false;
                QObject::disconnect(m_ribbonGroup, SIGNAL(hidePopup()), this, SLOT(resetPopopGroup()));
                m_eventLoop = Q_NULL;
                return;
            }
        }
    }
}

void RibbonDefaultGroupButton::mouseReleaseEvent(QMouseEvent* event)
{
    m_hasPopup = false;
    QToolButton::mouseReleaseEvent(event);
}


/*!
\class Qtitan::RibbonGroupOption
\internal
*/
RibbonGroupOption::RibbonGroupOption(QWidget* parent)
    : QToolButton(parent)
{
}

RibbonGroupOption::~RibbonGroupOption()
{
}

QString RibbonGroupOption::text() const
{
    return "";
}

QSize RibbonGroupOption::sizeHint() const
{
    return QSize(15, 14);
}

void RibbonGroupOption::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter p(this);
    QStyleOptionToolButton opt;
    initStyleOption(&opt);
    opt.iconSize = opt.icon.actualSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
    style()->drawPrimitive((QStyle::PrimitiveElement)RibbonStyle::PE_RibbonOptionButton, &opt, &p, this);
}

void RibbonGroupOption::actionEvent(QActionEvent* event)
{
    QToolButton::actionEvent(event);
    if (event->type() == QEvent::ActionChanged)
    {
        QAction* action = event->action();
        setPopupMode(action->menu() ? QToolButton::MenuButtonPopup : QToolButton::DelayedPopup);
    }
}


/*!
\class Qtitan::RibbonGroupScroll
\internal
*/
RibbonGroupScroll::RibbonGroupScroll(QWidget* parent, bool scrollLeft)
    : QToolButton(parent)
{
    m_scrollLeft = scrollLeft;
}

RibbonGroupScroll::~RibbonGroupScroll()
{
}

void RibbonGroupScroll::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter p(this);
    QStyleOptionToolButton opt;
    initStyleOption(&opt);
    opt.arrowType = m_scrollLeft ? Qt::LeftArrow : Qt::RightArrow;
    style()->drawPrimitive((QStyle::PrimitiveElement)RibbonStyle::PE_RibbonGroupScrollButton, &opt, &p, this);
}


/*!
\class Qtitan::KeyTipEvent
\internal
*/
KeyTipEvent::KeyTipEvent(RibbonKeyTip* kTip) 
    : QEvent(KeyTipEvent::eventNumber())
{
    keyTip = kTip;
}

RibbonKeyTip* KeyTipEvent::getKeyTip() const
{
    return keyTip;
}

QEvent::Type KeyTipEvent::eventNumber()
{
    if (keyTipEventNumber < 0)
        keyTipEventNumber = QEvent::registerEventType();
    return (QEvent::Type) keyTipEventNumber;
}

/*!
\class Qtitan::ShowKeyTipEvent
\internal
*/
ShowKeyTipEvent::ShowKeyTipEvent(QWidget* w) 
    : QEvent(ShowKeyTipEvent::eventNumber())
{
    m_buddy = w;
}

QWidget* ShowKeyTipEvent::buddy() const
{
    return m_buddy;
}

QEvent::Type ShowKeyTipEvent::eventNumber()
{
    if (showKeyTipEventNumber < 0)
        showKeyTipEventNumber = QEvent::registerEventType();
    return (QEvent::Type) showKeyTipEventNumber;
}

/*!
\class Qtitan::HideKeyTipEvent
\internal
*/
HideKeyTipEvent::HideKeyTipEvent() 
    : QEvent(HideKeyTipEvent::eventNumber())
{
}

QEvent::Type HideKeyTipEvent::eventNumber()
{
    if (hideKeyTipEventNumber < 0)
        hideKeyTipEventNumber = QEvent::registerEventType();
    return (QEvent::Type) hideKeyTipEventNumber;
}

/*!
\class Qtitan::HideKeyTipEvent
\internal
*/
MinimizedEvent::MinimizedEvent() 
    : QEvent(MinimizedEvent::eventNumber())
{
}

QEvent::Type MinimizedEvent::eventNumber()
{
    if (minimizedEventNumber < 0)
        minimizedEventNumber = QEvent::registerEventType();
    return (QEvent::Type) minimizedEventNumber;
}
