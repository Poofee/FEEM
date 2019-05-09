#ifndef COREAPP_H
#define COREAPP_H

#include <QObject>

class MainWindow;

class coreApp : public QObject
{
    Q_OBJECT
public:
    coreApp();
    ~coreApp();

    static coreApp* instance();    

    bool initialize();
private:
    MainWindow* m_mainWindow = nullptr;
};

#endif // COREAPP_H
