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

    class OpenProjectResult
    {
    public:
        OpenProjectResult(const QList<PF_Project *> &projects, const QList<PF_Project *> &alreadyOpen,
                          const QString &errorMessage)
            : m_projects(projects), m_alreadyOpen(alreadyOpen),
              m_errorMessage(errorMessage)
        { }

        explicit operator bool() const
        {
            return m_errorMessage.isEmpty() && m_alreadyOpen.isEmpty();
        }

        PF_Project *project() const
        {
            return m_projects.isEmpty() ? nullptr : m_projects.first();
        }

        QList<PF_Project *> projects() const
        {
            return m_projects;
        }

        QString errorMessage() const
        {
            return m_errorMessage;
        }

        QList<PF_Project *> alreadyOpen() const
        {
            return m_alreadyOpen;
        }
    private:
        QList<PF_Project *> m_projects;
        QList<PF_Project *> m_alreadyOpen;
        QString m_errorMessage;
    };

    static OpenProjectResult openProject(const QString &fileName);
    static OpenProjectResult openProjects(const QStringList &fileNames);

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
