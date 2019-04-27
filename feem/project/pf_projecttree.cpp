#include "pf_projecttree.h"

#include "pf_node.h"
#include "actionmanager/actionmanager.h"
#include <QMenu>

PF_ProjectTree* PF_ProjectTree::s_instance = nullptr;

PF_ProjectTree::PF_ProjectTree(QObject *parent) : QObject(parent)
{

}

PF_ProjectTree::~PF_ProjectTree()
{

}

PF_ProjectTree *PF_ProjectTree::instance()
{
    return s_instance;
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
