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
    painter->save();

    /** 绘制面 **/
    QPainterPath path;
    PF_Line* line1,*line2;
    PF_Vector pos;
    int indexLast = -1;
    for(auto e : data.faceData){
        /** 生成lineloop，实际的gui坐标 **/
        e->loop.clear();
        /** 保存上一次的终点 **/
        if(e->lines.size() < 2)
            break;
        /** 找到前两条线段的公共点，然后算出起始点 **/
        line1 = e->lines.at(0);
        line2 = e->lines.at(1);
        if(line1->data.startpoint->index() == line2->data.startpoint->index() ||
                line1->data.startpoint->index() == line2->data.endpoint->index()){
            indexLast = line1->data.endpoint->index();
        }else if(line1->data.endpoint->index() == line2->data.startpoint->index() ||
                line1->data.endpoint->index() == line2->data.endpoint->index()){
            indexLast = line1->data.startpoint->index();
        }else{
            break;/** 第一条和第二条并没有相连 **/
        }

        for(auto p : e->lines){
            /** 要注意线的方向 **/
            if(indexLast == p->data.startpoint->index()){
                pos = p->data.startpoint->getCenter();
                e->loop.append(QPointF(mParentPlot->toGuiX(pos.x),mParentPlot->toGuiY(pos.y)));
                pos = p->data.endpoint->getCenter();
                e->loop.append(QPointF(mParentPlot->toGuiX(pos.x),mParentPlot->toGuiY(pos.y)));
                indexLast = p->data.endpoint->index();
                continue;
            }else if(indexLast == p->data.endpoint->index()){
                pos = p->data.endpoint->getCenter();
                e->loop.append(QPointF(mParentPlot->toGuiX(pos.x),mParentPlot->toGuiY(pos.y)));
                pos = p->data.startpoint->getCenter();
                e->loop.append(QPointF(mParentPlot->toGuiX(pos.x),mParentPlot->toGuiY(pos.y)));
                indexLast = p->data.startpoint->index();
                continue;
            }else{
                break;/** 第一条和第二条并没有相连 **/
            }
        }
        path.addPolygon(e->loop);
    }
//    qDebug()<<path;
    QColor q;
    switch (this->index() % 7) {
    case 0:
        q.setNamedColor("#EA0000");
        break;
    case 1:
        q.setNamedColor("#0080FF");
        break;
    case 2:
        q.setNamedColor("#00CACA");
        break;
    case 3:
        q.setNamedColor("#FFBFFF");
        break;
    case 4:
        q.setNamedColor("#A6A600");
        break;
    case 5:
        q.setNamedColor("#FFA042");
        break;
    case 6:
        q.setNamedColor("#A6A6D2");
        break;
    default:
        q.setNamedColor("#B4B4F2");
        break;
    }
    q.setAlpha(180);
    path.setFillRule(Qt::OddEvenFill);
    if(isSelected()){
        pen.setColor(QColor(0,0,255));
        painter->setPen(pen);
        painter->setBrush(QColor(255,255,0,180));
    }else{
        painter->setBrush(q);
        painter->setPen(Qt::NoPen);
    }
    painter->drawPath(path);

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

    int indexLast;
    PF_Line* line1,*line2;
    /** 迭代所有的lineloop **/
    for(auto l : data.faceData){
        loopstr += QString("Curve Loop(%1) = {").arg(l->index());
        line1 = l->lines.at(0);
        line2 = l->lines.at(1);
        if(line1->data.startpoint->index() == line2->data.startpoint->index() ||
                line1->data.startpoint->index() == line2->data.endpoint->index()){
            indexLast = line1->data.endpoint->index();
        }else if(line1->data.endpoint->index() == line2->data.startpoint->index() ||
                line1->data.endpoint->index() == line2->data.endpoint->index()){
            indexLast = line1->data.startpoint->index();
        }else{
            break;/** 第一条和第二条并没有相连 **/
        }
        for(auto e : l->lines){
            /** 要注意线的方向 **/
            if(indexLast == e->data.startpoint->index()){
                indexLast = e->data.endpoint->index();
                loopstr += QString("%1,").arg(e->index());
                continue;
            }else if(indexLast == e->data.endpoint->index()){
                indexLast = e->data.startpoint->index();
                loopstr += QString("%1,").arg(e->index()*(-1));
                continue;
            }else{
                break;/** 第一条和第二条并没有相连 **/
            }
        }
        loopstr += "};\n";
        surfstr += QString("%1,").arg(l->index());
    }
    surfstr += "};\n";
    loopstr.append(surfstr);
    loopstr.replace(",};"," };");

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
    PF_LineLoop* l;
    for(auto d : data.faceData){
        l = new PF_LineLoop();
        l->lines = d->lines;
        l->loop = d->loop;
        faceData.push_back(l);
    }
    return *this;
}

PF_LineLoop::PF_LineLoop()
{
    m_index = lineloop_index;
}

int PF_LineLoop::index() const
{
    return m_index;
}
