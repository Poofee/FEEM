#ifndef PF_PROJECTTREE_H
#define PF_PROJECTTREE_H

#include <QObject>

class Node;

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

    static PF_Project *currentProject();
    static Node *findCurrentNode();

    static void showContextMenu(PF_ProjectTreeWidget *focus, const QPoint &globalPos, Node *node);

signals:

public slots:

private:
    static PF_ProjectTree* s_instance;
    PF_ProjectTreeWidget* m_projecttreewidget = nullptr;
    Node *m_currentNode = nullptr;
    PF_Project* m_currentproject = nullptr;

};

#endif // PF_PROJECTTREE_H
