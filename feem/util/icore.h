#ifndef ICORE_H
#define ICORE_H

//#include "core_global.h"
#include "id.h"

#include <QMainWindow>
#include <QObject>
#include <QRect>
#include <QSettings>

#include <functional>

QT_BEGIN_NAMESPACE
class QPrinter;
class QStatusBar;
class QWidget;
template <typename T> class QList;
QT_END_NAMESPACE

//namespace Core {
//class IWizardFactory;
class Context;
class IContext;
//class SettingsDatabase;

//namespace Internal {
class MainWindow;
//}

class ICore : public QObject
{
    Q_OBJECT

    friend class MainWindow;
//    friend class IWizardFactory;

    explicit ICore(MainWindow *mw);
    ~ICore() override;

public:
    enum class ContextPriority {
        High,
        Low
    };

    // This should only be used to acccess the signals, so it could
    // theoretically return an QObject *. For source compatibility
    // it returns a ICore.
    static ICore *instance();

//    static bool isNewItemDialogRunning();
//    static QWidget *newItemDialog();
//    static void showNewItemDialog(const QString &title,
//                                  const QList<IWizardFactory *> &factories,
//                                  const QString &defaultLocation = QString(),
//                                  const QVariantMap &extraVariables = QVariantMap());

//    static bool showOptionsDialog(Id page, QWidget *parent = nullptr);
//    static QString msgShowOptionsDialog();
//    static QString msgShowOptionsDialogToolTip();

//    static bool showWarningWithOptions(const QString &title, const QString &text,
//                                       const QString &details = QString(),
//                                       Id settingsId = Id(),
//                                       QWidget *parent = nullptr);

//    static QSettings *settings(QSettings::Scope scope = QSettings::UserScope);
//    static SettingsDatabase *settingsDatabase();
//    static QPrinter *printer();
//    static QString userInterfaceLanguage();

//    static QString resourcePath();
//    static QString userResourcePath();
//    static QString installerResourcePath();
//    static QString libexecPath();
//    static QString clangExecutable(const QString &clangBinDirectory);
//    static QString clangIncludeDirectory(const QString &clangVersion,
//                                         const QString &clangResourceDirectory);

//    static QString versionString();
//    static QString buildCompatibilityString();

    static QMainWindow *mainWindow();
//    static QWidget *dialogParent();
    static QStatusBar *statusBar();
    /* Raises and activates the window for the widget. This contains workarounds for X11. */
//    static void raiseWindow(QWidget *widget);

//    static IContext *currentContextObject();
//    static QWidget *currentContextWidget();
//    static IContext *contextObject(QWidget *widget);
    // Adds and removes additional active contexts, these contexts are appended
    // to the currently active contexts.
//    static void updateAdditionalContexts(const Context &remove, const Context &add,
//                                         ContextPriority priority = ContextPriority::Low);
//    static void addAdditionalContext(const Context &context,
//                                     ContextPriority priority = ContextPriority::Low);
//    static void removeAdditionalContext(const Context &context);
//    static void addContextObject(IContext *context);
//    static void removeContextObject(IContext *context);

    // manages the minimize, zoom and fullscreen actions for the window
//    static void registerWindow(QWidget *window, const Context &context);

    enum OpenFilesFlags {
        None = 0,
        SwitchMode = 1,
        CanContainLineAndColumnNumbers = 2,
         /// Stop loading once the first file fails to load
        StopOnLoadFail = 4,
        SwitchSplitIfAlreadyVisible = 8
    };
//    static void openFiles(const QStringList &fileNames, OpenFilesFlags flags = None);

//    static void addPreCloseListener(const std::function<bool()> &listener);

//    static QString systemInformation();
//    static void setupScreenShooter(const QString &name, QWidget *w, const QRect &rc = QRect());

public slots:
//    static void saveSettings();

signals:
//    void coreAboutToOpen();
//    void coreOpened();
//    void newItemDialogStateChanged();
//    void saveSettingsRequested();
//    void coreAboutToClose();
//    void contextAboutToChange(const QList<Core::IContext *> &context);
//    void contextChanged(const Core::Context &context);

public:
    /* internal use */
//    static QStringList additionalAboutInformation();
//    static void appendAboutInformation(const QString &line);

private:
//    static void updateNewItemDialogState();
};

//} // namespace Core

#endif // ICORE_H
