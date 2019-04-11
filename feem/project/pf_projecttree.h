#ifndef PF_PROJECTTREE_H
#define PF_PROJECTTREE_H

#include <QObject>

class Node;

class PF_ProjectTreeWidget;
class PF_Project;
/*!
 \brief

*/
class PF_ProjectTree : public QObject
{
    Q_OBJECT
public:
    explicit PF_ProjectTree(QObject *parent = nullptr);


    static void showContextMenu(PF_ProjectTreeWidget *focus, const QPoint &globalPos, Node *node);

signals:

public slots:

private:
    PF_ProjectTreeWidget* m_projecttreewidget = nullptr;
    PF_Project* m_currentproject = nullptr;

};

#endif // PF_PROJECTTREE_H
