#include "pf_projectexplorer.h"
#include "pf_projecttree.h"
#include "pf_sessionmanager.h"

/*!
 \brief projectexplore实现的私有类，具体的实现都放在这里

*/
class ProjectExplorerPluginPrivate : public QObject
{
public:

public:
    QList<QPair<QString, QString> > m_recentProjects; // pair of filename, displayname
    static const int m_maxRecentProjects = 25;

    QString m_lastOpenDirectory;

    /******************Actions********************/

    /******************Gui********************/
    PF_SessionManager m_sessionManager;
    PF_ProjectTree m_projectTree;
};

static ProjectExplorerPlugin* m_instance = nullptr;
static ProjectExplorerPluginPrivate*  dd = nullptr;

ProjectExplorerPlugin::ProjectExplorerPlugin(QObject *parent) : QObject(parent)
{
    m_instance = this;
}

ProjectExplorerPlugin::~ProjectExplorerPlugin()
{
    dd = nullptr;
    m_instance = nullptr;
}

ProjectExplorerPlugin*ProjectExplorerPlugin::instance()
{
    return m_instance;
}

bool ProjectExplorerPlugin::initialize()
{
    dd = new ProjectExplorerPluginPrivate;

    return true;
}
