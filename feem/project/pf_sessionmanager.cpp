#include "pf_sessionmanager.h"

#include "pf_projectexplorer.h"
#include "pf_project.h"
#include "icore.h"

#include <QMap>
#include <QVariant>
#include <QSet>
#include <QMessageBox>
#include <QDebug>

/*!
 \brief 用来管理以打开的项目，最近的项目，等等。

*/
class PF_SessionManagerPrivate
{
public:
//    void restoreValues(const PersistentSettingsReader &reader);
//    void restoreDependencies(const PersistentSettingsReader &reader);
//    void restoreStartupProject(const PersistentSettingsReader &reader);
//    void restoreEditors(const PersistentSettingsReader &reader);
    void restoreProjects(const QStringList &fileList);
    void askUserAboutFailedProjects();
    void sessionLoadingProgress();

//    bool recursiveDependencyCheck(const QString &newDep, const QString &checkDep) const;
//    QStringList dependencies(const QString &proName) const;
//    QStringList dependenciesOrder() const;
//    void dependencies(const QString &proName, QStringList &result) const;

//    static QString windowTitleAddition(const QString &filePath);
    static QString sessionTitle(const QString &filePath);

    bool hasProjects() const { return !m_projects.isEmpty(); }

    QString m_sessionName = QLatin1String("default");
    bool m_virginSession = true;
    bool m_loadingSession = false;
//    bool m_casadeSetActive = false;

    mutable QStringList m_sessions;
    mutable QHash<QString, QDateTime> m_sessionDateTimes;

    PF_Project *m_startupProject = nullptr;/** 当前项目 **/
    QList<PF_Project *> m_projects;/** 项目列表 **/
    QStringList m_failedProjects;/** 打开失败的项目 **/
    QMap<QString, QStringList> m_depMap;
    QMap<QString, QVariant> m_values;
//    QFutureInterface<void> m_future;
//    PersistentSettingsWriter *m_writer = nullptr;

private:
    static QString locationInProject(const QString &filePath);
};

static PF_SessionManager *m_instance = nullptr;
static PF_SessionManagerPrivate *d = nullptr;

PF_SessionManager::PF_SessionManager(QObject* parent)
{
    m_instance = this;
    d = new PF_SessionManagerPrivate;

    /**  **/

}

PF_SessionManager::~PF_SessionManager()
{
    emit m_instance->aboutToUnloadSession(d->m_sessionName);
//    delete d->m_writer;
    delete d;
    d = nullptr;
}

PF_SessionManager*PF_SessionManager::instance()
{
    return m_instance;
}

QString PF_SessionManager::activeSession()
{
    return d->m_sessionName;
}

QString PF_SessionManager::lastSession()
{
    return QString();
}

/*!
 \brief

 \return QStringList
*/
QStringList PF_SessionManager::sessions()
{
    if (d->m_sessions.isEmpty()) {
        // We are not initialized yet, so do that now
//        QDir sessionDir(ICore::userResourcePath());
//        QList<QFileInfo> sessionFiles = sessionDir.entryInfoList(QStringList() << QLatin1String("*.qws"), QDir::NoFilter, QDir::Time);
//        foreach (const QFileInfo &fileInfo, sessionFiles) {
//            const QString &name = fileInfo.completeBaseName();
//            d->m_sessionDateTimes.insert(name, fileInfo.lastModified());
//            if (name != QLatin1String("default"))
//                d->m_sessions << name;
//        }
        d->m_sessions.prepend(QLatin1String("default"));
    }
    return d->m_sessions;
}

QDateTime PF_SessionManager::sessionDateTime(const QString& session)
{
    return QDateTime();
}

/*!
 \brief 创建一个会话，但是没有创建文件

 \param session
 \return bool
*/
bool PF_SessionManager::createSession(const QString& session)
{
    if (sessions().contains(session))
        return false;
//    Q_ASSERT(d->m_sessions.size() > 0);
    d->m_sessions.insert(1, session);
    return true;
}

/*!
 \brief

 \param session
 \return bool
*/
bool PF_SessionManager::confirmSessionDelete(const QString& session)
{
    return QMessageBox::question(ICore::mainWindow(),
                                 tr("Delete Session"),
                                 tr("Delete session %1?").arg(session),
                                 QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes;
}

/*!
 \brief

 \param session
 \return bool
*/
bool PF_SessionManager::deleteSession(const QString& session)
{
    if (!d->m_sessions.contains(session))
        return false;
    d->m_sessions.removeOne(session);
//    QFile fi(sessionNameToFileName(session).toString());
//    if (fi.exists())
//        return fi.remove();
    return false;
}

bool PF_SessionManager::cloneSession(const QString& original, const QString& clone)
{
    return false;
}

bool PF_SessionManager::renameSession(const QString& original, const QString& newName)
{
    return false;
}

bool PF_SessionManager::loadSession(const QString& session)
{
    return false;
}

bool PF_SessionManager::save()
{
    return false;
}

void PF_SessionManager::closeAllProjects()
{
    removeProjects(projects());
}

void PF_SessionManager::addProject(PF_Project* pro)
{
    qDebug()<<Q_FUNC_INFO;
    if(pro == nullptr)
        return;
//    QTC_CHECK(!pro->displayName().isEmpty());
//    QTC_CHECK(pro->id().isValid());

    d->m_virginSession = false;
    if(d->m_projects.contains(pro))
        return;

    d->m_projects.append(pro);

    connect(pro, &PF_Project::displayNameChanged,
            m_instance, [pro]() { m_instance->projectDisplayNameChanged(pro); });

    emit m_instance->projectAdded(pro);
//    const auto updateFolderNavigation = [pro] {
//        // destructing projects might trigger changes, so check if the project is actually there
//        if (QTC_GUARD(d->m_projects.contains(pro))) {
//            const QIcon icon = pro->rootProjectNode() ? pro->rootProjectNode()->icon() : QIcon();
//            FolderNavigationWidgetFactory::insertRootDirectory({projectFolderId(pro),
//                                                                PROJECT_SORT_VALUE,
//                                                                pro->displayName(),
//                                                                pro->projectFilePath().parentDir(),
//                                                                icon});
//        }
//    };
//    updateFolderNavigation();
//    configureEditors(pro);
//    connect(pro, &Project::fileListChanged, m_instance, [pro, updateFolderNavigation]() {
//        configureEditors(pro);
//        updateFolderNavigation(); // update icon
//    });
//    connect(pro, &Project::displayNameChanged, m_instance, updateFolderNavigation);

    if (!startupProject())
        setStartupProject(pro);
}

/*!
 \brief 移除项目

 \param project
*/
void PF_SessionManager::removeProject(PF_Project* project)
{
    d->m_virginSession = false;
    if(project)
    {}
    else
        return;
    removeProjects({project});
}

/*!
 \brief 移除多个项目

 \param remove
*/
void PF_SessionManager::removeProjects(const QList<PF_Project*>& remove)
{
    QMap<QString, QStringList> resMap;

    for (PF_Project *pro : remove)
        emit m_instance->aboutToRemoveProject(pro);

    /** TODO 删除文件之前的一些操作 **/


    d->m_depMap = resMap;
    bool changeStartupProject = false;

    // Delete projects
    for (PF_Project* pro : remove) {
//        pro->saveSettings();

        // Remove the project node:
        d->m_projects.removeOne(pro);

        if (pro == d->m_startupProject)
            changeStartupProject = true;

//        FolderNavigationWidgetFactory::removeRootDirectory(projectFolderId(pro));
        disconnect(pro, nullptr, m_instance, nullptr);
        emit m_instance->projectRemoved(pro);
    }

    if (changeStartupProject)
        setStartupProject(hasProjects() ? projects().first() : nullptr);

     qDeleteAll(remove);
}

/*!
 \brief 设置当前项目

 \param startupProject
*/
void PF_SessionManager::setStartupProject(PF_Project* startupProject)
{
    qDebug()<<Q_FUNC_INFO;
    if((!startupProject && d->m_projects.isEmpty())
            || (startupProject && d->m_projects.contains(startupProject))){

    }else{
        return;
    }

    if (d->m_startupProject == startupProject)
        return;

    d->m_startupProject = startupProject;
    emit m_instance->startupProjectChanged(startupProject);
}

PF_Project* PF_SessionManager::startupProject()
{
    return d->m_startupProject;
}

const QList<PF_Project* > PF_SessionManager::projects()
{
    return d->m_projects;
}

bool PF_SessionManager::hasProjects()
{
    return d->hasProjects();
}

bool PF_SessionManager::hasProject(PF_Project* p)
{
    return d->m_projects.contains(p);
}

bool PF_SessionManager::isDefaultVirgin()
{
    return isDefaultSession(d->m_sessionName) && d->m_virginSession;
}

bool PF_SessionManager::isDefaultSession(const QString& session)
{
    return session == QLatin1String("default");
}

/*!
 \brief

 \param name
 \param value
*/
void PF_SessionManager::setValue(const QString& name, const QVariant& value)
{
    if (d->m_values.value(name) == value)
        return;
    d->m_values.insert(name, value);
}

QVariant PF_SessionManager::value(const QString& name)
{
    auto it = d->m_values.constFind(name);
    return (it == d->m_values.constEnd()) ? QVariant() : *it;
}

/*!
 \brief 给定文件名，返回project

 \param fileName
 \return PF_Project
*/
PF_Project* PF_SessionManager::projectForFile(const QString& fileName)
{
    return nullptr;
}

/*!
 \brief

 \param session
 \return QStringList
*/
QStringList PF_SessionManager::projectsForSessionName(const QString& session)
{
    return QStringList();
}

void PF_SessionManager::reportProjectLoadingProgress()
{

}

bool PF_SessionManager::loadingSession()
{
    return d->m_loadingSession;
}

void PF_SessionManagerPrivate::restoreProjects(const QStringList& fileList)
{

}

void PF_SessionManagerPrivate::askUserAboutFailedProjects()
{

}

void PF_SessionManagerPrivate::sessionLoadingProgress()
{

}

QString PF_SessionManagerPrivate::sessionTitle(const QString& filePath)
{
    if (PF_SessionManager::isDefaultSession(d->m_sessionName)) {
        if (filePath.isEmpty()) {
            // use single project's name if there is only one loaded.
            const QList<PF_Project *> projects = PF_SessionManager::projects();
            if (projects.size() == 1)
                return projects.first()->displayName();
        }
    } else {
        QString sessionName = d->m_sessionName;
        if (sessionName.isEmpty())
            sessionName = PF_SessionManager::tr("Untitled");
        return sessionName;
    }
    return QString();
}
