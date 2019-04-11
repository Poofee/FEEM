#include "messagemanager.h"

#include "messageoutputwindow.h"

static MessageManager* m_instance = nullptr;
static MessageOutputWindow* m_messageOutputWindow = nullptr;

MessageManager *MessageManager::instance()
{
    return m_instance;
}

void MessageManager::showOutputPane(PrintToOutputPaneFlags flags)
{

}

void MessageManager::write(const QString &text, PrintToOutputPaneFlags flags)
{
    if (!m_messageOutputWindow)
        return;
    showOutputPane(flags);
    m_messageOutputWindow->append(text + QLatin1Char('\n'));
}

MessageManager::MessageManager()
{
    m_instance = this;
    m_messageOutputWindow = nullptr;
}

MessageManager::~MessageManager()
{
    if (m_messageOutputWindow) {
//        ExtensionSystem::PluginManager::removeObject(m_messageOutputWindow);
        delete m_messageOutputWindow;
    }
    m_instance = nullptr;
}

void MessageManager::init()
{
    m_messageOutputWindow = new MessageOutputWindow;
//    ExtensionSystem::PluginManager::addObject(m_messageOutputWindow);
}
