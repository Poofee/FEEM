#ifndef PF_MATERIALTREEMODEL_H
#define PF_MATERIALTREEMODEL_H

#include "pf_node.h"
#include "pf_projectmodel.h"
#include "pf_material.h"

class CMaterialPropNode : public LeafNode{
public:
    CMaterialPropNode(CMaterialProp* material);
    ~CMaterialPropNode();

    CMaterialProp* m_material;
};

class PF_MaterialTreeModel : public BaseTreeModel
{
    Q_OBJECT

public:
    PF_MaterialTreeModel(QObject *parent);
    ~PF_MaterialTreeModel();

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

    bool loadBuiltinMaterials();

signals:
//    void renamed(const Utils::FileName &oldName, const Utils::FileName &newName);
//    void requestExpansion(const QModelIndex &index);

private:
//    bool m_filterProjects = false;
//    bool m_filterGeneratedFiles = true;
//    bool m_trimEmptyDirectories = true;

//    static const QLoggingCategory &logger();

    void updateSubtree(FolderNode *node);
    void rebuildModel();
    void addFolderNode(WrapperNode *parent, FolderNode *folderNode, QSet<Node *> *seen);
//    bool trimEmptyDirectories(WrapperNode *parent);

//    ExpandData expandDataForNode(const Node *node) const;
//    void loadExpandData();
//    void saveExpandData();
    void handleProjectAdded(PF_Project *project);
    void handleProjectRemoved(PF_Project *project);
    WrapperNode *nodeForProject(const PF_Project *project) const;
    void addOrRebuildProjectModel(PF_Project *project);

//    void parsingStateChanged(Project *project);

    QTimer m_timer;
//    QSet<ExpandData> m_toExpand;
    QColor m_enabledTextColor;
    QColor m_disabledTextColor;

    std::vector<std::unique_ptr<FolderNode>> nodes;
};

#endif // PF_MATERIALTREEMODEL_H
