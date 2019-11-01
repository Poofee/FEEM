#include "mainwindow.h"

#include <QApplication>
#include <QStatusBar>
#include <QMenuBar>
#include <QDockWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QMdiArea>
#include <QTextEdit>
#include <QHBoxLayout>

#include <QTreeWidget>
#include <QTableWidget>
#include <QTabWidget>

#include "pf_actionfactory.h"
#include "pf_actiongroupmanager.h"
#include "pf_actionhandler.h"
#include "pf_widgetfactory.h"
#include "pf_graphicview.h"
#include "pf_cadwidget.h"
#include "pf_projecttreewidget.h"
#include "pf_modelwidget.h"
#include "pf_materiallibrary.h"

#include "actionmanager/actioncontainer.h"
#include "actionmanager/actioncontainer_p.h"
#include "actionmanager/actionmanager.h"
#include "actionmanager/actionmanager_p.h"
#include "actionmanager/command.h"
#include "ouptput/messagemanager.h"
#include "pf_projectexplorer.h"

#include "pf_document.h"

MainWindow::MainWindow(QWidget *parent)
    : RibbonWindow(parent)
    , ag_manager(new PF_ActionGroupManager(this))
    , m_coreImpl(new ICore(this))
    , actionHandler(new PF_ActionHandler(this))
    , dock_modelBuilderTree(nullptr)
    , dock_messageOutputPane(nullptr)
    , dock_materialLibraryTree(nullptr)
{
    init();
}

MainWindow::~MainWindow()
{
    delete m_messageManager;
    m_messageManager = nullptr;
    delete m_proPlugin;
    m_proPlugin = nullptr;
}

void MainWindow::init()
{
    m_ribbonStyle->setAccentColor(OfficeStyle::AccentColorBlue);

    registerDefaultContainers();
    registerDefaultActions();

    /*创建动作列表*/
    PF_ActionFactory a_factory(this, actionHandler);
    a_factory.fillActionContainer(a_map,ag_manager);
    /*创建组件*/
    PF_WidgetFactory w_factory(this,a_map,ag_manager);
    w_factory.createRibbon();
    w_factory.createMenuFile();
    ribbonBar()->setFrameThemeEnabled();//不显示一条白带
    //ribbonBar()->setTitleBackground(QPixmap(":/main/splash.png") );

    PF_Document* doc = new PF_Document();

    PF_CADWidget* cad = new PF_CADWidget(doc,this);
    cad->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    setCentralWidget(cad);
    actionHandler->set_view(cad->getGraphicView());
    actionHandler->set_document(doc);

    /** 需要在设置dock之前初始化 **/
    m_proPlugin = new PF_ProjectExplorerPlugin;
    m_proPlugin->initialize();

    setupDockWidgets();
    setupStatusBar();

    m_messageManager = new MessageManager;

    /** Qribbon更新主题 **/
    Qtitan::OfficeStyle* st = (Qtitan::OfficeStyle*)qApp->style();
    Qtitan::OfficeStyle::Theme theme = Qtitan::OfficeStyle::Office2010Silver;

    st->setTheme(theme);

    updateActionsTheme();
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

/*!
 \brief 添加侧边栏以及周围的控件

*/
void MainWindow::setupDockWidgets()
{
    //QWidget *p = takeCentralWidget();
    //if(p) delete p;
    //setDockNestingEnabled(true);
    // make the left and right dock areas dominant
    setCorner(Qt::TopLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::TopRightCorner, Qt::RightDockWidgetArea);
    setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);

    if(!dock_modelBuilderTree){
        dock_modelBuilderTree = new QDockWidget(this);
        dock_modelBuilderTree->setWindowTitle(tr("Project tree"));
        dock_modelBuilderTree->setObjectName("project_dockwidget");
        QWidget * widgetProjectTree = new PF_ModelWidget(dock_modelBuilderTree);
//        fillSolutionExplorer(widgetProjectTree);
        dock_modelBuilderTree->setWidget(widgetProjectTree);
        addDockWidget(Qt::LeftDockWidgetArea,dock_modelBuilderTree);
    }

    if(!dock_messageOutputPane){
        dock_messageOutputPane = new QDockWidget(this);

        dock_messageOutputPane->setWindowTitle(tr("Log Infor"));
        dock_messageOutputPane->setObjectName("log_dockwidget");
        QWidget * widgetLog = new QWidget(dock_messageOutputPane);
        QHBoxLayout * hbox = new QHBoxLayout(widgetLog);
        QTabWidget *tabInfo = new QTabWidget(widgetLog);
        QTextEdit * logEdit = new QTextEdit;
        tabInfo->addTab(logEdit,tr("Run log"));
        QTextEdit * logEdit1 = new QTextEdit;
        tabInfo->addTab(logEdit1,tr("pythonconsole"));
        logEdit->append("hello, this is one");
        logEdit1->append(("hello, this is two."));
        hbox->addWidget(tabInfo);
        widgetLog->setLayout(hbox);
        dock_messageOutputPane->setWidget(widgetLog);
        addDockWidget(Qt::BottomDockWidgetArea,dock_messageOutputPane);
    }

    if(!dock_materialLibraryTree){
        dock_materialLibraryTree = new QDockWidget(this);
        dock_materialLibraryTree->setWindowTitle(tr("Material Library"));
        dock_materialLibraryTree->setObjectName("dock_materialLibraryTree");
        QWidget* materialLibraryTree = new PF_MaterialLibraryWidget(dock_materialLibraryTree);
        dock_materialLibraryTree->setWidget(materialLibraryTree);
        addDockWidget(Qt::RightDockWidgetArea,dock_materialLibraryTree);
    }
}

/*!
 \brief 初始化状态栏

*/
void MainWindow::setupStatusBar()
{
    //创建状态栏
    QStatusBar* status_bar = statusBar();
    QLabel* label1 = new QLabel("Made by Poofee");
    QLabel* statusLabel = new QLabel();
    statusLabel->setMinimumWidth(200);
    statusLabel->setAlignment(Qt::AlignHCenter);
    statusLabel->setText("x: 100, y: 200");

    label1->setMinimumWidth(200);
    label1->setAlignment(Qt::AlignHCenter);

    status_bar->addPermanentWidget(new QLabel());/**左侧占位**/
    status_bar->addPermanentWidget(statusLabel);
    status_bar->addPermanentWidget(label1);
}

/**
 * @brief
 *
 */
void MainWindow::registerDefaultContainers()
{

}

/**
 * @brief
 *
 */
void MainWindow::registerDefaultActions()
{

}

//void MainWindow::addContextObject(IContext *context)
//{
//    if (!context)
//        return;
//    QWidget *widget = context->widget();
//    if (m_contextWidgets.contains(widget))
//        return;

//    m_contextWidgets.insert(widget, context);
//}

//void MainWindow::removeContextObject(IContext *context)
//{
//    if (!context)
//        return;

//    QWidget *widget = context->widget();
//    if (!m_contextWidgets.contains(widget))
//        return;

//    m_contextWidgets.remove(widget);
//    if (m_activeContext.removeAll(context) > 0)
//        updateContextObject(m_activeContext);
//}



//void MainWindow::updateContext()
//{
//    Context contexts = m_highPrioAdditionalContexts;

//    foreach (IContext *context, m_activeContext)
//        contexts.add(context->context());

//    contexts.add(m_lowPrioAdditionalContexts);

//    Context uniquecontexts;
//    for (const Id &id : qAsConst(contexts)) {
//        if (!uniquecontexts.contains(id))
//            uniquecontexts.add(id);
//    }

//    ActionManager::setContext(uniquecontexts);
//    emit m_coreImpl->contextChanged(uniquecontexts);
//}
