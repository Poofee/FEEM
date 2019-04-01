#ifndef PF_PROJECTMODEL_H
#define PF_PROJECTMODEL_H

#include "treemodel.h"

#include <QPointer>
#include <QSet>
#include <QTimer>
#include <QTreeView>

class Node;


/*!
 \brief 该类用来存放不同的节点数据

*/
class WrapperNode : public TreeItem
{
public:
    explicit WrapperNode(Node *node) : m_node(node) {}
    Node *m_node = nullptr;
};

class PF_ProjectModel : public BaseTreeModel
{
    Q_OBJECT

public:
    PF_ProjectModel(QObject *parent);

    // QAbstractItemModel
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

//    Qt::DropActions supportedDragActions() const override;
//    QStringList mimeTypes() const override;
//    QMimeData *mimeData(const QModelIndexList &indexes) const override;

    Node *nodeForIndex(const QModelIndex &index) const;
    WrapperNode *wrapperForNode(const Node *node) const;
    QModelIndex indexForNode(const Node *node) const;

//    bool projectFilterEnabled();
//    bool generatedFilesFilterEnabled();
//    void setProjectFilterEnabled(bool filter);
//    void setGeneratedFilesFilterEnabled(bool filter);
//    void setTrimEmptyDirectories(bool filter);

    void onExpanded(const QModelIndex &idx);
    void onCollapsed(const QModelIndex &idx);

signals:
//    void renamed(const Utils::FileName &oldName, const Utils::FileName &newName);
//    void requestExpansion(const QModelIndex &index);

private:
//    bool m_filterProjects = false;
//    bool m_filterGeneratedFiles = true;
//    bool m_trimEmptyDirectories = true;

//    static const QLoggingCategory &logger();

//    void updateSubtree(FolderNode *node);
//    void rebuildModel();
//    void addFolderNode(WrapperNode *parent, FolderNode *folderNode, QSet<Node *> *seen);
//    bool trimEmptyDirectories(WrapperNode *parent);

//    ExpandData expandDataForNode(const Node *node) const;
//    void loadExpandData();
//    void saveExpandData();
//    void handleProjectAdded(Project *project);
//    void handleProjectRemoved(Project *project);
//    WrapperNode *nodeForProject(const Project *project) const;
//    void addOrRebuildProjectModel(Project *project);

//    void parsingStateChanged(Project *project);

    QTimer m_timer;
//    QSet<ExpandData> m_toExpand;
    QColor m_enabledTextColor;
    QColor m_disabledTextColor;
};

#endif // PF_PROJECTMODEL_H
