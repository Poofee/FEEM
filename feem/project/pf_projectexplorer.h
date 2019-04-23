#ifndef PF_PROJECTEXPLORER_H
#define PF_PROJECTEXPLORER_H

#include <QObject>

class ProjectExplorerPlugin : public QObject
{
    Q_OBJECT
public:
    explicit ProjectExplorerPlugin(QObject *parent = nullptr);

    static ProjectExplorerPlugin *instance();

    static bool openProject(const QString &fileName);
    static bool openProjects(const QStringList &fileNames);

    /** 与treemodel之间的接口 **/


signals:

public slots:
};

#endif // PF_PROJECTEXPLORER_H
