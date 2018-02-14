#include <QMouseEvent>

#include "pf_eventhandler.h"
#include "pf_actioninterface.h"

PF_EventHandler::PF_EventHandler(QObject *parent) : QObject(parent)
{

}

PF_EventHandler::~PF_EventHandler()
{
    delete defaultAction;
    defaultAction = nullptr;

    for(int i = 0; i < currentAction.size(); ++i){
        delete currentAction.at(i);
    }
    currentAction.clear();
}

void PF_EventHandler::mousePressEvent(QMouseEvent *e)
{
    //当前action未执行完
    if(hasAction()){
        currentAction.last()->mousePressEvent(e);
    }else{
        if(defaultAction){
            defaultAction->mousePressEvent(e);
            e->accept();
        }else{
            e->ignore();
        }
    }
}

void PF_EventHandler::mouseReleaseEvent(QMouseEvent *e)
{
    if(hasAction()){
        currentAction.last()->mouseReleaseEvent(e);


    }else{
        if(defaultAction){
            defaultAction->mouseReleaseEvent(e);
        }else{
            e->ignore();
        }
    }
}

void PF_EventHandler::mouseMoveEvent(QMouseEvent *e)
{
    if(hasAction())
        currentAction.last()->mouseMoveEvent(e);
    else if(defaultAction)
        defaultAction->mouseMoveEvent(e);
}

void PF_EventHandler::mouseLeaveEvent(QMouseEvent *e)
{
    if(hasAction()){
        currentAction.last()->suspend();
    }else{
        if(defaultAction){
            defaultAction->suspend();
        }
    }
}

void PF_EventHandler::mouseEnterEvent(QMouseEvent *e)
{
    if(hasAction())
        currentAction.last()->resume();
    else if(defaultAction)
        defaultAction->resume();
}

void PF_EventHandler::keyPressEvent(QKeyEvent *e)
{
    if(hasAction())
        currentAction.last()->keyPressEvent(e);
    else{
        if(defaultAction)
            defaultAction->keyPressEvent(e);
        else
            e->ignore();
    }
}

void PF_EventHandler::keyReleaseEvent(QKeyEvent *e)
{
    if(hasAction()){
        currentAction.last()->keyReleaseEvent(e);
    }else{
        if(defaultAction){
            defaultAction->keyReleaseEvent(e);
        }else{
            e->ignore();
        }
    }
}

void PF_EventHandler::setCurrentAction(PF_ActionInterface *action)
{
    if(action == NULL){
        return;
    }

}

PF_ActionInterface *PF_EventHandler::getCurrentAction()
{
    if(hasAction()){
        return currentAction.last();
    }else{
        return defaultAction;
    }
}

void PF_EventHandler::setDefaultAction(PF_ActionInterface *action)
{
    if(defaultAction){
        defaultAction->finish();
        delete defaultAction;
    }
    defaultAction = action;
}

PF_ActionInterface *PF_EventHandler::getDefaultAction()
{
    return defaultAction;
}

bool PF_EventHandler::hasAction()
{
    for(int i = 0; i < currentAction.size();++i){
        if(!currentAction.at(i)->isFinished()){
            return true;
        }
    }
    return false;
}
