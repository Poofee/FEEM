#include "pf_node.h"

Node::~Node() = default;

NodeType Node::nodeType() const
{
    return m_nodeType;
}

QString Node::displayName() const
{
    return m_displayName;
}
