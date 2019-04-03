#ifndef PF_PROJECT_H
#define PF_PROJECT_H

#include <QObject>
#include <functional>

#include "idocument.h"

class PF_ProjectPrivate;

// Auto-registers with the DocumentManager if a callback is set!
class ProjectDocument : public IDocument
{
public:
    using ProjectCallback = std::function<void()>;

    ProjectDocument(const QString &mimeType, const FileName &fileName,
                    const ProjectCallback &callback = {});

    IDocument::ReloadBehavior reloadBehavior(IDocument::ChangeTrigger state,
                                                   IDocument::ChangeType type) const final;
    bool reload(QString *errorString, IDocument::ReloadFlag flag,
                IDocument::ChangeType type) final;

private:
    ProjectCallback m_callback;
};


class PF_Project : public QObject
{
    Q_OBJECT
public:
    explicit PF_Project(QObject *parent = nullptr);

signals:

public slots:

private:
    PF_ProjectPrivate* d;
};

#endif // PF_PROJECT_H
