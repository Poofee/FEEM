#include "pf_materialtreemodel.h"

#include "pf_node.h"

PF_MaterialTreeModel::PF_MaterialTreeModel(QObject *parent)
    : BaseTreeModel(new WrapperNode(nullptr), parent)
{

}

QVariant PF_MaterialTreeModel::data(const QModelIndex &index, int role) const
{
    QVariant result;

    if (const Node *node = nodeForIndex(index)) {
        //        const FolderNode *folderNode = node->asFolderNode();
        //        const ContainerNode *containerNode = node->asContainerNode();
        //        const Project *project = containerNode ? containerNode->project() : nullptr;

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

            //            if (project) {
            //                if (project->activeTarget()) {
            //                    QString projectIssues = toHtml(project->projectIssues(project->activeTarget()->kit()));
            //                    if (!projectIssues.isEmpty())
            //                        tooltip += "<p>" + projectIssues;
            //                } else {
            //                    tooltip += "<p>" + tr("No kits are enabled for this project. "
            //                                          "Enable kits in the \"Projects\" mode.");
            //                }
            //            }
            //            result = tooltip;
            break;
        }
        case Qt::DecorationRole: {/**要以图标形式呈现为装饰的数据**/
            //            if (folderNode) {
            //                static QIcon warnIcon = Utils::Icons::WARNING.icon();
            //                static QIcon emptyIcon = Utils::Icons::EMPTY16.icon();
            //                if (project) {
            //                    if (project->isParsing())
            //                        result = emptyIcon;
            //                    else if (!project->activeTarget()
            //                             || !project->projectIssues(project->activeTarget()->kit()).isEmpty())
            //                        result = warnIcon;
            //                    else
            //                        result = containerNode->rootProjectNode() ? containerNode->rootProjectNode()->icon() :
            //                                                                    folderNode->icon();
            //                } else {
            //                    result = folderNode->icon();
            //                }
            //            } else {
            //                result = Core::FileIconProvider::icon(node->filePath().toString());
            //            }
            result = node->icon();
            break;
        }
        case Qt::FontRole: {/**用于使用默认委托呈现的项目的字体**/
            QFont font;
            //            if (project == SessionManager::startupProject())
            //                font.setBold(true);
            //            result = font;
            break;
        }
        case Qt::TextColorRole: {/**文本颜色**/
            //            result = node->isEnabled() ? m_enabledTextColor : m_disabledTextColor;
            break;
        }
            //        case Project::FilePathRole: {
            //            result = node->filePath().toString();
            //            break;
            //        }
            //        case Project::isParsingRole: {
            //            result = project ? project->isParsing() : false;
            //            break;
            //        }
        }
    }

    return result;
}

Qt::ItemFlags PF_MaterialTreeModel::flags(const QModelIndex &index) const
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

bool PF_MaterialTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;
    if (role != Qt::EditRole)
        return false;

    Node *node = nodeForIndex(index);
    if(!node) return false;

    return true;
}

Node *PF_MaterialTreeModel::nodeForIndex(const QModelIndex &index) const
{
    WrapperNode* flatNode = static_cast<WrapperNode* >(BaseTreeModel::itemForIndex(index));
    return flatNode ? flatNode->m_node : nullptr;
}

WrapperNode *PF_MaterialTreeModel::wrapperForNode(const Node *node) const
{
    auto pred = [node](WrapperNode *item) {
        return item->m_node == node;
    };
    const auto pred0 = [pred](TreeItem *treeItem) -> bool { return pred(static_cast<WrapperNode *>(treeItem)); };
    return static_cast<WrapperNode *>(m_root->findAnyChild(pred0));
}

QModelIndex PF_MaterialTreeModel::indexForNode(const Node *node) const
{
    WrapperNode *wrapper = wrapperForNode(node);
    return wrapper ? indexForItem(wrapper) : QModelIndex();
}

void PF_MaterialTreeModel::onExpanded(const QModelIndex &idx)
{

}

void PF_MaterialTreeModel::onCollapsed(const QModelIndex &idx)
{

}

void PF_MaterialTreeModel::addFolderNode(WrapperNode *parent, FolderNode *folderNode, QSet<Node *> *seen)
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
