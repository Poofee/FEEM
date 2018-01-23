#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>

class PF_ActionHandler;
class PF_WidgetFactory;
class PF_ActionGroupManager;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QMap<QString, QAction*> a_map;//动作列表
    PF_ActionGroupManager* ag_manager;

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
};

#endif // MAINWINDOW_H
