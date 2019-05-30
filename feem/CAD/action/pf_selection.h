#ifndef PF_SELECTION_H
#define PF_SELECTION_H

#include "pf_entitycontainer.h"
#include "pf_graphicview.h"

/**
 * API Class for selecting entities.
 * There's no interaction handled in this class.
 * This class is connected to an entity container and
 * can be connected to a graphic view.
 *
 * @author Andrew Mustun
 */
class PF_Selection {
public:
    PF_Selection(PF_EntityContainer* entityContainer,
                 PF_GraphicView* graphicView=NULL);

    void selectSingle(PF_Entity* e);
    void selectAll(bool select=true);
    void deselectAll() {
        selectAll(false);
    }
    void invertSelection();
//    void selectWindow(const PF_Vector& v1, const PF_Vector& v2,
//                      bool select=true, bool cross=false);
//    void deselectWindow(const PF_Vector& v1, const PF_Vector& v2) {
//        selectWindow(v1, v2, false);
//    }
//    void selectIntersected(const PF_Vector& v1, const PF_Vector& v2,
//                      bool select=true);
//    void deselectIntersected(const PF_Vector& v1, const PF_Vector& v2) {
//		selectIntersected(v1, v2, false);
//	}
//    void selectContour(PF_Entity* e);

//    void selectLayer(PF_Entity* e);
//    void selectLayer(const QString& layerName, bool select=true);
//    void deselectLayer(QString& layerName) {
//		selectLayer(layerName, false);
//	}

protected:
    PF_EntityContainer* container;
//    PF_Graphic* graphic;
    PF_GraphicView* graphicView;
};

#endif // PF_SELECTION_H
