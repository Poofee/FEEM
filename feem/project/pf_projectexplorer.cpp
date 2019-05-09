#include "pf_projectexplorer.h"
#include "pf_projecttree.h"
#include "pf_sessionmanager.h"

/*!
 \brief projectexplore实现的私有类，具体的实现都放在这里

*/
class PF_ProjectExplorerPluginPrivate : public QObject
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

    return true;
}
