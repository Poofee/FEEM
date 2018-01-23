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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ag_manager(new PF_ActionGroupManager(this))
    , actionHandler(new PF_ActionHandler(this))
{
    QStatusBar* status_bar = statusBar();

    PF_ActionFactory a_factory(this, actionHandler);

    PF_WidgetFactory w_factory(this,a_map,ag_manager);
    w_factory.createMenus(menuBar());
    w_factory.createStandardToolbars(actionHandler);
}

MainWindow::~MainWindow()
{

}
