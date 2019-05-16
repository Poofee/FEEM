#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "context.h"
#include "icore.h"

#include <QMainWindow>
#include <QMap>
#include <QDockWidget>

#include "ribbonwindow.h"

class QMdiArea;
class QMdiSubWindow;
class PF_ActionHandler;
class PF_WidgetFactory;
class PF_ActionGroupManager;
class FlexWidget;

class MessageManager;
class PF_ProjectExplorerPlugin;

class PF_ModelWidget;
class PF_MaterialLibraryWidget;

class MainWindow : public RibbonWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void init();
    void openView_N(int n);
//    void fillSolutionExplorer(QWidget * w);
//    void createTree(QWidget * w);
//    void createTable(QWidget * w);
    void setupDockWidgets();
    void setupStatusBar();

    void registerDefaultContainers();
    void registerDefaultActions();

//    void addContextObject(IContext *context);
//    void removeContextObject(IContext *context);

//    void updateContextObject(const QList<IContext *> &context);
//    void updateContext();

    QMap<QString, QAction*> a_map;/**动作列表**/
    PF_ActionGroupManager* ag_manager;
    FlexWidget* content;

public slots:
    void slotFileOpen();
    void slotFileNew();
    void slotFileQuit();
    void slotFileSave();
    void slotFileSaveAs();
    void slotImportDXF();
    void slotExportDXF();
    void slotPrint();
    void slotPrintView();

private:
    /** 插件  **/
    ICore* m_coreImpl = nullptr;
    MessageManager* m_messageManager = nullptr;
    PF_ProjectExplorerPlugin* m_proPlugin = nullptr;

    QList<IContext *> m_activeContext;

    QMap<QWidget *, IContext *> m_contextWidgets;

    /**   **/
    PF_WidgetFactory* dialogFactory;
    PF_ActionHandler* actionHandler;

    // GUI

    QDockWidget* dock_modelBuilderTree;
    QDockWidget* dock_messageOutputPane;
    QDockWidget* dock_materialLibraryTree;
};

#endif // MAINWINDOW_H
