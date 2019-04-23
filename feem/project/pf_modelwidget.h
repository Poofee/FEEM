#ifndef PF_MODELWIDGET_H
#define PF_MODELWIDGET_H

#include <QWidget>

class QToolButton;
class PF_ProjectTreeWidget;

class PF_ModelWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PF_ModelWidget(QWidget *parent = nullptr);
    ~PF_ModelWidget();

    void init();
private:
    QWidget* m_toolbar;
    QToolButton* tb_previousNode;
    QToolButton* tb_nextNode;
    QToolButton* tb_moveup;
    QToolButton* tb_movedown;
    QToolButton* tb_show;
    QToolButton* tb_collapseAll;
    QToolButton* tb_expandAll;
    PF_ProjectTreeWidget* m_modelTreeWidget;
signals:

public slots:
};

#endif // PF_MODELWIDGET_H
