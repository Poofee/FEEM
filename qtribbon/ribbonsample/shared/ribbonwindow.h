#ifndef RIBBONWINDOW_H
#define RIBBONWINDOW_H

#include <QtitanRibbon.h>


/* RibbonWindow */
class RibbonWindow : public Qtitan::RibbonMainWindow
{
  Q_OBJECT
public:
    RibbonWindow(QWidget* parent = Q_NULL);
    virtual ~RibbonWindow();

private:
    void createOptions();

protected Q_SLOTS:
    void about();
    virtual void options(QAction*);
    void optionsDialog();
    void includingAnimation(bool);
    void maximizeToggle();
    void minimizationChanged(bool minimized);
    void optionsFont(QAction* act);
    void setDPIToggled(bool);
    void showQuickAccessCustomizeMenu(QMenu* menu);
    void switchQuickAccessBarPosition();

#ifdef Q_OS_WIN
    void frameTheme(bool);
#endif // Q_OS_WIN

protected:
    RibbonStyle::Theme theme() const;
    void setOptions(RibbonStyle::Theme themeId);
    void setButtonStyleTheme(RibbonStyle::Theme themeId);
    void updateActionsTheme();

protected:
    virtual bool event(QEvent* event);

protected:
    QMenu* m_menuOptions;
    QActionGroup* m_styleActions;
    QAction* m_actionRibbonMinimize;
    QAction* m_actionRibbonMinimizeMenu;
    QAction* m_actionDefault;
    QAction* m_actionNormal;
    QAction* m_actionLarge;
    QAction* m_actionExLarge;
    QAction* m_actionShowBelowRibbon;
    QAction* m_actionShowAboveRibbon;

    int m_defaultFont;

    Qtitan::RibbonStyle* m_ribbonStyle;
};

#endif // RIBBONWINDOW_H
