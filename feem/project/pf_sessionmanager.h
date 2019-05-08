#ifndef PF_SESSIONMANAGER_H
#define PF_SESSIONMANAGER_H

#include <QObject>
#include <QDateTime>
#include <QString>
#include <QStringList>

class PF_Project;

class PF_SessionManager : public QObject
{
    Q_OBJECT

public:
    explicit PF_SessionManager(QObject *parent = nullptr);
    ~PF_SessionManager() override;

    static PF_SessionManager *instance();

    // higher level session management
    static QString activeSession();
    static QString lastSession();
    static QStringList sessions();
    static QDateTime sessionDateTime(const QString &session);

    static bool createSession(const QString &session);

    static bool confirmSessionDelete(const QString &session);
    static bool deleteSession(const QString &session);

    static bool cloneSession(const QString &original, const QString &clone);
    static bool renameSession(const QString &original, const QString &newName);

    static bool loadSession(const QString &session);

    static bool save();
    static void closeAllProjects();

    static void addProject(PF_Project *project);
    static void removeProject(PF_Project *project);
    static void removeProjects(const QList<PF_Project *> &remove);

    static void setStartupProject(PF_Project *startupProject);

//    static QList<Project *> dependencies(const PF_Project *project);
//    static bool hasDependency(const PF_Project *project, const PF_Project *depProject);
//    static bool canAddDependency(const PF_Project *project, const Project *depProject);
//    static bool addDependency(Project *project, PF_Project *depProject);
//    static void removeDependency(Project *project, Project *depProject);

//    static bool isProjectConfigurationCascading();
//    static void setProjectConfigurationCascading(bool b);

//    static void setActiveTarget(Project *p, Target *t, SetActive cascade);
//    static void setActiveBuildConfiguration(Target *t, BuildConfiguration *bc, SetActive cascade);
//    static void setActiveDeployConfiguration(Target *t, DeployConfiguration *dc, SetActive cascade);

//    static Utils::FileName sessionNameToFileName(const QString &session);
    static PF_Project *startupProject();

    static const QList<PF_Project *> projects();
    static bool hasProjects();
    static bool hasProject(PF_Project *p);

    static bool isDefaultVirgin();
    static bool isDefaultSession(const QString &session);

    // Let other plugins store persistent values within the session file
    static void setValue(const QString &name, const QVariant &value);
    static QVariant value(const QString &name);

    // NBS rewrite projectOrder (dependency management)
//    static QList<PF_Project *> projectOrder(const PF_Project *project = nullptr);

    static PF_Project *projectForFile(const QString &fileName);

    static QStringList projectsForSessionName(const QString &session);

    static void reportProjectLoadingProgress();
    static bool loadingSession();

signals:
    void projectAdded(PF_Project *project);
    void aboutToRemoveProject(PF_Project *project);
    void projectDisplayNameChanged(PF_Project *project);
    void projectRemoved(PF_Project *project);

    void startupProjectChanged(PF_Project *project);

    void aboutToUnloadSession(QString sessionName);
    void aboutToLoadSession(QString sessionName);
    void sessionLoaded(QString sessionName);
    void aboutToSaveSession();
//    void dependencyChanged(PF_Project *a, PF_Project *b);

signals: // for tests only
    void projectFinishedParsing(PF_Project *project);

private:
//    static void saveActiveMode(Core::Id mode);
//    static void configureEditor(Core::IEditor *editor, const QString &fileName);
//    static void markSessionFileDirty();
//    static void configureEditors(PF_Project *project);
};

#endif // PF_SESSIONMANAGER_H
