#pragma once

//#include <coreplugin/core_global.h>
#include "id.h"

//#include <utils/hostosinfo.h>

#include <QObject>

QT_BEGIN_NAMESPACE
class QAction;
class QKeySequence;
class QToolButton;
QT_END_NAMESPACE


//namespace Core {

class Context;

//constexpr bool useMacShortcuts = Utils::HostOsInfo::isMacHost();

class Command : public QObject
{
    Q_OBJECT
public:
    enum CommandAttribute {
        CA_Hide = 1,
        CA_UpdateText = 2,
        CA_UpdateIcon = 4,
        CA_NonConfigurable = 8
    };
    Q_DECLARE_FLAGS(CommandAttributes, CommandAttribute)

    virtual void setDefaultKeySequence(const QKeySequence &key) = 0;
    virtual QKeySequence defaultKeySequence() const = 0;
    virtual QKeySequence keySequence() const = 0;
    /**明确设置描述（例如在快捷方式设置中使用）
    默认是使用动作文本进行操作，或者使用whatsThis作为快捷方式，
    或者，如果这些是空的，则作为最后一个回退，命令ID字符串
    覆盖默认值，例如 如果文本是依赖于上下文的并且包含文件名等**/
    virtual void setDescription(const QString &text) = 0;
    virtual QString description() const = 0;

    virtual Id id() const = 0;

    virtual QAction *action() const = 0;
    virtual Context context() const = 0;

    virtual void setAttribute(CommandAttribute attr) = 0;
    virtual void removeAttribute(CommandAttribute attr) = 0;
    virtual bool hasAttribute(CommandAttribute attr) const = 0;

    virtual bool isActive() const = 0;

    virtual void setKeySequence(const QKeySequence &key) = 0;
    virtual QString stringWithAppendedShortcut(const QString &str) const = 0;
    void augmentActionWithShortcutToolTip(QAction *action) const;
    static QToolButton *toolButtonWithAppendedShortcut(QAction *action, Command *cmd);

    virtual bool isScriptable() const = 0;
    virtual bool isScriptable(const Context &) const = 0;

signals:
    void keySequenceChanged();
    void activeStateChanged();
};

//} // namespace Core

//Q_DECLARE_OPERATORS_FOR_FLAGS(Command::CommandAttributes)
