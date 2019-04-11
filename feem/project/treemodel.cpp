#include "treemodel.h"


TreeItem::TreeItem() = default;

TreeItem::~TreeItem()
{
//    QTC_CHECK(m_parent == nullptr);
//    QTC_CHECK(m_model == nullptr);
    removeChildren();
}

TreeItem *TreeItem::childAt(int pos) const
{
    if(pos >= 0)
        ;
    else
        return nullptr;
    return pos < childCount() ? *(begin() + pos) : nullptr;
}

int TreeItem::indexOf(const TreeItem *item) const
{
    auto it = std::find(begin(), end(), item);
    return it == end() ? -1 : it - begin();
}

QVariant TreeItem::data(int column, int role) const
{
    Q_UNUSED(column);
    Q_UNUSED(role);
    return QVariant();
}

bool TreeItem::setData(int column, const QVariant &data, int role)
{
    Q_UNUSED(column);
    Q_UNUSED(data);
    Q_UNUSED(role);
    return false;
}

Qt::ItemFlags TreeItem::flags(int column) const
{
    Q_UNUSED(column);
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

//bool TreeItem::hasChildren() const
//{
//    return canFetchMore() || childCount() > 0;
//}

//bool TreeItem::canFetchMore() const
//{
//    return false;
//}

void TreeItem::prependChild(TreeItem *item)
{
    insertChild(0, item);
}

void TreeItem::appendChild(TreeItem *item)
{
    insertChild(childCount(), item);
}

void TreeItem::insertChild(int pos, TreeItem *item)
{
//    QTC_CHECK(!item->model());
//    QTC_CHECK(!item->parent());
    if(0 <= pos && pos <= childCount())
        ;
    else
        return; // '<=' is intentional.

    if (m_model) {
        QModelIndex idx = index();
        m_model->beginInsertRows(idx, pos, pos);
        item->m_parent = this;
        item->propagateModel(m_model);
        m_children.insert(m_children.begin() + pos, item);
        m_model->endInsertRows();
    } else {
        item->m_parent = this;
        m_children.insert(m_children.begin() + pos, item);
    }
}

//void TreeItem::insertOrderedChild(TreeItem *item,
//    const std::function<bool (const TreeItem *, const TreeItem *)> &cmp)
//{
//    auto where = std::lower_bound(begin(), end(), item, cmp);
//    insertChild(int(where - begin()), item);
//}

void TreeItem::removeChildAt(int pos)
{
    if(0 <= pos && pos < m_children.count())
        ;
    else
        return;

    if (m_model) {
        QModelIndex idx = index();
        m_model->beginRemoveRows(idx, pos, pos);
        removeItemAt(pos);
        m_model->endRemoveRows();
    } else {
        removeItemAt(pos);
    }
}

void TreeItem::removeChildren()
{
    if (childCount() == 0)
        return;
    if (m_model) {
        QModelIndex idx = index();
        m_model->beginRemoveRows(idx, 0, childCount() - 1);
        clear();
        m_model->endRemoveRows();
    } else {
        clear();
    }
}

//void TreeItem::sortChildren(const std::function<bool(const TreeItem *, const TreeItem *)> &cmp)
//{
//    if (m_model) {
//        if (const int n = childCount()) {
//            QVector<TreeItem *> tmp = m_children;
//            std::sort(tmp.begin(), tmp.end(), cmp);
//            if (tmp == m_children) {
//                // Nothing changed.
//            } else {
//                QModelIndex idx = index();
//                m_model->beginRemoveRows(idx, 0, n - 1);
//                m_children.clear();
//                m_model->endRemoveRows();
//                m_model->beginInsertRows(idx, 0, n - 1);
//                tmp.swap(m_children);
//                m_model->endInsertRows();
//            }
//        }
//    } else {
//        std::sort(m_children.begin(), m_children.end(), cmp);
//    }
//}

void TreeItem::update()
{
    if (m_model) {
        QModelIndex idx = index();
        m_model->dataChanged(idx.sibling(idx.row(), 0), idx.sibling(idx.row(), m_model->m_columnCount - 1));
    }
}

void TreeItem::updateAll()
{
    if (m_model) {
        QModelIndex idx = index();
        m_model->dataChanged(idx, idx.sibling(idx.row(), m_model->m_columnCount - 1));
        for (TreeItem *item : *this)
            item->updateAll();
    }
}

void TreeItem::updateColumn(int column)
{
    if (m_model) {
        QModelIndex idx = index();
        m_model->dataChanged(idx.sibling(idx.row(), column), idx.sibling(idx.row(), column));
    }
}

TreeItem *TreeItem::firstChild() const
{
    return childCount() == 0 ? nullptr : *begin();
}

TreeItem *TreeItem::lastChild() const
{
    return childCount() == 0 ? nullptr : *(end() - 1);
}

int TreeItem::level() const
{
    int l = 0;
    for (TreeItem *item = this->parent(); item; item = item->parent())
        ++l;
    return l;
}

int TreeItem::indexInParent() const
{
    return m_parent ? m_parent->indexOf(this) : -1;
}

QModelIndex TreeItem::index() const
{
    if(!m_model) return QModelIndex();
    return m_model->indexForItem(this);
}

QAbstractItemModel *TreeItem::model() const
{
    return m_model;
}

//void TreeItem::forAllChildren(const std::function<void (TreeItem *)> &pred) const
//{
//    for (TreeItem *item : *this) {
//        pred(item);
//        item->forAllChildren(pred);
//    }
//}

//void TreeItem::forSelectedChildren(const std::function<bool (TreeItem *)> &pred) const
//{
//    for (TreeItem *item : *this) {
//        if (pred(item))
//            item->forSelectedChildren(pred);
//    }
//}

//void TreeItem::forChildrenAtLevel(int level, const std::function<void(TreeItem *)> &pred) const
//{
//    QTC_ASSERT(level > 0, return);
//    if (level == 1) {
//        for (TreeItem *item : *this)
//            pred(item);
//    } else {
//        for (TreeItem *item : *this)
//            item->forChildrenAtLevel(level - 1, pred);
//    }
//}

//TreeItem *TreeItem::findChildAtLevel(int level, const std::function<bool(TreeItem *)> &pred) const
//{
//    QTC_ASSERT(level > 0, return nullptr);
//    if (level == 1) {
//        for (TreeItem *item : *this)
//            if (pred(item))
//                return item;
//    } else {
//        for (TreeItem *item : *this) {
//            if (auto found = item->findChildAtLevel(level - 1, pred))
//                return found;
//        }
//    }
//    return nullptr;
//}

TreeItem *TreeItem::findAnyChild(const std::function<bool(TreeItem *)> &pred) const
{
    for (TreeItem *item : *this) {
        if (pred(item))
            return item;
        if (TreeItem *found = item->findAnyChild(pred))
            return found;
    }
    return nullptr;
}
//TreeItem *TreeItem::reverseFindAnyChild(const std::function<bool (TreeItem *)> &pred) const
//{
//    auto end = m_children.rend();
//    for (auto it = m_children.rbegin(); it != end; ++it) {
//        if (TreeItem *found = (*it)->reverseFindAnyChild(pred))
//            return found;
//        if (pred(*it))
//            return *it;
//    }
//    return nullptr;
//}

void TreeItem::clear()
{
    while (childCount() != 0) {
        TreeItem *item = m_children.takeLast();
        item->m_model = nullptr;
        item->m_parent = nullptr;
        delete item;
    }
}

void TreeItem::removeItemAt(int pos)
{
    TreeItem *item = m_children.at(pos);
    item->m_model = nullptr;
    item->m_parent = nullptr;
    delete item;
    m_children.removeAt(pos);
}

void TreeItem::expand()
{
    if(!m_model) return;
    m_model->requestExpansion(index());
}

void TreeItem::collapse()
{
    if(!m_model) return;
    m_model->requestCollapse(index());
}

void TreeItem::propagateModel(BaseTreeModel *m)
{
    if(!m) return;
    if(m_model == nullptr || m_model == m)
        ;
    else
        return;
    if (m && !m_model) {
        m_model = m;
        for (TreeItem *item : *this)
            item->propagateModel(m);
    }
}


/*!
    \class Utils::TreeModel

    \brief The TreeModel class is a convienience base class for models
    to use in a QTreeView.
*/

BaseTreeModel::BaseTreeModel(QObject *parent)
    : QAbstractItemModel(parent),
      m_root(new TreeItem)
{
    m_columnCount = 1;
    m_root->m_model = this;
//#if USE_MODEL_TEST
//    new ModelTest(this, this);
//#endif
}

BaseTreeModel::BaseTreeModel(TreeItem *root, QObject *parent)
    : QAbstractItemModel(parent),
      m_root(root)
{
    m_columnCount = 1;
    m_root->propagateModel(this);
}

BaseTreeModel::~BaseTreeModel()
{
    if(!m_root) return;
    if(m_root->m_parent != nullptr) return;
    if(m_root->m_model != this) return;
    m_root->m_model = nullptr;
    delete m_root;
}

QModelIndex BaseTreeModel::parent(const QModelIndex &idx) const
{
//    CHECK_INDEX(idx);
    if (!idx.isValid())
        return QModelIndex();

    const TreeItem *item = itemForIndex(idx);
    if(!item) return QModelIndex();
    TreeItem *parent = item->parent();
    if (!parent || parent == m_root)
        return QModelIndex();

    const TreeItem *grandparent = parent->parent();
    if (!grandparent)
        return QModelIndex();

    // This is on the performance-critical path for ItemViewFind.
    const int i = grandparent->m_children.indexOf(parent);
    return createIndex(i, 0, static_cast<void*>(parent));
}

/*!
 \brief 返回index处项目的行和列的兄弟节点，如果该位置没有兄弟节点，
则返回无效的QModelIndex。

 \param row
 \param column
 \param idx
 \return QModelIndex
*/
QModelIndex BaseTreeModel::sibling(int row, int column, const QModelIndex &idx) const
{
    const TreeItem *item = itemForIndex(idx);
    if(!item) return QModelIndex();
    QModelIndex result;
    if (TreeItem *parent = item->parent()) {
        if (TreeItem *sibl = parent->childAt(row))
            result = createIndex(row, column, static_cast<void*>(sibl));
    }
    return result;
}

int BaseTreeModel::rowCount(const QModelIndex &idx) const
{
//    CHECK_INDEX(idx);
    if (!idx.isValid())
        return m_root->childCount();
    if (idx.column() > 0)
        return 0;
    const TreeItem *item = itemForIndex(idx);
    return item ? item->childCount() : 0;
}

int BaseTreeModel::columnCount(const QModelIndex &idx) const
{
//    CHECK_INDEX(idx);
    if (idx.column() > 0)
        return 0;
    return m_columnCount;
}

bool BaseTreeModel::setData(const QModelIndex &idx, const QVariant &data, int role)
{
    TreeItem *item = itemForIndex(idx);
    bool res = item ? item->setData(idx.column(), data, role) : false;
    if (res)
        emit dataChanged(idx, idx);
    return res;
}

/*!
 \brief 返回索引引用的项目的给定角色下存储的数据

 \param idx
 \param role
 \return QVariant
*/
QVariant BaseTreeModel::data(const QModelIndex &idx, int role) const
{
    TreeItem *item = itemForIndex(idx);
    return item ? item->data(idx.column(), role) : QVariant();
}

QVariant BaseTreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section < m_header.size())
        return m_header.at(section);
    if (role == Qt::ToolTipRole && section < m_headerToolTip.size())
        return m_headerToolTip.at(section);
    return QVariant();
}

bool BaseTreeModel::hasChildren(const QModelIndex &idx) const
{
    if (idx.column() > 0)
        return false;
    TreeItem *item = itemForIndex(idx);
    return !item || item->hasChildren();
}

Qt::ItemFlags BaseTreeModel::flags(const QModelIndex &idx) const
{
    if (!idx.isValid())
        return nullptr;
    TreeItem *item = itemForIndex(idx);
    return item ? item->flags(idx.column())
                : (Qt::ItemIsEnabled|Qt::ItemIsSelectable);
}

//bool BaseTreeModel::canFetchMore(const QModelIndex &idx) const
//{
//    if (!idx.isValid())
//        return false;
//    TreeItem *item = itemForIndex(idx);
//    return item ? item->canFetchMore() : false;
//}

//void BaseTreeModel::fetchMore(const QModelIndex &idx)
//{
//    if (!idx.isValid())
//        return;
//    TreeItem *item = itemForIndex(idx);
//    if (item)
//        item->fetchMore();
//}

TreeItem *BaseTreeModel::rootItem() const
{
    return m_root;
}

void BaseTreeModel::setRootItem(TreeItem *item)
{
    if(!item) return;
    if(item->m_model == nullptr)
        ;
    else
        return;
    if(item->m_parent == nullptr)
        ;
    else
        return;
    if(item != m_root)
        ;
    else
        return;
//    QTC_CHECK(m_root);

    beginResetModel();
    if (m_root) {
//        QTC_CHECK(m_root->m_parent == nullptr);
//        QTC_CHECK(m_root->m_model == this);
        // needs to be done explicitly before setting the model to 0, otherwise it might lead to a
        // crash inside a view or proxy model, especially if there are selected items
        m_root->removeChildren();
        m_root->m_model = nullptr;
        delete m_root;
    }
    m_root = item;
    item->propagateModel(this);
    endResetModel();
}

void BaseTreeModel::setHeader(const QStringList &displays)
{
    m_header = displays;
    m_columnCount = displays.size();
}

void BaseTreeModel::setHeaderToolTip(const QStringList &tips)
{
    m_headerToolTip = tips;
}

/*!
 \brief 模型必须实现一个index()函数，以便在访问数据时为视图和委托提供索引。
当其他组件的行号和列号及其父模型索引引用时，将为其他组件创建索引。如果将无效
模型索引指定为父级，则由模型决定是否返回与模型中的顶级项对应的索引。

当提供模型索引时，我们首先检查它是否有效。如果不是，我们假设正在引用顶级项目;
否则，我们使用internalPointer（）函数从模型索引中获取数据指针，并使用它来
引用一个TreeItem对象。请注意，我们构造的所有模型索引都将包含指向现有的模型
索引TreeItem，因此我们可以保证我们收到的任何有效模型索引都将包含有效的数据指针。

 \param row
 \param column
 \param parent
 \return QModelIndex
*/
QModelIndex BaseTreeModel::index(int row, int column, const QModelIndex &parent) const
{
//    CHECK_INDEX(parent);
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeItem *item = itemForIndex(parent);
    if(!item) return QModelIndex();
    if (row >= item->childCount())
        return QModelIndex();
    return createIndex(row, column, static_cast<void*>(item->childAt(row)));
}

TreeItem *BaseTreeModel::itemForIndex(const QModelIndex &idx) const
{
//    CHECK_INDEX(idx);
    TreeItem *item = idx.isValid() ? static_cast<TreeItem*>(idx.internalPointer()) : m_root;
    if(!item) return nullptr;
    if(item->m_model == static_cast<const BaseTreeModel *>(this))
        ;
    else
        return nullptr;
    return item;
}

QModelIndex BaseTreeModel::indexForItem(const TreeItem *item) const
{
    if(!item) return QModelIndex();
    if (item == m_root)
        return QModelIndex();

    TreeItem *p = item->parent();
    if(!p) return QModelIndex();

    auto mitem = const_cast<TreeItem *>(item);
    int row = p->indexOf(mitem);
    return createIndex(row, 0, mitem);
}

/*!
  Destroys all items in them model except the invisible root item.
*/
void BaseTreeModel::clear()
{
    if (m_root)
        m_root->removeChildren();
}

/*!
   Removes the specified item from the model.

   \note The item is not destroyed, ownership is effectively passed to the caller.
   */

TreeItem *BaseTreeModel::takeItem(TreeItem *item)
{
//#if USE_MODEL_TEST
//    (void) new ModelTest(this, this);
//#endif

    if(!item) return item;
    TreeItem *parent = item->parent();
    if(!parent) return item;
    int pos = parent->indexOf(item);
    if(pos != -1)
        ;
    else
        return item;

    QModelIndex idx = indexForItem(parent);
    beginRemoveRows(idx, pos, pos);
    item->m_parent = nullptr;
    item->m_model = nullptr;
    parent->m_children.removeAt(pos);
    endRemoveRows();
    return item;
}

void BaseTreeModel::destroyItem(TreeItem *item)
{
    delete takeItem(item);
}
