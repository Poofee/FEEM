#include "pf_projecttreewidget.h"

#include "navigationtreeview.h"
#include "pf_projectmodel.h"
#include "pf_projecttree.h"

#include <QApplication>
#include <QSettings>

#include <QStyledItemDelegate>
#include <QAbstractItemView>
#include <QVBoxLayout>
#include <QToolButton>
#include <QPainter>
#include <QAction>
#include <QLineEdit>
#include <QMenu>



class PF_ProjectTreeItemDelegate : public QStyledItemDelegate
{
public:
    PF_ProjectTreeItemDelegate(QTreeView *view)
        : QStyledItemDelegate(view)
        ,m_view(view)
    {
        connect(m_view->model(), &QAbstractItemModel::modelReset,
                this, &PF_ProjectTreeItemDelegate::deleteAllIndicators);

        // Actually this only needs to delete the indicators in the effected rows and *after* it,
        // but just be lazy and nuke all the indicators.
        connect(m_view->model(), &QAbstractItemModel::rowsAboutToBeRemoved,
                this, &PF_ProjectTreeItemDelegate::deleteAllIndicators);
        connect(m_view->model(), &QAbstractItemModel::rowsAboutToBeInserted,
                this, &PF_ProjectTreeItemDelegate::deleteAllIndicators);
    }

    ~PF_ProjectTreeItemDelegate() override
    {
        deleteAllIndicators();
    }

//    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override
//    {
//        QStyledItemDelegate::paint(painter, option, index);

//        if (index.data(Project::isParsingRole).toBool()) {
//            QStyleOptionViewItem opt = option;
//            initStyleOption(&opt, index);
//            ProgressIndicatorPainter *indicator = findOrCreateIndicatorPainter(index);

//            QStyle *style = option.widget ? option.widget->style() : QApplication::style();
//            const QRect rect = style->subElementRect(QStyle::SE_ItemViewItemDecoration, &opt, opt.widget);

//            indicator->paint(*painter, rect);
//        } else {
//            delete m_indicators.value(index);
//            m_indicators.remove(index);
//        }
//    }

private:
//    ProgressIndicatorPainter *findOrCreateIndicatorPainter(const QModelIndex &index) const
//    {
//        ProgressIndicatorPainter *indicator = m_indicators.value(index);
//        if (!indicator)
//            indicator = createIndicatorPainter(index);
//        return indicator;
//    }

//    ProgressIndicatorPainter *createIndicatorPainter(const QModelIndex &index) const
//    {
//        auto indicator = new ProgressIndicatorPainter(ProgressIndicatorSize::Small);
//        indicator->setUpdateCallback([index, this]() { m_view->update(index); });
//        indicator->startAnimation();
//        m_indicators.insert(index, indicator);
//        return indicator;
//    }

    void deleteAllIndicators()
    {
//        qDeleteAll(m_indicators);
//        m_indicators.clear();
    }

//    mutable QHash<QModelIndex, ProgressIndicatorPainter *> m_indicators;
    QTreeView *m_view;
};


class PF_ProjectTreeView : public NavigationTreeView
{
public:
    PF_ProjectTreeView()
    {
        setEditTriggers(QAbstractItemView::EditKeyPressed);
        setContextMenuPolicy(Qt::CustomContextMenu);
        setDragEnabled(true);
        setDragDropMode(QAbstractItemView::DragOnly);
        //        m_context = new IContext(this);
        //        m_context->setContext(Context(ProjectExplorer::Constants::C_PROJECT_TREE));
        //        m_context->setWidget(this);

        //        ICore::addContextObject(m_context);

        connect(this, &PF_ProjectTreeView::expanded,
                this, &PF_ProjectTreeView::invalidateSize);
        connect(this, &PF_ProjectTreeView::collapsed,
                this, &PF_ProjectTreeView::invalidateSize);
    }

    void invalidateSize()
    {
        m_cachedSize = -1;
    }

    void setModel(QAbstractItemModel *newModel) override
    {
        // Note: Don't connect to column signals, as we have only one column
        if (model()) {
            QAbstractItemModel *m = model();
            disconnect(m, &QAbstractItemModel::dataChanged,
                       this, &PF_ProjectTreeView::invalidateSize);
            disconnect(m, &QAbstractItemModel::layoutChanged,
                       this, &PF_ProjectTreeView::invalidateSize);
            disconnect(m, &QAbstractItemModel::modelReset,
                       this, &PF_ProjectTreeView::invalidateSize);
            disconnect(m, &QAbstractItemModel::rowsInserted,
                       this, &PF_ProjectTreeView::invalidateSize);
            disconnect(m, &QAbstractItemModel::rowsMoved,
                       this, &PF_ProjectTreeView::invalidateSize);
            disconnect(m, &QAbstractItemModel::rowsRemoved,
                       this, &PF_ProjectTreeView::invalidateSize);
        }
        if (newModel) {
            connect(newModel, &QAbstractItemModel::dataChanged,
                    this, &PF_ProjectTreeView::invalidateSize);
            connect(newModel, &QAbstractItemModel::layoutChanged,
                    this, &PF_ProjectTreeView::invalidateSize);
            connect(newModel, &QAbstractItemModel::modelReset,
                    this, &PF_ProjectTreeView::invalidateSize);
            connect(newModel, &QAbstractItemModel::rowsInserted,
                    this, &PF_ProjectTreeView::invalidateSize);
            connect(newModel, &QAbstractItemModel::rowsMoved,
                    this, &PF_ProjectTreeView::invalidateSize);
            connect(newModel, &QAbstractItemModel::rowsRemoved,
                    this, &PF_ProjectTreeView::invalidateSize);
        }
        NavigationTreeView::setModel(newModel);
    }

    ~PF_ProjectTreeView() override
    {
        //        ICore::removeContextObject(m_context);
        //        delete m_context;
    }

    int sizeHintForColumn(int column) const override
    {
        if (m_cachedSize < 0)
            m_cachedSize = NavigationTreeView::sizeHintForColumn(column);

        return m_cachedSize;
    }

private:
    mutable int m_cachedSize = -1;
    //    IContext *m_context;
};
/**
 * @brief 以树的形式保存一个工程
 *
 * @param parent
 */
PF_ProjectTreeWidget::PF_ProjectTreeWidget(QWidget *parent) : QWidget(parent)
{
    m_model = new PF_ProjectModel(this);
    m_view = new PF_ProjectTreeView;
    m_view->setModel(m_model);
    m_view->setItemDelegate(new PF_ProjectTreeItemDelegate(m_view));
    setFocusProxy(m_view);
    m_view->installEventFilter(this);

    /** 信号连接 **/
//    connect(m_model, &PF_ProjectModel::renamed,
//            this, &PF_ProjectTreeWidget::renamed);
//    connect(m_model, &PF_ProjectModel::requestExpansion,
//            m_view, &QTreeView::expand);
    connect(m_view, &QAbstractItemView::activated,
            this, &PF_ProjectTreeWidget::openItem);
    connect(m_view->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &PF_ProjectTreeWidget::handleCurrentItemChange);
    connect(m_view, &QWidget::customContextMenuRequested,
            this, &PF_ProjectTreeWidget::showContextMenu);
    connect(m_view, &QTreeView::expanded,
            m_model, &PF_ProjectModel::onExpanded);
    connect(m_view, &QTreeView::collapsed,
            m_model, &PF_ProjectModel::onCollapsed);

}

PF_ProjectTreeWidget::~PF_ProjectTreeWidget()
{

}

/*!
 \brief 当前节点

 \return Node
*/
Node *PF_ProjectTreeWidget::currentNode()
{
    return m_model->nodeForIndex(m_view->currentIndex());
}

void PF_ProjectTreeWidget::collapseAll()
{
    m_view->collapseAll();
}

/*!
 \brief 处理选中的node的变化。

 \param current
*/
void PF_ProjectTreeWidget::handleCurrentItemChange(const QModelIndex &current)
{

}

/*!
 \brief 根据不同的node，显示不同的右键菜单。

 \param pos
*/
void PF_ProjectTreeWidget::showContextMenu(const QPoint &pos)
{
    QModelIndex index = m_view->indexAt(pos);
    Node *node = m_model->nodeForIndex(index);
    PF_ProjectTree::showContextMenu(this, m_view->mapToGlobal(pos), node);
}

/*!
 \brief node选中激活之后的动作。

 \param mainIndex
*/
void PF_ProjectTreeWidget::openItem(const QModelIndex &mainIndex)
{
    Node *node = m_model->nodeForIndex(mainIndex);
}
