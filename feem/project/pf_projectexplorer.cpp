#include "pf_projectexplorer.h"
#include "pf_projecttree.h"
#include "pf_project.h"
#include "pf_sessionmanager.h"
#include "actionmanager/actionmanager.h"
#include "actionmanager/actioncontainer.h"
#include "actionmanager/command.h"

#include <QAction>
#include <QMenu>

/*!
 \brief projectexplore实现的私有类，具体的实现都放在这里

*/
class PF_ProjectExplorerPluginPrivate : public QObject
{
public:
//    void deploy(QList<Project *>);
//    int queue(QList<Project *>, QList<Id> stepIds);
    void updateContextMenuActions();
//    void updateLocationSubMenus();
//    void executeRunConfiguration(RunConfiguration *, Core::Id mode);
//    QPair<bool, QString> buildSettingsEnabledForSession();
//    QPair<bool, QString> buildSettingsEnabled(const Project *pro);

    void addToRecentProjects(const QString &fileName, const QString &displayName);
//    void startRunControl(RunControl *runControl);

    void updateActions();
    void updateContext();
//    void updateDeployActions();
//    void updateRunWithoutDeployMenu();

    void buildQueueFinished(bool success);

    void buildStateChanged(PF_Project * pro);
    void loadAction();
    void handleUnloadProject();
    void unloadProjectContextMenu();
    void closeAllProjects();
//    void showSessionManager();
    void updateSessionMenu();
    void setSession(QAction *action);

//    void determineSessionToRestoreAtStartup();
    void restoreSession();
    void runProjectContextMenu();
//    void savePersistentSettings();

//    void addNewFile();
//    void handleAddExistingFiles();
//    void addExistingDirectory();
//    void addNewSubproject();
    void removeProject();
//    void openFile();
//    void searchOnFileSystem();
//    void showInGraphicalShell();
//    void removeFile();
//    void duplicateFile();
//    void deleteFile();
//    void handleRenameFile();
    void handleSetStartupProject();
    void setStartupProject(PF_Project *project);

    void updateRecentProjectMenu();
    void clearRecentProjects();
    void openRecentProject(const QString &fileName);
    void updateUnloadProjectMenu();
//    using EnvironmentGetter = std::function<Utils::optional<Utils::Environment>(const Project *project)>;
//    void openTerminalHere(const EnvironmentGetter &env);

    void invalidateProject(PF_Project *project);

    void projectAdded(PF_Project *pro);
    void projectRemoved(PF_Project *pro);
    void projectDisplayNameChanged(PF_Project *pro);
    void startupProjectChanged(); // Calls updateRunAction
    void activeTargetChanged();
    void activeRunConfigurationChanged();
    void activeBuildConfigurationChanged();

    void slotUpdateRunActions();

//    void currentModeChanged(Core::Id mode, Core::Id oldMode);

//    void updateWelcomePage();

//    void runConfigurationConfigurationFinished();

//    void checkForShutdown();
//    void timerEvent(QTimerEvent *) override;

    QList<QPair<QString, QString> > recentProjects() const;
public:
    QMenu *m_sessionMenu;
    QMenu *m_openWithMenu;
    QMenu *m_openTerminalMenu;

    QMultiMap<int, QObject*> m_actionMap;
    QAction *m_sessionManagerAction;
    QAction *m_newAction;
    QAction *m_loadAction;
//    Utils::ParameterAction *m_unloadAction;
//    Utils::ParameterAction *m_unloadActionContextMenu;
    QAction *m_closeAllProjects;
    QAction *m_buildProjectOnlyAction;
//    Utils::ParameterAction *m_buildAction;
//    Utils::ProxyAction *m_modeBarBuildAction;
    QAction *m_buildActionContextMenu;
//    QAction *m_buildDependenciesActionContextMenu;
    QAction *m_buildSessionAction;
    QAction *m_rebuildProjectOnlyAction;
//    Utils::ParameterAction *m_rebuildAction;
//    QAction *m_rebuildActionContextMenu;
//    QAction *m_rebuildDependenciesActionContextMenu;
//    QAction *m_rebuildSessionAction;
//    QAction *m_cleanProjectOnlyAction;
//    QAction *m_deployProjectOnlyAction;
//    Utils::ParameterAction *m_deployAction;
//    QAction *m_deployActionContextMenu;
//    QAction *m_deploySessionAction;
//    Utils::ParameterAction *m_cleanAction;
//    QAction *m_cleanActionContextMenu;
//    QAction *m_cleanDependenciesActionContextMenu;
    QAction *m_cleanSessionAction;
    QAction *m_runAction;
    QAction *m_runActionContextMenu;
//    QAction *m_runWithoutDeployAction;
    QAction *m_cancelBuildAction;
//    QAction *m_addNewFileAction;
//    QAction *m_addExistingFilesAction;
//    QAction *m_addExistingDirectoryAction;
//    QAction *m_addNewSubprojectAction;
//    QAction *m_removeFileAction;
//    QAction *m_duplicateFileAction;
    QAction *m_removeProjectAction;
//    QAction *m_deleteFileAction;
//    QAction *m_renameFileAction;
//    QAction *m_filePropertiesAction = nullptr;
//    QAction *m_diffFileAction;
//    QAction *m_openFileAction;
//    QAction *m_projectTreeCollapseAllAction;
//    QAction *m_searchOnFileSystem;
//    QAction *m_showInGraphicalShell;
//    QAction *m_openTerminalHere;
//    QAction *m_openTerminalHereBuildEnv;
//    QAction *m_openTerminalHereRunEnv;
//    Utils::ParameterAction *m_setStartupProjectAction;
//    QAction *m_projectSelectorAction;
//    QAction *m_projectSelectorActionMenu;
//    QAction *m_projectSelectorActionQuick;
//    QAction *m_runSubProject;

//    ProjectWindow *m_proWindow = nullptr;
//    QString m_sessionToRestoreAtStartup;

//    QStringList m_profileMimeTypes;
//    int m_activeRunControlCount = 0;
//    int m_shutdownWatchDogId = -1;

//    QHash<QString, std::function<Project *(const Utils::FileName &)>> m_projectCreators;
    QList<QPair<QString, QString> > m_recentProjects; // pair of filename, displayname
    static const int m_maxRecentProjects = 25;

    QString m_lastOpenDirectory;
//    QPointer<RunConfiguration> m_delayedRunConfiguration;
//    QList<QPair<RunConfiguration *, Core::Id>> m_delayedRunConfigurationForRun;
//    QString m_projectFilterString;
//    MiniProjectTargetSelector * m_targetSelector;
//    ProjectExplorerSettings m_projectExplorerSettings;
//    bool m_shouldHaveRunConfiguration = false;
//    bool m_shuttingDown = false;
//    Core::Id m_runMode = Constants::NO_RUN_MODE;

//    KitManager *m_kitManager = nullptr;
//    ToolChainManager *m_toolChainManager = nullptr;
//    QStringList m_arguments;

//    QThreadPool m_threadPool;

//    DeviceManager m_deviceManager{true};

//    GccToolChainFactory m_gccToolChainFactory;
//    ClangToolChainFactory m_clangToolChainFactory;
//    CustomToolChainFactory m_customToolChainFactory;

//    DesktopDeviceFactory m_desktopDeviceFactory;

//    ToolChainOptionsPage m_toolChainOptionsPage;
//    KitOptionsPage m_kitOptionsPage;

//    TaskHub m_taskHub;

//    ProjectWelcomePage m_welcomePage;

//    CustomWizardMetaFactory<CustomProjectWizard> m_customProjectWizard{IWizardFactory::ProjectWizard};
//    CustomWizardMetaFactory<CustomWizard> m_fileWizard{IWizardFactory::FileWizard};

//    ProjectsMode m_projectsMode;

//    CopyTaskHandler m_copyTaskHandler;
//    ShowInEditorTaskHandler m_showInEditorTaskHandler;
//    VcsAnnotateTaskHandler m_vcsAnnotateTaskHandler;
//    RemoveTaskHandler m_removeTaskHandler;
//    ConfigTaskHandler m_configTaskHandler{Task::compilerMissingTask(), Constants::KITS_SETTINGS_PAGE_ID};

    PF_SessionManager m_sessionManager;
//    AppOutputPane m_outputPane;

    PF_ProjectTree m_projectTree;

//    AllProjectsFilter m_allProjectsFilter;
//    CurrentProjectFilter m_currentProjectFilter;

//    ProcessStepFactory m_processStepFactory;

//    AllProjectsFind m_allProjectsFind;
//    CurrentProjectFind m_curretProjectFind;

//    CustomExecutableRunConfigurationFactory m_customExecutableRunConfigFactory;

//    ProjectFileWizardExtension m_projectFileWizardExtension;

    // Settings pages
//    ProjectExplorerSettingsPage m_projectExplorerSettingsPage;
//    DeviceSettingsPage m_deviceSettingsPage;

//    ProjectTreeWidgetFactory m_projectTreeFactory;
//    FolderNavigationWidgetFactory m_folderNavigationWidgetFactory;
//    DefaultDeployConfigurationFactory m_defaultDeployConfigFactory;

//    IDocumentFactory m_documentFactory;
};

static PF_ProjectExplorerPlugin* m_instance = nullptr;
static PF_ProjectExplorerPluginPrivate*  dd = nullptr;

PF_ProjectExplorerPlugin::PF_ProjectExplorerPlugin(QObject *parent) : QObject(parent)
{
    m_instance = this;
}

PF_ProjectExplorerPlugin::~PF_ProjectExplorerPlugin()
{
    if(dd)
        delete dd;
    dd = nullptr;
    m_instance = nullptr;
}

PF_ProjectExplorerPlugin* PF_ProjectExplorerPlugin::instance()
{
    return m_instance;
}

bool PF_ProjectExplorerPlugin::initialize()
{
    dd = new PF_ProjectExplorerPluginPrivate;

    PF_SessionManager *sessionManager = &dd->m_sessionManager;
    connect(sessionManager, &PF_SessionManager::projectAdded,
            this, &PF_ProjectExplorerPlugin::fileListChanged);
    connect(sessionManager, &PF_SessionManager::aboutToRemoveProject,
            dd, &PF_ProjectExplorerPluginPrivate::invalidateProject);
    connect(sessionManager, &PF_SessionManager::projectRemoved,
            this, &PF_ProjectExplorerPlugin::fileListChanged);
    connect(sessionManager, &PF_SessionManager::projectAdded,
            dd, &PF_ProjectExplorerPluginPrivate::projectAdded);
    connect(sessionManager, &PF_SessionManager::projectRemoved,
            dd, &PF_ProjectExplorerPluginPrivate::projectRemoved);
    connect(sessionManager, &PF_SessionManager::startupProjectChanged,
            dd, &PF_ProjectExplorerPluginPrivate::startupProjectChanged);
    connect(sessionManager, &PF_SessionManager::projectDisplayNameChanged,
            dd, &PF_ProjectExplorerPluginPrivate::projectDisplayNameChanged);
//    connect(sessionManager, &PF_SessionManager::dependencyChanged,
//            dd, &PF_ProjectExplorerPluginPrivate::updateActions);
    connect(sessionManager, &PF_SessionManager::sessionLoaded,
            dd, &PF_ProjectExplorerPluginPrivate::updateActions);
//    connect(sessionManager, &PF_SessionManager::sessionLoaded,
//            dd, &PF_ProjectExplorerPluginPrivate::updateWelcomePage);

    PF_ProjectTree *tree = &dd->m_projectTree;
    connect(tree, &PF_ProjectTree::currentProjectChanged,
            dd, &PF_ProjectExplorerPluginPrivate::updateContextMenuActions);
    connect(tree, &PF_ProjectTree::currentNodeChanged,
            dd, &PF_ProjectExplorerPluginPrivate::updateContextMenuActions);
    connect(tree, &PF_ProjectTree::currentProjectChanged,
            dd, &PF_ProjectExplorerPluginPrivate::updateActions);
//    connect(tree, &PF_ProjectTree::currentProjectChanged, this, [](Project *project) {
//        TextEditor::FindInFiles::instance()->setBaseDirectory(project ? project->projectDirectory()
//                                                                      : Utils::FileName());
//    });

    //   actions

    return true;
}

void PF_ProjectExplorerPluginPrivate::updateContextMenuActions()
{

}

void PF_ProjectExplorerPluginPrivate::addToRecentProjects(const QString &fileName, const QString &displayName)
{

}

void PF_ProjectExplorerPluginPrivate::updateActions()
{

}

void PF_ProjectExplorerPluginPrivate::updateContext()
{

}

void PF_ProjectExplorerPluginPrivate::buildQueueFinished(bool success)
{

}

void PF_ProjectExplorerPluginPrivate::buildStateChanged(PF_Project *pro)
{

}

void PF_ProjectExplorerPluginPrivate::loadAction()
{

}

void PF_ProjectExplorerPluginPrivate::handleUnloadProject()
{

}

void PF_ProjectExplorerPluginPrivate::unloadProjectContextMenu()
{

}

void PF_ProjectExplorerPluginPrivate::closeAllProjects()
{

}

void PF_ProjectExplorerPluginPrivate::updateSessionMenu()
{

}

void PF_ProjectExplorerPluginPrivate::setSession(QAction *action)
{

}

void PF_ProjectExplorerPluginPrivate::restoreSession()
{

}

void PF_ProjectExplorerPluginPrivate::runProjectContextMenu()
{

}

void PF_ProjectExplorerPluginPrivate::removeProject()
{

}

void PF_ProjectExplorerPluginPrivate::handleSetStartupProject()
{

}

void PF_ProjectExplorerPluginPrivate::setStartupProject(PF_Project *project)
{

}

void PF_ProjectExplorerPluginPrivate::updateRecentProjectMenu()
{

}

void PF_ProjectExplorerPluginPrivate::clearRecentProjects()
{

}

void PF_ProjectExplorerPluginPrivate::openRecentProject(const QString &fileName)
{

}

void PF_ProjectExplorerPluginPrivate::updateUnloadProjectMenu()
{

}

void PF_ProjectExplorerPluginPrivate::invalidateProject(PF_Project *project)
{

}

void PF_ProjectExplorerPluginPrivate::projectAdded(PF_Project *pro)
{

}

void PF_ProjectExplorerPluginPrivate::projectRemoved(PF_Project *pro)
{

}

void PF_ProjectExplorerPluginPrivate::projectDisplayNameChanged(PF_Project *pro)
{

}

void PF_ProjectExplorerPluginPrivate::startupProjectChanged()
{

}

void PF_ProjectExplorerPluginPrivate::activeTargetChanged()
{

}

void PF_ProjectExplorerPluginPrivate::activeRunConfigurationChanged()
{

}

void PF_ProjectExplorerPluginPrivate::activeBuildConfigurationChanged()
{

}

void PF_ProjectExplorerPluginPrivate::slotUpdateRunActions()
{

}

QList<QPair<QString, QString> > PF_ProjectExplorerPluginPrivate::recentProjects() const
{
    return m_recentProjects;
}
