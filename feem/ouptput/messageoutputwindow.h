#ifndef MESSAGEOUTPUTWINDOW_H
#define MESSAGEOUTPUTWINDOW_H

#include "ioutputpane.h"

class OutputWindow;

class MessageOutputWindow : public IOutputPane
{
    Q_OBJECT
public:
    MessageOutputWindow();
    ~MessageOutputWindow() override;


};

#endif // MESSAGEOUTPUTWINDOW_H
