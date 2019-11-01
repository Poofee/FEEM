#include "pf_modelwidget.h"

#include "pf_projecttreewidget.h"
#include "pf_projecttree.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QToolButton>
#include <QDebug>

PF_ModelWidget::PF_ModelWidget(QWidget* parent)
    : QWidget (parent)
    ,m_toolbar(new QWidget(this))
    ,tb_previousNode(new QToolButton(this))
    ,tb_nextNode(new QToolButton(this))
    ,tb_moveup(new QToolButton(this))
    ,tb_movedown(new QToolButton(this))
    ,tb_show(new QToolButton(this))
    ,tb_collapseAll(new QToolButton(this))
    ,tb_expandAll(new QToolButton(this))

{
    init();
}

PF_ModelWidget::~PF_ModelWidget()
{

}

void PF_ModelWidget::init()
{
    m_modelTreeWidget = new PF_ProjectTreeWidget(this);

    QHBoxLayout* toolbarLayout = new QHBoxLayout(this);
    toolbarLayout->setMargin(0);
    toolbarLayout->setSpacing(1);

    /** previous Node QToolbutton **/
    tb_previousNode->setIcon(QIcon(":/tree/report_previous.png"));
    tb_previousNode->setToolTip(tr("Previous Node"));
    tb_previousNode->setAutoRaise(true);

    tb_nextNode->setIcon(QIcon(":/tree/report_next.png"));
    tb_nextNode->setToolTip(tr("Next Node"));
    tb_nextNode->setAutoRaise(true);

    tb_moveup->setIcon(QIcon(":/tree/report_up.png"));
    tb_moveup->setToolTip(tr("Move Up"));
    tb_moveup->setAutoRaise(true);

    tb_movedown->setIcon(QIcon(":/tree/report_down.png"));
    tb_movedown->setToolTip(tr("Move Down"));
    tb_movedown->setAutoRaise(true);

    tb_show->setIcon(QIcon(":/tree/node_labels.png"));
    tb_show->setToolTip(tr("Show"));
    tb_show->setAutoRaise(true);

    tb_collapseAll->setIcon(QIcon(":/tree/hide_tree.png"));
    tb_collapseAll->setToolTip(tr("Collapse All"));
    tb_collapseAll->setAutoRaise(true);
    connect(tb_collapseAll,&QToolButton::clicked,[]()
    {
//        qDebug()<<"Collapse All";
        PF_ProjectTree::instance()->collapseAll();
    });

    tb_expandAll->setIcon(QIcon(":/tree/show_tree.png"));
    tb_expandAll->setToolTip(tr("Expand All"));
    tb_expandAll->setAutoRaise(true);
    connect(tb_expandAll,&QToolButton::clicked,[]()
    {
//        qDebug()<<"Expand All";
        PF_ProjectTree::instance()->expandAll();
    });

    toolbarLayout->addWidget(tb_previousNode);
    toolbarLayout->addWidget(tb_nextNode);
    toolbarLayout->addWidget(tb_moveup);
    toolbarLayout->addWidget(tb_movedown);
    toolbarLayout->addWidget(tb_show);
    toolbarLayout->addWidget(tb_collapseAll);
    toolbarLayout->addWidget(tb_expandAll);
    toolbarLayout->addStretch(1);

    m_toolbar->setLayout(toolbarLayout);

    QVBoxLayout* modellayout = new QVBoxLayout(this);
    modellayout->setMargin(5);
    modellayout->setSpacing(5);
    modellayout->addWidget(m_toolbar);
    modellayout->addWidget(m_modelTreeWidget);

    setLayout(modellayout);
}
