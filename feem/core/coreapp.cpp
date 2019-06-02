#include "coreapp.h"

#include "actionmanager/actionmanager.h"
#include "mainwindow.h"
#include "pf_sessionmanager.h"
#include "pf_project.h"
#include "pf_projecttree.h"

#include <QSplashScreen>
#include <QMessageBox>
#include <QDateTime>

static coreApp *m_instance = nullptr;

coreApp::coreApp()
{
    m_instance = this;
}

coreApp::~coreApp()
{
    delete m_mainWindow;
}

coreApp *coreApp::instance()
{
    return m_instance;
}

bool coreApp::initialize()
{
    /** 创建action管理器，保存所有的actions **/
    new ActionManager(this);
    /** 欢迎页面 **/
    QSplashScreen* splash = new QSplashScreen;

    QPixmap pixmap(":/main/splash.png");
    splash->setPixmap(pixmap);
    splash->setAttribute(Qt::WA_DeleteOnClose);
    splash->show();
    splash->showMessage(QObject::tr("Loading..."),
                        Qt::AlignRight|Qt::AlignBottom,Qt::black);
    QDateTime n=QDateTime::currentDateTime();
    QDateTime now;
    do{
        now=QDateTime::currentDateTime();
    } while (n.secsTo(now)<=0.5);//6 为需要延时的秒数

    /** 软件主界面 **/
    m_mainWindow = new MainWindow;
    m_mainWindow->setWindowTitle("FEEM");

    m_mainWindow->showMaximized();
    m_mainWindow->show();
    m_mainWindow->setFocus();

    splash->raise();
    splash->showMessage(QObject::tr("Loading..."),
                        Qt::AlignRight|Qt::AlignBottom,Qt::black);

    splash->finish(m_mainWindow);
    delete splash;

    ActionManager::instance()->setContext(Context(Constants::C_GLOBAL));

    PF_Project* pro = new PF_Project(this);
//    PF_Project* pro1 = new PF_Project(this);
    PF_SessionManager::instance()->addProject(pro);
//    PF_SessionManager::instance()->addProject(pro1);
    PF_ProjectTree::instance()->expandAll();
    return true;
}
