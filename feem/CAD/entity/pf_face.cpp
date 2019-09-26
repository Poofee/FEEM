#include "pf_line.h"
#include "pf_face.h"
#include "pf_graphicview.h"
#include <QPainter>

int PF_Face::face_index = 1;
int PF_LineLoop::lineloop_index = 1;

PF_Face::PF_Face(PF_EntityContainer *parent, PF_GraphicView *view, const PF_FaceData &d)
    :PF_AtomicEntity(parent,view)
    ,data(d)
{
    m_index = face_index;
}

PF_Face::PF_Face(PF_EntityContainer *parent, PF_GraphicView *view, const PF_FaceData &d, PF_Line *mouse)
    :PF_AtomicEntity(parent,view)
    ,data(d)
{
    if(!data.faceData.isEmpty())
        data.faceData.last()->lines.append(mouse);/**有问题**/
    m_index = face_index;
}

PF_VectorSolutions PF_Face::getRefPoints() const
{
    return {};
}

PF_Vector PF_Face::getMiddlePoint() const
{
    return {};
}

PF_Vector PF_Face::getNearestEndpoint(const PF_Vector &coord, double *dist) const
{
    return {};
}

PF_Vector PF_Face::getNearestPointOnEntity(const PF_Vector &coord, bool onEntity, double *dist, PF_Entity **entity) const
{
    return {};
}

PF_Vector PF_Face::getNearestCenter(const PF_Vector &coord, double *dist) const
{
    return {};
}

PF_Vector PF_Face::getNearestMiddle(const PF_Vector &coord, double *dist, int middlePoints) const
{
    return {};
}

PF_Vector PF_Face::getNearestDist(double distance, const PF_Vector &coord, double *dist) const
{
    return {};
}

PF_Vector PF_Face::getNearestDist(double distance, bool startp) const
{
    return {};
}

PF_Vector PF_Face::getNearestOrthTan(const PF_Vector &coord, const PF_Line &normal, bool onEntity) const
{
    return {};
}

bool PF_Face::offset(const PF_Vector &coord, const double &distance)
{
    return true;
}

void PF_Face::move(const PF_Vector &offset)
{

}

void PF_Face::rotate(const PF_Vector &center, const double &angle)
{

}

void PF_Face::rotate(const PF_Vector &center, const PF_Vector &angleVector)
{

}

void PF_Face::scale(const PF_Vector &center, const PF_Vector &factor)
{

}

void PF_Face::mirror(const PF_Vector &axisPoint1, const PF_Vector &axisPoint2)
{

}

void PF_Face::moveRef(const PF_Vector &ref, const PF_Vector &offset)
{

}

void PF_Face::draw(QCPPainter *painter)
{
    //    qDebug()<<Q_FUNC_INFO;
    if(!(painter && mParentPlot)){
        qDebug()<<Q_FUNC_INFO<<":NULL";
        return;
    }
    /** set Pen **/
    //    QPen oldpen = painter->pen();
    //    QBrush oldbursh = painter->brush();
    painter->save();
    if(isSelected()){
        pen.setColor(QColor(0,0,255));
        painter->setPen(pen);
    }
    /** 绘制面 **/
    QPainterPath path;
    for(auto e : data.faceData){
        /** 生成lineloop，实际的gui坐标 **/
        e->loop.clear();
        for(auto p : e->lines){
            /** 要注意线的方向 **/
            PF_Vector pos = p->data.startpoint->getCenter();
//            qDebug()<<p->index()<<","<<pos.toString();
//            qDebug()<<p->data.startpoint->index();
            e->loop.append(QPointF(mParentPlot->toGuiX(pos.x),mParentPlot->toGuiY(pos.y)));
            pos = p->data.endpoint->getCenter();
            e->loop.append(QPointF(mParentPlot->toGuiX(pos.x),mParentPlot->toGuiY(pos.y)));
//            qDebug()<<p->index()<<","<<pos.toString();
//            qDebug()<<p->data.endpoint->index();
        }
        path.addPolygon(e->loop);
    }
//    qDebug()<<path;

    path.setFillRule(Qt::OddEvenFill);
    painter->setBrush(QColor(180,180,242,180));
    painter->setPen(Qt::NoPen);
    painter->drawPath(path);
    //QPointF start(mParentPlot->toGuiX(data.startpoint.x),mParentPlot->toGuiY(data.startpoint.y));

    //painter->drawLine(start,end);

    //painter->drawText(start,data.startpoint.toString());
    //painter->drawText(end,data.endpoint.toString());
    //painter->drawText((start+end)/2,QString("line:%1").arg(m_index));

    /** 绘制控制点 **/
//    if (isSelected() || isHighlighted()) {
//        //		if (!e->isParentSelected()) {
//        PF_VectorSolutions const& s = this->getRefPoints();
//        double x,y;
//        int size = 4;
//        for (size_t i=0; i<s.getNumber(); ++i) {
//            x = mParentPlot->toGuiX(s.get(i).x);
//            y = mParentPlot->toGuiY(s.get(i).y);

//            painter->drawLine(QPointF(x-size,y-size),QPointF(x+size,y-size));
//            painter->drawLine(QPointF(x+size,y-size),QPointF(x+size,y+size));
//            painter->drawLine(QPointF(x+size,y+size),QPointF(x-size,y+size));
//            painter->drawLine(QPointF(x-size,y+size),QPointF(x-size,y-size));
//        }
//        //		}
//    }
    //    painter->setPen(oldpen);
    //    painter->setBrush(oldbursh);
    painter->restore();
}

void PF_Face::calculateBorders()
{

}

/*!
 \brief 应该添加线的正反判断

Curve Loop(1) = {2, 3, -11, -6, -5, -7, -4, 9};
Plane Surface(1) = {1};

 \return QString
*/
QString PF_Face::toGeoString()
{
    QString loopstr;
    QString surfstr = QString("Plane Surface(%1) = {").arg(this->index());

    /** 迭代所有的lineloop **/
    for(auto l : data.faceData){
        loopstr += QString("Curve Loop(%1) = {").arg(l->index());
        for(auto e : l->lines){
            loopstr += QString("%1,").arg(e->index());
        }
        loopstr += "};\n";
        surfstr += QString("%1,").arg(l->index());
//        qDebug()<<loopstr;
    }
    surfstr += "};\n";
    loopstr.append(surfstr);
    loopstr.replace(",};"," };");
//    qDebug()<<loopstr;
    return loopstr;
}

int PF_Face::index() const
{
    return m_index;
}

/*!
 \brief 拷贝构造函数

 \param data
*/
PF_FaceData::PF_FaceData(const PF_FaceData &data)
{
    faceData.clear();
    for(auto d : data.faceData){
        PF_LineLoop* l = new PF_LineLoop();
        l->lines = d->lines;
        l->loop = d->loop;
        l->m_index = d->m_index;
        faceData.push_back(l);
    }
}

/*!
 \brief 赋值构造函数

 \param data
 \return PF_FaceData &PF_FaceData::operator
*/
PF_FaceData &PF_FaceData::operator=(const PF_FaceData &data)
{
    faceData.clear();
    for(auto d : data.faceData){
        PF_LineLoop* l = new PF_LineLoop();
        l->lines = d->lines;
        l->loop = d->loop;
        faceData.push_back(l);
    }
    return *this;
}

PF_LineLoop::PF_LineLoop()
{
    m_index = lineloop_index;
//    qDebug()<<"lineloop_index"<<m_index;
}

int PF_LineLoop::index() const
{
    return m_index;
}
