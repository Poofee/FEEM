#ifndef PF_ACTIONINTERFACE_H
#define PF_ACTIONINTERFACE_H

#include <QObject>

class QKeyEvent;
class QAction;
class QMouseEvent;

class PF_ActionInterface : public QObject
{
    Q_OBJECT
public:
    PF_ActionInterface(QObject *parent = nullptr);
    virtual ~PF_ActionInterface()=default;

    void setName(const char* _name);
    QString getName();

    virtual void init(int status=0);
    virtual void mouseMoveEvent(QMouseEvent*);
    virtual void mousePressEvent(QMouseEvent*);
    virtual void mouseReleaseEvent(QMouseEvent*);
    virtual void keyPressEvent(QKeyEvent* e);

    virtual void trigger();
    virtual void finish();
    virtual void suspend();
    virtual void resume();

signals:

public slots:
};

#endif // PF_ACTIONINTERFACE_H
