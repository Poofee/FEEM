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
#include "pf_centralwidget.h"
#include "pf_graphicwindow.h"

#include "QtFlexWidget.h"
#include "QtFlexHelper.h"
#include "QtDockGuider.h"
#include "QtDockWidget.h"
#include "QtFlexManager.h"

#include "pf_document.h"

MainWindow::MainWindow(QWidget *parent)
    : RibbonWindow(parent)
    , ag_manager(new PF_ActionGroupManager(this))
    , actionHandler(new PF_ActionHandler(this))
    , dock_modelBuilderTree(nullptr)
    , dock_messageOutputPane(nullptr)
    , dock_materialLibraryTree(nullptr)
{
    m_ribbonStyle->setAccentColor(OfficeStyle::AccentColorBlue);

    PF_CentralWidget* central = new PF_CentralWidget(this);
    setCentralWidget(central);
    mdiAreaCAD = central->getMdiArea();

    /*创建动作列表*/
    PF_ActionFactory a_factory(this, actionHandler);
    a_factory.fillActionContainer(a_map,ag_manager);
    /*创建组件*/
    PF_WidgetFactory w_factory(this,a_map,ag_manager);
    //w_factory.createMenus(menuBar());
    //w_factory.createStandardToolbars(actionHandler);
    w_factory.createRibbon();
    w_factory.createMenuFile();
    ribbonBar()->setFrameThemeEnabled();//不显示一条白带
    //ribbonBar()->setTitleBackground(QPixmap(":/main/splash.png") );

    PF_Document* doc = new PF_Document();
    PF_MdiSubWindow* m = new PF_MdiSubWindow(doc,mdiAreaCAD);
    actionHandler->set_view(m->getGraphicView());
    actionHandler->set_document(m->getDocument());
    m->showMaximized();

    setupDockWidgets();
    setupStatusBar();

    Qtitan::OfficeStyle* st = (Qtitan::OfficeStyle*)qApp->style();
    Qtitan::OfficeStyle::Theme theme = Qtitan::OfficeStyle::Office2010Silver;

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

void MainWindow::fillSolutionExplorer(QWidget * w){
    QWidget* baseSheet = w;
    /*QToolBar* toolBar = new QToolBar();
    toolBar->setIconSize(QSize(20, 20));
    QAction* act = toolBar->addAction(QIcon(":/res/backward.svg"), QString("Back"));
    act->setToolTip(tr("Back"));
    act = toolBar->addAction(QIcon(":/res/forward.svg"), QString("Forward"));
    act->setToolTip(tr("Forward"));*/

    //m_panelSolutionExplorer->setWidget(baseSheet);
    QVBoxLayout* layout = new QVBoxLayout;
    layout->setMargin(1);
    layout->setSpacing(0);

    QTreeWidget* treeWidget = new QTreeWidget;
    treeWidget->setAlternatingRowColors(false);

    treeWidget->setMinimumWidth(190);
    treeWidget->setHeaderHidden(true);

    QStringList strings;
    strings.append(tr("FEM project"));
    QTreeWidgetItem* mainItem = new QTreeWidgetItem(strings);
    mainItem->setExpanded(true);
    mainItem->setIcon(0, QIcon(":/main/project.svg"));
    treeWidget->insertTopLevelItem(0, mainItem);
    treeWidget->expandItem(mainItem);

    //    strings.clear();
    //    strings.append(tr("Definitions"));
    //    QTreeWidgetItem* item = new QTreeWidgetItem(mainItem, strings);
    //    QFont font = item->font(0);
    //    font.setBold(true);
    //    item->setFont(0, font);
    //    item->setIcon(0, QIcon(":/Resources/cpp_project.png"));
    //    item->setExpanded(true);
    //    treeWidget->insertTopLevelItem(1, item);
    //    treeWidget->expandItem(item);

    strings.clear();
    strings.append(tr("Definitions"));
    QTreeWidgetItem* itemSource = new QTreeWidgetItem(mainItem, strings);
    itemSource->setIcon(0, QIcon(":/main/definition.svg"));
    itemSource->setExpanded(true);
    treeWidget->insertTopLevelItem(1, itemSource);
    treeWidget->expandItem(mainItem);

    //add 5 definitions
    for(int i = 0;i < 5;i++){
        strings.clear();
        strings.append(tr("Definition"));
        strings[0] += tr("%1").arg(i);
        QTreeWidgetItem * itemDef = new QTreeWidgetItem(itemSource, strings);
        itemDef->setIcon(0, QIcon(":/main/definition.svg"));
        treeWidget->insertTopLevelItem(1, itemSource);
        treeWidget->expandItem(itemSource);
    }

    //Geometry
    strings.clear();
    strings.append(tr("Geometry"));
    itemSource = new QTreeWidgetItem(mainItem, strings);
    itemSource->setIcon(0, QIcon(":/main/geometry.svg"));
    itemSource->setExpanded(true);
    treeWidget->insertTopLevelItem(1, itemSource);
    treeWidget->expandItem(itemSource);

    strings.clear();
    strings.append(tr("Geometry Entity"));
    QTreeWidgetItem* item1 = new QTreeWidgetItem(itemSource, strings);
    item1->setIcon(0, QIcon(":/main/geoentity.svg"));
    treeWidget->insertTopLevelItem(1, itemSource);
    treeWidget->expandItem(item1);

    strings.clear();
    strings.append(tr("Points"));
    QTreeWidgetItem * item = new QTreeWidgetItem(item1, strings);
    item->setIcon(0, QIcon(":/main/point.svg"));
    treeWidget->insertTopLevelItem(1, item1);
    treeWidget->expandItem(item1);

    //add 5 points
    for(int i = 0;i < 5;i++){
        strings.clear();
        strings.append(tr("Point"));
        strings[0] += tr("%1").arg(i);
        QTreeWidgetItem * itemPoint = new QTreeWidgetItem(item, strings);
        itemPoint->setIcon(0, QIcon(":/main/point.svg"));
        treeWidget->insertTopLevelItem(1, item);
        treeWidget->expandItem(item);
    }


    strings.clear();
    strings.append(tr("Lines"));
    item = new QTreeWidgetItem(item1, strings);
    item->setIcon(0, QIcon(":/main/line.svg"));
    treeWidget->insertTopLevelItem(1, item1);
    treeWidget->expandItem(item1);

    //add 5 lines
    for(int i = 0;i < 5;i++){
        strings.clear();
        strings.append(tr("Line"));
        strings[0] += tr("%1").arg(i);
        QTreeWidgetItem * itemLine = new QTreeWidgetItem(item, strings);
        itemLine->setIcon(0, QIcon(":/main/line.svg"));
        treeWidget->insertTopLevelItem(1, item);
        treeWidget->expandItem(item);
    }

    strings.clear();
    strings.append(tr("Faces"));
    item = new QTreeWidgetItem(item1, strings);
    item->setIcon(0, QIcon(":/main/face.svg"));
    treeWidget->insertTopLevelItem(1, item1);
    treeWidget->expandItem(item1);

    //add 5 faces
    for(int i = 0;i < 5;i++){
        strings.clear();
        strings.append(tr("Face"));
        strings[0] += tr("%1").arg(i);
        QTreeWidgetItem * itemFace = new QTreeWidgetItem(item, strings);
        itemFace->setIcon(0, QIcon(":/main/face.svg"));
        treeWidget->insertTopLevelItem(1, item);
        treeWidget->expandItem(item);
    }

    //mesh
    strings.clear();
    strings.append(tr("Mesh"));
    itemSource = new QTreeWidgetItem(mainItem, strings);
    itemSource->setIcon(0, QIcon(":/main/mesh.svg"));
    itemSource->setExpanded(true);
    treeWidget->insertTopLevelItem(1, mainItem);
    treeWidget->expandItem(mainItem);

    strings.clear();
    strings.append(tr("Mesh Points"));
    item = new QTreeWidgetItem(itemSource, strings);
    item->setIcon(0, QIcon(":/main/point.svg"));
    treeWidget->insertTopLevelItem(1, itemSource);
    treeWidget->expandItem(itemSource);

    //add 5 mesh points
    for(int i = 0;i < 5;i++){
        strings.clear();
        strings.append(tr("Mesh Point"));
        strings[0] += tr("%1").arg(i);
        QTreeWidgetItem * itemPoint = new QTreeWidgetItem(item, strings);
        itemPoint->setIcon(0, QIcon(":/main/point.svg"));
        treeWidget->insertTopLevelItem(1, item);
        treeWidget->expandItem(item);
    }

    strings.clear();
    strings.append(tr("Mesh  Lines"));
    item = new QTreeWidgetItem(itemSource, strings);
    item->setIcon(0, QIcon(":/main/line.svg"));
    treeWidget->insertTopLevelItem(1, itemSource);
    treeWidget->expandItem(itemSource);

    //add 5 lines
    for(int i = 0;i < 5;i++){
        strings.clear();
        strings.append(tr("Mesh Line"));
        strings[0] += tr("%1").arg(i);
        QTreeWidgetItem * itemLine = new QTreeWidgetItem(item, strings);
        itemLine->setIcon(0, QIcon(":/main/line.svg"));
        treeWidget->insertTopLevelItem(1, item);
        treeWidget->expandItem(item);
    }

    strings.clear();
    strings.append(tr("Mesh Faces"));
    item = new QTreeWidgetItem(itemSource, strings);
    item->setIcon(0, QIcon(":/main/face.svg"));
    treeWidget->insertTopLevelItem(1, itemSource);
    treeWidget->expandItem(itemSource);

    //add 5 faces
    for(int i = 0;i < 5;i++){
        strings.clear();
        strings.append(tr("Mesh Face")+tr("%1").arg(i));
        QTreeWidgetItem * itemFace = new QTreeWidgetItem(item, strings);
        itemFace->setIcon(0, QIcon(":/main/face.svg"));
        treeWidget->insertTopLevelItem(1, item);
        treeWidget->expandItem(item);
    }

    //material
    strings.clear();
    strings.append(tr("Material"));
    itemSource = new QTreeWidgetItem(mainItem, strings);
    itemSource->setIcon(0, QIcon(":/main/material.svg"));
    itemSource->setExpanded(true);
    treeWidget->insertTopLevelItem(1, mainItem);
    treeWidget->expandItem(mainItem);
    //add 5 material
    for(int i = 0;i < 5;i++){
        strings.clear();
        strings.append(tr("Material"));
        strings[0] += tr("%1").arg(i);
        QTreeWidgetItem * itemMaterial = new QTreeWidgetItem(itemSource, strings);
        itemMaterial->setIcon(0, QIcon(":/main/face.svg"));
        treeWidget->insertTopLevelItem(1, itemSource);
        treeWidget->expandItem(itemSource);
    }

    //Solver
    strings.clear();
    strings.append(tr("Solver"));
    itemSource = new QTreeWidgetItem(mainItem, strings);
    itemSource->setIcon(0, QIcon(":/main/solver.svg"));
    itemSource->setExpanded(true);
    treeWidget->insertTopLevelItem(1, mainItem);
    treeWidget->expandItem(mainItem);

    //Post Processing
    strings.clear();
    strings.append(tr("Post Processing"));
    itemSource = new QTreeWidgetItem(mainItem, strings);
    itemSource->setIcon(0, QIcon(":/main/post.svg"));
    itemSource->setExpanded(true);
    treeWidget->insertTopLevelItem(1, mainItem);
    treeWidget->expandItem(mainItem);

    layout->addWidget(treeWidget, 1);
    baseSheet->setLayout(layout);
}

void MainWindow::createTree(QWidget * w){
    QTreeWidget* treeWidget = new QTreeWidget(w);
    treeWidget->setAlternatingRowColors(true);

    treeWidget->setMinimumWidth(190);
    treeWidget->setHeaderHidden(true);

    QStringList strings;
    strings.append(tr("Item 1"));
    QTreeWidgetItem* mainItem = new QTreeWidgetItem(strings);
    mainItem->setCheckState(0, Qt::Checked);
    mainItem->setExpanded(false);
    mainItem->setIcon(0, QIcon(":/Resources/open16x16.png"));
    treeWidget->insertTopLevelItem(0, mainItem);

    strings.clear();
    strings.append(tr("Item 2"));
    QTreeWidgetItem* item = new QTreeWidgetItem(mainItem, strings);
    item->setCheckState(0, Qt::Checked);
    item->setIcon(0, QIcon(":/Resources/open16x16.png"));
    item->setExpanded(true);
    treeWidget->insertTopLevelItem(1, item);

    strings.clear();
    strings.append(tr("Item 3"));
    item = new QTreeWidgetItem(mainItem, strings);
    item->setCheckState(0, Qt::Checked);
    item->setIcon(0, QIcon(":/Resources/open16x16.png"));
    item->setExpanded(true);
    treeWidget->insertTopLevelItem(1, item);

    strings.clear();
    strings.append(tr("Item 4"));
    item = new QTreeWidgetItem(mainItem, strings);
    item->setCheckState(0, Qt::Checked);
    item->setIcon(0, QIcon(":/Resources/open16x16.png"));
    item->setExpanded(true);
    treeWidget->insertTopLevelItem(1, item);

    strings.clear();
    strings.append(tr("Item 5"));
    item = new QTreeWidgetItem(mainItem, strings);
    item->setCheckState(0, Qt::Checked);
    item->setIcon(0, QIcon(":/Resources/open16x16.png"));
    item->setExpanded(true);
    treeWidget->insertTopLevelItem(1, item);

    strings.clear();
    strings.append(tr("Item 6"));
    item = new QTreeWidgetItem(mainItem, strings);
    item->setCheckState(0, Qt::Checked);
    item->setIcon(0, QIcon(":/Resources/open16x16.png"));
    item->setExpanded(true);
    treeWidget->insertTopLevelItem(1, item);
    treeWidget->expandAll();

    QVBoxLayout* layout = new QVBoxLayout;
    layout->setMargin(1);
    layout->setSpacing(0);

    layout->addWidget(treeWidget, 1);
    w->setLayout(layout);
}

void MainWindow::createTable(QWidget * w){
    QTableWidget* tableWidget = new QTableWidget(w);
    tableWidget->setAlternatingRowColors(true);

    tableWidget->setColumnCount(11);
    tableWidget->setRowCount(6);
    tableWidget->setShowGrid(false);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("Name")));
    tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("Path")));
    tableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem(tr("Optimized")));
    tableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem(tr("User Code")));
    tableWidget->setHorizontalHeaderItem(4, new QTableWidgetItem(tr("Symbol Status")));
    tableWidget->setHorizontalHeaderItem(5, new QTableWidgetItem(tr("Symbol File")));
    tableWidget->setHorizontalHeaderItem(6, new QTableWidgetItem(tr("Order")));
    tableWidget->setHorizontalHeaderItem(7, new QTableWidgetItem(tr("Version")));
    tableWidget->setHorizontalHeaderItem(8, new QTableWidgetItem(tr("Timestamp")));
    tableWidget->setHorizontalHeaderItem(9, new QTableWidgetItem(tr("Addresswewe")));
    tableWidget->setHorizontalHeaderItem(10, new QTableWidgetItem(tr("Process")));

    int height = 20;//tableWidget->horizontalHeader()->sizeHint().height();

    tableWidget->setRowHeight(0, height);
    tableWidget->setVerticalHeaderItem(0, new QTableWidgetItem(QIcon(":/Resources/modules.png"), tr("")));
    tableWidget->setItem(0, 0, new QTableWidgetItem(tr("ntdll.dll")));
    tableWidget->setItem(0, 1, new QTableWidgetItem(tr("C:/Windows/System32/ntdll.dll")));
    tableWidget->setItem(0, 2, new QTableWidgetItem(tr("N/A")));
    tableWidget->setItem(0, 3, new QTableWidgetItem(tr("N/A")));
    tableWidget->setItem(0, 4, new QTableWidgetItem(tr("Symbols loaded.")));
    tableWidget->setItem(0, 5, new QTableWidgetItem(tr("C:/Qtitan/customstyles.pdb")));
    tableWidget->setItem(0, 6, new QTableWidgetItem(tr("1")));
    tableWidget->setItem(0, 7, new QTableWidgetItem(tr("6.1.7600.16385 (win7_rtm.090713-1255)")));
    tableWidget->setItem(0, 8, new QTableWidgetItem(tr("27.10.2010 8:32")));
    tableWidget->setItem(0, 9, new QTableWidgetItem(tr("773A0000-774DD000")));
    tableWidget->setItem(0, 10, new QTableWidgetItem(tr("[3376] customstyles.exe: Native")));

    tableWidget->setRowHeight(1, height);
    tableWidget->setVerticalHeaderItem(1, new QTableWidgetItem(QIcon(":/Resources/modules.png"), tr("")));
    tableWidget->setItem(1, 0, new QTableWidgetItem(tr("ntdll.dll")));
    tableWidget->setItem(1, 1, new QTableWidgetItem(tr("C:/Windows/System32/kernel32.dll")));
    tableWidget->setItem(1, 2, new QTableWidgetItem(tr("N/A")));
    tableWidget->setItem(1, 3, new QTableWidgetItem(tr("N/A")));
    tableWidget->setItem(1, 4, new QTableWidgetItem(tr("No symbols loaded.")));
    tableWidget->setItem(1, 5, new QTableWidgetItem(tr("")));
    tableWidget->setItem(1, 6, new QTableWidgetItem(tr("2")));
    tableWidget->setItem(1, 7, new QTableWidgetItem(tr("6.1.7600.16385 (win7_rtm.090713-1255)")));
    tableWidget->setItem(1, 8, new QTableWidgetItem(tr("27.10.2010 8:32")));
    tableWidget->setItem(1, 9, new QTableWidgetItem(tr("872A0000-766DD000")));
    tableWidget->setItem(1, 10, new QTableWidgetItem(tr("[3376] customstyles.exe: Native")));

    tableWidget->setRowHeight(2, height);
    tableWidget->setVerticalHeaderItem(2, new QTableWidgetItem(QIcon(":/Resources/modules.png"), tr("")));
    tableWidget->setItem(2, 0, new QTableWidgetItem(tr("kernelBase.dll")));
    tableWidget->setItem(2, 1, new QTableWidgetItem(tr("C:/Windows/System32/kernelBase.dll")));
    tableWidget->setItem(2, 2, new QTableWidgetItem(tr("N/A")));
    tableWidget->setItem(2, 3, new QTableWidgetItem(tr("N/A")));
    tableWidget->setItem(2, 4, new QTableWidgetItem(tr("No symbols loaded.")));
    tableWidget->setItem(2, 5, new QTableWidgetItem(tr("")));
    tableWidget->setItem(2, 6, new QTableWidgetItem(tr("3")));
    tableWidget->setItem(2, 7, new QTableWidgetItem(tr("6.1.7600.16385 (win7_rtm.090713-1255)")));
    tableWidget->setItem(2, 8, new QTableWidgetItem(tr("08.10.2010 8:32")));
    tableWidget->setItem(2, 9, new QTableWidgetItem(tr("772A0000-566DD000")));
    tableWidget->setItem(2, 10, new QTableWidgetItem(tr("[3376] customstyles.exe: Native")));

    tableWidget->setRowHeight(3, height);
    tableWidget->setVerticalHeaderItem(3, new QTableWidgetItem(QIcon(":/Resources/modules.png"), tr("")));
    tableWidget->setItem(3, 0, new QTableWidgetItem(tr("QtCoreed4.dll")));
    tableWidget->setItem(3, 1, new QTableWidgetItem(tr("C:/Qt/4.6.2/Bin/QtCored4.dll")));
    tableWidget->setItem(3, 2, new QTableWidgetItem(tr("N/A")));
    tableWidget->setItem(3, 3, new QTableWidgetItem(tr("N/A")));
    tableWidget->setItem(3, 4, new QTableWidgetItem(tr("Symbols loaded.")));
    tableWidget->setItem(3, 5, new QTableWidgetItem(tr("C:/Qt/4.6.2/lib/QtCored4.pdb")));
    tableWidget->setItem(3, 6, new QTableWidgetItem(tr("4")));
    tableWidget->setItem(3, 7, new QTableWidgetItem(tr("6.1.7600.16385 (win7_rtm.090713-1255)")));
    tableWidget->setItem(3, 8, new QTableWidgetItem(tr("08.10.2010 8:32")));
    tableWidget->setItem(3, 9, new QTableWidgetItem(tr("772A0000-566DD000")));
    tableWidget->setItem(3, 10, new QTableWidgetItem(tr("[3376] customstyles.exe: Native")));

    tableWidget->setRowHeight(4, height);
    tableWidget->setVerticalHeaderItem(4, new QTableWidgetItem(QIcon(":/Resources/modules.png"), tr("")));
    tableWidget->setItem(4, 0, new QTableWidgetItem(tr("user32.dll")));
    tableWidget->setItem(4, 1, new QTableWidgetItem(tr("C:/Windows/System32/user32.dll")));
    tableWidget->setItem(4, 2, new QTableWidgetItem(tr("N/A")));
    tableWidget->setItem(4, 3, new QTableWidgetItem(tr("N/A")));
    tableWidget->setItem(4, 4, new QTableWidgetItem(tr("No symbols loaded.")));
    tableWidget->setItem(4, 5, new QTableWidgetItem(tr("")));
    tableWidget->setItem(4, 6, new QTableWidgetItem(tr("3")));
    tableWidget->setItem(4, 7, new QTableWidgetItem(tr("6.1.7600.16385 (win7_rtm.090713-1255)")));
    tableWidget->setItem(4, 8, new QTableWidgetItem(tr("08.10.2010 8:32")));
    tableWidget->setItem(4, 9, new QTableWidgetItem(tr("552A0000-544AD000")));
    tableWidget->setItem(4, 10, new QTableWidgetItem(tr("[3376] customstyles.exe: Native")));

    tableWidget->setRowHeight(5, height);
    tableWidget->setVerticalHeaderItem(5, new QTableWidgetItem(QIcon(":/Resources/modules.png"), tr("")));
    tableWidget->setItem(5, 0, new QTableWidgetItem(tr("usp10.dll")));
    tableWidget->setItem(5, 1, new QTableWidgetItem(tr("C:/Windows/System32/usp10.dll")));
    tableWidget->setItem(5, 2, new QTableWidgetItem(tr("N/A")));
    tableWidget->setItem(5, 3, new QTableWidgetItem(tr("N/A")));
    tableWidget->setItem(5, 4, new QTableWidgetItem(tr("No symbols loaded.")));
    tableWidget->setItem(5, 5, new QTableWidgetItem(tr("")));
    tableWidget->setItem(5, 6, new QTableWidgetItem(tr("3")));
    tableWidget->setItem(5, 7, new QTableWidgetItem(tr("6.1.7600.16385 (win7_rtm.090713-1255)")));
    tableWidget->setItem(5, 8, new QTableWidgetItem(tr("08.10.2010 8:32")));
    tableWidget->setItem(5, 9, new QTableWidgetItem(tr("552A0000-544AD000")));
    tableWidget->setItem(5, 10, new QTableWidgetItem(tr("[3376] customstyles.exe: Native")));

    QVBoxLayout* layout = new QVBoxLayout;
    layout->setMargin(1);
    layout->setSpacing(0);

    layout->addWidget(tableWidget,1);
    w->setLayout(layout);
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
        QWidget * widgetProjectTree = new QWidget(dock_modelBuilderTree);
        fillSolutionExplorer(widgetProjectTree);
        dock_modelBuilderTree->setWidget(widgetProjectTree);
        addDockWidget(Qt::LeftDockWidgetArea,dock_modelBuilderTree);
    }

    if(!dock_messageOutputPane){
        dock_messageOutputPane = new QDockWidget(this);

        dock_messageOutputPane->setWindowTitle(tr("Log Infor"));
        dock_messageOutputPane->setObjectName("log_dockwidget");
        QWidget * widgetLog = new QWidget(dock_messageOutputPane);
        QHBoxLayout * hbox = new QHBoxLayout(dock_messageOutputPane);
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

    }
}

/*!
 \brief 初始化状态栏

*/
void MainWindow::setupStatusBar()
{
    //创建状态栏
    QStatusBar* status_bar = statusBar();
    QLabel* label1 = new QLabel("Make by Poofee");
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
