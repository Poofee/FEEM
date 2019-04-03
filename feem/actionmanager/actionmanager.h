#pragma once

//#include "coreplugin/core_global.h"
#include "constants.h"
#include "context.h"
#include "command.h"

#include <QObject>
#include <QList>

QT_BEGIN_NAMESPACE
class QAction;
class QString;
QT_END_NAMESPACE

//namespace Core {

class ActionContainer;
class Command;
class Context;

//namespace Internal {
//class CorePlugin;
//class MainWindow;
//} // Internal

class ActionManager : public QObject
{
    Q_OBJECT
public:
    static ActionManager *instance();

    static ActionContainer *createMenu(Id id);
    static ActionContainer *createMenuBar(Id id);

    static Command *registerAction(QAction *action, Id id,
                                   const Context &context = Context(Constants::C_GLOBAL),
                                   bool scriptable = false);

    static Command *command(Id id);
    static ActionContainer *actionContainer(Id id);

    static QList<Command *> commands();

    static void unregisterAction(QAction *action, Id id);

    static void setPresentationModeEnabled(bool enabled);
    static bool isPresentationModeEnabled();

    static QString withNumberAccelerator(const QString &text, const int number);

signals:
    void commandListChanged();
    void commandAdded(Id id);

private:
    ActionManager(QObject *parent = nullptr);
    ~ActionManager() override;
    static void saveSettings();
    static void setContext(const Context &context);

//    friend class Core::Internal::CorePlugin; // initialization
//    friend class Core::Internal::MainWindow; // saving settings and setting context
};

//} // namespace Core
