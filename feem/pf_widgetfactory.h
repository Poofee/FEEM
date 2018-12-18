#ifndef PF_WIDGETFACTORY_H
#define PF_WIDGETFACTORY_H

#include "ribbonwindow.h"
#include <QObject>
#include <QMap>
#include <QString>

class QAction;
class QMenu;
class QMenuBar;
class QToolBar;
class QActionGroup;

class MainWindow;
class PF_ActionGroupManager;
class PF_ActionHandler;

//2018-01-23
//by Poofee
//这个类实现菜单、工具栏的创建
class PF_WidgetFactory : public QObject
{
    Q_OBJECT
public:
    PF_WidgetFactory(MainWindow* mainwin,
                              QMap<QString, QAction*>& action_map,
                              PF_ActionGroupManager* agm);
    void createStandardToolbars(PF_ActionHandler* a_handler);
    void createMenus(QMenuBar* menu_bar);

    void createMenuFile();//创建左上角的系统菜单
    void createQuickAccessBar();//创建顶部的快速访问按钮
    void createRibbon();
    void createStatusBar();
    //Home
    void createGroupProject(Qtitan::RibbonPage* page);
    //definitions
    void createGroupVariable(Qtitan::RibbonPage* page);
    //geometry
    void createGroupImExportGeometry(Qtitan::RibbonPage* page);
    void createGroupBuildGeometry(Qtitan::RibbonPage* page);
    void createGroupDrawSetting(Qtitan::RibbonPage* page);
    void createGroupDraw(Qtitan::RibbonPage* page);
    void createGroupDrawOperation(Qtitan::RibbonPage* page);
    //materials
    void createGroupMaterial(Qtitan::RibbonPage* page);
    //mesh
    void createGroupBuildMesh(Qtitan::RibbonPage* page);
    void createGroupGenerator(Qtitan::RibbonPage* page);
    void createGroupImExportMesh(Qtitan::RibbonPage* page);
    void createGroupStatics(Qtitan::RibbonPage* page);
    void createGroupClearMesh(Qtitan::RibbonPage* page);
    //solve
    void createGroupSolve(Qtitan::RibbonPage* page);
    void createGroupSolverSetting(Qtitan::RibbonPage* page);
    void createGroupClearSolution(Qtitan::RibbonPage* page);
    //results
    void createGroupPlot(Qtitan::RibbonPage* page);
    //
    QMenu* file_menu;//留出接口，添加recentfile

signals:

public slots:

private:
    MainWindow* mainwindow;
    QMap<QString, QAction*>& a_map;
    PF_ActionGroupManager* ag_manager;

    QList<QAction*> file_action;
    QList<QAction*> edit_action;
    QList<QAction*> view_action;
    QList<QAction*> grid_action;
    QList<QAction*> operation_action;
    QList<QAction*> mesh_action;
    QList<QAction*> solve_action;
    QList<QAction*> about_action;
};

#endif // PF_WIDGETFACTORY_H
