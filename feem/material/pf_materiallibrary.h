#ifndef PF_MATERIALLIBRARY_H
#define PF_MATERIALLIBRARY_H

#include <QWidget>

class QTreeView;
class Node;
class PF_MaterialTreeModel;

class PF_MaterialLibraryWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PF_MaterialLibraryWidget(QWidget *parent = nullptr);
    ~PF_MaterialLibraryWidget() override;

    Node *currentNode();
    void sync(Node *node);

    void collapseAll();
    void expandAll();

private:
    void handleCurrentItemChange(const QModelIndex &current);
    void showContextMenu(const QPoint &pos);
    void openItem(const QModelIndex &mainIndex);

    void setCurrentItem(Node *node);

    QTreeView *m_view = nullptr;
    PF_MaterialTreeModel *m_model = nullptr;

    QString m_modelId;
    bool m_autoSync = true;

signals:

public slots:
};

#endif // PF_MATERIALLIBRARY_H
