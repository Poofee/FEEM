#include "mainwindow.h"

#include <QStatusBar>
#include <QMenuBar>
#include <QDockWidget>
#include <QFileDialog>
#include <QMessageBox>

#include "pf_actionfactory.h"
#include "pf_actiongroupmanager.h"
#include "pf_actionhandler.h"
#include "pf_widgetfactory.h"
#include "pf_centralwidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ag_manager(new PF_ActionGroupManager(this))
    , actionHandler(new PF_ActionHandler(this))
{
    //创建状态栏
    QStatusBar* status_bar = statusBar();

    PF_CentralWidget* central = new PF_CentralWidget(this);
    setCentralWidget(central);
    mdiAreaCAD = central->getMdiArea();

    //创建动作列表
    PF_ActionFactory a_factory(this, actionHandler);
    a_factory.fillActionContainer(a_map,ag_manager);
    //创建组件
    PF_WidgetFactory w_factory(this,a_map,ag_manager);
    w_factory.createMenus(menuBar());
    w_factory.createStandardToolbars(actionHandler);
}

MainWindow::~MainWindow()
{

}

void MainWindow::slotFileNew() {
	
}

void MainWindow::slotFileOpen() {

}

void MainWindow::slotFileSave() {

}

void MainWindow::slotFileSaveAs() {

}

void MainWindow::slotImportDXF() {

}

void MainWindow::slotExportDXF() {

}

void MainWindow::slotPrint() {

}

void MainWindow::slotPrintView() {

}

void MainWindow::slotFileQuit() {

}
