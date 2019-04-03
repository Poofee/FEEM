#include "pf_project.h"

#include "idocument.h"

#include <memory>

// --------------------------------------------------------------------
// ProjectDocument:
// --------------------------------------------------------------------

ProjectDocument::ProjectDocument(const QString &mimeType, const FileName &fileName,
                                 const ProjectDocument::ProjectCallback &callback) :
    m_callback(callback)
{
    setFilePath(fileName);
    setMimeType(mimeType);
//    if (m_callback)
//        DocumentManager::addDocument(this);
}

IDocument::ReloadBehavior
ProjectDocument::reloadBehavior(IDocument::ChangeTrigger state,
                                IDocument::ChangeType type) const
{
    Q_UNUSED(state);
    Q_UNUSED(type);
    return BehaviorSilent;
}

bool ProjectDocument::reload(QString *errorString, IDocument::ReloadFlag flag,
                             IDocument::ChangeType type)
{
    Q_UNUSED(errorString);
    Q_UNUSED(flag);
    Q_UNUSED(type);

    if (m_callback)
        m_callback();
    return true;
}
/*!
 \brief Project的具体实现。

*/
class PF_ProjectPrivate{
public:
    PF_ProjectPrivate(const QString &mimeType, const FileName &fileName,
                   const ProjectDocument::ProjectCallback &callback)
    {
        m_document = std::make_unique<ProjectDocument>(mimeType, fileName, callback);
    }
    ~PF_ProjectPrivate();

    /** 保存真实的文件 **/
    std::unique_ptr<IDocument> m_document;

    QString m_displayName;
};

PF_Project::PF_Project(QObject *parent) : QObject(parent)
{

}
