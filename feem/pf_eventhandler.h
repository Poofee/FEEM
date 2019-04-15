#ifndef PF_EVENTHANDLER_H
#define PF_EVENTHANDLER_H

#include <QObject>

class QAction;
class QMouseEvent;
class QKeyEvent;
class PF_ActionInterface;

struct PF_SnapMode;

class PF_EventHandler : public QObject
{
    Q_OBJECT
public:
    explicit PF_EventHandler(QObject *parent = nullptr);
    ~PF_EventHandler();

    void mousePressEvent(QMouseEvent* e);
    void mouseReleaseEvent(QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent* e);
    void mouseLeaveEvent(QMouseEvent* e);
    void mouseEnterEvent(QMouseEvent* e);

    void keyPressEvent(QKeyEvent* e);
    void keyReleaseEvent(QKeyEvent* e);

    void back();

    void setCurrentAction(PF_ActionInterface* action);
    PF_ActionInterface* getCurrentAction();
    void setDefaultAction(PF_ActionInterface* action);
    PF_ActionInterface* getDefaultAction();

    void setSnapMode(PF_SnapMode sm);

    bool hasAction();
    void cleanUp();
signals:

public slots:
private:
    PF_ActionInterface* defaultAction{nullptr};
    QList<PF_ActionInterface*> currentAction; /**< TODO: describe */
};

#endif // PF_EVENTHANDLER_H
