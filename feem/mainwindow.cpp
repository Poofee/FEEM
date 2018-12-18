#include "mainwindow.h"

#include <QApplication>
#include <QStatusBar>
#include <QMenuBar>
#include <QDockWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QMdiArea>
#include <QTextEdit>

#include "pf_actionfactory.h"
#include "pf_actiongroupmanager.h"
#include "pf_actionhandler.h"
#include "pf_widgetfactory.h"
#include "pf_centralwidget.h"
#include "pf_mdisubwindow.h"

#include "pf_document.h"

MainWindow::MainWindow(QWidget *parent)
    : RibbonWindow(parent)
    , ag_manager(new PF_ActionGroupManager(this))
    , actionHandler(new PF_ActionHandler(this))
{
    m_ribbonStyle->setAccentColor(OfficeStyle::AccentColorGreen);
    //创建状态栏
    //QStatusBar* status_bar = statusBar();

    //PF_CentralWidget* central = new PF_CentralWidget(this);
    //setCentralWidget(central);
    //mdiAreaCAD = central->getMdiArea();

    //创建动作列表
    PF_ActionFactory a_factory(this, actionHandler);
    a_factory.fillActionContainer(a_map,ag_manager);
    //创建组件
    PF_WidgetFactory w_factory(this,a_map,ag_manager);
    //w_factory.createMenus(menuBar());
    //w_factory.createStandardToolbars(actionHandler);
    w_factory.createRibbon();
    w_factory.createMenuFile();
    ribbonBar()->setFrameThemeEnabled();//不显示一条白带
    ribbonBar()->setTitleBackground(QPixmap(":/res/background.png") );

    //PF_Document* doc = new PF_Document();
    //PF_MdiSubWindow* m = new PF_MdiSubWindow(doc,mdiAreaCAD);
    //actionHandler->set_view(m->getGraphicView());
    //actionHandler->set_document(m->getDocument());
    //m->showMaximized();

    QTextEdit * m_textEdit = new QTextEdit(this);

    setCentralWidget(m_textEdit);
    m_textEdit->setFocus();

    Qtitan::OfficeStyle* st = (Qtitan::OfficeStyle*)qApp->style();
    Qtitan::OfficeStyle::Theme theme = Qtitan::OfficeStyle::Office2010Silver;

//    if (theme == Qtitan::OfficeStyle::Windows7Scenic   ||
//        theme == Qtitan::OfficeStyle::Office2010Silver ||
//        theme == Qtitan::OfficeStyle::Office2010Blue   ||
//        theme == Qtitan::OfficeStyle::Office2010Black  ||
//        theme == Qtitan::OfficeStyle::Office2013White)
//        ribbonBar()->getSystemButton()->setToolButtonStyle(Qt::ToolButtonTextOnly);
//    else
//        ribbonBar()->getSystemButton()->setToolButtonStyle(Qt::ToolButtonFollowStyle);

    if (theme != st->getTheme())
        st->setTheme(theme);

    updateActionsTheme();
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
