#ifndef OUTPUTWINDOW_H
#define OUTPUTWINDOW_H

#include <QPlainTextEdit>

class OutputWindow : public QPlainTextEdit
{
public:
    OutputWindow(QWidget* parent = nullptr);
    ~OutputWindow() override;
};

#endif // OUTPUTWINDOW_H
