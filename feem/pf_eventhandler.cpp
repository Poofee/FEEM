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
    qDebug()<<Q_FUNC_INFO;
    /**当前action未执行完**/
    if(hasAction()){
        currentAction.last()->mousePressEvent(e);
        e->accept();
    }else{
        if(defaultAction){
            defaultAction->mousePressEvent(e);
            e->accept();
        }else{
            e->ignore();
        }
    }
    qDebug()<<"PF_EventHandler::mousePressEvent: OK.";
}

void PF_EventHandler::mouseReleaseEvent(QMouseEvent *e)
{
    qDebug()<<Q_FUNC_INFO;
    if(hasAction()){
        currentAction.last()->mouseReleaseEvent(e);

        /**有的action可能会右键结束，清理一下**/
        cleanUp();
        e->accept();
    }else{
        if(defaultAction){
            defaultAction->mouseReleaseEvent(e);
        }else{
            e->ignore();
        }
    }
    qDebug()<<"PF_EventHandler::mouseReleaseEvent: OK.";
}

void PF_EventHandler::mouseMoveEvent(QMouseEvent *e)
{
    qDebug()<<Q_FUNC_INFO;
    if(hasAction())
        currentAction.last()->mouseMoveEvent(e);
    else if(defaultAction)
        defaultAction->mouseMoveEvent(e);
    qDebug()<<"PF_EventHandler::mouseMoveEvent: OK.";
}

void PF_EventHandler::mouseLeaveEvent(QMouseEvent *e)
{
    qDebug()<<Q_FUNC_INFO;
    if(hasAction()){
        currentAction.last()->suspend();
    }else{
        if(defaultAction){
            defaultAction->suspend();
        }
    }
    qDebug()<<"PF_EventHandler::mouseLeaveEvent: OK.";
}

void PF_EventHandler::mouseEnterEvent(QMouseEvent *e)
{
    qDebug()<<Q_FUNC_INFO;
    if(hasAction())
        currentAction.last()->resume();
    else if(defaultAction)
        defaultAction->resume();
    qDebug()<<"PF_EventHandler::mouseEnterEvent: OK.";
}

void PF_EventHandler::keyPressEvent(QKeyEvent *e)
{
    qDebug()<<"PF_EventHandler::keyPressEvent";
    if(hasAction())
        currentAction.last()->keyPressEvent(e);
    else{
        if(defaultAction)
            defaultAction->keyPressEvent(e);
        else
            e->ignore();
    }
    qDebug()<<"PF_EventHandler::keyPressEvent: OK.";
}

void PF_EventHandler::keyReleaseEvent(QKeyEvent *e)
{
    qDebug()<<"PF_EventHandler::keyReleaseEvent";
    if(hasAction()){
        currentAction.last()->keyReleaseEvent(e);
    }else{
        if(defaultAction){
            defaultAction->keyReleaseEvent(e);
        }else{
            e->ignore();
        }
    }
    qDebug()<<"PF_EventHandler::keyReleaseEvent: OK.";
}

void PF_EventHandler::back()
{
    QMouseEvent e(QEvent::MouseButtonRelease,QPoint(0,0),
                  Qt::RightButton,Qt::RightButton,Qt::NoModifier);
    mouseReleaseEvent(&e);
    if(!hasAction()){

    }
}

void PF_EventHandler::setCurrentAction(PF_ActionInterface *action)
{
    qDebug()<<"PF_EventHandler::setCurrentAction";
    if(action == NULL){
        return;
    }

    PF_ActionInterface* predecessor = NULL;
    /**处理一下当前运行的action**/
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

    /*初始化新的action*/
    action->init();
    if(action->isFinished()==false){
        currentAction.last()->showOptions();
        action->setPredecessor(predecessor);
    }
    qDebug()<<"PF_EventHandler::setCurrentAction: OK.";
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
            qDebug()<<"PF_EventHandler::hasAction():"<<currentAction.size();
            return true;
        }
    }
    qDebug()<<"PF_EventHandler::hasAction(): 0";
    return false;
}

/*清除掉已经完成的action*/
void PF_EventHandler::cleanUp()
{
    qDebug()<<"PF_EventHandler::cleanUp";
    for(int i=0; i < currentAction.size();++i){
        if(currentAction.at(i)->isFinished()){
            qDebug()<<"PF_EventHandler::cleanUp():   "<<i<<"   finished!";
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
