#ifndef PF_PROJECTTREE_H
#define PF_PROJECTTREE_H

#include <QObject>

class Node;
class FolderNode;

class PF_ProjectTreeWidget;
class PF_Project;
/*!
 \brief 用来管理多个tree的接口，保存状态

*/
class PF_ProjectTree : public QObject
{
    Q_OBJECT
public:
    explicit PF_ProjectTree(QObject *parent = nullptr);
    ~PF_ProjectTree() override;

    static PF_ProjectTree *instance();

    // Integration with ProjectTreeWidget
    static void registerWidget(PF_ProjectTreeWidget *widget);
    static void unregisterWidget(PF_ProjectTreeWidget *widget);
    static void nodeChanged(PF_ProjectTreeWidget *widget);

    // Node
    static bool hasNode(const Node *node);
    static PF_Project *projectForNode(const Node *node);

    //
    static PF_Project *currentProject();
    static Node *findCurrentNode();

    static void showContextMenu(PF_ProjectTreeWidget *focus, const QPoint &globalPos, Node *node);

    static void highlightProject(PF_Project *project, const QString &message);

    void collapseAll();
    void expandAll();

    // for nodes to emit signals, do not call unless you are a node
    static void emitSubtreeChanged(FolderNode *node);
signals:
    void currentProjectChanged(PF_Project *project);
    void currentNodeChanged();

    // Emitted whenever the model needs to send a update signal.
    void subtreeChanged(FolderNode *node);

    void aboutToShowContextMenu(PF_Project *project, Node *node);

    // Emitted on any change to the tree
    void treeChanged();
public slots:
private:
    void sessionAndTreeChanged();
    void sessionChanged();
    void update();
    void updateFromProjectTreeWidget(PF_ProjectTreeWidget *widget);
//    void updateFromDocumentManager();
    void updateFromNode(Node *node);
    void setCurrent(Node *node, PF_Project *project);
    void updateContext();

    void updateFromFocus();

//    void updateExternalFileWarning();
    static bool hasFocus(PF_ProjectTreeWidget *widget);
    void hideContextMenu();
private:
    static PF_ProjectTree* s_instance;
    PF_ProjectTreeWidget* m_projecttreewidget = nullptr;
    Node *m_currentNode = nullptr;
    PF_Project* m_currentProject = nullptr;
};

#endif // PF_PROJECTTREE_H
