#include "pf_projecttree.h"
#include "pf_projecttreewidget.h"
#include "projectexplorerconstants.h"

#include "pf_node.h"
#include "pf_project.h"
#include "actionmanager/actionmanager.h"
#include "actionmanager/actioncontainer.h"
#include "pf_sessionmanager.h"
#include <QMenu>

#include <QApplication>
#include <QDebug>

PF_ProjectTree* PF_ProjectTree::s_instance = nullptr;

PF_ProjectTree::PF_ProjectTree(QObject *parent) : QObject(parent)
{
    s_instance = this;
    /** TODO 连接CAD模块 **/

    connect(qApp, &QApplication::focusChanged,
            this, &PF_ProjectTree::update);
    connect(PF_SessionManager::instance(), &PF_SessionManager::projectAdded,
            this, &PF_ProjectTree::sessionAndTreeChanged);
    connect(PF_SessionManager::instance(), &PF_SessionManager::projectRemoved,
            this, &PF_ProjectTree::sessionAndTreeChanged);
    connect(PF_SessionManager::instance(), &PF_SessionManager::startupProjectChanged,
            this, &PF_ProjectTree::sessionChanged);
    connect(this, &PF_ProjectTree::subtreeChanged, this, &PF_ProjectTree::treeChanged);
}

PF_ProjectTree::~PF_ProjectTree()
{
    s_instance = nullptr;
}

PF_ProjectTree *PF_ProjectTree::instance()
{
    return s_instance;
}

/*!
 \brief 注册控件，也就是在创建treewidget之后，同步给这里

 \param widget
*/
void PF_ProjectTree::registerWidget(PF_ProjectTreeWidget* widget)
{
    s_instance->m_projecttreewidget = widget;
}

/*!
 \brief 将绑定的treewidget解绑

 \param widget
*/
void PF_ProjectTree::unregisterWidget(PF_ProjectTreeWidget* widget)
{
    s_instance->m_projecttreewidget = nullptr;
}

void PF_ProjectTree::nodeChanged(PF_ProjectTreeWidget* widget)
{
    if (hasFocus(widget))
        s_instance->updateFromProjectTreeWidget(widget);
}

/*!
 \brief 在所有的项目当中寻找是否有node

 \param node
 \return bool
*/
bool PF_ProjectTree::hasNode(const Node* node)
{
    const QList<PF_Project *> projects = PF_SessionManager::projects();
    for (PF_Project *project : projects) {
        if (ProjectNode *projectNode = project->rootProjectNode()) {
            if(project->rootProjectNode()->findNode([node](const Node *n) { return n == node; }))
                return true;
        }
    }
//    return Utils::contains(SessionManager::projects(), [node](const Project *p) {
//        return p && p->rootProjectNode() && (
//                    p->containerNode() == node
//                    || p->rootProjectNode()->findNode([node](const Node *n) { return n == node; }));
//    });
    return false;
}

/*!
 \brief 寻找node所在的项目

 \param node
 \return PF_Project
*/
PF_Project* PF_ProjectTree::projectForNode(const Node* node)
{
    if (!node)
        return nullptr;

//    /** 寻找对应的projectnode **/
//    const FolderNode *folder = node->asFolderNode();
//    if (!folder)
//        folder = node->parentFolderNode();

//    while (folder && folder->parentFolderNode())
//        folder = folder->parentFolderNode();

//    /** 寻找projectnode对应的project **/
//    return Utils::findOrDefault(SessionManager::projects(), [folder](const Project *pro) {
//        return pro->containerNode() == folder;
//    });
}

/*!
 \brief 当前项目

 \return PF_Project
*/
PF_Project* PF_ProjectTree::currentProject()
{
    return s_instance->m_currentProject;
}

/*!
 \brief 当前节点

 \return Node
*/
Node *PF_ProjectTree::findCurrentNode()
{
    s_instance->update();
    return s_instance->m_currentNode;
}

/*!
 \brief 每一个node的右键菜单

 \param focus
 \param globalPos
 \param node
*/
void PF_ProjectTree::showContextMenu(PF_ProjectTreeWidget *focus, const QPoint &globalPos, Node *node)
{
    QMenu *contextMenu = nullptr;
    PF_Project *project = projectForNode(node);
    emit s_instance->aboutToShowContextMenu(project, node);

    if (!node) {
//        contextMenu = Core::ActionManager::actionContainer(Constants::M_SESSIONCONTEXT)->menu();
    } else {
        switch (node->nodeType()) {
        case NodeType::Project: {
//            if ((node->parentFolderNode() && node->parentFolderNode()->asContainerNode())
//                    || node->asContainerNode())
                contextMenu = ActionManager::actionContainer(Constants::M_PROJECTCONTEXT)->menu();
//            else
//                contextMenu = ActionManager::actionContainer(Constants::M_SUBPROJECTCONTEXT)->menu();
            break;
        }
//        case NodeType::VirtualFolder:
        case NodeType::Material:{
            contextMenu = ActionManager::actionContainer(Constants::M_MATERIALCONTEXT)->menu();
            break;
        }
        case NodeType::Folder:
//            contextMenu = ActionManager::actionContainer(Constants::M_FOLDERCONTEXT)->menu();
            break;
        case NodeType::Leaf:
//            contextMenu = ActionManager::actionContainer(Constants::M_FILECONTEXT)->menu();
            break;
        default:
            qWarning("ProjectExplorerPlugin::showContextMenu - Missing handler for node type");
        }
    }

    if (contextMenu && contextMenu->actions().count() > 0) {
        contextMenu->popup(globalPos);
//        s_instance->m_focusForContextMenu = focus;
//        connect(contextMenu, &QMenu::aboutToHide,
//                s_instance, &ProjectTree::hideContextMenu,
//                Qt::ConnectionType(Qt::UniqueConnection | Qt::QueuedConnection));
    }
}

void PF_ProjectTree::highlightProject(PF_Project* project, const QString& message)
{

}

/*!
 \brief 折叠所有node

*/
void PF_ProjectTree::collapseAll()
{
//    if(hasFocus(m_projecttreewidget))
        m_projecttreewidget->collapseAll();
}

void PF_ProjectTree::expandAll()
{
//    if(hasFocus(m_projecttreewidget))
    m_projecttreewidget->expandAll();
}

void PF_ProjectTree::emitSubtreeChanged(FolderNode *node)
{
    if (hasNode(node))
        emit s_instance->subtreeChanged(node);
}

/*!
 \brief

*/
void PF_ProjectTree::sessionAndTreeChanged()
{
    qDebug()<<Q_FUNC_INFO;
    sessionChanged();
    emit treeChanged();
}

/*!
 \brief

*/
void PF_ProjectTree::sessionChanged()
{
    /** 当前有没有新文件需要保存 **/
    if (m_currentProject) {
//        Core::DocumentManager::setDefaultLocationForNewFiles(m_currentProject->projectDirectory().toString());
    } else if (PF_Project *project = PF_SessionManager::startupProject()) {
//        Core::DocumentManager::setDefaultLocationForNewFiles(project->projectDirectory().toString());
        updateFromNode(nullptr); // Make startup project current if there is no other current
    } else {
//        Core::DocumentManager::setDefaultLocationForNewFiles(QString());
    }
    update();
}

/*!
 \brief 更新，判断是哪一边发生了改变，就从哪一边更新

*/
void PF_ProjectTree::update()
{
    if(hasFocus((m_projecttreewidget))){
        updateFromProjectTreeWidget(m_projecttreewidget);
    }else{
        /** 可能是从CAD更新 **/
    }
}

/*!
 \brief 根据treewidget的状态进行更新

 \param widget
*/
void PF_ProjectTree::updateFromProjectTreeWidget(PF_ProjectTreeWidget* widget)
{
    Node *currentNode = widget->currentNode();
    PF_Project *project = projectForNode(currentNode);

    if (!project)
        updateFromNode(nullptr); // Project was removed!
    else
        setCurrent(currentNode, project);
}

/*!
 \brief 从节点node开始更新，如果为空，则

 \param node
*/
void PF_ProjectTree::updateFromNode(Node* node)
{
    PF_Project *project;
    if (node)
        project = projectForNode(node);
    else
        project = PF_SessionManager::startupProject();

    setCurrent(node, project);
    m_projecttreewidget->sync(node);
}

/*!
 \brief 将节点设置为project当中的node

 \param node
 \param project
*/
void PF_ProjectTree::setCurrent(Node* node, PF_Project* project)
{
    const bool changedProject = project != m_currentProject;
    if (changedProject) {
        if (m_currentProject) {
//            disconnect(m_currentProject, &PF_Project::projectLanguagesUpdated,
//                       this, &PF_ProjectTree::updateContext);
        }

        m_currentProject = project;

        if (m_currentProject) {
//            connect(m_currentProject, &Project::projectLanguagesUpdated,
//                    this, &ProjectTree::updateContext);
        }
    }

//    if (Core::IDocument *document = Core::EditorManager::currentDocument()) {
//        if (node) {
//            disconnect(document, &Core::IDocument::changed,
//                       this, &ProjectTree::updateExternalFileWarning);
//            document->infoBar()->removeInfo(EXTERNAL_FILE_WARNING);
//        } else {
//            connect(document, &Core::IDocument::changed,
//                    this, &ProjectTree::updateExternalFileWarning,
//                    Qt::UniqueConnection);
//        }
//    }

    if (node != m_currentNode) {
        m_currentNode = node;
        emit currentNodeChanged();
    }

    if (changedProject) {
        emit currentProjectChanged(m_currentProject);
        sessionChanged();
        updateContext();
    }
}

void PF_ProjectTree::updateContext()
{

}

void PF_ProjectTree::updateFromFocus()
{

}

/*!
 \brief widget是否获得焦点

 \param widget
 \return bool
*/
bool PF_ProjectTree::hasFocus(PF_ProjectTreeWidget* widget)
{
    return widget
            && ((widget->focusWidget() && widget->focusWidget()->hasFocus()));
//                || s_instance->m_focusForContextMenu == widget);
}

void PF_ProjectTree::hideContextMenu()
{

}
