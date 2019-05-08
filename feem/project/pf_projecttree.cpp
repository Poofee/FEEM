#include "pf_projecttree.h"

#include "pf_node.h"
#include "pf_project.h"
#include "actionmanager/actionmanager.h"
#include "pf_sessionmanager.h"
#include <QMenu>

#include <QApplication>

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

void PF_ProjectTree::registerWidget(PF_ProjectTreeWidget* widget)
{
    s_instance->m_projecttreewidget = widget;
}

void PF_ProjectTree::unregisterWidget(PF_ProjectTreeWidget* widget)
{
    s_instance->m_projecttreewidget = nullptr;
}

void PF_ProjectTree::nodeChanged(PF_ProjectTreeWidget* widget)
{

}

PF_Project *PF_ProjectTree::currentProject()
{
    return nullptr;
}

Node *PF_ProjectTree::findCurrentNode()
{
    return nullptr;
}

void PF_ProjectTree::showContextMenu(PF_ProjectTreeWidget *focus, const QPoint &globalPos, Node *node)
{
    QMenu *contextMenu = nullptr;
//    Project *project = projectForNode(node);
//    emit s_instance->aboutToShowContextMenu(project, node);

    if (!node) {
//        contextMenu = Core::ActionManager::actionContainer(Constants::M_SESSIONCONTEXT)->menu();
    } else {
        switch (node->nodeType()) {
        case NodeType::Project: {
//            if ((node->parentFolderNode() && node->parentFolderNode()->asContainerNode())
//                    || node->asContainerNode())
//                contextMenu = ActionManager::actionContainer(Constants::M_PROJECTCONTEXT)->menu();
//            else
//                contextMenu = ActionManager::actionContainer(Constants::M_SUBPROJECTCONTEXT)->menu();
            break;
        }
//        case NodeType::VirtualFolder:
        case NodeType::Folder:
//            contextMenu = ActionManager::actionContainer(Constants::M_FOLDERCONTEXT)->menu();
            break;
        case NodeType::File:
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

void PF_ProjectTree::collapseAll()
{

}

void PF_ProjectTree::sessionAndTreeChanged()
{

}

void PF_ProjectTree::sessionChanged()
{

}

void PF_ProjectTree::update()
{

}

void PF_ProjectTree::updateFromProjectTreeWidget(PF_ProjectTreeWidget* widget)
{

}

void PF_ProjectTree::updateFromNode(Node* node)
{

}

void PF_ProjectTree::setCurrent(Node* node, PF_Project* project)
{

}

void PF_ProjectTree::updateContext()
{

}

void PF_ProjectTree::updateFromFocus()
{

}

bool PF_ProjectTree::hasFocus(PF_ProjectTreeWidget* widget)
{
    return false;
}

void PF_ProjectTree::hideContextMenu()
{

}
