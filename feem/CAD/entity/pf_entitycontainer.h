#ifndef PF_ENTITYCONTAINER_H
#define PF_ENTITYCONTAINER_H

#include "pf_entity.h"
#include <QList>

//2018-02-15
//by Poofee
/**该类实现entity的组合功能，也就是一个数组列表**/
enum GmshElementType{
    LINE_NODE2=1,
    TRIANGLE_NODE3,
    QUAD_NODE4
};
typedef struct _CNode
{
    double x, y, z;
}CNode;

typedef struct _CElement
{
    int n[3];// ni, nj, nk;//
    int ele_type;
    int physic_tag;
    int geometry_tag;
}CElement;
typedef struct _CMesh{
    int numNode;
    int numEle;
    CNode* nodes;
    CElement* eles;
}CMesh;

class PF_EntityContainer: public PF_Entity
{
public:
    PF_EntityContainer(PF_EntityContainer * parent=nullptr, PF_GraphicView* view=nullptr, bool owner=true);
    ~PF_EntityContainer() override;

    virtual void clear();

    bool isAtomic() const override{
        return false;
    }
    bool isContainer() const override{
        return true;
    }

    /** @return PF::EntityContainer */
    PF::EntityType rtti() const override{
        return PF::EntityContainer;
    }

    unsigned count() const override;

    void setOwner(bool owner) {autoDelete=owner;}

    PF_Entity* first() const;
    PF_Entity* last() const;

    virtual void addEntity(PF_Entity* entity);
    virtual void appendEntity(PF_Entity* entity);
    virtual void prependEntity(PF_Entity* entity);
    virtual void moveEntity(int index, QList<PF_Entity *>& entList);
    virtual void insertEntity(int index, PF_Entity* entity);
    virtual bool removeEntity(PF_Entity* entity);


    /**一系列对Entity的操作**/
    PF_Vector getNearestEndpoint(const PF_Vector& coord,
                                         double* dist = nullptr)const override;
    PF_Vector getNearestEndpoint(const PF_Vector& coord,
                                         double* dist, PF_Entity** pEntity ) const;

    PF_Entity* getNearestEntity(const PF_Vector& point,
                                double* dist = nullptr,
                                PF::ResolveLevel level=PF::ResolveAll) const;

    PF_Vector getNearestPointOnEntity(const PF_Vector& coord,
            bool onEntity = true,
                        double* dist = nullptr,
            PF_Entity** entity=nullptr)const override;

    PF_Vector getNearestCenter(const PF_Vector& coord,
                                       double* dist = nullptr)const override;
    PF_Vector getNearestMiddle(const PF_Vector& coord,
                                       double* dist = nullptr,
                                       int middlePoints = 1
                                       )const override;
    PF_Vector getNearestDist(double distance,
                                     const PF_Vector& coord,
                                     double* dist = nullptr) const override;
    PF_Vector getNearestIntersection(const PF_Vector& coord,
            double* dist = nullptr);
    PF_Vector getNearestRef(const PF_Vector& coord,
                                     double* dist = nullptr) const override;
    PF_Vector getNearestSelectedRef(const PF_Vector& coord,
                                     double* dist = nullptr) const override;

    double getDistanceToPoint(const PF_Vector& coord,
                                      PF_Entity** entity,
                                      PF::ResolveLevel level=PF::ResolveNone,
                                      double solidDist = PF_MAXDOUBLE) const override;

//    virtual bool optimizeContours();

//	bool hasEndpointsWithinWindow(const PF_Vector& v1, const PF_Vector& v2) override;

    void move(const PF_Vector& offset) override;
    void rotate(const PF_Vector& center, const double& angle) override;
    void rotate(const PF_Vector& center, const PF_Vector& angleVector) override;
    void scale(const PF_Vector& center, const PF_Vector& factor) override;
    void mirror(const PF_Vector& axisPoint1, const PF_Vector& axisPoint2a) override;

//    void stretch(const PF_Vector& firstCorner,
//                         const PF_Vector& secondCorner,
//                         const PF_Vector& offset) override;
    void moveRef(const PF_Vector& ref, const PF_Vector& offset) override;
    void moveSelectedRef(const PF_Vector& ref, const PF_Vector& offset) override;
    void revertDirection() override;

    void draw(QCPPainter *painter);

    virtual void adjustBorders(PF_Entity* entity);
    void calculateBorders() override;

    /**
     * @brief begin/end to support range based loop
     * @return iterator
     */
    QList<PF_Entity *>::const_iterator begin() const;
    QList<PF_Entity *>::const_iterator end() const;
    QList<PF_Entity *>::iterator begin() ;
    QList<PF_Entity *>::iterator end() ;

    const QList<PF_Entity*>& getEntityList();

    QString toGeoString() override;
    bool exportGeofile();
    void doMesh();
    CMesh *loadGmsh22(const char fn[]);
    int index() const override;
protected:
    QList<PF_Entity*> entities;/**保存所有实体**/
private:
    bool autoDelete;
};

#endif // PF_ENTITYCONTAINER_H
