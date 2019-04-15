#include "outputpanemanager.h"

#include "ioutputpane.h"

OutputPaneManager::OutputPaneManager(QWidget *parent) : QWidget(parent)
{

}

IOutputPane::IOutputPane(QObject *parent)
    : QObject(parent)
{

}

IOutputPane::~IOutputPane()
{

}
