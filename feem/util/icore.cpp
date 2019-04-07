#include "icore.h"

//#include "windowsupport.h"
//#include "dialogs/settingsdialog.h"

//#include <app/app_version.h>
//#include <extensionsystem/pluginmanager.h>

//#include <utils/qtcassert.h>

#include <QSysInfo>
#include <QApplication>

/*!
    \namespace Core
    \brief Core命名空间包含构成Core插件的所有类，这些类构成了\ QC的基本功能。
*/

/*!
    \namespace Core::Internal
    \internal
*/

/*!
    \class Core::ICore
    \brief ICore类允许访问构成\ QC基本功能的不同部分。

您永远不应该创建此接口的子类。
唯一的实例是由Core插件创建的。
您可以通过\ c {Core :: instance（）}从插件访问此实例。

    \mainclass
*/

/*!
    \fn void ICore::showNewItemDialog(const QString &title,
                                      const QList<IWizard *> &wizards,
                                      const QString &defaultLocation = QString(),
                                      const QVariantMap &extraVariables = QVariantMap())

    打开一个对话框，用户可以从中选择一组创建新文件，类或项目的向导。

\ _标题参数显示为对话框标题。
创建文件的路径（如果用户不更改它）在\ defaultLocation中设置。
它默认为文件管理器当前文件的路径。

    \sa Core::DocumentManager
*/

/*!
    \fn bool ICore::showOptionsDialog(Id group, Id page, QWidget *parent = 0);

    打开application \ gui选项（或\ gui首选项）对话框，并在指定的\ a组中预先选择\ a页面。

    参数引用相应IOptionsPage的字符串ID。
*/

/*!
    \fn bool ICore::showWarningWithOptions(const QString &title, const QString &text,
                                   const QString &details = QString(),
                                   Id settingsCategory = Id(),
                                   Id settingsId = Id(),
                                   QWidget *parent = 0)

    显示警告消息，其中包含用于打开设置页面的按钮。

应该用于显示配置错误并将用户指向该设置。
如果接受设置对话框，则返回\ c true。
*/


/*!
    \fn QSettings *ICore::settings(QSettings::Scope scope = QSettings::UserScope)

    返回应用程序的主设置对象。

    您可以使用它来检索或设置应用程序范围的设置（与会话或项目特定设置相反）。

    如果\ a范围是\ c QSettings :: UserScope（默认值），则将从用户设置中读取用户设置，并回退\ QC提供的全局设置。

如果\ a范围是\ c QSettings :: SystemScope，则只会读取当前版本的\ QC附带的系统设置。
此功能仅供内部使用。

    \see settingsDatabase()
*/

/*!
    \fn SettingsDatabase *ICore::settingsDatabase()

    返回应用程序的设置数据库。

设置数据库用作常规设置对象的替代方法。
它更适合存储大量数据。
这些设置适用于应用程序。

    \see SettingsDatabase
*/

/*!
    \fn QPrinter *ICore::printer()

    Returns the application's printer object.

    Always use this printer object for printing, so the different parts of the
    application re-use its settings.
*/

/*!
    \fn QString ICore::resourcePath()

    Returns the absolute path that is used for resources like
    project templates and the debugger macros.

    This abstraction is needed to avoid platform-specific code all over
    the place, since on Mac OS X, for example, the resources are part of the
    application bundle.
*/


/*!
    \fn QString ICore::userResourcePath()

    Returns the absolute path in the users directory that is used for
    resources like project templates.

    Use this function for finding the place for resources that the user may
    write to, for example, to allow for custom palettes or templates.
*/

/*!
    \fn QWidget *ICore::mainWindow()

    Returns the main application window.

    For dialog parents use \c dialogParent().
*/

/*!
    \fn QWidget *ICore::dialogParent()

    Returns a widget pointer suitable to use as parent for QDialogs.
 */

/*!
    \fn IContext *ICore::currentContextObject()

    Returns the context object of the current main context.

    \sa ICore::updateAdditionalContexts()
    \sa ICore::addContextObject()
*/

/*!
    \fn void ICore::updateAdditionalContexts(const Context &remove, const Context &add)
    Changes the currently active additional contexts.

    Removes the list of additional contexts specified by \a remove and adds the
    list of additional contexts specified by \a add.

    \sa ICore::hasContext()
*/

/*!
    \fn bool ICore::hasContext(int context) const
    Returns whether the given \a context is currently one of the active contexts.

    \sa ICore::updateAdditionalContexts()
    \sa ICore::addContextObject()
*/

/*!
    \fn void ICore::addContextObject(IContext *context)
    Registers an additional \a context object.

    After registration this context object gets automatically the
    current context object whenever its widget gets focus.

    \sa ICore::removeContextObject()
    \sa ICore::updateAdditionalContexts()
    \sa ICore::currentContextObject()
*/

/*!
    \fn void ICore::removeContextObject(IContext *context)
    Unregisters a \a context object from the list of know contexts.

    \sa ICore::addContextObject()
    \sa ICore::updateAdditionalContexts()
    \sa ICore::currentContextObject()
}
*/

/*!
    \fn void ICore::openFiles(const QStringList &fileNames, OpenFilesFlags flags = None)
    Opens all files from a list of \a fileNames like it would be
    done if they were given to \QC on the command line, or
    they were opened via \gui File > \gui Open.
*/

/*!
    \fn ICore::ICore(Internal::MainWindow *mw)
    \internal
*/

/*!
    \fn ICore::~ICore()
    \internal
*/

/*!
    \fn void ICore::coreOpened()
    Indicates that all plugins have been loaded and the main window is shown.
*/

/*!
    \fn void ICore::saveSettingsRequested()
    Signals that the user has requested that the global settings
    should be saved to disk.

    At the moment that happens when the application is closed, and on \gui{Save All}.
*/

/*!
    \fn void ICore::optionsDialogRequested()
    Enables plugins to perform actions just before the \gui Tools > \gui Options
    dialog is shown.
*/

/*!
    \fn void ICore::coreAboutToClose()
    Enables plugins to perform some pre-end-of-life actions.

    The application is guaranteed to shut down after this signal is emitted.
    It is there as an addition to the usual plugin lifecycle functions, namely
    \c IPlugin::aboutToShutdown(), just for convenience.
*/

/*!
    \fn void ICore::contextAboutToChange(const QList<Core::IContext *> &context)
    Indicates that a new \a context will shortly become the current context
    (meaning that its widget got focus).
*/

/*!
    \fn void ICore::contextChanged(const Core::Context &context)
    Indicates that a new \a context just became the current context. This includes the context
    from the focus object as well as the additional context.
*/

//#include "dialogs/newdialog.h"
//#include "iwizardfactory.h"
#include "mainwindow.h"
//#include "documentmanager.h"

//#include <utils/hostosinfo.h>

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QMessageBox>
#include <QPushButton>
#include <QStatusBar>
#include <QTimer>

//using namespace Core::Internal;
//using namespace ExtensionSystem;

//namespace Core {

// The Core Singleton
static ICore *m_instance = nullptr;
static MainWindow *m_mainwindow = nullptr;

ICore *ICore::instance()
{
    return m_instance;
}

//bool ICore::isNewItemDialogRunning()
//{
//    return NewDialog::currentDialog() || IWizardFactory::isWizardRunning();
//}

//QWidget *ICore::newItemDialog()
//{
//    if (NewDialog::currentDialog())
//        return NewDialog::currentDialog();
//    return IWizardFactory::currentWizard();
//}

ICore::ICore(MainWindow *mainwindow)
{
    m_instance = this;
    m_mainwindow = mainwindow;
    // Save settings once after all plugins are initialized:
//    connect(PluginManager::instance(), &PluginManager::initializationDone,
//            this, &ICore::saveSettings);
//    connect(PluginManager::instance(), &PluginManager::testsFinished, [this] (int failedTests) {
//        emit coreAboutToClose();
//        QCoreApplication::exit(failedTests);
//    });
}

ICore::~ICore()
{
    m_instance = nullptr;
    m_mainwindow = nullptr;
}

//void ICore::showNewItemDialog(const QString &title,
//                              const QList<IWizardFactory *> &factories,
//                              const QString &defaultLocation,
//                              const QVariantMap &extraVariables)
//{
//    QTC_ASSERT(!isNewItemDialogRunning(), return);
//    auto newDialog = new NewDialog(dialogParent());
//    connect(newDialog, &QObject::destroyed, m_instance, &ICore::updateNewItemDialogState);
//    newDialog->setWizardFactories(factories, defaultLocation, extraVariables);
//    newDialog->setWindowTitle(title);
//    newDialog->showDialog();

//    updateNewItemDialogState();
//}

//bool ICore::showOptionsDialog(const Id page, QWidget *parent)
//{
//    return executeSettingsDialog(parent ? parent : dialogParent(), page);
//}

//QString ICore::msgShowOptionsDialog()
//{
//    return QCoreApplication::translate("Core", "Configure...", "msgShowOptionsDialog");
//}

//QString ICore::msgShowOptionsDialogToolTip()
//{
//    if (Utils::HostOsInfo::isMacHost())
//        return QCoreApplication::translate("Core", "Open Preferences dialog.",
//                                           "msgShowOptionsDialogToolTip (mac version)");
//    else
//        return QCoreApplication::translate("Core", "Open Options dialog.",
//                                           "msgShowOptionsDialogToolTip (non-mac version)");
//}

//// Display a warning with an additional button to open
//// the settings dialog at a specified page if settingsId is nonempty.
//bool ICore::showWarningWithOptions(const QString &title, const QString &text,
//                                   const QString &details, Id settingsId, QWidget *parent)
//{
//    if (!parent)
//        parent = m_mainwindow;
//    QMessageBox msgBox(QMessageBox::Warning, title, text,
//                       QMessageBox::Ok, parent);
//    if (!details.isEmpty())
//        msgBox.setDetailedText(details);
//    QAbstractButton *settingsButton = nullptr;
//    if (settingsId.isValid())
//        settingsButton = msgBox.addButton(tr("Settings..."), QMessageBox::AcceptRole);
//    msgBox.exec();
//    if (settingsButton && msgBox.clickedButton() == settingsButton)
//        return showOptionsDialog(settingsId);
//    return false;
//}

//QSettings *ICore::settings(QSettings::Scope scope)
//{
//    if (scope == QSettings::UserScope)
//        return PluginManager::settings();
//    else
//        return PluginManager::globalSettings();
//}

//SettingsDatabase *ICore::settingsDatabase()
//{
//    return m_mainwindow->settingsDatabase();
//}

//QPrinter *ICore::printer()
//{
//    return m_mainwindow->printer();
//}

//QString ICore::userInterfaceLanguage()
//{
//    return qApp->property("qtc_locale").toString();
//}

//QString ICore::resourcePath()
//{
//    return QDir::cleanPath(QCoreApplication::applicationDirPath() + '/' + RELATIVE_DATA_PATH);
//}

//QString ICore::userResourcePath()
//{
//    // Create qtcreator dir if it doesn't yet exist
//    const QString configDir = QFileInfo(settings(QSettings::UserScope)->fileName()).path();
//    const QString urp = configDir + '/' + QLatin1String(Constants::IDE_ID);

//    if (!QFileInfo::exists(urp + QLatin1Char('/'))) {
//        QDir dir;
//        if (!dir.mkpath(urp))
//            qWarning() << "could not create" << urp;
//    }

//    return urp;
//}

//QString ICore::installerResourcePath()
//{
//    return QFileInfo(settings(QSettings::SystemScope)->fileName()).path() + '/'
//           + Constants::IDE_ID;
//}

///*!
//    Returns the path to the command line tools that are shipped with \QC (corresponding
//    to the IDE_LIBEXEC_PATH qmake variable).
// */
//QString ICore::libexecPath()
//{
//    return QDir::cleanPath(QApplication::applicationDirPath() + '/' + RELATIVE_LIBEXEC_PATH);
//}

//static QString clangIncludePath(const QString &clangVersion)
//{
//    return "/lib/clang/" + clangVersion + "/include";
//}

//QString ICore::clangIncludeDirectory(const QString &clangVersion, const QString &clangResourceDirectory)
//{
//    QDir dir(libexecPath() + "/clang" + clangIncludePath(clangVersion));
//    if (!dir.exists() || !QFileInfo(dir, "stdint.h").exists())
//        dir = QDir(clangResourceDirectory);
//    return QDir::toNativeSeparators(dir.canonicalPath());
//}

//QString ICore::clangExecutable(const QString &clangBinDirectory)
//{
//    const QString hostExeSuffix(QTC_HOST_EXE_SUFFIX);
//    QFileInfo executable(libexecPath() + "/clang/bin/clang" + hostExeSuffix);
//    if (!executable.exists())
//        executable = QFileInfo(clangBinDirectory + "/clang" + hostExeSuffix);
//    return QDir::toNativeSeparators(executable.canonicalFilePath());
//}

//static QString compilerString()
//{
//#if defined(Q_CC_CLANG) // must be before GNU, because clang claims to be GNU too
//    QString isAppleString;
//#if defined(__apple_build_version__) // Apple clang has other version numbers
//    isAppleString = QLatin1String(" (Apple)");
//#endif
//    return QLatin1String("Clang " ) + QString::number(__clang_major__) + QLatin1Char('.')
//            + QString::number(__clang_minor__) + isAppleString;
//#elif defined(Q_CC_GNU)
//    return QLatin1String("GCC " ) + QLatin1String(__VERSION__);
//#elif defined(Q_CC_MSVC)
//    if (_MSC_VER > 1999)
//        return QLatin1String("MSVC <unknown>");
//    if (_MSC_VER >= 1910)
//        return QLatin1String("MSVC 2017");
//    if (_MSC_VER >= 1900)
//        return QLatin1String("MSVC 2015");
//#endif
//    return QLatin1String("<unknown compiler>");
//}

//QString ICore::versionString()
//{
//    QString ideVersionDescription;
//    if (QLatin1String(Constants::IDE_VERSION_LONG) != QLatin1String(Constants::IDE_VERSION_DISPLAY))
//        ideVersionDescription = tr(" (%1)").arg(QLatin1String(Constants::IDE_VERSION_LONG));
//    return tr("%1 %2%3").arg(QLatin1String(Constants::IDE_DISPLAY_NAME),
//                             QLatin1String(Constants::IDE_VERSION_DISPLAY),
//                             ideVersionDescription);
//}

//QString ICore::buildCompatibilityString()
//{
//    return tr("Based on Qt %1 (%2, %3 bit)").arg(QLatin1String(qVersion()),
//                                                 compilerString(),
//                                                 QString::number(QSysInfo::WordSize));
//}

//IContext *ICore::currentContextObject()
//{
//    return m_mainwindow->currentContextObject();
//}

//QWidget *ICore::currentContextWidget()
//{
//    IContext *context = currentContextObject();
//    return context ? context->widget() : nullptr;
//}

//IContext *ICore::contextObject(QWidget *widget)
//{
//    return m_mainwindow->contextObject(widget);
//}


QMainWindow *ICore::mainWindow()
{
    return m_mainwindow;
}

//QWidget *ICore::dialogParent()
//{
//    QWidget *active = QApplication::activeModalWidget();
//    if (!active)
//        active = QApplication::activeWindow();
//    if (!active)
//        active = m_mainwindow;
//    return active;
//}

QStatusBar *ICore::statusBar()
{
    return m_mainwindow->statusBar();
}

//void ICore::raiseWindow(QWidget *widget)
//{
//    if (!widget)
//        return;
//    QWidget *window = widget->window();
//    if (window && window == m_mainwindow) {
//        m_mainwindow->raiseWindow();
//    } else {
//        window->raise();
//        window->activateWindow();
//    }
//}

//void ICore::updateAdditionalContexts(const Context &remove, const Context &add,
//                                     ContextPriority priority)
//{
//    m_mainwindow->updateAdditionalContexts(remove, add, priority);
//}

//void ICore::addAdditionalContext(const Context &context, ContextPriority priority)
//{
//    m_mainwindow->updateAdditionalContexts(Context(), context, priority);
//}

//void ICore::removeAdditionalContext(const Context &context)
//{
//    m_mainwindow->updateAdditionalContexts(context, Context(), ContextPriority::Low);
//}

//void ICore::addContextObject(IContext *context)
//{
//    m_mainwindow->addContextObject(context);
//}

//void ICore::removeContextObject(IContext *context)
//{
//    m_mainwindow->removeContextObject(context);
//}

//void ICore::registerWindow(QWidget *window, const Context &context)
//{
//    new WindowSupport(window, context); // deletes itself when widget is destroyed
//}

//void ICore::openFiles(const QStringList &arguments, ICore::OpenFilesFlags flags)
//{
//    m_mainwindow->openFiles(arguments, flags);
//}

/*!
    \fn ICore::addCloseCoreListener

    \brief The \c ICore::addCloseCoreListener function provides a hook for plugins
    to veto on closing the application.

    When the application window requests a close, all listeners are called.
    If one if these calls returns \c false, the process is aborted and the
    event is ignored. If all calls return \c true, \c ICore::coreAboutToClose()
    is emitted and the event is accepted or performed..
*/
//void ICore::addPreCloseListener(const std::function<bool ()> &listener)
//{
//    m_mainwindow->addPreCloseListener(listener);
//}

//QString ICore::systemInformation()
//{
//    QString result = PluginManager::instance()->systemInformation() + '\n';
//    result += versionString() + '\n';
//    result += buildCompatibilityString() + '\n';
//#ifdef IDE_REVISION
//    result += QString("From revision %1\n").arg(QString::fromLatin1(Constants::IDE_REVISION_STR).left(10));
//#endif
//#ifdef QTC_SHOW_BUILD_DATE
//     result += QString("Built on %1 %2\n").arg(QLatin1String(__DATE__), QLatin1String(__TIME__));
//#endif
//     return result;
//}

//static const QByteArray &screenShotsPath()
//{
//    static const QByteArray path = qgetenv("QTC_SCREENSHOTS_PATH");
//    return path;
//}

//class ScreenShooter : public QObject
//{
//public:
//    ScreenShooter(QWidget *widget, const QString &name, const QRect &rc)
//        : m_widget(widget), m_name(name), m_rc(rc)
//    {
//        m_widget->installEventFilter(this);
//    }

//    bool eventFilter(QObject *watched, QEvent *event) override
//    {
//        QTC_ASSERT(watched == m_widget, return false);
//        if (event->type() == QEvent::Show)
//            QTimer::singleShot(0, this, &ScreenShooter::helper);
//        return false;
//    }

//    void helper()
//    {
//        if (m_widget) {
//            QRect rc = m_rc.isValid() ? m_rc : m_widget->rect();
//            QPixmap pm = m_widget->grab(rc);
//            for (int i = 0; ; ++i) {
//                QString fileName = screenShotsPath() + '/' + m_name + QString("-%1.png").arg(i);
//                if (!QFileInfo::exists(fileName)) {
//                    pm.save(fileName);
//                    break;
//                }
//            }
//        }
//        deleteLater();
//    }

//    QPointer<QWidget> m_widget;
//    QString m_name;
//    QRect m_rc;
//};

//void ICore::setupScreenShooter(const QString &name, QWidget *w, const QRect &rc)
//{
//    if (!screenShotsPath().isEmpty())
//        new ScreenShooter(w, name, rc);
//}

//void ICore::saveSettings()
//{
//    emit m_instance->saveSettingsRequested();
//    m_mainwindow->saveSettings();

//    ICore::settings(QSettings::SystemScope)->sync();
//    ICore::settings(QSettings::UserScope)->sync();
//}

//QStringList ICore::additionalAboutInformation()
//{
//    return m_mainwindow->additionalAboutInformation();
//}

//void ICore::appendAboutInformation(const QString &line)
//{
//    m_mainwindow->appendAboutInformation(line);
//}

//void ICore::updateNewItemDialogState()
//{
//    static bool wasRunning = false;
//    static QWidget *previousDialog = nullptr;
//    if (wasRunning == isNewItemDialogRunning() && previousDialog == newItemDialog())
//        return;
//    wasRunning = isNewItemDialogRunning();
//    previousDialog = newItemDialog();
//    emit instance()->newItemDialogStateChanged();
//}

//} // namespace Core
