#include "pf_node.h"

Node::~Node() = default;

Node::Node(QString _displayname, NodeType nodetype, QIcon icon)
    :m_displayName(_displayname)
    ,m_nodeType(nodetype)
    ,m_icon(icon)
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
