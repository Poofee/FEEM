#include "actionmanager.h"
#include "actionmanager_p.h"
#include "actioncontainer_p.h"
#include "command_p.h"

#include "id.h"
//#include <coreplugin/icore.h>
//#include <coreplugin/id.h>
//#include <utils/fadingindicator.h>
//#include <utils/qtcassert.h>

#include <QAction>
#include <QApplication>
#include <QDebug>
#include <QDesktopWidget>
#include <QMenu>
#include <QMenuBar>
#include <QSettings>

namespace {
    enum { warnAboutFindFailures = 0 };
}

static const char kKeyboardSettingsKey[] = "KeyboardShortcuts";

//using namespace Core;
//using namespace Core::Internal;

/*!
    \class Core::ActionManager
    \mainclass
    \inmodule Qt Creator

    \brief ActionManager类负责菜单的注册和菜单项和键盘快捷键。

    ActionManager是操作及其快捷方式和布局的中央簿记员。它是一
个主要包含静态函数的单例。如果需要访问实例，例如，用于连接信号，
是它的ActionManager :: instance（）函数。

    需要这个类的主要原因是为用户提供一个中心位置可以指定他的所
有键盘快捷键，并为应该采取的操作提供解决方案在不同的上下文中表
现不同（例如复制/替换/撤消/重做操作）。

    \section1 Contexts

    使用相同Id（但不同的上下文列表）注册的所有操作都被视为同
一命令的重载，由Command类的实例表示。
确切地说，只有一个具有相同ID的已注册操作在任何时候都是活动的。
这是哪个操作，是由注册操作的上下文列表定义的：

如果当前焦点窗口小部件是通过\ l {ICore :: addContextObject（）}注册的，则其IContext对象返回的所有上下文都是活动的。
此外，通过\ l {ICore :: addAdditionalContext（）}设置的所有上下文也是活动的。
如果其中一个操作已为其中一个活动上下文注册，则它是一个活动操作，并接收\ c触发和\ c切换信号。
此ID的可见操作的外观也可能适用于此活动操作（取决于相应的\ l {Command}对象的设置）。

用户可见的操作是Command :: action（）返回的操作。
如果您为自己提供了用户可见的动作表示，则需要使用Command :: action（）。
当用户调用此操作时，信号将转发到对当前上下文有效的已注册操作。

    \section1 Registering Actions

    要注册全局激活的操作“我的操作”，请在插件的IPlugin :: initialize函数中添加以下内容：
    \code
        QAction *myAction = new QAction(tr("My Action"), this);
        Command *cmd = ActionManager::registerAction(myAction,
                                                 "myplugin.myaction",
                                                 Context(C_GLOBAL));
        cmd->setDefaultKeySequence(QKeySequence(tr("Ctrl+Alt+u")));
        connect(myAction, &QAction::triggered, this, &MyPlugin::performMyAction);
    \endcode

所以\ c connect连接到你自己的QAction实例。
如果您创建例如 一个工具按钮，应该表示从Command :: action（）向其添加操作的操作：
    \code
        QToolButton *myButton = new QToolButton(someParentWidget);
        myButton->setDefaultAction(cmd->action());
    \endcode

还可以使用ActionManager将项添加到已注册的操作容器，如应用程序菜单栏或该菜单栏中的菜单。
为此，您可以通过registerAction函数注册您的操作，通过调用actionContainer（const Id＆）
获取特定ID（如Core :: Constants命名空间中指定的）的操作容器，并将您的命令添加到此容器中。

    按照示例将“我的操作”添加到“工具”菜单中即可完成
    \code
        ActionManager::actionContainer(M_TOOLS)->addAction(cmd);
    \endcode

    \section1 重要指南：
    \list
    \li 始终注册您的操作和快捷方式！
    \li 在插件的\ l {ExtensionSystem :: IPlugin :: initialize（）}或\ l {ExtensionSystem :: IPlugin :: extensionsInitialized（）}函数中注册您的操作和快捷方式，否则快捷方式将不会出现在键盘设置对话框中 一开始。
    \li 使用\ c {cmd = registerAction（action，id，contexts）}注册操作时，请确保连接您自己的操作\ c {connect（action，SIGNAL ...）}但是make \ c {cmd-> action（） 对用户可见，即 \ C {小窗口>的addAction（CMD->动作（））}。
    \li 使用此类可以向应用程序菜单添加操作
    \endlist

    \sa Core::ICore
    \sa Core::Command
    \sa Core::ActionContainer
    \sa Core::IContext
*/

/*!
    \fn void ActionManager::commandListChanged()

    命令列表发生更改时发出。
*/

/*!
    \fn void ActionManager::commandAdded(const QString &id)

    添加命令（带有\ id）时发出。
*/

static ActionManager *m_instance = nullptr;
static ActionManagerPrivate *d;

/*!
    \internal
*/
ActionManager::ActionManager(QObject *parent)
    : QObject(parent)
{
    m_instance = this;
    d = new ActionManagerPrivate;
//    if (Utils::HostOsInfo::isMacHost())
//        QCoreApplication::setAttribute(Qt::AA_DontShowIconsInMenus);
}

/*!
    \internal
*/
ActionManager::~ActionManager()
{
    delete d;
}

/*!
    Returns the pointer to the instance, which is only used for connecting to signals.
*/
ActionManager *ActionManager::instance()
{
    return m_instance;
}

/*!
    Creates a new menu with the given \a id.

返回一个新的ActionContainer，您可以使用它来获取QMenu实例或将菜单项添加到菜单中。
ActionManager拥有返回的ActionContainer。
通过ActionManager :: actionContainer和ActionContainer :: addMenu函数将菜单添加到其他菜单或菜单栏。
*/
ActionContainer *ActionManager::createMenu(Id id)
{
    const ActionManagerPrivate::IdContainerMap::const_iterator it = d->m_idContainerMap.constFind(id);
    if (it !=  d->m_idContainerMap.constEnd())
        return it.value();

    auto mc = new MenuActionContainer(id);

    d->m_idContainerMap.insert(id, mc);
    connect(mc, &QObject::destroyed, d, &ActionManagerPrivate::containerDestroyed);

    return mc;
}

/*!
使用给定的\ a id创建一个新的菜单栏。
返回一个新的ActionContainer，可用于获取QMenuBar实例或将菜单添加到菜单栏。
ActionManager拥有返回的ActionContainer。
*/
ActionContainer *ActionManager::createMenuBar(Id id)
{
    const ActionManagerPrivate::IdContainerMap::const_iterator it = d->m_idContainerMap.constFind(id);
    if (it !=  d->m_idContainerMap.constEnd())
        return it.value();

    auto mb = new QMenuBar; // No parent (System menu bar on macOS)
    mb->setObjectName(id.toString());

    auto mbc = new MenuBarActionContainer(id);
    mbc->setMenuBar(mb);

    d->m_idContainerMap.insert(id, mbc);
    connect(mbc, &QObject::destroyed, d, &ActionManagerPrivate::containerDestroyed);

    return mbc;
}

/*!
    在指定的\ a id下制作一个系统已知的\动作。

返回一个命令对象，该对象表示应用程序中的操作，并由ActionManager拥有。
只要\ _上下文不同，您就可以使用相同的\ id注册多个操作。
在这种情况下，实际操作的触发器被转发到当前活动上下文的已注册QAction。
如果未指定可选的\ a上下文参数，则将假定全局上下文。
可以从脚本调用脚本化操作，而无需用户与其进行交互。
*/
Command *ActionManager::registerAction(QAction *action, Id id, const Context &context, bool scriptable)
{
    Action *a = d->overridableAction(id);
    if (a) {
        a->addOverrideAction(action, context, scriptable);
        emit m_instance->commandListChanged();
        emit m_instance->commandAdded(id);
    }
    return a;
}

/*!
    返回给定\ a id下系统已知的Command对象。

    \sa ActionManager::registerAction()
*/
Command *ActionManager::command(Id id)
{
    const ActionManagerPrivate::IdCmdMap::const_iterator it = d->m_idCmdMap.constFind(id);
    if (it == d->m_idCmdMap.constEnd()) {
        if (warnAboutFindFailures)
            qWarning() << "ActionManagerPrivate::command(): failed to find :"
                       << id.name();
        return nullptr;
    }
    return it.value();
}

/*!
    返回给定\ _id下系统知道的IActionContainter对象。

    \sa ActionManager::createMenu()
    \sa ActionManager::createMenuBar()
*/
ActionContainer *ActionManager::actionContainer(Id id)
{
    const ActionManagerPrivate::IdContainerMap::const_iterator it = d->m_idContainerMap.constFind(id);
    if (it == d->m_idContainerMap.constEnd()) {
        if (warnAboutFindFailures)
            qWarning() << "ActionManagerPrivate::actionContainer(): failed to find :"
                       << id.name();
        return nullptr;
    }
    return it.value();
}

/*!
    返回已注册的所有命令。
*/
QList<Command *> ActionManager::commands()
{
    // transform list of Action into list of Command
    QList<Command *> result;
    foreach (Command *cmd, d->m_idCmdMap)
        result << cmd;
    return result;
}

/*!
    删除有关指定\ a id下的\ a操作的知识。

通常，您无需取消注册操作。
取消注册操作的唯一有效用例是用于表示用户可定义操作的操作，例如自定义定位器过滤器。
如果用户删除了此类操作，则还必须从操作管理器中取消注册，以使其从快捷方式设置等中消失。
*/
void ActionManager::unregisterAction(QAction *action, Id id)
{
    Action *a = d->m_idCmdMap.value(id, 0);
    if (!a) {
        qWarning() << "unregisterAction: id" << id.name()
                   << "is registered with a different command type.";
        return;
    }
    a->removeOverrideAction(action);
    if (a->isEmpty()) {
        // clean up
        d->saveSettings(a);
        ICore::mainWindow()->removeAction(a->action());
        // ActionContainers listen to the commands' destroyed signals
        delete a->action();
        d->m_idCmdMap.remove(id);
        delete a;
    }
    emit m_instance->commandListChanged();
}

/*!
在演示模式下处理使用过的快捷方式的显示。
使用命令行参数\ c {-presentationMode}启动\ QC时启用演示模式。
在演示模式中，\ QC在灰色框中显示任何按下的快捷方式。
*/
void ActionManager::setPresentationModeEnabled(bool enabled)
{
    if (enabled == isPresentationModeEnabled())
        return;

    // Signal/slots to commands:
    foreach (Command *c, commands()) {
        if (c->action()) {
            if (enabled)
                connect(c->action(), &QAction::triggered, d, &ActionManagerPrivate::actionTriggered);
            else
                disconnect(c->action(), &QAction::triggered, d, &ActionManagerPrivate::actionTriggered);
        }
    }

    d->m_presentationModeEnabled = enabled;
}

bool ActionManager::isPresentationModeEnabled()
{
    return d->m_presentationModeEnabled;
}

QString ActionManager::withNumberAccelerator(const QString &text, const int number)
{
//    if (Utils::HostOsInfo::isMacHost() || number > 9)
//        return text;
    return QString("&%1 | %2").arg(number).arg(text);
}

void ActionManager::saveSettings()
{
    d->saveSettings();
}

void ActionManager::setContext(const Context &context)
{
    d->setContext(context);
}

/*!
    \class ActionManagerPrivate
    \inheaderfile actionmanager_p.h
    \internal
*/

ActionManagerPrivate::~ActionManagerPrivate()
{
    // first delete containers to avoid them reacting to command deletion
    foreach (ActionContainerPrivate *container, m_idContainerMap)
        disconnect(container, &QObject::destroyed, this, &ActionManagerPrivate::containerDestroyed);
    qDeleteAll(m_idContainerMap);
    qDeleteAll(m_idCmdMap);
}

void ActionManagerPrivate::setContext(const Context &context)
{
    // here are possibilities for speed optimization if necessary:
    // let commands (de-)register themselves for contexts
    // and only update commands that are either in old or new contexts
    m_context = context;
    const IdCmdMap::const_iterator cmdcend = m_idCmdMap.constEnd();
    for (IdCmdMap::const_iterator it = m_idCmdMap.constBegin(); it != cmdcend; ++it)
        it.value()->setCurrentContext(m_context);
}

bool ActionManagerPrivate::hasContext(const Context &context) const
{
    for (int i = 0; i < m_context.size(); ++i) {
        if (context.contains(m_context.at(i)))
            return true;
    }
    return false;
}

void ActionManagerPrivate::containerDestroyed()
{
    auto container = static_cast<ActionContainerPrivate *>(sender());
    m_idContainerMap.remove(m_idContainerMap.key(container));
}

void ActionManagerPrivate::actionTriggered()
{
    auto action = qobject_cast<QAction *>(QObject::sender());
    if (action)
        showShortcutPopup(action->shortcut().toString());
}

void ActionManagerPrivate::showShortcutPopup(const QString &shortcut)
{
    if (shortcut.isEmpty() || !ActionManager::isPresentationModeEnabled())
        return;

    QWidget *window = QApplication::activeWindow();
    if (!window) {
        if (!QApplication::topLevelWidgets().isEmpty()) {
            window = QApplication::topLevelWidgets().first();
        } else {
            if(QApplication::desktop())
                ;
            else
                return;
            window = QApplication::desktop()->screen();
            if(!window)
                return;
        }
    }

    Utils::FadingIndicator::showText(window, shortcut);
}

Action *ActionManagerPrivate::overridableAction(Id id)
{
    Action *a = m_idCmdMap.value(id, 0);
    if (!a) {
        a = new Action(id);
        m_idCmdMap.insert(id, a);
        readUserSettings(id, a);
        ICore::mainWindow()->addAction(a->action());
        a->action()->setObjectName(id.toString());
        a->action()->setShortcutContext(Qt::ApplicationShortcut);
        a->setCurrentContext(m_context);

        if (ActionManager::isPresentationModeEnabled())
            connect(a->action(), &QAction::triggered, this, &ActionManagerPrivate::actionTriggered);
    }

    return a;
}

void ActionManagerPrivate::readUserSettings(Id id, Action *cmd)
{
    QSettings *settings = ICore::settings();
    settings->beginGroup(QLatin1String(kKeyboardSettingsKey));
    if (settings->contains(id.toString()))
        cmd->setKeySequence(QKeySequence(settings->value(id.toString()).toString()));
    settings->endGroup();
}

void ActionManagerPrivate::saveSettings(Action *cmd)
{
    const QString settingsKey = QLatin1String(kKeyboardSettingsKey) + QLatin1Char('/')
            + cmd->id().toString();
    QKeySequence key = cmd->keySequence();
    if (key != cmd->defaultKeySequence())
        ICore::settings()->setValue(settingsKey, key.toString());
    else
        ICore::settings()->remove(settingsKey);
}

void ActionManagerPrivate::saveSettings()
{
    const IdCmdMap::const_iterator cmdcend = m_idCmdMap.constEnd();
    for (IdCmdMap::const_iterator j = m_idCmdMap.constBegin(); j != cmdcend; ++j) {
        saveSettings(j.value());
    }
}
