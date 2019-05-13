#include "pf_materiallibrary.h"

#include "navigationtreeview.h"
#include "pf_projecttree.h"
#include "pf_node.h"
#include "pf_materialtreemodel.h"

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
#include <QDebug>



class PF_MaterialTreeItemDelegate : public QStyledItemDelegate
{
public:
    PF_MaterialTreeItemDelegate(QTreeView *view)
        : QStyledItemDelegate(view)
        ,m_view(view)
    {
        connect(m_view->model(), &QAbstractItemModel::modelReset,
                this, &PF_MaterialTreeItemDelegate::deleteAllIndicators);

        // Actually this only needs to delete the indicators in the effected rows and *after* it,
        // but just be lazy and nuke all the indicators.
        connect(m_view->model(), &QAbstractItemModel::rowsAboutToBeRemoved,
                this, &PF_MaterialTreeItemDelegate::deleteAllIndicators);
        connect(m_view->model(), &QAbstractItemModel::rowsAboutToBeInserted,
                this, &PF_MaterialTreeItemDelegate::deleteAllIndicators);
    }

    ~PF_MaterialTreeItemDelegate() override
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


class PF_MaterialTreeView : public NavigationTreeView
{
public:
    PF_MaterialTreeView()
    {
        setEditTriggers(QAbstractItemView::EditKeyPressed);
        setContextMenuPolicy(Qt::CustomContextMenu);
        setDragEnabled(true);
        setDragDropMode(QAbstractItemView::DragOnly);
        //        m_context = new IContext(this);
        //        m_context->setContext(Context(ProjectExplorer::Constants::C_PROJECT_TREE));
        //        m_context->setWidget(this);

        //        ICore::addContextObject(m_context);

        //        connect(this, &PF_ProjectTreeView::expanded,
        //                this, &PF_ProjectTreeView::invalidateSize);
        //        connect(this, &PF_ProjectTreeView::collapsed,
        //                this, &PF_ProjectTreeView::invalidateSize);
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
                       this, &PF_MaterialTreeView::invalidateSize);
            disconnect(m, &QAbstractItemModel::layoutChanged,
                       this, &PF_MaterialTreeView::invalidateSize);
            disconnect(m, &QAbstractItemModel::modelReset,
                       this, &PF_MaterialTreeView::invalidateSize);
            disconnect(m, &QAbstractItemModel::rowsInserted,
                       this, &PF_MaterialTreeView::invalidateSize);
            disconnect(m, &QAbstractItemModel::rowsMoved,
                       this, &PF_MaterialTreeView::invalidateSize);
            disconnect(m, &QAbstractItemModel::rowsRemoved,
                       this, &PF_MaterialTreeView::invalidateSize);
        }
        if (newModel) {
            connect(newModel, &QAbstractItemModel::dataChanged,
                    this, &PF_MaterialTreeView::invalidateSize);
            connect(newModel, &QAbstractItemModel::layoutChanged,
                    this, &PF_MaterialTreeView::invalidateSize);
            connect(newModel, &QAbstractItemModel::modelReset,
                    this, &PF_MaterialTreeView::invalidateSize);
            connect(newModel, &QAbstractItemModel::rowsInserted,
                    this, &PF_MaterialTreeView::invalidateSize);
            connect(newModel, &QAbstractItemModel::rowsMoved,
                    this, &PF_MaterialTreeView::invalidateSize);
            connect(newModel, &QAbstractItemModel::rowsRemoved,
                    this, &PF_MaterialTreeView::invalidateSize);
        }
        NavigationTreeView::setModel(newModel);
    }

    ~PF_MaterialTreeView() override
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



PF_MaterialLibraryWidget::PF_MaterialLibraryWidget(QWidget *parent)
{
    m_model = new PF_MaterialTreeModel(this);

    m_view = new PF_MaterialTreeView;
    m_view->setModel(m_model);
    m_model->loadBuiltinMaterials();
    m_view->setItemDelegate(new PF_MaterialTreeItemDelegate(m_view));
    setFocusProxy(m_view);
    m_view->installEventFilter(this);

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(m_view);
    layout->setContentsMargins(5,5,5,5);
    this->setLayout(layout);

    /** 信号连接 **/
    connect(m_view, &QAbstractItemView::activated,
            this, &PF_MaterialLibraryWidget::openItem);
    connect(m_view->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &PF_MaterialLibraryWidget::handleCurrentItemChange);
    connect(m_view, &QWidget::customContextMenuRequested,
            this, &PF_MaterialLibraryWidget::showContextMenu);
    connect(m_view, &QTreeView::expanded,
            m_model, &PF_MaterialTreeModel::onExpanded);
    connect(m_view, &QTreeView::collapsed,
            m_model, &PF_MaterialTreeModel::onCollapsed);
}

PF_MaterialLibraryWidget::~PF_MaterialLibraryWidget()
{

}

Node *PF_MaterialLibraryWidget::currentNode()
{
    return m_model->nodeForIndex(m_view->currentIndex());
}

void PF_MaterialLibraryWidget::sync(Node *node)
{
    if (m_autoSync)
        setCurrentItem(node);
}

void PF_MaterialLibraryWidget::collapseAll()
{
    m_view->collapseAll();
}

void PF_MaterialLibraryWidget::expandAll()
{
    m_view->expandAll();
}

void PF_MaterialLibraryWidget::handleCurrentItemChange(const QModelIndex &current)
{

}

void PF_MaterialLibraryWidget::showContextMenu(const QPoint &pos)
{
    qDebug()<<Q_FUNC_INFO;
    QModelIndex index = m_view->indexAt(pos);
    Node *node = m_model->nodeForIndex(index);
}

void PF_MaterialLibraryWidget::openItem(const QModelIndex &mainIndex)
{

}

void PF_MaterialLibraryWidget::setCurrentItem(Node *node)
{

}
