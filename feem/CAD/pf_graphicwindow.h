#ifndef PF_MDISUBWINDOW_H
#define PF_MDISUBWINDOW_H

#include <QMdiSubWindow>

class PF_GraphicView;
class QMdiArea;
class PF_Document;

class PF_MdiSubWindow : public QMdiSubWindow
{
    Q_OBJECT
public:
    PF_MdiSubWindow(PF_Document *doc, QWidget* parent);
    ~PF_MdiSubWindow();

    PF_GraphicView* getGraphicView() const;
    PF_Document* getDocument() const;

private:
    PF_GraphicView* graphicView;
    PF_Document* document;
    QMdiArea* cadMdiArea;
};

#endif // PF_MDISUBWINDOW_H
