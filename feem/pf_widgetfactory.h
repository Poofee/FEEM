#ifndef PF_WIDGETFACTORY_H
#define PF_WIDGETFACTORY_H

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

class PF_WidgetFactory : public QObject
{
    Q_OBJECT
public:
    PF_WidgetFactory(MainWindow* mainwin,
                              QMap<QString, QAction*>& action_map,
                              PF_ActionGroupManager* agm);
    void createStandardToolbars(PF_ActionHandler* a_handler);
    void createMenus(QMenuBar* menu_bar);

signals:

public slots:

private:
    MainWindow* mainwindow;
    QMap<QString, QAction*>& a_map;
    PF_ActionGroupManager* ag_manager;
};

#endif // PF_WIDGETFACTORY_H
