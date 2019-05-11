#include "coreapp.h"
#include "QtnRibbonStyle.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setStyle( new Qtitan::RibbonStyle() );
    QCoreApplication::setOrganizationName("HIT");
    QCoreApplication::setApplicationName("FEEM");
    QCoreApplication::setApplicationVersion("0.0.1");

    a.processEvents();

    QTranslator translator;
    if (translator.load("feem_zh.qm",":/translations")) {
        qApp->installTranslator(&translator);
    }

    coreApp* core = new coreApp();
    core->initialize();

    return a.exec();
}
