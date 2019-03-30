#ifndef VIEWITEM_H
#define VIEWITEM_H

#include <QTreeView>
#include <QKeyEvent>

static const char activationModeC[] = "ActivationMode";

enum ActivationMode {
    DoubleClickActivation = 0,
    SingleClickActivation = 1,
    PlatformDefaultActivation = 2
};

class View : public QTreeView
{
public:
    View(QWidget* parent = nullptr)
        :QTreeView(parent)
    {}

    void setActivationMode(ActivationMode mode)
    {
        if (mode == PlatformDefaultActivation)
            QTreeView::setProperty(activationModeC, QVariant());
        else
            QTreeView::setProperty(activationModeC, QVariant(bool(mode)));
    }

    ActivationMode activationMode() const
    {
        QVariant v = QTreeView::property(activationModeC);
        if (!v.isValid())
            return PlatformDefaultActivation;
        return v.toBool() ? SingleClickActivation : DoubleClickActivation;
    }

    void keyPressEvent(QKeyEvent *event) override
    {
        // Note: This always eats the event
        // whereas QAbstractItemView never eats it
        if ((event->key() == Qt::Key_Return
             || event->key() == Qt::Key_Enter)
                && event->modifiers() == 0
                && QTreeView::currentIndex().isValid()
                && QTreeView::state() != QAbstractItemView::EditingState) {
            emit QTreeView::activated(QTreeView::currentIndex());
            return;
        }
        QTreeView::keyPressEvent(event);
    }
};

class TreeView : public View
{
    Q_OBJECT
public:
    TreeView(QWidget *parent = nullptr)
        : View(parent)
    {}
};
#endif // VIEWITEM_H
