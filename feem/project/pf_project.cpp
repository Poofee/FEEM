#include "pf_project.h"
#include "pf_node.h"

#include "idocument.h"
#include "pf_nodetreebuilder.h"
#include "pf_projecttree.h"

#include <memory>
#include <QDebug>

// --------------------------------------------------------------------
// ProjectDocument:
// --------------------------------------------------------------------

ProjectDocument::ProjectDocument(const QString &mimeType, const FileName &fileName,
                                 const ProjectDocument::ProjectCallback &callback) :
    m_callback(callback)
{
    setFilePath(fileName);
    setMimeType(mimeType);
//    if (m_callback)
//        DocumentManager::addDocument(this);
}

IDocument::ReloadBehavior
ProjectDocument::reloadBehavior(IDocument::ChangeTrigger state,
                                IDocument::ChangeType type) const
{
    Q_UNUSED(state);
    Q_UNUSED(type);
    return BehaviorSilent;
}

bool ProjectDocument::reload(QString *errorString, IDocument::ReloadFlag flag,
                             IDocument::ChangeType type)
{
    Q_UNUSED(errorString);
    Q_UNUSED(flag);
    Q_UNUSED(type);

    if (m_callback)
        m_callback();
    return true;
}
/*!
 \brief Project的具体实现。

*/
class PF_ProjectPrivate{
public:
//    PF_ProjectPrivate(const QString &mimeType, const FileName &fileName,
//                   const ProjectDocument::ProjectCallback &callback)
//    {
//        m_document = std::make_unique<ProjectDocument>(mimeType, fileName, callback);
//    }
    PF_ProjectPrivate();
    ~PF_ProjectPrivate();

    /** 保存真实的文件 **/
    std::unique_ptr<IDocument> m_document;
    std::unique_ptr<ProjectNode> m_rootProjectNode;

    QString m_displayName;
};

PF_Project::PF_Project(QObject *parent)
    : QObject(parent)
    , d(new PF_ProjectPrivate())
{
    setRootProjectNode(PF_NodeTreeBuilder::buildTree(this));
}

PF_Project::~PF_Project()
{

}

QString PF_Project::displayName() const
{
    if(d->m_displayName.isEmpty()){
        d->m_displayName = QString(tr("untitled"))+QString(".mph");
    }

    return d->m_displayName;
}

void PF_Project::setDisplayName(const QString &displayName)
{
    d->m_displayName = displayName;
}

/**
 * @brief 简单的测试tree
 *
 */
void PF_Project::creatTree()
{
    std::unique_ptr<ProjectNode> root = std::make_unique<ProjectNode>(this);
    std::vector<std::unique_ptr<FolderNode>> nodes;
    nodes.emplace_back(std::make_unique<FolderNode>(QString(tr("Global Definitions")),NodeType::Definition,QIcon(":/tree/global_branch.png")));
    nodes.emplace_back(std::make_unique<FolderNode>(QString(tr("Materials: Materials")),NodeType::Material,QIcon(":/tree/material.png")));
    nodes.emplace_back(std::make_unique<FolderNode>(QString(tr("Component: Component1")),NodeType::Component,QIcon(":/tree/model_2d_axi.png")));
    nodes.emplace_back(std::make_unique<FolderNode>(QString(tr("Definitions")),NodeType::Variable,QIcon(":/tree/definitions.png")));
    nodes.emplace_back(std::make_unique<FolderNode>(QString(tr("Geometry1")),NodeType::Geometry,QIcon(":/tree/geometry.png")));
    nodes.emplace_back(std::make_unique<FolderNode>(QString(tr("Mesh1")),NodeType::Mesh,QIcon(":/tree/mesh.png")));

    nodes.at(0)->addNode(std::move(nodes.at(3)));
    nodes.at(0)->addNode(std::move(nodes.at(1)));
    root->addNode(std::move(nodes.at(0)));

    nodes.at(2)->addNode(std::move(nodes.at(4)));
    nodes.at(2)->addNode(std::move(nodes.at(5)));
    root->addNode(std::move(nodes.at(2)));

    setRootProjectNode(std::move(root));
}

/*!
 \brief 将root设置为项目的节点，在此之前，应该已经对project进行了处理，
 并得到projectnode。

 \param root
*/
void PF_Project::setRootProjectNode(std::unique_ptr<ProjectNode> &&root)
{
    qDebug()<<Q_FUNC_INFO;
    if(d->m_rootProjectNode.get() == root.get() && root)
        return;

    if (root && root->isEmpty()) {
        // Something went wrong with parsing: At least the project file needs to be
        // shown so that the user can fix the breakage.
        // Do not leak root and use default project tree in this case.
        root.reset();
    }

//    if (root) {
//        ProjectTree::applyTreeManager(root.get());
//        root->setParentFolderNode(d->m_containerNode.get());
//    }

    std::unique_ptr<ProjectNode> oldNode = std::move(d->m_rootProjectNode);

    d->m_rootProjectNode = std::move(root);
    if (oldNode || d->m_rootProjectNode)
        handleSubTreeChanged(d->m_rootProjectNode.get());
}

/*!
 \brief 应该是让tree控件生效，让model重新生成一下tree数据，应该有
 两层的关系，一层是node，一层是treeitem，

 \param node
*/
void PF_Project::handleSubTreeChanged(FolderNode* node)
{
    qDebug()<<Q_FUNC_INFO;

    PF_ProjectTree::emitSubtreeChanged(node);
//    emit fileListChanged();
}

/*!
 \brief 返回根部的节点

 \return ProjectNode
*/
ProjectNode* PF_Project::rootProjectNode() const
{
    return d->m_rootProjectNode.get();
}

PF_ProjectPrivate::PF_ProjectPrivate()
{

}

PF_ProjectPrivate::~PF_ProjectPrivate()
{
    // Make sure our root node is null when deleting the actual node
    std::unique_ptr<ProjectNode> oldNode = std::move(m_rootProjectNode);
}
