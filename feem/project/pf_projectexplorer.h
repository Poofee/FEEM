#ifndef PF_PROJECTEXPLORER_H
#define PF_PROJECTEXPLORER_H

#include <QObject>

class Node;
class PF_Project;
class Id;

/*!
 \brief 主要实现关于project的

*/
class PF_ProjectExplorerPlugin : public QObject
{
    Q_OBJECT
public:
    explicit PF_ProjectExplorerPlugin(QObject *parent = nullptr);
    ~PF_ProjectExplorerPlugin();

    static PF_ProjectExplorerPlugin *instance();

    bool initialize();

    static bool openProject(const QString &fileName);
    static bool openProjects(const QStringList &fileNames);

    static void showContextMenu(QWidget *view, const QPoint &globalPos, Node *node);

//    static void setProjectExplorerSettings(const Internal::ProjectExplorerSettings &pes);
//    static const Internal::ProjectExplorerSettings &projectExplorerSettings();

//    static void startRunControl(RunControl *runControl);
//    static void showRunErrorMessage(const QString &errorMessage);

    // internal public for FlatModel
//    static void renameFile(Node *node, const QString &newFilePath);
//    static QStringList projectFilePatterns();
//    static bool isProjectFile(const Utils::FileName &filePath);
    static QList<QPair<QString, QString> > recentProjects();

    static bool canRunStartupProject(Id runMode, QString *whyNot = nullptr);
    static void runProject(PF_Project *pro, Id, const bool forceSkipDeploy = false);
    static void runStartupProject(Id runMode, bool forceSkipDeploy = false);
//    static void runRunConfiguration(RunConfiguration *rc, Core::Id runMode,
//                             const bool forceSkipDeploy = false);
//    static QList<QPair<Runnable, Utils::ProcessHandle>> runningRunControlProcesses();

//    static void addExistingFiles(FolderNode *folderNode, const QStringList &filePaths);

    static void buildProject(PF_Project *p);

//    static void initiateInlineRenaming();

//    static QString displayNameForStepId(Core::Id stepId);

//    static QString directoryFor(Node *node);
//    static QStringList projectFileGlobs();

    static void updateContextMenuActions();

//    static QThreadPool *sharedThreadPool();

    static void openNewProjectDialog();
    static void openOpenProjectDialog();

signals:
    void finishedInitialization();

    // Is emitted when a project has been added/removed,
    // or the file list of a specific project has changed.
    void fileListChanged();

    void recentProjectsChanged();

//    void settingsChanged();

    void updateRunActions();

private:
    static bool coreAboutToClose();
};

#endif // PF_PROJECTEXPLORER_H
