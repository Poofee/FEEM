#include "pf_projectmodel.h"

PF_ProjectModel::PF_ProjectModel(QObject *parent)
    : BaseTreeModel(new WrapperNode(nullptr), parent)
{
//    ProjectTree *tree = ProjectTree::instance();
//    connect(tree, &ProjectTree::subtreeChanged, this, &PF_ProjectModel::updateSubtree);

//    SessionManager *sm = SessionManager::instance();
//    connect(sm, &SessionManager::projectRemoved, this, &PF_ProjectModel::handleProjectRemoved);
//    connect(sm, &SessionManager::aboutToLoadSession, this, &PF_ProjectModel::loadExpandData);
//    connect(sm, &SessionManager::aboutToSaveSession, this, &PF_ProjectModel::saveExpandData);
//    connect(sm, &SessionManager::projectAdded, this, &PF_ProjectModel::handleProjectAdded);
//    connect(sm, &SessionManager::startupProjectChanged, this, [this] { layoutChanged(); });

//    for (Project *project : SessionManager::projects())
//        handleProjectAdded(project);

//    m_disabledTextColor = Utils::creatorTheme()->color(Utils::Theme::TextColorDisabled);
//    m_enabledTextColor = Utils::creatorTheme()->color(Utils::Theme::TextColorNormal);
}

/*!
 \brief 提供获取数据的方法

 \param index
 \param role
 \return QVariant
*/
QVariant PF_ProjectModel::data(const QModelIndex &index, int role) const
{
    QVariant result;

    if (const Node *node = nodeForIndex(index)) {
        const FolderNode *folderNode = node->asFolderNode();
        const ContainerNode *containerNode = node->asContainerNode();
        const Project *project = containerNode ? containerNode->project() : nullptr;

        switch (role) {
        case Qt::DisplayRole: {
            result = node->displayName();
            break;
        }
        case Qt::EditRole: {
            result = node->filePath().fileName();
            break;
        }
        case Qt::ToolTipRole: {
            QString tooltip = node->tooltip();

            if (project) {
                if (project->activeTarget()) {
                    QString projectIssues = toHtml(project->projectIssues(project->activeTarget()->kit()));
                    if (!projectIssues.isEmpty())
                        tooltip += "<p>" + projectIssues;
                } else {
                    tooltip += "<p>" + tr("No kits are enabled for this project. "
                                          "Enable kits in the \"Projects\" mode.");
                }
            }
            result = tooltip;
            break;
        }
        case Qt::DecorationRole: {
            if (folderNode) {
                static QIcon warnIcon = Utils::Icons::WARNING.icon();
                static QIcon emptyIcon = Utils::Icons::EMPTY16.icon();
                if (project) {
                    if (project->isParsing())
                        result = emptyIcon;
                    else if (!project->activeTarget()
                             || !project->projectIssues(project->activeTarget()->kit()).isEmpty())
                        result = warnIcon;
                    else
                        result = containerNode->rootProjectNode() ? containerNode->rootProjectNode()->icon() :
                                                                    folderNode->icon();
                } else {
                    result = folderNode->icon();
                }
            } else {
                result = Core::FileIconProvider::icon(node->filePath().toString());
            }
            break;
        }
        case Qt::FontRole: {
            QFont font;
            if (project == SessionManager::startupProject())
                font.setBold(true);
            result = font;
            break;
        }
        case Qt::TextColorRole: {
            result = node->isEnabled() ? m_enabledTextColor : m_disabledTextColor;
            break;
        }
        case Project::FilePathRole: {
            result = node->filePath().toString();
            break;
        }
        case Project::isParsingRole: {
            result = project ? project->isParsing() : false;
            break;
        }
        }
    }

    return result;
}

Qt::ItemFlags PF_ProjectModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return nullptr;
    // We claim that everything is editable
    // That's slightly wrong
    // We control the only view, and that one does the checks
    Qt::ItemFlags f = Qt::ItemIsSelectable|Qt::ItemIsEnabled|Qt::ItemIsDragEnabled;
    if (Node *node = nodeForIndex(index)) {
        if (!node->asProjectNode()) {
            // either folder or file node
            if (node->supportsAction(Rename, node))
                f = f | Qt::ItemIsEditable;
        }
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

    Utils::FileName orgFilePath = node->filePath();
    Utils::FileName newFilePath = orgFilePath.parentDir().appendPath(value.toString());

    ProjectExplorerPlugin::renameFile(node, newFilePath.toString());
    emit renamed(orgFilePath, newFilePath);
    return true;
}

//static bool compareProjectNames(const WrapperNode *lhs, const WrapperNode *rhs)
//{
//    Node *p1 = lhs->m_node;
//    Node *p2 = rhs->m_node;
//    const int displayNameResult = caseFriendlyCompare(p1->displayName(), p2->displayName());
//    if (displayNameResult != 0)
//        return displayNameResult < 0;
//    return p1 < p2; // sort by pointer value
//}

//void PF_ProjectModel::addOrRebuildProjectModel(Project *project)
//{
//    WrapperNode *container = nodeForProject(project);
//    if (container) {
//        container->removeChildren();
//        project->containerNode()->removeAllChildren();
//    } else {
//        container = new WrapperNode(project->containerNode());
//        rootItem()->insertOrderedChild(container, &compareProjectNames);
//    }

//    QSet<Node *> seen;

//    if (ProjectNode *projectNode = project->rootProjectNode()) {
//        addFolderNode(container, projectNode, &seen);
//        if (m_trimEmptyDirectories)
//            trimEmptyDirectories(container);
//    }
//    if (container->childCount() == 0) {
//        auto projectFileNode = std::make_unique<FileNode>(project->projectFilePath(),
//                                                          FileType::Project, false);
//        seen.insert(projectFileNode.get());
//        container->appendChild(new WrapperNode(projectFileNode.get()));
//        project->containerNode()->addNestedNode(std::move(projectFileNode));
//    }

//    container->sortChildren(&sortWrapperNodes);

//    container->forAllChildren([this](WrapperNode *node) {
//        if (node->m_node) {
//            const QString path = node->m_node->filePath().toString();
//            const QString displayName = node->m_node->displayName();
//            ExpandData ed(path, displayName);
//            if (m_toExpand.contains(ed))
//                emit requestExpansion(node->index());
//        } else {
//            emit requestExpansion(node->index());
//        }
//    });

//    const QString path = container->m_node->filePath().toString();
//    const QString displayName = container->m_node->displayName();
//    ExpandData ed(path, displayName);
//    if (m_toExpand.contains(ed))
//        emit requestExpansion(container->index());
//}

//void PF_ProjectModel::parsingStateChanged(Project *project)
//{
//    const WrapperNode *const node = nodeForProject(project);
//    const QModelIndex nodeIdx = indexForNode(node->m_node);
//    emit dataChanged(nodeIdx, nodeIdx);
//}

//void PF_ProjectModel::updateSubtree(FolderNode *node)
//{
//    // FIXME: This is still excessive, should be limited to the affected subtree.
//    while (FolderNode *parent = node->parentFolderNode())
//        node = parent;
//    if (ContainerNode *container = node->asContainerNode())
//        addOrRebuildProjectModel(container->project());
//}

//void PF_ProjectModel::rebuildModel()
//{
//    const QList<Project *> projects = SessionManager::projects();
//    for (Project *project : projects)
//        addOrRebuildProjectModel(project);
//}

//void PF_ProjectModel::onCollapsed(const QModelIndex &idx)
//{
//    m_toExpand.remove(expandDataForNode(nodeForIndex(idx)));
//}

//void PF_ProjectModel::onExpanded(const QModelIndex &idx)
//{
//    m_toExpand.insert(expandDataForNode(nodeForIndex(idx)));
//}

//ExpandData PF_ProjectModel::expandDataForNode(const Node *node) const
//{
//    QTC_ASSERT(node, return ExpandData());
//    const QString path = node->filePath().toString();
//    const QString displayName = node->displayName();
//    return ExpandData(path, displayName);
//}

//void PF_ProjectModel::handleProjectAdded(Project *project)
//{
//    QTC_ASSERT(project, return);

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
//    addOrRebuildProjectModel(project);
//}

//void PF_ProjectModel::handleProjectRemoved(Project *project)
//{
//    destroyItem(nodeForProject(project));
//}

//WrapperNode *PF_ProjectModel::nodeForProject(const Project *project) const
//{
//    QTC_ASSERT(project, return nullptr);
//    ContainerNode *containerNode = project->containerNode();
//    QTC_ASSERT(containerNode, return nullptr);
//    return rootItem()->findFirstLevelChild([containerNode](WrapperNode *node) {
//        return node->m_node == containerNode;
//    });
//}

//void PF_ProjectModel::loadExpandData()
//{
//    const QList<QVariant> data = SessionManager::value("ProjectTree.ExpandData").value<QList<QVariant>>();
//    m_toExpand = Utils::transform<QSet>(data, &ExpandData::fromSettings);
//    m_toExpand.remove(ExpandData());
//}

//void PF_ProjectModel::saveExpandData()
//{
//    // TODO if there are multiple ProjectTreeWidgets, the last one saves the data
//    QList<QVariant> data = Utils::transform<QList>(m_toExpand, &ExpandData::toSettings);
//    SessionManager::setValue(QLatin1String("ProjectTree.ExpandData"), data);
//}

//void PF_ProjectModel::addFolderNode(WrapperNode *parent, FolderNode *folderNode, QSet<Node *> *seen)
//{
//    for (Node *node : folderNode->nodes()) {
//        if (FolderNode *subFolderNode = node->asFolderNode()) {
//            const bool isHidden = m_filterProjects && !subFolderNode->showInSimpleTree();
//            if (!isHidden && !seen->contains(subFolderNode)) {
//                seen->insert(subFolderNode);
//                auto node = new WrapperNode(subFolderNode);
//                parent->appendChild(node);
//                addFolderNode(node, subFolderNode, seen);
//                node->sortChildren(&sortWrapperNodes);
//            } else {
//                addFolderNode(parent, subFolderNode, seen);
//            }
//        } else if (FileNode *fileNode = node->asFileNode()) {
//            const bool isHidden = m_filterGeneratedFiles && fileNode->isGenerated();
//            if (!isHidden && !seen->contains(fileNode)) {
//                seen->insert(fileNode);
//                parent->appendChild(new WrapperNode(fileNode));
//            }
//        }
//    }
//}

//bool PF_ProjectModel::trimEmptyDirectories(WrapperNode *parent)
//{
//    const FolderNode *fn = parent->m_node->asFolderNode();
//    if (!fn)
//        return false;

//    for (int i = parent->childCount() - 1; i >= 0; --i) {
//        if (trimEmptyDirectories(parent->childAt(i)))
//            parent->removeChildAt(i);
//    }
//    return parent->childCount() == 0 && !fn->showWhenEmpty();
//}

//Qt::DropActions PF_ProjectModel::supportedDragActions() const
//{
//    return Qt::MoveAction;
//}

//QStringList PF_ProjectModel::mimeTypes() const
//{
//    return Utils::DropSupport::mimeTypesForFilePaths();
//}

//QMimeData *PF_ProjectModel::mimeData(const QModelIndexList &indexes) const
//{
//    auto data = new Utils::DropMimeData;
//    foreach (const QModelIndex &index, indexes) {
//        if (Node *node = nodeForIndex(index)) {
//            if (node->asFileNode())
//                data->addFile(node->filePath().toString());
//            data->addValue(QVariant::fromValue(node));
//        }
//    }
//    return data;
//}

WrapperNode *PF_ProjectModel::wrapperForNode(const Node *node) const
{
    return findNonRootItem([node](WrapperNode *item) {
        return item->m_node == node;
    });
}

//QModelIndex PF_ProjectModel::indexForNode(const Node *node) const
//{
//    WrapperNode *wrapper = wrapperForNode(node);
//    return wrapper ? indexForItem(wrapper) : QModelIndex();
//}

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

Node *PF_ProjectModel::nodeForIndex(const QModelIndex &index) const
{
    WrapperNode *flatNode = itemForIndex(index);
    return flatNode ? flatNode->m_node : nullptr;
}

//const QLoggingCategory &PF_ProjectModel::logger()
//{
//    static QLoggingCategory logger("qtc.projectexplorer.PF_ProjectModel", QtWarningMsg);
//    return logger;
//}
