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
    PF_Line(PF_EntityContainer* parent, const PF_LineData& d);
    PF_Line(const PF_Vector& pStart, const PF_Vector& pEnd);
    PF_Line(PF_EntityContainer *parent, const PF_Vector& pStart, const PF_Vector& pEnd);

    void draw(QPainter *painter, PF_GraphicView *view) override;
protected:
    PF_LineData data;
};

#endif // PF_LINE_H
