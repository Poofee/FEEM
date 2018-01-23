#ifndef PF_GRAPHICVIEW_H
#define PF_GRAPHICVIEW_H

#include <QWidget>

class PF_GraphicView : public QWidget
{
    Q_OBJECT
public:
    explicit PF_GraphicView(QWidget *parent = nullptr);

signals:

public slots:

protected:
    void paintEvent(QPaintEvent*);
};

#endif // PF_GRAPHICVIEW_H
