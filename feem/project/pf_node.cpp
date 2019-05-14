#include "pf_node.h"

#include "pf_project.h"

#include <QDebug>



Node::~Node() = default;

Node::Node(QString _displayname, NodeType nodetype, QIcon icon)
    :m_displayName(_displayname)
    ,m_nodeType(nodetype)
    ,m_icon(icon)
{

}

/*!
 \brief 上一级的projectnode，因为只有一级，所以也就是根部节点，
 这里进行了递归调用。

 \return ProjectNode
*/
ProjectNode* Node::parentProjectNode() const
{
    if (!m_parentFolderNode)
        return nullptr;
    auto pn = m_parentFolderNode->asProjectNode();
    if (pn)
        return pn;
    return m_parentFolderNode->parentProjectNode();
}

/*!
 \brief 返回父节点

 \return FolderNode
*/
FolderNode* Node::parentFolderNode() const
{
    return m_parentFolderNode;
}

/*!
 \brief 返回根部project节点
// project managing this node.
// result is the container's rootProject node if this is a project container node
// (i.e. possibly null)
// or node if node is a top-level ProjectNode directly below a container
// or node->parentProjectNode() for all other cases.
 \return ProjectNode
*/
//ProjectNode* Node::managingProject()
//{
//    if (asProjectNode())
//        return asProjectNode()->rootProjectNode();
//    if(!m_parentFolderNode) return nullptr;
//    ProjectNode *pn = parentProjectNode();
//    return pn ? pn : asProjectNode(); // projects manage themselves...
//}

NodeType Node::nodeType() const
{
    return m_nodeType;
}

QString Node::displayName() const
{
    return m_displayName;
}

QString Node::tooltip() const
{
    return QString("");
}

QIcon Node::icon() const
{
    return  m_icon;
}

void Node::setIcon(const QIcon& icon)
{
    m_icon = icon;
}

void Node::setDisplayName(const QString& displayName)
{
    if(displayName == m_displayName)
        return;
    m_displayName = displayName;
}

bool Node::supportsAction(ProjectAction action, const Node* node) const
{
    return false;
}

void Node::setParentFolderNode(FolderNode* parentFolder)
{
    m_parentFolderNode = parentFolder;
}

/*!
 \brief 删除所有节点

*/
void ProjectNode::removeAllChildren()
{
    m_nodes.clear();
}



ProjectNode::ProjectNode(PF_Project *project)
    :FolderNode (project->displayName(),NodeType::Project,QIcon(":/tree/model_3d.png"))
    ,m_project(project)
{

}
/*!
 \brief

 \param action
 \param node
 \return bool
*/
bool ProjectNode::supportsAction(ProjectAction action, const Node *node) const
{
    return false;
}
/*!
 \brief

 \return ProjectNode
*/
ProjectNode* ProjectNode::rootProjectNode() const
{
    return m_project->rootProjectNode();
}



void ProjectNode::handleSubTreeChanged(FolderNode* node)
{
    m_project->handleSubTreeChanged(node);
}

LeafNode::LeafNode(const QString &_displayname, const LeafType leafType, NodeType nodeType, QIcon icon)
    :Node (_displayname,NodeType::Leaf,icon)
    ,m_leafType(leafType)
{

}

LeafNode::~LeafNode()
{

}

LeafNode* LeafNode::clone() const
{
    auto ln = new LeafNode(displayName(),leafType());
    return ln;
}

LeafType LeafNode::leafType() const
{
    return  m_leafType;
}

bool LeafNode::supportsAction(ProjectAction action, const Node *node) const
{
    return false;
}

/*!
 \brief

 \param filter
 \return Node
*/
FolderNode::FolderNode(const QString &displayName, NodeType nodeType, QIcon icon)
    :Node(displayName,nodeType,icon)
{

}

FolderNode::~FolderNode()
{
    qDebug()<<Q_FUNC_INFO;
}

Node *FolderNode::findNode(const std::function<bool(Node *)> &filter)
{
    if (filter(this))
        return this;

    for (const std::unique_ptr<Node> &n : m_nodes) {
        if (n->asLeafNode() && filter(n.get())) {
            return n.get();
        } else if (FolderNode *folder = n->asFolderNode()) {
            Node *result = folder->findNode(filter);
            if (result)
                return result;
        }
    }
    return nullptr;
}

/*!
 \brief

 \param filter
 \return QList<Node *>
*/
QList<Node *> FolderNode::findNodes(const std::function<bool(Node *)> &filter)
{
    QList<Node *> result;
    if (filter(this))
        result.append(this);
    for (const std::unique_ptr<Node> &n  : m_nodes) {
        if (n->asLeafNode() && filter(n.get()))
            result.append(n.get());
        else if (FolderNode *folder = n->asFolderNode())
            result.append(folder->findNodes(filter));
    }
    return result;
}
/*!
 \brief

 \param leafTask
 \param folderTask
 \param folderFilterTask
*/
void FolderNode::forEachNode(const std::function<void(LeafNode *)> &leafTask,
                             const std::function<void(FolderNode *)> &folderTask,
                             const std::function<bool(const FolderNode *)> &folderFilterTask) const
{
    if (folderFilterTask) {
        if (!folderFilterTask(this))
            return;
    }
    if (leafTask) {
        for (const std::unique_ptr<Node> &n : m_nodes) {
            if (LeafNode *fn = n->asLeafNode())
                leafTask(fn);
        }
    }
    for (const std::unique_ptr<Node> &n : m_nodes) {
        if (FolderNode *fn = n->asFolderNode()) {
            if (folderTask)
                folderTask(fn);
            fn->forEachNode(leafTask, folderTask, folderFilterTask);
        }
    }
}

/*!
 \brief

 \param genericTask
*/
void FolderNode::forEachGenericNode(const std::function<void(Node *)> &genericTask) const
{
    for (const std::unique_ptr<Node> &n : m_nodes) {
        genericTask(n.get());
        if (FolderNode *fn = n->asFolderNode())
            fn->forEachGenericNode(genericTask);
    }
}
const QList<Node *> FolderNode::nodes() const
{
//    return Utils::toRawPointer<QList>(m_nodes);
    QList<Node *> nodes;
    for(const std::unique_ptr<Node> & node : m_nodes){
        nodes.push_back(node.get());
    }
    return nodes;
}
/*!
 \brief 返回节点下的所有LeafNode类型的Node

 \return QList<LeafNode *>
*/
QList<LeafNode *> FolderNode::LeafNodes() const
{
    QList<LeafNode *> result;
    for (const std::unique_ptr<Node> &n : m_nodes) {
        if (LeafNode *fn = n->asLeafNode())
            result.append(fn);
    }
    return result;
}
/*!
 \brief 返回节点下所有的folder类型node

 \return QList<FolderNode *>
*/
QList<FolderNode *> FolderNode::folderNodes() const
{
    QList<FolderNode *> result;
    for (const std::unique_ptr<Node> &n : m_nodes) {
        if (FolderNode *fn = n->asFolderNode())
            result.append(fn);
    }
    return result;
}

bool FolderNode::isAncesterOf(Node *n)
{
    if (n == this)
        return true;
    FolderNode *p = n->parentFolderNode();
    while (p && p != this)
        p = p->parentFolderNode();
    return p == this;
}

bool FolderNode::replaceSubtree(Node *oldNode, std::unique_ptr<Node> &&newNode)
{
    std::unique_ptr<Node> keepAlive;
    if (!oldNode) {
        addNode(std::move(newNode)); // Happens e.g. when a project is registered
    } else {
        auto it = std::find_if(m_nodes.begin(), m_nodes.end(),
                               [oldNode](const std::unique_ptr<Node> &n) {
            return oldNode == n.get();
        });
        if(it == m_nodes.end()) return false;
        if (newNode) {
            newNode->setParentFolderNode(this);
            keepAlive = std::move(*it);
            *it = std::move(newNode);
        } else {
            keepAlive = takeNode(oldNode); // Happens e.g. when project is shutting down
        }
    }
    handleSubTreeChanged(this);
    return true;
}

bool FolderNode::supportsAction(ProjectAction action, const Node *node) const
{
    return false;
}
/*!
 \brief 添加一个node

 \param node
*/
void FolderNode::addNode(std::unique_ptr<Node> &&node)
{
    if(!node)
        return;
    if(node->parentFolderNode())
    {
        qDebug()<<"node already has parent.";
        return;
    }
    node->setParentFolderNode(this);
    m_nodes.emplace_back(std::move(node));
}

/*!
 \brief 返回列表中node的unique_ptr指针，

 \param node
 \return std::unique_ptr<Node>
*/
std::unique_ptr<Node> FolderNode::takeNode(Node *node)
{
//    return Utils::takeOrDefault(m_nodes, node);
    return std::unique_ptr<Node> ();
}

/*!
 \brief 是否为空

 \return bool
*/
bool FolderNode::isEmpty() const
{
    return m_nodes.size() == 0;
}

void FolderNode::handleSubTreeChanged(FolderNode *node)
{
    if (FolderNode *parent = parentFolderNode())
        parent->handleSubTreeChanged(node);
}
