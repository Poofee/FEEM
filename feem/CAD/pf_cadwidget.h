#ifndef PF_CADWIDGET_H
#define PF_CADWIDGET_H

#include <QWidget>

class QToolButton;
class PF_GraphicView;
class QToolBar;
class QAction;

class QStatusBar;

class PF_Document;
/*!
 \brief 包含坐标轴和上方的按钮。

*/
class PF_CADWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PF_CADWidget(PF_Document* doc, QWidget *parent = nullptr);
    ~PF_CADWidget();

    void init();
    PF_GraphicView* getGraphicView();
    PF_Document* getDocument();

    void exportGmshGeo();

    void paintEvent(QPaintEvent *e);

    static QStatusBar* statusbar;

private:
    QWidget* toolBar;
    QToolButton* zoom;
    QToolButton* zoomextents;
    QToolButton* zoomin;
    QToolButton* zoomout;
    QToolButton* zoomselected;

    PF_GraphicView* view;
    PF_Document* document;
signals:

public slots:
};

#endif // PF_CADWIDGET_H
