#include "inavigationwidgetfactory.h"

/*!
    \class Core::INavigationWidgetFactory
    \mainclass
    \inmodule Qt Creator
    \brief The INavigationWidgetFactory class provides new instances of navigation widgets.

    A navigation widget factory is necessary because there can be more than one navigation widget of
    the same type at a time. Each navigation widget is wrapped in a \l{Core::NavigationView} for
    delivery.
*/

/*!
    \class Core::NavigationView
    \inmodule Qt Creator
    \brief The NavigationView class is a C struct for wrapping a widget and a list of tool buttons.
    Wrapping the widget that is shown in the content area of the navigation widget and a list of
    tool buttons that is shown in the header above it.
*/

/*!
    \fn INavigationWidgetFactory::INavigationWidgetFactory()

    Constructs a navigation widget factory.
*/

/*!
    \fn QString INavigationWidgetFactory::displayName() const

    Returns the display name of the navigation widget, which is shown in the dropdown menu above the
    navigation widget.
*/

/*!
    \fn int INavigationWidgetFactory::priority() const

    Determines the position of the navigation widget in the dropdown menu.

    0 to 1000 from top to bottom
*/

/*!
    \fn Id INavigationWidgetFactory::id() const

    Returns a unique identifier for referencing the navigation widget factory.
*/

/*!
    \fn NavigationView INavigationWidgetFactory::createWidget()

    Returns a \l{Core::NavigationView} containing the widget and the buttons. The ownership is given
    to the caller.
*/


static QList<INavigationWidgetFactory *> g_navigationWidgetFactories;

/*!
    Creates a \l{Core::NavigationViewFactory}.
*/
INavigationWidgetFactory::INavigationWidgetFactory()
{
    g_navigationWidgetFactories.append(this);
}

INavigationWidgetFactory::~INavigationWidgetFactory()
{
    g_navigationWidgetFactories.removeOne(this);
}

const QList<INavigationWidgetFactory *> INavigationWidgetFactory::allNavigationFactories()
{
    return g_navigationWidgetFactories;
}

/*!
    Sets the display name for the factory.

    \sa displayName()
*/
void INavigationWidgetFactory::setDisplayName(const QString &displayName)
{
    m_displayName = displayName;
}

