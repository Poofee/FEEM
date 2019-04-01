#ifndef PF_PROJECTTREEWIDGET_H
#define PF_PROJECTTREEWIDGET_H

#include <QWidget>

class QTreeView;
class Node;
class PF_ProjectModel;

class PF_ProjectTreeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PF_ProjectTreeWidget(QWidget *parent = nullptr);
    ~PF_ProjectTreeWidget() override;

private:
    QTreeView *m_view = nullptr;
    PF_ProjectModel *m_model = nullptr;

    QString m_modelId;

signals:

public slots:
};

#endif // PF_PROJECTTREEWIDGET_H
