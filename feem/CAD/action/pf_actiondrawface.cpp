#include "pf_actiondrawface.h"

#include <QAction>
#include <QMouseEvent>
#include <QDebug>
#include "pf_graphicview.h"
#include "pf_point.h"
#include "pf_face.h"
#include "pf_preview.h"

#include "pf_cadwidget.h"


PF_ActionDrawFace::PF_ActionDrawFace(PF_EntityContainer *container, PF_GraphicView *graphicView)
    :PF_ActionPreviewInterface("Draw Face",container,graphicView)
    ,data(new PF_FaceData())
{
    actionType = PF::ActionDrawFace;
    reset();
    /** 设置捕捉模式 **/
    this->snapMode.snapGrid = false;
    this->snapMode.snapFree = false;
    this->snapMode.snapEndpoint = true;
}

PF_ActionDrawFace::~PF_ActionDrawFace()
{
    if(data){
        delete data;
        data = nullptr;
    }
}

void PF_ActionDrawFace::reset()
{
    if(data){
        delete data;
    }
    data = new PF_FaceData();
}

void PF_ActionDrawFace::init(int status)
{
    PF_ActionPreviewInterface::init(status);
    reset();
}

/*!
 \brief 好像是所有的操作结束之后到这里

*/
void PF_ActionDrawFace::trigger()
{
    PF_ActionPreviewInterface::trigger();
    /** 添加对象 **/
    PF_Face * face = new PF_Face(container,view, *data);
    container->addEntity(face);
    setStatus(SetFirstLoop);
    reset();

    PF_Face::face_index++;

    //view->redraw(PF::RedrawDrawing);
    view->replot();
//    qDebug()<<line->toGeoString();
}

/*!
 \brief 主要是实现绘图过程当中的预览效果

 \param e
*/
void PF_ActionDrawFace::mouseMoveEvent(QMouseEvent *e)
{
    PF_Vector mouse = snapPoint(e);
    if(!mouse.valid)
        return;

    //    PF_CADWidget::statusbar->clearMessage();
    switch(getStatus()){
    case SetFirstLoop:
        PF_CADWidget::statusbar->showMessage(mouse.toString()+QString(tr("Set Surface loop.(Press right click to abort)")));
        break;
    case SetOtherLoop:
        PF_CADWidget::statusbar->showMessage(mouse.toString()+QString(tr("Continue to set sureface loop.(Press right click to abort)")));
        break;
    default:
        PF_CADWidget::statusbar->showMessage(mouse.toString()+QString(tr("Status error.")));
        break;
    }
    /**只有起始点设置好之后才有预览**/
//    if(getStatus() == SetEndpoint && data->startpoint.valid){
//        deletePreview();
//        view->setCurrentLayer(QLatin1String("overlay"));
//        PF_Line* line = new PF_Line(container,view,data->startpoint,mouse);
//        view->setCurrentLayer(QLatin1String("main"));
//        preview->addEntity(line);
//        drawPreview();
//    }
}

/*!
 \brief 处理捕捉点后的动作

 \param e
*/
void PF_ActionDrawFace::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton){
        PF_Vector mouse = snapPoint(e);
        if(!mouse.valid)
            return;
        PF_Entity* entity = catchEntity(mouse,PF::EntityPoint);
        if(!entity)
            return;
        switch(getStatus()){
        /** 应当是设置loop中的点，直到闭合 **/
        case SetFirstLoop:
            qDebug()<<Q_FUNC_INFO<<"SetFirstLoop";
            data->faceData.append(new PF_LineLoop());
            data->faceData.last()->points.append(dynamic_cast<PF_Point*>(entity));
            setStatus(SetOtherLoop);
            updateMouseButtonHints();
            break;
        case SetOtherLoop:/** 不断地设置下一个点，直到闭合 **/
            qDebug()<<Q_FUNC_INFO<<"SetOtherLoop";
            data->faceData.last()->points.append(dynamic_cast<PF_Point*>(entity));

            /** 判断最后一个点与第一个点是否重合 **/
            if(data->faceData.last()->points.last()->index() == data->faceData.last()->points.first()->index()){
//                qDebug()<<Q_FUNC_INFO<<"over";
//                trigger();
                setStatus(SetFirstLoop);
            }
            break;
        default:
            break;
        }
    }else if(e->button() == Qt::RightButton){
//        deletePreview();
//        drawPreview();
        trigger();
        init(getStatus()-1);

        PF_CADWidget::statusbar->clearMessage();
    }
}

void PF_ActionDrawFace::hideOptions()
{

}

void PF_ActionDrawFace::showOptions()
{

}

void PF_ActionDrawFace::updateMouseButtonHints()
{

}

void PF_ActionDrawFace::updateMouseCursor()
{

}
