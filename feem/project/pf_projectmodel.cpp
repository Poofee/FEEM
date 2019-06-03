#include "pf_projectmodel.h"

#include "pf_project.h"
#include "pf_projecttree.h"
#include "pf_sessionmanager.h"

#include "pf_node.h"

#include <QDebug>
#include <functional>

PF_ProjectModel::PF_ProjectModel(QObject *parent)
    : BaseTreeModel(new WrapperNode(nullptr), parent)
{
    PF_ProjectTree *tree = PF_ProjectTree::instance();
    connect(tree, &PF_ProjectTree::subtreeChanged, this, &PF_ProjectModel::updateSubtree);

    PF_SessionManager *sm = PF_SessionManager::instance();
    connect(sm, &PF_SessionManager::projectRemoved, this, &PF_ProjectModel::handleProjectRemoved);
//    connect(sm, &PF_SessionManager::aboutToLoadSession, this, &PF_ProjectModel::loadExpandData);
//    connect(sm, &PF_SessionManager::aboutToSaveSession, this, &PF_ProjectModel::saveExpandData);
    connect(sm, &PF_SessionManager::projectAdded, this, &PF_ProjectModel::handleProjectAdded);
//    connect(sm, &PF_SessionManager::startupProjectChanged, this, [this] { layoutChanged(); });

//    for (Project *project : SessionManager::projects())
//        handleProjectAdded(project);

//    m_disabledTextColor = Utils::creatorTheme()->color(Utils::Theme::TextColorDisabled);
//    m_enabledTextColor = Utils::creatorTheme()->color(Utils::Theme::TextColorNormal);
}

/*!
 \brief 提供获取数据的方法，参考 https://doc.qt.io/qt-5/qt.html#ItemDataRole-enum

 \param index
 \param role
 \return QVariant
*/
QVariant PF_ProjectModel::data(const QModelIndex &index, int role) const
{
    QVariant result;

    if (const Node *node = nodeForIndex(index)) {
        const FolderNode *folderNode = node->asFolderNode();
        const ProjectNode *containerNode = node->asProjectNode();
        const PF_Project *project = containerNode ? containerNode->project() : nullptr;

        switch (role) {
        case Qt::DisplayRole: {/**要以文本形式呈现的关键数据**/
            result = node->displayName();
            break;
        }
        case Qt::EditRole: {/**适合在编辑器中编辑的形式的数据**/
//            result = node->filePath().fileName();
            break;
        }
        case Qt::ToolTipRole: {/**项目工具提示中显示的数据**/
            QString tooltip = node->tooltip();
            break;
        }
        case Qt::DecorationRole: {/**要以图标形式呈现为装饰的数据**/
            result = node->icon();
            break;
        }
        case Qt::FontRole: {/**用于使用默认委托呈现的项目的字体**/
            QFont font;
            if (project == PF_SessionManager::startupProject())
                font.setBold(true);
            result = font;
            break;
        }
        case Qt::TextColorRole: {/**文本颜色**/
//            result = node->isEnabled() ? m_enabledTextColor : m_disabledTextColor;
            break;
        }
        }
    }

    return result;
}

/*!
 \brief 返回给定索引的项标志。

 \param index
 \return Qt::ItemFlags
*/
Qt::ItemFlags PF_ProjectModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return nullptr;
    // We claim that everything is editable
    // That's slightly wrong
    // We control the only view, and that one does the checks
    Qt::ItemFlags f = Qt::ItemIsSelectable|Qt::ItemIsEnabled|Qt::ItemIsDragEnabled;
    if (Node *node = nodeForIndex(index)) {
//        if (!node->asProjectNode()) {
            // either folder or file node
//            if (node->supportsAction(Rename, node))
//                f = f | Qt::ItemIsEditable;
//        }
    }
    return f;
}

bool PF_ProjectModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;
    if (role != Qt::EditRole)
        return false;

    Node *node = nodeForIndex(index);
    if(!node) return false;

    return true;
}


/*!
 \brief 返回包含node的项。

 \param node
 \return WrapperNode
*/
WrapperNode *PF_ProjectModel::wrapperForNode(const Node *node) const
{
    auto pred = [node](WrapperNode *item) {
        return item->m_node == node;
    };
    const auto pred0 = [pred](TreeItem *treeItem) -> bool { return pred(static_cast<WrapperNode *>(treeItem)); };
    return static_cast<WrapperNode *>(m_root->findAnyChild(pred0));
}

/*!
 \brief 返回包含node的项的索引。

 \param node
 \return QModelIndex
*/
QModelIndex PF_ProjectModel::indexForNode(const Node *node) const
{
    WrapperNode *wrapper = wrapperForNode(node);
    return wrapper ? indexForItem(wrapper) : QModelIndex();
}

void PF_ProjectModel::onExpanded(const QModelIndex &idx)
{
//    qDebug()<<Q_FUNC_INFO;
//    qDebug()<<"onExpanded:"<<idx.column()<<idx.row();
}

void PF_ProjectModel::onCollapsed(const QModelIndex &idx)
{
//    qDebug()<<Q_FUNC_INFO;
//    qDebug()<<"onCollapsed:"<<idx.column()<<idx.row();
}

/*!
 \brief 更新tree，代码确实有一点暴力，全部都更新了。找出node所属的pro。

 \param node
*/
void PF_ProjectModel::updateSubtree(FolderNode *node)
{
    qDebug()<<Q_FUNC_INFO;
    // FIXME: This is still excessive, should be limited to the affected subtree.
    while (FolderNode *parent = node->parentFolderNode())
        node = parent;
    if (ProjectNode *container = node->asProjectNode())
        addOrRebuildProjectModel(container->project());
}

/**
 * @brief
 *
 */
void PF_ProjectModel::rebuildModel()
{
    const QList<PF_Project *> projects = PF_SessionManager::projects();
    for (PF_Project *project : projects)
        addOrRebuildProjectModel(project);
}

/**
 * @brief 递归式的将foldernode转化为treeitem
 *
 * @param parent
 * @param folderNode
 * @param seen
 */
void PF_ProjectModel::addFolderNode(WrapperNode *parent, FolderNode *folderNode, QSet<Node *> *seen)
{
    for (Node *node : folderNode->nodes()) {
        if (FolderNode *subFolderNode = node->asFolderNode()) {
//            const bool isHidden = m_filterProjects && !subFolderNode->showInSimpleTree();
//            if (!isHidden && !seen->contains(subFolderNode)) {
//                seen->insert(subFolderNode);
                auto node = new WrapperNode(subFolderNode);
                parent->appendChild(node);
                addFolderNode(node, subFolderNode, seen);
//                node->sortChildren(&sortWrapperNodes);
//            } else {
//                addFolderNode(parent, subFolderNode, seen);
//            }
        } else if (LeafNode *leafNode = node->asLeafNode()) {
//            const bool isHidden = m_filterGeneratedFiles && fileNode->isGenerated();
//            if (!isHidden && !seen->contains(leafNode)) {
//                seen->insert(leafNode);
                parent->appendChild(new WrapperNode(leafNode));
//            }
        }
    }
}

/**
 * @brief 通常是要处理从session传过来的project
 *
 * @param project
 */
void PF_ProjectModel::handleProjectAdded(PF_Project *project)
{
    qDebug()<<Q_FUNC_INFO;
    if(!project)
        return;
//    connect(project, &Project::parsingStarted,
//            this, [this, project]() {
//        if (nodeForProject(project))
//            parsingStateChanged(project);
//    });
//    connect(project, &Project::parsingFinished,
//            this, [this, project]() {
//        if (nodeForProject(project))
//            parsingStateChanged(project);
//    });
    addOrRebuildProjectModel(project);
}

void PF_ProjectModel::handleProjectRemoved(PF_Project *project)
{
    destroyItem(nodeForProject(project));
}

/**
 * @brief 返回project对应树节点上的位置
 *
 * @param project
 * @return WrapperNode
 */
WrapperNode *PF_ProjectModel::nodeForProject(const PF_Project *project) const
{
    qDebug()<<Q_FUNC_INFO;
    if(!project)
        return nullptr;
    /** 计算project的rootnode **/
    ProjectNode *containerNode = project->rootProjectNode();
    if(!containerNode)
        return nullptr;
    /** 在tree当中查找containerNode，项目节点肯定在第一层 **/
    auto pre0 = [containerNode](TreeItem *node) {
        return static_cast<WrapperNode *>(node)->m_node == containerNode;
    };

    return static_cast<WrapperNode *>(rootItem()->findChildAtLevel(1,pre0));
}

/**
 * @brief 重新生成project模型，如有有就重新生成，如果没有就添加
 *
 * @param project
 */
void PF_ProjectModel::addOrRebuildProjectModel(PF_Project *project)
{
    qDebug()<<Q_FUNC_INFO;
    /** 查找模型当中有没有project，注意，如果存在的话，会先清空 **/
    WrapperNode *container = nodeForProject(project);
    if (container) {
        container->removeChildren();
//        project->rootProjectNode()->removeAllChildren();
    } else {
        /** project的rootnode应该在构造的时候生成好 **/
        container = new WrapperNode(project->rootProjectNode());
        rootItem()->appendChild(container);
//        rootItem()->insertOrderedChild(container, &compareProjectNames);
    }

    QSet<Node *> seen;

    if (ProjectNode *projectNode = project->rootProjectNode()) {
        addFolderNode(container, projectNode, &seen);
    }

//    container->sortChildren(&sortWrapperNodes);

    container->forAllChildren([this](TreeItem *node) {
        /** 不知道为什么这里不能使用WrapperNode作为参数，可能是编译器的问题，
            所以就写成了TreeItem  **/
        if (dynamic_cast<WrapperNode*>(node)->m_node) {
//            const QString path = node->m_node->filePath().toString();
//            const QString displayName = node->m_node->displayName();
//            ExpandData ed(path, displayName);
//            if (m_toExpand.contains(ed))
                emit requestExpansion(node->index());
        } else {
            emit requestExpansion(node->index());
        }
    });

//    const QString path = container->m_node->filePath().toString();
//    const QString displayName = container->m_node->displayName();
//    ExpandData ed(path, displayName);
//    if (m_toExpand.contains(ed))
//        emit requestExpansion(container->index());
}

//void PF_ProjectModel::setProjectFilterEnabled(bool filter)
//{
//    if (filter == m_filterProjects)
//        return;
//    m_filterProjects = filter;
//    rebuildModel();
//}

//void PF_ProjectModel::setGeneratedFilesFilterEnabled(bool filter)
//{
//    if (filter == m_filterGeneratedFiles)
//        return;
//    m_filterGeneratedFiles = filter;
//    rebuildModel();
//}

//void PF_ProjectModel::setTrimEmptyDirectories(bool filter)
//{
//    if (filter == m_trimEmptyDirectories)
//        return;
//    m_trimEmptyDirectories = filter;
//    rebuildModel();
//}

//bool PF_ProjectModel::projectFilterEnabled()
//{
//    return m_filterProjects;
//}

//bool PF_ProjectModel::generatedFilesFilterEnabled()
//{
//    return m_filterGeneratedFiles;
//}

/*!
 \brief 返回索引Index处数据的Node值。

 \param index
 \return Node
*/
Node *PF_ProjectModel::nodeForIndex(const QModelIndex &index) const
{
    WrapperNode* flatNode = static_cast<WrapperNode* >(BaseTreeModel::itemForIndex(index));
    return flatNode ? flatNode->m_node : nullptr;
}

//const QLoggingCategory &PF_ProjectModel::logger()
//{
//    static QLoggingCategory logger("qtc.projectexplorer.PF_ProjectModel", QtWarningMsg);
//    return logger;
//}
