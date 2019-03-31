#ifndef NAVIGATIONTREEVIEW_H
#define NAVIGATIONTREEVIEW_H

#include "viewitem.h"

class NavigationTreeView : public TreeView
{
    Q_OBJECT
public:
    explicit NavigationTreeView(QWidget *parent = nullptr);
    void scrollTo(const QModelIndex &index, ScrollHint hint = EnsureVisible) override;

protected:
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
};

#endif // NAVIGATIONTREEVIEW_H
