#ifndef PF_DOCUMENT_H
#define PF_DOCUMENT_H
#include "pf_entitycontainer.h"

class PF_Document : public PF_EntityContainer
{
public:
    PF_Document(PF_EntityContainer* parent=nullptr);
    ~PF_Document()=default;

    void exportGmshGeo(const QString& filename);

    virtual bool isDocument() const{
        return true;
    }
    QString toGeoString() override;
    int index() const override;
protected:
    bool modified;
    QString fileName;
    QString autoSaveFileName;
    PF_GraphicView* gv;
};

#endif // PF_DOCUMENT_H
