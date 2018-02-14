#ifndef PF_ACTIONINTERFACE_H
#define PF_ACTIONINTERFACE_H

#include <QObject>
#include "pf_snapper.h"

class QKeyEvent;
class QAction;

class PF_ActionInterface : public QObject,public PF_Snapper
{
    Q_OBJECT
public:
    PF_ActionInterface(PF_GraphicView* view);
    virtual ~PF_ActionInterface()=default;

    void setName(const char* _name);
    QString getName();

    virtual void init(int status=0);
    virtual void mouseMoveEvent(QMouseEvent*);
    virtual void mousePressEvent(QMouseEvent*);
    virtual void mouseReleaseEvent(QMouseEvent*);
    virtual void keyPressEvent(QKeyEvent* e);
    virtual void keyReleaseEvent(QKeyEvent* e);

    virtual void trigger();
    virtual void finish();
    virtual void suspend();
    virtual void resume();

    virtual bool isFinished();
    virtual void setFinished();
signals:

public slots:
private:
    int status;
protected:
    bool finished;
};

#endif // PF_ACTIONINTERFACE_H
