#ifndef PF_LINE_H
#define PF_LINE_H

#include "pf_atomicentity.h"

struct PF_LineData{
    PF_LineData()=default;
    PF_LineData(PF_Vector& startpoint,PF_Vector& endpoint);
    PF_Vector startpoint;
    PF_Vector endpoint;
};

class PF_Line : public PF_AtomicEntity
{
public:
    PF_Line()=default;
    PF_Line(PF_EntityContainer* parent,PF_GraphicView *view, const PF_LineData& d);
    PF_Line(PF_EntityContainer* parent,PF_GraphicView *view, const PF_Vector& pStart, const PF_Vector& pEnd);

    /** @return Start point of the entity */
    PF_Vector getStartpoint() const override{
        return data.startpoint;
    }
    /** @return End point of the entity */
    PF_Vector getEndpoint() const override{
        return data.endpoint;
    }

    void draw(QCPPainter *painter) ;
protected:
    PF_LineData data;
};

#endif // PF_LINE_H
