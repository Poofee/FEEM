#ifndef PF_MDISUBWINDOW_H
#define PF_MDISUBWINDOW_H

#include <QMdiSubWindow>

class PF_GraphicView;
class QMdiArea;

class PF_MdiSubWindow : public QMdiSubWindow
{
    Q_OBJECT
public:
    PF_MdiSubWindow(QWidget* parent);
    ~PF_MdiSubWindow();

private:
    PF_GraphicView* graphicView;
    QMdiArea* cadMdiArea;
};

#endif // PF_MDISUBWINDOW_H
