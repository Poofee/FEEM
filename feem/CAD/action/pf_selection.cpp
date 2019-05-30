#include "pf_selection.h"

#include "pf_line.h"
//#include "pf_polyline.h"
#include "pf_entity.h"
//#include "PF_graphic.h"
//#include "PF_layer.h"



/**
 * Default constructor.
 *
 * @param container The container to which we will add
 *        entities. Usually that's an PF_Graphic entity but
 *        it can also be a polyline, text, ...
 */
PF_Selection::PF_Selection(PF_EntityContainer* container,
                           PF_GraphicView* graphicView) {
    this->container = container;
    this->graphicView = graphicView;
//    graphic = container.getGraphic();
}



/**
 * Selects or deselects the given entity.
 */
void PF_Selection::selectSingle(PF_Entity* e) {
    if (e /*&& (! (e->getLayer() && e->getLayer()->isLocked()))*/) {

//        if (graphicView) {
//            graphicView->deleteEntity(e);
//        }

        e->toggleSelected();

//        if (graphicView) {
//            graphicView->drawEntity(e);
//        }
        if (graphicView) {
            //graphicView->drawEntity(container);
            graphicView->replot();
        }
    }
}



/**
 * Selects all entities on visible layers.
 */
void PF_Selection::selectAll(bool select) {
    if (graphicView) {
        //graphicView->deleteEntity(container);
    }

    //container->setSelected(select);
    for(auto e: *container){
    //for (unsigned i=0; i<container->count(); ++i) {
        //PF_Entity* e = container->entityAt(i);

        if (e && e->isVisible()) {
            e->setSelected(select);
        }
    }

    if (graphicView) {
        //graphicView->drawEntity(container);
        graphicView->replot();
    }
}



/**
 * Selects all entities on visible layers.
 */
void PF_Selection::invertSelection() {
    if (graphicView) {
        //graphicView->deleteEntity(container);
    }

    for(auto e: *container){
    //for (unsigned i=0; i<container->count(); ++i) {
        //PF_Entity* e = container->entityAt(i);

        if (e && e->isVisible()) {
            e->toggleSelected();
        }
    }

    if (graphicView) {
        //graphicView->drawEntity(container);
        graphicView->replot();
    }
}
