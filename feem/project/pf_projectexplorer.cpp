#include "pf_projectexplorer.h"
#include "pf_projecttree.h"
#include "pf_project.h"
#include "pf_sessionmanager.h"
#include "actionmanager/actionmanager.h"
#include "actionmanager/actioncontainer.h"
#include "actionmanager/command.h"
#include "projectexplorerconstants.h"
#include "pf_magmaterialdialog.h"
#include "pf_node.h"

#include <QAction>
#include <QMenu>
#include <QDir>
#include <QFileInfo>
#include <QDebug>

namespace Constants {
const int  P_MODE_SESSION         = 85;

// Actions
const char ADD3DMODEL[] = "ProjectExplorer.Add3DModel";
const char ADD2DAXIS[] = "ProjectExplorer.Add2DAxisymmetric";
const char ADD2DMODEL[] = "ProjectExplorer.Add2DModel";

const char ADDSTATICMAG[] = "ProjectExplorer.AddStaticMagnetic";
const char ADDTRANSIENTMAG[] = "ProjectExplorer.AddTransientMagnetic";
const char ADDHEAT[] = "ProjectExplorer.AddHeat";

const char ADDMATERIAL[] = "ProjectExplorer.AddMaterial";
const char ADDBLANKMATERIAL[] = "ProjectExplorer.AddBlankMaterial";

const char HELP[] = "ProjectExplorer.Help";

// Action priorities
const int  P_ACTION_RUN            = 100;
const int  P_ACTION_BUILDPROJECT   = 80;

// Context
const char C_PROJECTEXPLORER[]    = "Project Explorer";

// Menus
const char M_RECENTPROJECTS[]     = "ProjectExplorer.Menu.Recent";
const char M_UNLOADPROJECTS[]     = "ProjectExplorer.Menu.Unload";
const char M_SESSION[]            = "ProjectExplorer.Menu.Session";

const char RUNMENUCONTEXTMENU[]   = "Project.RunMenu";
const char FOLDER_OPEN_LOCATIONS_CONTEXT_MENU[]  = "Project.F.OpenLocation.CtxMenu";
const char PROJECT_OPEN_LOCATIONS_CONTEXT_MENU[]  = "Project.P.OpenLocation.CtxMenu";

} // namespace Constants

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

    void addBlankMaterial();
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
    QMenu* m_addmodel;
    QMenu* m_addstudy;

    QMultiMap<int, QObject*> m_actionMap;
    QAction* m_add3Dmodel;
    QAction* m_add2DAxismodel;
    QAction* m_add2Dmodel;
    QAction* m_addStaticMag;
    QAction* m_addTransientMag;
    QAction* m_addHeat;

    QAction* m_addMaterial;
    QAction* m_addBlankMaterial;

    QAction* m_solve;

    QAction* m_help;

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
    qDebug()<<Q_FUNC_INFO;
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

    Context projecTreeContext(Constants::C_PROJECT_TREE);
    //   actions

    /** 项目 节点 **/
    ActionContainer *mprojectContextMenu =
        ActionManager::createMenu(Constants::M_PROJECTCONTEXT);
    mprojectContextMenu->appendGroup(Constants::G_PROJECT_ADD);
    mprojectContextMenu->appendGroup(Constants::G_PROJECT_SOLVE);
    mprojectContextMenu->appendGroup(Constants::G_HELP);

    /** 全局定义 节点 **/
    ActionContainer *mglobaldefsContextMenu =
        ActionManager::createMenu(Constants::M_GLOBALDEFSCONTEXT);

    /** 材料 节点 **/
    ActionContainer *mmaterialContextMenu =
        ActionManager::createMenu(Constants::M_MATERIALCONTEXT);
    mmaterialContextMenu->appendGroup(Constants::G_HELP);

    /** 分网 节点 **/
    ActionContainer *mmeshContextMenu =
        ActionManager::createMenu(Constants::M_MESHCONTEXT);

    /** 几何 节点 **/
    ActionContainer *mgeometryContextMenu =
        ActionManager::createMenu(Constants::M_GEOMETRYCONTEXT);

    /** 模型 节点 **/
    ActionContainer *mcomponentContextMenu =
        ActionManager::createMenu(Constants::M_COMPONENTCONTEXT);

    ActionContainer * const addModel =
            ActionManager::createMenu(Constants::M_ADDMODELCONTEXT);
    addModel->setOnAllDisabledBehavior(ActionContainer::Show);
    dd->m_addmodel = addModel->menu();
    dd->m_addmodel->setTitle(tr("Add Model"));
    dd->m_addmodel->setIcon(QIcon(":/tree/model_3d.png"));

    ActionContainer * const addStudy =
            ActionManager::createMenu(Constants::M_ADDSTUDYCONTEXT);
    addStudy->setOnAllDisabledBehavior(ActionContainer::Show);
    dd->m_addstudy = addStudy->menu();
    dd->m_addstudy->setTitle(tr("Add Study"));
    dd->m_addstudy->setIcon(QIcon(":/add_study.png"));

    Command* cmd;

    /************add model******************/
    dd->m_add3Dmodel = new QAction(QIcon(":/tree/model_3d.png"),tr("3D"), this);
    cmd = ActionManager::registerAction(dd->m_add3Dmodel, Constants::ADD3DMODEL);
//    cmd->setDefaultKeySequence(QKeySequence(tr("Ctrl+Shift+N")));
    addModel->addAction(cmd,Constants::G_DEFAULT_ONE);

    dd->m_add2DAxismodel = new QAction(QIcon(":/tree/model_2d_axi.png"),tr("2D Axisymmetric"), this);
    cmd = ActionManager::registerAction(dd->m_add2DAxismodel, Constants::ADD2DAXIS);
//    cmd->setDefaultKeySequence(QKeySequence(tr("Ctrl+Shift+N")));
    addModel->addAction(cmd,Constants::G_DEFAULT_ONE);

    dd->m_add2Dmodel = new QAction(QIcon(":/tree/model_2d.png"),tr("2D"), this);
    cmd = ActionManager::registerAction(dd->m_add2Dmodel, Constants::ADD2DMODEL);
    //    cmd->setDefaultKeySequence(QKeySequence(tr("Ctrl+Shift+N")));
    addModel->addAction(cmd,Constants::G_DEFAULT_ONE);

    mprojectContextMenu->addMenu(addModel,Constants::G_PROJECT_ADD);

    /************add study******************/
    dd->m_addStaticMag = new QAction(QIcon(":/tree/magnetic_fields.png"),tr("Static Magnetic Field"), this);
    cmd = ActionManager::registerAction(dd->m_addStaticMag, Constants::ADDSTATICMAG);
    //    cmd->setDefaultKeySequence(QKeySequence(tr("Ctrl+Shift+N")));
    addStudy->addAction(cmd,Constants::G_DEFAULT_ONE);

    dd->m_addTransientMag = new QAction(QIcon(":/tree/phys_magnetic_fields_no_currents.png"),tr("Transient Magnetic Field"), this);
    cmd = ActionManager::registerAction(dd->m_addTransientMag, Constants::ADDTRANSIENTMAG);
    //    cmd->setDefaultKeySequence(QKeySequence(tr("Ctrl+Shift+N")));
    addStudy->addAction(cmd,Constants::G_DEFAULT_ONE);

    dd->m_addHeat = new QAction(QIcon(":/tree/modlib_heat.png"),tr("Heat Field"), this);
    cmd = ActionManager::registerAction(dd->m_addHeat, Constants::ADDHEAT);
    //    cmd->setDefaultKeySequence(QKeySequence(tr("Ctrl+Shift+N")));
    addStudy->addAction(cmd,Constants::G_DEFAULT_ONE);

    mprojectContextMenu->addMenu(addStudy,Constants::G_PROJECT_ADD);

    /************material******************/
    dd->m_addMaterial = new QAction(QIcon(":/material_picker.png"),tr("add Material"), this);
    cmd = ActionManager::registerAction(dd->m_addMaterial, Constants::ADDMATERIAL);
    //    cmd->setDefaultKeySequence(QKeySequence(tr("Ctrl+Shift+N")));
    mmaterialContextMenu->addAction(cmd,Constants::G_DEFAULT_ONE);

    dd->m_addBlankMaterial = new QAction(QIcon(":/more_materials.png"),tr("add Blank Material"), this);
    cmd = ActionManager::registerAction(dd->m_addBlankMaterial, Constants::ADDBLANKMATERIAL);
    //    cmd->setDefaultKeySequence(QKeySequence(tr("Ctrl+Shift+N")));

    mmaterialContextMenu->addAction(cmd,Constants::G_DEFAULT_ONE);


    // help
    dd->m_help = new QAction(QIcon(":/tree/help_16.png"),tr("Help"), this);
    cmd = ActionManager::registerAction(dd->m_help, Constants::HELP);
    //    cmd->setDefaultKeySequence(QKeySequence(tr("Ctrl+Shift+N")));
    mmaterialContextMenu->addSeparator(Constants::G_HELP);
    mmaterialContextMenu->addAction(cmd,Constants::G_HELP);
    mprojectContextMenu->addSeparator(Constants::G_HELP);
    mprojectContextMenu->addAction(cmd,Constants::G_HELP);

    connect(dd->m_addBlankMaterial,&QAction::triggered,dd,&PF_ProjectExplorerPluginPrivate::addBlankMaterial);
    return true;
}

PF_ProjectExplorerPlugin::OpenProjectResult PF_ProjectExplorerPlugin::openProject(const QString &fileName)
{
    OpenProjectResult result = openProjects(QStringList(fileName));
    PF_Project *project = result.project();
    if (!project)
        return result;
    dd->addToRecentProjects(fileName, project->displayName());
    PF_SessionManager::setStartupProject(project);
//    project->projectLoaded();
    return result;
}

PF_ProjectExplorerPlugin::OpenProjectResult PF_ProjectExplorerPlugin::openProjects(const QStringList &fileNames)
{
    QList<PF_Project*> openedPro;
    QList<PF_Project *> alreadyOpen;
    QString errorString;
//    foreach (const QString &fileName, fileNames) {
//        if(fileName.isEmpty()) continue;

//        const QFileInfo fi(fileName);
////        const auto filePath = Utils::FileName::fromString(fi.absoluteFilePath());
////        PF_Project *found = Utils::findOrDefault(SessionManager::projects(),
////                                              Utils::equal(&Project::projectFilePath, filePath));
//        if (found) {
//            alreadyOpen.append(found);
//            SessionManager::reportProjectLoadingProgress();
//            continue;
//        }

//        Utils::MimeType mt = Utils::mimeTypeForFile(fileName);
//        if (ProjectManager::canOpenProjectForMimeType(mt)) {
//            if (!filePath.toFileInfo().isFile()) {
//                appendError(errorString,
//                            tr("Failed opening project \"%1\": Project is not a file.").arg(fileName));
//            } else if (Project *pro = ProjectManager::openProject(mt, filePath)) {
//                QObject::connect(pro, &Project::parsingFinished, [pro]() {
//                    emit SessionManager::instance()->projectFinishedParsing(pro);
//                });
//                QString restoreError;
//                Project::RestoreResult restoreResult = pro->restoreSettings(&restoreError);
//                if (restoreResult == Project::RestoreResult::Ok) {
//                    connect(pro, &Project::fileListChanged,
//                            m_instance, &ProjectExplorerPlugin::fileListChanged);
//                    SessionManager::addProject(pro);
//                    openedPro += pro;
//                } else {
//                    if (restoreResult == Project::RestoreResult::Error)
//                        appendError(errorString, restoreError);
//                    delete pro;
//                }
//            }
//        } else {
//            appendError(errorString, tr("Failed opening project \"%1\": No plugin can open project type \"%2\".")
//                        .arg(QDir::toNativeSeparators(fileName))
//                        .arg(mt.name()));
//        }
//        if (fileNames.size() > 1)
//            SessionManager::reportProjectLoadingProgress();
//    }
//    dd->updateActions();

//    bool switchToProjectsMode = Utils::anyOf(openedPro, &Project::needsConfiguration);

//    if (!openedPro.isEmpty()) {
//        if (switchToProjectsMode)
//            ModeManager::activateMode(Constants::MODE_SESSION);
//        else
//            ModeManager::activateMode(Core::Constants::MODE_EDIT);
//        ModeManager::setFocusToCurrentMode();
//    }

    return OpenProjectResult(openedPro, alreadyOpen, errorString);
}

void PF_ProjectExplorerPlugin::updateContextMenuActions()
{

}

void PF_ProjectExplorerPlugin::openNewProjectDialog()
{

}

void PF_ProjectExplorerPlugin::openOpenProjectDialog()
{

}

void PF_ProjectExplorerPluginPrivate::updateContextMenuActions()
{

}

/**
 * @brief 将项目文件添加到最近的项目列表当中
 *
 * @param fileName
 * @param displayName
 */
void PF_ProjectExplorerPluginPrivate::addToRecentProjects(const QString &fileName, const QString &displayName)
{
    if (fileName.isEmpty())
        return;
    QString prettyFileName(QDir::toNativeSeparators(fileName));

    QList<QPair<QString, QString> >::iterator it;
    for (it = m_recentProjects.begin(); it != m_recentProjects.end();)
        if ((*it).first == prettyFileName)
            it = m_recentProjects.erase(it);
        else
            ++it;

    if (m_recentProjects.count() > m_maxRecentProjects)
        m_recentProjects.removeLast();
    m_recentProjects.prepend(qMakePair(prettyFileName, displayName));
    QFileInfo fi(prettyFileName);
    m_lastOpenDirectory = fi.absolutePath();
    emit m_instance->recentProjectsChanged();
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

void PF_ProjectExplorerPluginPrivate::addBlankMaterial()
{
    /** 这里有问题，如果不是从tree操作进来的，那么node就不对了 **/
    Node *node = PF_ProjectTree::findCurrentNode();
    FolderNode *folderNode = node ? node->asFolderNode() : nullptr;
    /** 需要判断为文件夹，不清楚需不需要判断是材料类型
        感觉不需要，因为右键菜单就是根据材料进来的   **/
    if(!folderNode) return;

    PF_MagMaterialDialog* dialog = new PF_MagMaterialDialog();
    int result = dialog->exec();
    if(result == QDialog::Accepted){
        qDebug()<<"addBlankMaterial OK";
        folderNode->addNode(std::make_unique<LeafNode>(QString(QObject::tr("Material")),LeafType::Header));
        PF_ProjectTree::emitSubtreeChanged(folderNode);
    }else{
        qDebug()<<"addBlankMaterial Cancle";
    }
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
    qDebug()<<Q_FUNC_INFO;
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
