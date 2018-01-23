#ifndef PF_CENTRALWIDGET_H
#define PF_CENTRALWIDGET_H

#include <QFrame>

class QMdiArea;

class PF_CentralWidget : public QFrame
{
    Q_OBJECT
public:
    PF_CentralWidget(QWidget *parent = nullptr);
    QMdiArea* getMdiArea();

protected:
    QMdiArea* mdi_area;
};

#endif // PF_CENTRALWIDGET_H
