#ifndef PF_DOCUMENT_H
#define PF_DOCUMENT_H
#include "pf_entitycontainer.h"

class PF_Document : public PF_EntityContainer
{
public:
    PF_Document(PF_EntityContainer* parent=nullptr);
    ~PF_Document()=default;
protected:
    bool modified;
    QString fileName;
    QString autoSaveFileName;
    PF_GraphicView* gv;
};

#endif // PF_DOCUMENT_H
