#include "mainwindow.h"
#include <QApplication>
#include <QSplashScreen>
#include <QMessageBox>
#include <QDateTime>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("HIT");
    QCoreApplication::setApplicationName("FEEM");
    QCoreApplication::setApplicationVersion("0.0.1");

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
    a.processEvents();

    MainWindow w;
    w.setWindowTitle(a.applicationName());

    w.resize(QSize(800,600));
    w.showMaximized();

    w.show();

    w.setFocus();

    splash->raise();
    splash->showMessage(QObject::tr("Loading..."),
                        Qt::AlignRight|Qt::AlignBottom,Qt::black);

    splash->finish(&w);

    return a.exec();
}
