#ifndef MESSAGEMANAGER_H
#define MESSAGEMANAGER_H

#include "ioutputpane.h"

#include <QObject>

class MainWindow;

class MessageManager : public QObject
{
    Q_OBJECT

public:
    static MessageManager* instance();

    enum PrintToOutputPaneFlag {
        NoModeSwitch   = IOutputPane::NoModeSwitch,
        ModeSwitch     = IOutputPane::ModeSwitch,
        WithFocus      = IOutputPane::WithFocus,
        EnsureSizeHint = IOutputPane::EnsureSizeHint,
        Silent         = 256,
        Flash          = 512
    };

    Q_DECLARE_FLAGS(PrintToOutputPaneFlags, PrintToOutputPaneFlag)

    static void showOutputPane(PrintToOutputPaneFlags flags = NoModeSwitch);

public slots:
    static void write(const QString& text, PrintToOutputPaneFlags flags = NoModeSwitch);
private:
    MessageManager();
    ~MessageManager() override;
    static void init();
    friend class MainWindow;
};

#endif // MESSAGEMANAGER_H
