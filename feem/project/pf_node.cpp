#include "pf_node.h"

Node::~Node() = default;

Node::Node(QString _displayname, NodeType nodetype)
    :m_displayName(_displayname)
    ,m_nodeType(nodetype)
{

}

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
