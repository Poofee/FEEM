#pragma once

#include "command_p.h"
#include "actioncontainer_p.h"
#include "context.h"

#include <QMap>
#include <QHash>
#include <QMultiHash>
#include <QTimer>

//namespace Core {

//namespace Internal {

class Action;
class ActionContainerPrivate;

class ActionManagerPrivate : public QObject
{
    Q_OBJECT

public:
    using IdCmdMap = QHash<Id, Action *>;
    using IdContainerMap = QHash<Id, ActionContainerPrivate *>;

    ~ActionManagerPrivate() override;

    void setContext(const Context &context);
    bool hasContext(int context) const;

    void saveSettings();
    void saveSettings(Action *cmd);

    void showShortcutPopup(const QString &shortcut);
    bool hasContext(const Context &context) const;
    Action *overridableAction(Id id);

    void readUserSettings(Id id, Action *cmd);

    void containerDestroyed();
    void actionTriggered();

    IdCmdMap m_idCmdMap;

    IdContainerMap m_idContainerMap;

    Context m_context;

    bool m_presentationModeEnabled = false;
};

//} // namespace Internal
//} // namespace Core
