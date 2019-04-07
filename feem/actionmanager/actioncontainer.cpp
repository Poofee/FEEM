#include "actioncontainer_p.h"
#include "actionmanager.h"

#include "id.h"
#include "constants.h"
#include "context.h"

//#include <utils/hostosinfo.h>
//#include <utils/qtcassert.h>

#include <QDebug>
#include <QTimer>
#include <QAction>
#include <QMenuBar>

//Q_DECLARE_METATYPE(Core::Internal::MenuActionContainer*)

//namespace Core {
//namespace Internal {

/*!
    \class ActionContainer
    \mainclass

    \brief ActionContainer类表示Qt Creator中的菜单或菜单栏。

    您不直接创建此类的实例，而是使用\ l {ActionManager :: createMenu（）}和\ l {ActionManager :: createMenuBar（）}函数。
使用\ l {ActionManager :: actionContainer（）}检索ID的现有操作容器。

在菜单或菜单栏中，您可以通过定义组（组的顺序由\ l {ActionContainer :: appendGroup（）}调用的顺序定义）将菜单和项目组合在一起，并向这些组添加菜单/操作 。
如果未定义自定义组，则操作容器具有三个默认组\ c {Core :: Constants :: G_DEFAULT_ONE}，\ c {Core :: Constants :: G_DEFAULT_TWO}和\ c {Core :: Constants :: G_DEFAULT_THREE}。

您可以通过设置相应的\ l {ActionContainer :: setOnAllDisabledBehavior（）} {OnAllDisabledBehavior}来定义此操作容器所表示的菜单是否应自动禁用或隐藏，只要它包含禁用的项目和子菜单。
对于菜单，默认值为ActionContainer :: Disable，对于菜单栏，默认为ActionContainer :: Show。
*/

/*!
    \enum ActionContainer::OnAllDisabledBehavior
    定义当表示的菜单为空或仅包含禁用/不可见项时发生的情况。
    \value Disable
        菜单将可见但已禁用。
    \value Hide
        在子项的状态发生变化之前，菜单将不可见。
    \value Show
        菜单仍然可见并处于活动状态。
*/

/*!
    \fn ActionContainer::setOnAllDisabledBehavior(OnAllDisabledBehavior behavior)
定义此操作容器所表示的菜单的行为，只要它包含禁用的项目和子菜单。
对于菜单，默认值为ActionContainer :: Disable，对于菜单栏，默认为ActionContainer :: Show。
    \sa ActionContainer::OnAllDisabledBehavior
    \sa ActionContainer::onAllDisabledBehavior()
*/

/*!
    \fn ActionContainer::onAllDisabledBehavior() const
只有当它包含禁用的项和子菜单时，才返回由此操作容器表示的菜单的行为。
对于菜单，默认值为ActionContainer :: Disable，对于菜单栏，默认为ActionContainer :: Show。
    \sa ActionContainer::OnAllDisabledBehavior
    \sa ActionContainer::setOnAllDisabledBehavior()
*/

/*!
    \fn int ActionContainer::id() const
    \internal
*/

/*!
    \fn QMenu *ActionContainer::menu() const
    返回此操作容器表示的QMenu实例，如果此操作容器表示菜单栏，则返回0。
*/

/*!
    \fn QMenuBar *ActionContainer::menuBar() const
    返回此操作容器表示的QMenuBar实例，如果此操作容器表示菜单，则返回0。
*/

/*!
    \fn QAction *ActionContainer::insertLocation(Id group) const
    返回表示\ a组的动作，该动作可与\ c {QWidget :: insertAction}一起使用。
*/

/*!
    \fn void ActionContainer::appendGroup(Id group)
将具有给定\标识符的组添加到操作容器。
使用组，您可以将操作容器分段为逻辑部分，并将操作和菜单直接添加到这些部分。
    \sa addAction()
    \sa addMenu()
*/

/*!
    \fn void ActionContainer::addAction(Command *action, Id group = Id())
将\ a操作作为菜单项添加到此操作容器中。
该操作将添加为指定\ a组的最后一项。
    \sa appendGroup()
    \sa addMenu()
*/

/*!
    \fn void ActionContainer::addMenu(ActionContainer *menu, Id group = Id())
将\ a菜单作为子菜单添加到此操作容器中。
菜单将添加为指定\ a组的最后一项。
    \sa appendGroup()
    \sa addAction()
*/

// ---------- ActionContainerPrivate ------------

/*!
    \class Core::Internal::ActionContainerPrivate
    \internal
*/

ActionContainerPrivate::ActionContainerPrivate(Id id)
    : m_onAllDisabledBehavior(Disable), m_id(id), m_updateRequested(false)
{
    appendGroup(Constants::G_DEFAULT_ONE);
    appendGroup(Constants::G_DEFAULT_TWO);
    appendGroup(Constants::G_DEFAULT_THREE);
    scheduleUpdate();
}

void ActionContainerPrivate::setOnAllDisabledBehavior(OnAllDisabledBehavior behavior)
{
    m_onAllDisabledBehavior = behavior;
}

ActionContainer::OnAllDisabledBehavior ActionContainerPrivate::onAllDisabledBehavior() const
{
    return m_onAllDisabledBehavior;
}

void ActionContainerPrivate::appendGroup(Id groupId)
{
    m_groups.append(Group(groupId));
}

void ActionContainerPrivate::insertGroup(Id before, Id groupId)
{
    QList<Group>::iterator it = m_groups.begin();
    while (it != m_groups.end()) {
        if (it->id == before) {
            m_groups.insert(it, Group(groupId));
            break;
        }
        ++it;
    }
}

QList<Group>::const_iterator ActionContainerPrivate::findGroup(Id groupId) const
{
    QList<Group>::const_iterator it = m_groups.constBegin();
    while (it != m_groups.constEnd()) {
        if (it->id == groupId)
            break;
        ++it;
    }
    return it;
}


QAction *ActionContainerPrivate::insertLocation(Id groupId) const
{
    QList<Group>::const_iterator it = findGroup(groupId);
    if(it == m_groups.constEnd())
        return nullptr;
    return insertLocation(it);
}

QAction *ActionContainerPrivate::insertLocation(QList<Group>::const_iterator group) const
{
    if (group == m_groups.constEnd())
        return nullptr;
    ++group;
    while (group != m_groups.constEnd()) {
        if (!group->items.isEmpty()) {
            QObject *item = group->items.first();
            /**本方法返回object向下的转型T，如果转型不成功则返回0，如果传入的object本身就是0则返回0。**/
            if (auto cmd = qobject_cast<Command *>(item)) {
                return cmd->action();
            } else if (auto container = qobject_cast<ActionContainer *>(item)) {
                if (container->menu())
                    return container->menu()->menuAction();
            }
//            QTC_ASSERT(false, return nullptr);
            return nullptr;
        }
        ++group;
    }
    return nullptr;
}

void ActionContainerPrivate::addAction(Command *command, Id groupId)
{
    if (!canAddAction(command))
        return;

    const Id actualGroupId = groupId.isValid() ? groupId : Id(Constants::G_DEFAULT_TWO);
    QList<Group>::const_iterator groupIt = findGroup(actualGroupId);
    if(groupIt != m_groups.constEnd())
        ;
    else{
        qDebug() << "Can't find group"
               << groupId.name() << "in container" << id().name();
        return;
    }
    QAction *beforeAction = insertLocation(groupIt);
    m_groups[groupIt-m_groups.constBegin()].items.append(command);

    connect(command, &Command::activeStateChanged, this, &ActionContainerPrivate::scheduleUpdate);
    connect(command, &QObject::destroyed, this, &ActionContainerPrivate::itemDestroyed);
    insertAction(beforeAction, command->action());
    scheduleUpdate();
}

void ActionContainerPrivate::addMenu(ActionContainer *menu, Id groupId)
{
    auto containerPrivate = static_cast<ActionContainerPrivate *>(menu);
    if (!containerPrivate->canBeAddedToMenu())
        return;

    auto container = static_cast<MenuActionContainer *>(containerPrivate);
    const Id actualGroupId = groupId.isValid() ? groupId : Id(Constants::G_DEFAULT_TWO);
    QList<Group>::const_iterator groupIt = findGroup(actualGroupId);
    if(groupIt != m_groups.constEnd())
        ;
    else
        return;
    QAction *beforeAction = insertLocation(groupIt);
    m_groups[groupIt-m_groups.constBegin()].items.append(menu);

    connect(menu, &QObject::destroyed, this, &ActionContainerPrivate::itemDestroyed);
    insertMenu(beforeAction, container->menu());
    scheduleUpdate();
}

void ActionContainerPrivate::addMenu(ActionContainer *before, ActionContainer *menu, Id groupId)
{
    auto containerPrivate = static_cast<ActionContainerPrivate *>(menu);
    if (!containerPrivate->canBeAddedToMenu())
        return;

    auto container = static_cast<MenuActionContainer *>(containerPrivate);
    const Id actualGroupId = groupId.isValid() ? groupId : Id(Constants::G_DEFAULT_TWO);
    QList<Group>::const_iterator groupIt = findGroup(actualGroupId);
    if(groupIt != m_groups.constEnd())
        ;
    else
        return;
    QAction *beforeAction = before->menu()->menuAction();
    m_groups[groupIt-m_groups.constBegin()].items.append(menu);

    connect(menu, &QObject::destroyed, this, &ActionContainerPrivate::itemDestroyed);
    insertMenu(beforeAction, container->menu());
    scheduleUpdate();
}

/*!
 * Adds a separator to the end of the given \a group to the action container, which is enabled
 * for a given \a context. The created separator action is returned through \a outSeparator.
 *
 * Returns the created Command for the separator.
 */
/*! \a context \a group \a outSeparator
 * \internal
 */
Command *ActionContainerPrivate::addSeparator(const Context &context, Id group, QAction **outSeparator)
{
    static int separatorIdCount = 0;
    auto separator = new QAction(this);
    separator->setSeparator(true);
    Id sepId = id().withSuffix(".Separator.").withSuffix(++separatorIdCount);
    Command *cmd = ActionManager::registerAction(separator, sepId, context);
    addAction(cmd, group);
    if (outSeparator)
        *outSeparator = separator;
    return cmd;
}

void ActionContainerPrivate::clear()
{
    QMutableListIterator<Group> it(m_groups);
    while (it.hasNext()) {
        Group &group = it.next();
        foreach (QObject *item, group.items) {
            if (auto command = qobject_cast<Command *>(item)) {
                removeAction(command->action());
                disconnect(command, &Command::activeStateChanged,
                           this, &ActionContainerPrivate::scheduleUpdate);
                disconnect(command, &QObject::destroyed, this, &ActionContainerPrivate::itemDestroyed);
            } else if (auto container = qobject_cast<ActionContainer *>(item)) {
                container->clear();
                disconnect(container, &QObject::destroyed,
                           this, &ActionContainerPrivate::itemDestroyed);
                removeMenu(container->menu());
            }
        }
        group.items.clear();
    }
    scheduleUpdate();
}

void ActionContainerPrivate::itemDestroyed()
{
    QObject *obj = sender();
    QMutableListIterator<Group> it(m_groups);
    while (it.hasNext()) {
        Group &group = it.next();
        if (group.items.removeAll(obj) > 0)
            break;
    }
}

Id ActionContainerPrivate::id() const
{
    return m_id;
}

QMenu *ActionContainerPrivate::menu() const
{
    return nullptr;
}

QMenuBar *ActionContainerPrivate::menuBar() const
{
    return nullptr;
}

bool ActionContainerPrivate::canAddAction(Command *action) const
{
    return action && action->action();
}

void ActionContainerPrivate::scheduleUpdate()
{
    if (m_updateRequested)
        return;
    m_updateRequested = true;
    QTimer::singleShot(0, this, &ActionContainerPrivate::update);
}

void ActionContainerPrivate::update()
{
    updateInternal();
    m_updateRequested = false;
}

// ---------- MenuActionContainer ------------

/*!
    \class Core::Internal::MenuActionContainer
    \internal
*/

MenuActionContainer::MenuActionContainer(Id id)
    : ActionContainerPrivate(id),
      m_menu(new QMenu)
{
    m_menu->setObjectName(id.toString());
    m_menu->menuAction()->setMenuRole(QAction::NoRole);
    setOnAllDisabledBehavior(Disable);
}

MenuActionContainer::~MenuActionContainer()
{
    delete m_menu;
}

QMenu *MenuActionContainer::menu() const
{
    return m_menu;
}

void MenuActionContainer::insertAction(QAction *before, QAction *action)
{
    m_menu->insertAction(before, action);
}

void MenuActionContainer::insertMenu(QAction *before, QMenu *menu)
{
    menu->setParent(m_menu, menu->windowFlags()); // work around issues with Qt Wayland (QTBUG-68636)
    m_menu->insertMenu(before, menu);
}

void MenuActionContainer::removeAction(QAction *action)
{
    m_menu->removeAction(action);
}

void MenuActionContainer::removeMenu(QMenu *menu)
{
    m_menu->removeAction(menu->menuAction());
}

bool MenuActionContainer::updateInternal()
{
    if (onAllDisabledBehavior() == Show)
        return true;

    bool hasitems = false;
    QList<QAction *> actions = m_menu->actions();

    QListIterator<Group> it(m_groups);
    while (it.hasNext()) {
        const Group &group = it.next();
        foreach (QObject *item, group.items) {
            if (auto container = qobject_cast<ActionContainerPrivate*>(item)) {
                actions.removeAll(container->menu()->menuAction());
                if (container == this) {
                    QByteArray warning = Q_FUNC_INFO + QByteArray(" container '");
                    if (this->menu())
                        warning += this->menu()->title().toLocal8Bit();
                    warning += "' contains itself as subcontainer";
                    qWarning("%s", warning.constData());
                    continue;
                }
                if (container->updateInternal()) {
                    hasitems = true;
                    break;
                }
            } else if (auto command = qobject_cast<Command *>(item)) {
                actions.removeAll(command->action());
                if (command->isActive()) {
                    hasitems = true;
                    break;
                }
            } else {
//                QTC_ASSERT(false, continue);
                continue;
            }
        }
        if (hasitems)
            break;
    }
    if (!hasitems) {
        // look if there were actions added that we don't control and check if they are enabled
        foreach (const QAction *action, actions) {
            if (!action->isSeparator() && action->isEnabled()) {
                hasitems = true;
                break;
            }
        }
    }

    if (onAllDisabledBehavior() == Hide)
        m_menu->menuAction()->setVisible(hasitems);
    else if (onAllDisabledBehavior() == Disable)
        m_menu->menuAction()->setEnabled(hasitems);

    return hasitems;
}

bool MenuActionContainer::canBeAddedToMenu() const
{
    return true;
}


// ---------- MenuBarActionContainer ------------

/*!
    \class Core::Internal::MenuBarActionContainer
    \internal
*/

MenuBarActionContainer::MenuBarActionContainer(Id id)
    : ActionContainerPrivate(id), m_menuBar(nullptr)
{
    setOnAllDisabledBehavior(Show);
}

void MenuBarActionContainer::setMenuBar(QMenuBar *menuBar)
{
    m_menuBar = menuBar;
}

QMenuBar *MenuBarActionContainer::menuBar() const
{
    return m_menuBar;
}

void MenuBarActionContainer::insertAction(QAction *before, QAction *action)
{
    m_menuBar->insertAction(before, action);
}

void MenuBarActionContainer::insertMenu(QAction *before, QMenu *menu)
{
    menu->setParent(m_menuBar, menu->windowFlags()); // work around issues with Qt Wayland (QTBUG-68636)
    m_menuBar->insertMenu(before, menu);
}

void MenuBarActionContainer::removeAction(QAction *action)
{
    m_menuBar->removeAction(action);
}

void MenuBarActionContainer::removeMenu(QMenu *menu)
{
    m_menuBar->removeAction(menu->menuAction());
}

bool MenuBarActionContainer::updateInternal()
{
    if (onAllDisabledBehavior() == Show)
        return true;

    bool hasitems = false;
    QList<QAction *> actions = m_menuBar->actions();
    for (int i=0; i<actions.size(); ++i) {
        if (actions.at(i)->isVisible()) {
            hasitems = true;
            break;
        }
    }

    if (onAllDisabledBehavior() == Hide)
        m_menuBar->setVisible(hasitems);
    else if (onAllDisabledBehavior() == Disable)
        m_menuBar->setEnabled(hasitems);

    return hasitems;
}

bool MenuBarActionContainer::canBeAddedToMenu() const
{
    return false;
}

//} // namespace Internal

Command *ActionContainer::addSeparator(Id group)
{
    static const Context context(Constants::C_GLOBAL);
    return addSeparator(context, group);
}

//} // namespace Core
