#ifndef PF_ENTITYCONTAINER_H
#define PF_ENTITYCONTAINER_H

#include "pf_entity.h"
#include <QList>

//2018-02-15
//by Poofee
/**该类实现entity的组合功能，也就是一个数组列表**/

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

    void setOwner(bool owner) {autoDelete=owner;}

    PF_Entity* first() const;
    PF_Entity* last() const;

    /**一系列对Entity的操作**/
    virtual void addEntity(PF_Entity* entity);
    virtual void appendEntity(PF_Entity* entity);
    virtual void prependEntity(PF_Entity* entity);
    virtual void moveEntity(int index, QList<PF_Entity *>& entList);
    virtual void insertEntity(int index, PF_Entity* entity);
    virtual bool removeEntity(PF_Entity* entity);

    virtual void addRectangle(PF_Vector const& corner1, PF_Vector const & corner2);
    void draw(QCPPainter *painter) override;

    const QList<PF_Entity *> &getEntityList();
    /** 继承的虚函数 **/
    PF_VectorSolutions getRefPoints() const override;

    PF_Vector getMiddlePoint(void)const override;
    PF_Vector getNearestEndpoint(const PF_Vector& coord,
                                 double* dist = nullptr) const override;
    PF_Vector getNearestPointOnEntity(const PF_Vector& coord,
                                      bool onEntity = true, double* dist = nullptr, PF_Entity** entity=nullptr)const override;
    PF_Vector getNearestCenter(const PF_Vector& coord,
                               double* dist = nullptr)const override;
    PF_Vector getNearestMiddle(const PF_Vector& coord,
                               double* dist = nullptr,
                               int middlePoints = 1 ) const override;
    PF_Vector getNearestDist(double distance,
                             const PF_Vector& coord,
                             double* dist = nullptr)const override;
    PF_Vector getNearestDist(double distance,
                             bool startp)const override;
    PF_Vector getNearestOrthTan(const PF_Vector& coord,
                                const PF_Line& normal,
                                bool onEntity = false) const override;

    bool offset(const PF_Vector& coord, const double& distance) override;
    //	PF_VectorSolutions getTangentPoint(const PF_Vector& point) const override;//find the tangential points seeing from given point
    //	PF_Vector getTangentDirection(const PF_Vector& point)const override;
    void move(const PF_Vector& offset) override;
    void rotate(const PF_Vector& center, const double& angle) override;
    void rotate(const PF_Vector& center, const PF_Vector& angleVector) override;
    void scale(const PF_Vector& center, const PF_Vector& factor) override;
    void mirror(const PF_Vector& axisPoint1, const PF_Vector& axisPoint2) override;
    void moveRef(const PF_Vector& ref, const PF_Vector& offset) override;
protected:
    QList<PF_Entity*> entities;/**保存所有实体**/
private:
    bool autoDelete;
};

#endif // PF_ENTITYCONTAINER_H
