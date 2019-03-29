#include "pf_centralwidget.h"

#include <QMdiArea>
#include <QVBoxLayout>

PF_CentralWidget::PF_CentralWidget(QWidget *parent)
    : QFrame(parent)
    , mdi_area(new QMdiArea(this))
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(mdi_area);

    mdi_area->setObjectName("mdi_area");
    mdi_area->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdi_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdi_area->setFocusPolicy(Qt::ClickFocus);
    mdi_area->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mdi_area->setActivationOrder(QMdiArea::ActivationHistoryOrder);
    mdi_area->setTabsMovable(true);
    mdi_area->setTabsClosable(true);

    setLayout(layout);
}

QMdiArea* PF_CentralWidget::getMdiArea(){
    return mdi_area;
}
