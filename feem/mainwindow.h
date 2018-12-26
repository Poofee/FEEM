#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>

#include "ribbonwindow.h"

class QMdiArea;
class QMdiSubWindow;
class PF_ActionHandler;
class PF_WidgetFactory;
class PF_ActionGroupManager;
class FlexWidget;

class MainWindow : public RibbonWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void openView_N(int n);
    void fillSolutionExplorer(QWidget * w);
    void createTree(QWidget * w);
    void createTable(QWidget * w);

    QMap<QString, QAction*> a_map;//动作列表
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
    PF_WidgetFactory* dialogFactory;
    PF_ActionHandler* actionHandler;

    QMdiArea* mdiAreaCAD{nullptr};
    QMdiSubWindow* activeMdiSubWindow;
    QMdiSubWindow* currentSubWindow;
};

#endif // MAINWINDOW_H
