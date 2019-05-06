#ifndef INAVIGATIONWIDGETFACTORY_H
#define INAVIGATIONWIDGETFACTORY_H

#include <QObject>
#include <QList>
#include <QKeySequence>

class QSettings;
class QToolButton;
class QWidget;

struct NavigationView
{
    NavigationView(QWidget *w = nullptr) : widget(w) {}

    QWidget *widget;
    QList<QToolButton *> dockToolBarWidgets;
};

class INavigationWidgetFactory : public QObject
{
    Q_OBJECT

public:
    INavigationWidgetFactory();
    ~INavigationWidgetFactory() override;

    static const QList<INavigationWidgetFactory *> allNavigationFactories();

    void setDisplayName(const QString &displayName);
    void setPriority(int priority);

    QString displayName() const { return m_displayName ; }

    virtual NavigationView createWidget() = 0;

private:
    QString m_displayName;
};

#endif // INAVIGATIONWIDGETFACTORY_H
