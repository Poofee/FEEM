#ifndef PF_PROJECTEXPLORER_H
#define PF_PROJECTEXPLORER_H

#include <QObject>

class Node;

/*!
 \brief 主要实现关于project的

*/
class ProjectExplorerPlugin : public QObject
{
    Q_OBJECT
public:
    explicit ProjectExplorerPlugin(QObject *parent = nullptr);
    ~ProjectExplorerPlugin();

    static ProjectExplorerPlugin *instance();

    bool initialize();

    static bool openProject(const QString &fileName);
    static bool openProjects(const QStringList &fileNames);

    static void showContextMenu(QWidget *view, const QPoint &globalPos, Node *node);

    /** 与treemodel之间的接口 **/


signals:

public slots:
};

#endif // PF_PROJECTEXPLORER_H
