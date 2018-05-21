#include <QMouseEvent>
#include <QDebug>
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

        //有的action可能会右键结束，清理一下
        cleanUp();
        e->accept();
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
    qDebug()<<"PF_EventHandler::setCurrentAction";
    if(action == NULL){
        return;
    }

    PF_ActionInterface* predecessor = NULL;
    //处理一下当前运行的action
    if(hasAction()){
        predecessor = currentAction.last();
        predecessor->suspend();
        predecessor->hideOptions();
    }else{
        if(defaultAction){
            predecessor = defaultAction;
            predecessor->suspend();
            predecessor->hideOptions();
        }
    }

    currentAction.push_back(action);

    //初始化新的action
    action->init();
    if(action->isFinished()==false){
        currentAction.last()->showOptions();
        action->setPredecessor(predecessor);
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

//清除掉已经完成的action
void PF_EventHandler::cleanUp()
{
    qDebug()<<"PF_EventHandler::cleanUp";
    for(int i=0; i < currentAction.size();++i){
        if(currentAction.at(i)->isFinished()){
            delete currentAction.at(i);
            currentAction.removeAt(i--);
        }
    }
    if(hasAction()){
        currentAction.last()->resume();
        currentAction.last()->showOptions();
    }else{
        if(defaultAction){
            defaultAction->resume();
            defaultAction->showOptions();
        }
    }
    qDebug()<<"PF_EventHandler::cleanUp: OK.";
}
