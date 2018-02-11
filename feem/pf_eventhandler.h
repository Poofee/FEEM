#ifndef PF_EVENTHANDLER_H
#define PF_EVENTHANDLER_H

#include <QObject>

class QAction;
class QMouseEvent;
class QKeyEvent;


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

signals:

public slots:
};

#endif // PF_EVENTHANDLER_H