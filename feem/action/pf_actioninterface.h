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
    PF_ActionInterface(const char* name,PF_GraphicView* view);
    virtual ~PF_ActionInterface()=default;

    virtual PF::ActionType rtti() const;
    void setName(const char* _name);
    QString getName();

    virtual void init(int status=0);
    virtual void mouseMoveEvent(QMouseEvent*);
    virtual void mousePressEvent(QMouseEvent*);
    virtual void mouseReleaseEvent(QMouseEvent*);
    virtual void keyPressEvent(QKeyEvent* e);
    virtual void keyReleaseEvent(QKeyEvent* e);

    virtual void setStatus(int status);
    virtual int  getStatus();

    virtual void updateMouseButtonHints();
    virtual void updateMouseCursor();

    virtual void trigger();
    virtual void finish();
    virtual void setPredecessor(PF_ActionInterface* pre);
    virtual void suspend();
    virtual void resume();
    virtual void showOptions();
    virtual void hideOptions();

    virtual bool isFinished();
    virtual void setFinished();

    void setActionType(PF::ActionType actionType);
signals:

public slots:
private:
    int status;
protected:
    QString name;
    bool finished;
    PF_ActionInterface* predecessor;
    PF::ActionType actionType;
};

#endif // PF_ACTIONINTERFACE_H
