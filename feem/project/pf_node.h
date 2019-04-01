#ifndef PF_NODE_H
#define PF_NODE_H

#include <QIcon>
#include <QStringList>
/*!
 \brief 定义Node的类型

*/
enum class NodeType : quint16 {
    File = 1,
    Folder,
    Geometry,
    Component,
    Material,
    Variable,
    Parameter,
    GolobalDefinition,
    Project
};

/*!
 \brief 项目树的节点的基类，用来保存节点内的数据

*/
class Node
{
public:
    virtual ~Node();
    Node(const Node &other) = delete;

    NodeType nodeType() const;

    virtual QString displayName() const;
    virtual QString tooltip() const;

private:
    QString m_displayName;
    const NodeType m_nodeType;
};

#endif // PF_NODE_H
