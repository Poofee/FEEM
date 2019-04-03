#include "command_p.h"

#include "id.h"
#include "constants.h"
#include "context.h"

//#include <utils/hostosinfo.h>
//#include <utils/stringutils.h>

#include <QAction>
#include <QRegExp>
#include <QToolButton>
#include <QTextStream>


/*!
    \class Core::Command
    \mainclass

    \brief Command类表示一个操作，例如菜单项，工具按钮或快捷方式。
您不直接创建Command对象，而是使用\ l {ActionManager :: registerAction（）}来注册操作并检索命令。
Command对象表示用户可见操作及其属性。
如果使用相同的ID（但是不同的上下文）注册了多个操作，则返回的Command是这些操作之间的共享命令。

命令有两个基本属性：默认快捷方式和默认文本。
默认快捷方式是用户可用于触发Command所代表的活动操作的键序列。
默认文本是例如 用于在键盘快捷方式首选项窗格中表示命令。
如果默认文本为空，则使用可见操作的文本。

更新用户可见操作以表示活动操作的状态（如果有）。
出于性能原因，默认情况下仅考虑启用和可见状态。
您可以通过设置相应的\ l {Command :: CommandAttribute} {attribute}来告诉命令也更新操作图标和文本。

如果没有活动操作，则禁用可见操作的默认行为。
您可以通过Command的\ l {Command :: CommandAttribute} {attributes}更改该行为以隐藏可见操作。
*/

/*!
    \enum Command::CommandAttribute
此枚举定义了当活动操作更改时用户可见操作的更新方式。
默认设置是更新启用和可见状态，并在没有活动操作时禁用用户可见操作。
    \omitvalue CA_Mask
    \value CA_UpdateText
        还要更新操作文本。
    \value CA_UpdateIcon
       还要更新操作图标。
    \value CA_Hide
        如果没有激活操作，请隐藏用户“可见”操作，而不是仅禁用它。
    \value CA_NonConfigurable
        用于指示此命令的键盘快捷键不应由用户配置的标志。
*/

/*!
    \fn void Command::setDefaultKeySequence(const QKeySequence &key)
设置可用于将此命令激活到\ a键的默认键盘快捷键。
如果用户未自定义快捷方式，则使用此选项，或将快捷方式重置为默认快捷方式。
*/

/*!
    \fn void Command::defaultKeySequence() const
    返回可用于激活此命令的默认键盘快捷键。
    \sa setDefaultKeySequence()
*/

/*!
    \fn void Command::keySequenceChanged()
    分配给此命令的键盘快捷键更改时发送，例如 当用户在键盘快捷键设置对话框中设置它时。
*/

/*!
    \fn QKeySequence Command::keySequence() const
    返回分配给此Command的当前键盘快捷键。
    \sa defaultKeySequence()
*/

/*!
    \fn void Command::setKeySequence(const QKeySequence &key)
    \internal
*/

/*!
    \fn void Command::setDescription(const QString &text)
设置\ a用于在键盘快捷键设置对话框中表示Command的文本。
如果不设置此项，则会采用用户可见操作的当前文本（在许多情况下这很好）。
*/

/*!
    \fn QString Command::description() const
    返回用于向用户显示此命令的文本。
    \sa setDescription()
*/

/*!
    \fn int Command::id() const
    \internal
*/

/*!
    \fn QString Command::stringWithAppendedShortcut(const QString &string) const
    返回\ a字符串，其中包含当前分配给此Command的键盘快捷键的附加表示形式。
*/

/*!
    \fn QAction *Command::action() const
返回此Command的用户可见操作。
如果Command表示快捷方式，则返回null。
使用此操作将其置于例如 工具按钮。
该操作会自动将触发器和切换信号转发到此命令当前处于活动状态的操作。
它还在其工具提示中显示当前键盘快捷键（除了活动操作的工具提示之外），并在当前上下文没有活动操作时被禁用/隐藏。
*/

/*!
    \fn void Command::setAttribute(CommandAttribute attribute)
    将\ a属性添加到此Command的属性。
    \sa CommandAttribute
    \sa removeAttribute()
    \sa hasAttribute()
*/

/*!
    \fn void Command::removeAttribute(CommandAttribute attribute)
    从此Command的属性中删除\ a属性。
    \sa CommandAttribute
    \sa setAttribute()
*/

/*!
    \fn bool Command::hasAttribute(CommandAttribute attribute) const
    返回Command是否具有\ a属性集。
    \sa CommandAttribute
    \sa removeAttribute()
    \sa setAttribute()
*/

/*!
    \fn bool Command::isActive() const
    返回Command是否具有当前上下文的活动操作或快捷方式。
*/

/*!
    \fn bool Command::isScriptable() const
返回Command是否可编写脚本。
可以从脚本调用可编写脚本的命令，而无需用户与其进行交互。
*/

/*!
    \fn bool Command::isScriptable(const Context &) const
返回Command是否可用于给定上下文的脚本。
可以从脚本调用可编写脚本的命令，而无需用户与其进行交互。
*/

//namespace Core {
//namespace Internal {

/*!
  \class Action
  \internal
*/
Action::Action(Id id)
    : m_attributes({}),
      m_id(id),
      m_action(new ProxyAction(this))
{
    m_action->setShortcutVisibleInToolTip(true);
    connect(m_action, &QAction::changed, this, &Action::updateActiveState);
}

Id Action::id() const
{
    return m_id;
}

void Action::setDefaultKeySequence(const QKeySequence &key)
{
    if (!m_isKeyInitialized)
        setKeySequence(key);
    m_defaultKey = key;
}

QKeySequence Action::defaultKeySequence() const
{
    return m_defaultKey;
}

QAction *Action::action() const
{
    return m_action;
}

QString Action::stringWithAppendedShortcut(const QString &str) const
{
    return ProxyAction::stringWithAppendedShortcut(str, keySequence());
}

Context Action::context() const
{
    return m_context;
}

void Action::setKeySequence(const QKeySequence &key)
{
    m_isKeyInitialized = true;
    m_action->setShortcut(key);
    emit keySequenceChanged();
}

QKeySequence Action::keySequence() const
{
    return m_action->shortcut();
}

void Action::setDescription(const QString &text)
{
    m_defaultText = text;
}

QString Action::description() const
{
    if (!m_defaultText.isEmpty())
        return m_defaultText;
    if (QAction *act = action()) {
//        const QString text = stripAccelerator(act->text());
//        if (!text.isEmpty())
//            return text;
    }
    return id().toString();
}

void Action::setCurrentContext(const Context &context)
{
    m_context = context;

    QAction *currentAction = nullptr;
    for (int i = 0; i < m_context.size(); ++i) {
        if (QAction *a = m_contextActionMap.value(m_context.at(i), nullptr)) {
            currentAction = a;
            break;
        }
    }

    m_action->setAction(currentAction);
    updateActiveState();
}

void Action::updateActiveState()
{
    setActive(m_action->isEnabled() && m_action->isVisible() && !m_action->isSeparator());
}

static QString msgActionWarning(QAction *newAction, Id id, QAction *oldAction)
{
    QString msg;
    QTextStream str(&msg);
    str << "addOverrideAction " << newAction->objectName() << '/' << newAction->text()
         << ": Action ";
    if (oldAction)
        str << oldAction->objectName() << '/' << oldAction->text();
    str << " is already registered for context " << id.toString() << '.';
    return msg;
}

void Action::addOverrideAction(QAction *action, const Context &context, bool scriptable)
{
    // disallow TextHeuristic menu role, because it doesn't work with translations,
    // e.g. QTCREATORBUG-13101
    if (action->menuRole() == QAction::TextHeuristicRole)
        action->setMenuRole(QAction::NoRole);
    if (isEmpty())
        m_action->initialize(action);
    if (context.isEmpty()) {
        m_contextActionMap.insert(Constants::C_GLOBAL, action);
    } else {
        for (const Id &id : context) {
            if (m_contextActionMap.contains(id))
                qWarning("%s", qPrintable(msgActionWarning(action, id, m_contextActionMap.value(id, nullptr))));
            m_contextActionMap.insert(id, action);
        }
    }
    m_scriptableMap[action] = scriptable;
    setCurrentContext(m_context);
}

void Action::removeOverrideAction(QAction *action)
{
    QMutableMapIterator<Id, QPointer<QAction> > it(m_contextActionMap);
    while (it.hasNext()) {
        it.next();
        if (it.value() == nullptr)
            it.remove();
        else if (it.value() == action)
            it.remove();
    }
    setCurrentContext(m_context);
}

bool Action::isActive() const
{
    return m_active;
}

void Action::setActive(bool state)
{
    if (state != m_active) {
        m_active = state;
        emit activeStateChanged();
    }
}

bool Action::isEmpty() const
{
    return m_contextActionMap.isEmpty();
}

bool Action::isScriptable() const
{
    return m_scriptableMap.values().contains(true);
}

bool Action::isScriptable(const Context &context) const
{
    if (context == m_context && m_scriptableMap.contains(m_action->action()))
        return m_scriptableMap.value(m_action->action());

    for (int i = 0; i < context.size(); ++i) {
        if (QAction *a = m_contextActionMap.value(context.at(i), nullptr)) {
            if (m_scriptableMap.contains(a) && m_scriptableMap.value(a))
                return true;
        }
    }
    return false;
}

void Action::setAttribute(CommandAttribute attr)
{
    m_attributes |= attr;
    switch (attr) {
    case Command::CA_Hide:
        m_action->setAttribute(ProxyAction::Hide);
        break;
    case Command::CA_UpdateText:
        m_action->setAttribute(ProxyAction::UpdateText);
        break;
    case Command::CA_UpdateIcon:
        m_action->setAttribute(ProxyAction::UpdateIcon);
        break;
    case Command::CA_NonConfigurable:
        break;
    }
}

void Action::removeAttribute(CommandAttribute attr)
{
    m_attributes &= ~attr;
    switch (attr) {
    case Command::CA_Hide:
        m_action->removeAttribute(ProxyAction::Hide);
        break;
    case Command::CA_UpdateText:
        m_action->removeAttribute(ProxyAction::UpdateText);
        break;
    case Command::CA_UpdateIcon:
        m_action->removeAttribute(ProxyAction::UpdateIcon);
        break;
    case Command::CA_NonConfigurable:
        break;
    }
}

bool Action::hasAttribute(Command::CommandAttribute attr) const
{
    return (m_attributes & attr);
}

//} // namespace Internal

void Command::augmentActionWithShortcutToolTip(QAction *a) const
{
    a->setToolTip(stringWithAppendedShortcut(a->text()));
    QObject::connect(this, &Command::keySequenceChanged, a, [this, a]() {
        a->setToolTip(stringWithAppendedShortcut(a->text()));
    });
    QObject::connect(a, &QAction::changed, this, [this, a]() {
        a->setToolTip(stringWithAppendedShortcut(a->text()));
    });
}

QToolButton *Command::toolButtonWithAppendedShortcut(QAction *action, Command *cmd)
{
    auto button = new QToolButton;
    button->setDefaultAction(action);
    if (cmd)
        cmd->augmentActionWithShortcutToolTip(action);
    return button;
}

//} // namespace Core
