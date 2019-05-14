#ifndef PF_NODE_H
#define PF_NODE_H

#include <QIcon>
#include <QStringList>

#include <functional>
#include <memory>

class PF_Project;

class LeafNode;/** 特质在最末端的节点 **/
class FolderNode;/** 指可以包含别的node的节点 **/
class ProjectNode;/** 项目节点 **/
//class ContainerNode;/**  **/
/*!
 \brief 定义Node的类型，并不是所有的node都有明显的继承关系，
 所以，定义单个的子类就好，有的node只能成为特定类型node的子节点，
 所以，这个地方要判断一些，不然不能作为父类。

*/
enum class NodeType : quint16 {
    Leaf = 1,
    Folder,
    Geometry,
    Mesh,
    Definition,
    Component,
    Material,
    Variable,
    Parameter,
    GolobalDefinition,
    Project
};
enum class LeafType : quint16 {
    Unknown = 0,
    Header,
    Source,
    Form,
    StateChart,
    Resource,
    QML,
    Project,
    FileTypeSize,
    CMaterialProp
};
enum ProjectAction {
    // Special value to indicate that the actions are handled by the parent
    InheritedFromParent,
    AddSubProject,
    RemoveSubProject,
    // Let's the user select to which project file
    // the file is added
    AddNewFile,
    AddExistingFile,
    // Add files, which match user defined filters,
    // from an existing directory and its subdirectories
    AddExistingDirectory,
    // Removes a file from the project, optionally also
    // delete it on disc
    RemoveFile,
    // Deletes a file from the file system, informs the project
    // that a file was deleted
    // DeleteFile is a define on windows...
    EraseFile,
    Rename,
    DuplicateFile,
    // hides actions that use the path(): Open containing folder, open terminal here and Find in Directory
    HidePathActions,
    HideFileActions,
    HideFolderActions,
    HasSubProjectRunConfigurations
};
/*!
 \brief 项目树的节点的基类，用来保存节点内的数据。

 项目树上的每一个节点都是Node，有的Node能够包含更多的Node，
 类似于枝干，而有的Node则不再细分，成为树叶。

*/
class Node
{
public:
    virtual ~Node();
    Node(const Node &other) = delete;
    Node(QString _displayname, NodeType nodetype, QIcon icon=QIcon());

    ProjectNode* parentProjectNode() const; // parent project, will be nullptr for the top-level project
    FolderNode* parentFolderNode() const; // parent folder or project

    NodeType nodeType() const;

    virtual QString displayName() const;
    virtual QString tooltip() const;
    virtual QIcon icon() const;

    void setIcon(const QIcon& icon);
    void setDisplayName(const QString& displayName);

    virtual bool supportsAction(ProjectAction action, const Node *node) const;

    virtual LeafNode *asLeafNode() { return nullptr; }
    virtual const LeafNode *asLeafNode() const { return nullptr; }
    virtual FolderNode *asFolderNode() { return nullptr; }
    virtual const FolderNode *asFolderNode() const { return nullptr; }
    virtual ProjectNode *asProjectNode() { return nullptr; }
    virtual const ProjectNode *asProjectNode() const { return nullptr; }

    void setParentFolderNode(FolderNode *parentFolder);

private:
    FolderNode *m_parentFolderNode = nullptr;
    QString m_displayName;
    const NodeType m_nodeType;
    mutable QIcon m_icon;
};

/*!
 \brief 最末端的，不能再添加子节点的节点

*/
class LeafNode : public Node
{
public:
    LeafNode(const QString &_displayname, const LeafType leafType,NodeType nodeType = NodeType::Leaf, QIcon icon=QIcon());
    ~LeafNode();

    virtual LeafNode* clone() const;

    LeafType leafType() const;

    LeafNode* asLeafNode() final { return this; }
    const LeafNode* asLeafNode() const final { return this; }

    bool supportsAction(ProjectAction action, const Node *node) const override;

private:
    LeafType m_leafType;
};

/*!
 \brief

*/
class FolderNode : public Node
{
public:
    explicit FolderNode(const QString &displayName, NodeType nodeType = NodeType::Folder, QIcon icon=QIcon());
    ~FolderNode();
//    QString displayName() const override;
//    QIcon icon() const override;

    Node* findNode(const std::function<bool(Node *)> &filter);
    QList<Node* > findNodes(const std::function<bool(Node *)> &filter);

    void forEachNode(const std::function<void(LeafNode *)> &fileTask,
                     const std::function<void(FolderNode *)> &folderTask = {},
                     const std::function<bool(const FolderNode *)> &folderFilterTask = {}) const;
    void forEachGenericNode(const std::function<void(Node *)> &genericTask) const;
    const QList<Node* > nodes() const;
    QList<LeafNode* > LeafNodes() const;

    QList<FolderNode *> folderNodes() const;

    bool isAncesterOf(Node *n);

    // takes ownership of newNode.
    // Will delete newNode if oldNode is not a child of this node.
    bool replaceSubtree(Node *oldNode, std::unique_ptr<Node> &&newNode);

    bool supportsAction(ProjectAction action, const Node *node) const override;

//    class AddNewInformation
//    {
//    public:
//        AddNewInformation(const QString &name, int p)
//            :displayName(name), priority(p)
//        { }
//        QString displayName;
//        int priority;
//    };

//    virtual AddNewInformation addNewInformation(const QStringList &files, Node *context) const;

    void addNode(std::unique_ptr<Node> &&node);
    std::unique_ptr<Node> takeNode(Node *node);

    bool isEmpty() const;

    FolderNode *asFolderNode() override { return this; }
    const FolderNode *asFolderNode() const override { return this; }

protected:
    virtual void handleSubTreeChanged(FolderNode *node);

    std::vector<std::unique_ptr<Node>> m_nodes;
};

/*!
 \brief

*/
class ProjectNode : public FolderNode
{
public:
    explicit ProjectNode(PF_Project* project);

    bool supportsAction(ProjectAction action, const Node *node) const override;

    ProjectNode* rootProjectNode() const;

    PF_Project* project() const { return m_project; }

    ProjectNode* asProjectNode() final { return this; }
    const ProjectNode* asProjectNode() const final { return this; }

    void removeAllChildren();

    void handleSubTreeChanged(FolderNode *node) final;

private:
    PF_Project *m_project;
};

#endif // PF_NODE_H
