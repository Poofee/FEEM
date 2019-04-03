#pragma once

#include "command.h"

#include "id.h"
#include "context.h"

#include "proxyaction.h"

#include <QList>
#include <QMultiMap>
#include <QPointer>
#include <QMap>
#include <QKeySequence>

//namespace Core {
//namespace Internal {

class Action : public Command
{
    Q_OBJECT
public:
    Action(Id id);

    Id id() const override;

    void setDefaultKeySequence(const QKeySequence &key) override;
    QKeySequence defaultKeySequence() const override;

    void setKeySequence(const QKeySequence &key) override;
    QKeySequence keySequence() const override;

    void setDescription(const QString &text) override;
    QString description() const override;

    QAction *action() const override;

    QString stringWithAppendedShortcut(const QString &str) const override;

    Context context() const override;
    void setCurrentContext(const Context &context);

    bool isActive() const override;
    void addOverrideAction(QAction *action, const Context &context, bool scriptable);
    void removeOverrideAction(QAction *action);
    bool isEmpty() const;

    bool isScriptable() const override;
    bool isScriptable(const Context &context) const override;

    void setAttribute(CommandAttribute attr) override;
    void removeAttribute(CommandAttribute attr) override;
    bool hasAttribute(CommandAttribute attr) const override;

private:
    void updateActiveState();
    void setActive(bool state);

    Context m_context;
    CommandAttributes m_attributes;
    Id m_id;
    QKeySequence m_defaultKey;
    QString m_defaultText;
    bool m_isKeyInitialized = false;

    ProxyAction *m_action = nullptr;
    QString m_toolTip;

    QMap<Id, QPointer<QAction> > m_contextActionMap;
    QMap<QAction*, bool> m_scriptableMap;
    bool m_active = false;
    bool m_contextInitialized = false;
};

//} // namespace Internal
//} // namespace Core
