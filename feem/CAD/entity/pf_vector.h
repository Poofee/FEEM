#ifndef PF_VECTOR_H
#define PF_VECTOR_H

#include <vector>
#include <iosfwd>
#include "pf.h"
#include <QString>

class PF_Vector
{
public:
    PF_Vector()=default;
    PF_Vector(double vx, double vy, double vz=0.0);
    PF_Vector(const PF_Vector& v);
    PF_Vector& operator=(const PF_Vector& v);
    explicit PF_Vector(double angle);
    explicit PF_Vector(bool valid);
    ~PF_Vector()=default;

    explicit operator bool() const;

    void set(double angle);// set to unit vector by the direction of angle
    void set(double vx, double vy, double vz=0.0);
    void setPolar(double radius, double angle);

    static PF_Vector polar(double rho, double theta);

    double distanceTo(const PF_Vector & v) const;
    double angle() const;
    double angleTo(const PF_Vector & v) const;
    double angleBetween(const PF_Vector& v1, const PF_Vector& v2) const;
    double magnitude() const;
    double squared() const;
    double squaredTo(const PF_Vector& v1) const; //return square of length

//    bool isInWindow(const PF_Vector& firstCorner, const PF_Vector& secondCorner) const;

    PF_Vector move(const PF_Vector& offset);
    PF_Vector rotate(double angle);
    PF_Vector rotate(const PF_Vector& angleVector);
    PF_Vector rotate(const PF_Vector& center, double ang);
    PF_Vector rotate(const PF_Vector& center, const PF_Vector& angleVector);

    PF_Vector scale(double factor);
    PF_Vector scale(const PF_Vector& factor);
    PF_Vector scale(const PF_Vector& factor) const;
    PF_Vector scale(const PF_Vector& center, const PF_Vector& factor);

    PF_Vector mirror(const PF_Vector& axisPoint1, const PF_Vector& axisPoint2);

    double dotP(const PF_Vector& v1) const;

    PF_Vector operator + (const PF_Vector& v) const;
    PF_Vector operator + (double d) const;
    PF_Vector operator - (const PF_Vector& v) const;
    PF_Vector operator - (double d) const;
    PF_Vector operator * (const PF_Vector& v) const;
    PF_Vector operator / (const PF_Vector& v) const;
    PF_Vector operator * (double s) const;
    PF_Vector operator / (double s) const;

    PF_Vector operator += (const PF_Vector& v);
    PF_Vector operator -= (const PF_Vector& v);
    PF_Vector operator *= (const PF_Vector& v);
    PF_Vector operator /= (const PF_Vector& v);
    PF_Vector operator *= (double s);
    PF_Vector operator /= (double s);

    bool operator == (const PF_Vector& v) const;
    bool operator != (const PF_Vector& v) const{
        return !operator==(v);
    }

    bool operator == (bool valid) const;
    bool operator != (bool valid) const;

    static PF_Vector minimum(const PF_Vector& v1, const PF_Vector& v2);
    static PF_Vector maximum(const PF_Vector& v1, const PF_Vector& v2);

    static PF_Vector crossP(const PF_Vector& v1, const PF_Vector& v2);
    static double dotP(const PF_Vector& v1, const PF_Vector& v2);

    PF_Vector flipXY(void) const;

    QString toString() const;
public:
    double x = 0.;
    double y = 0.;
    double z = 0.;
    bool valid = false;
};


/*!
 \brief PF_Vector 的一个容易，并提供一些操作。

*/
class PF_VectorSolutions {
public:
    typedef PF_Vector value_type;

    PF_VectorSolutions();
    PF_VectorSolutions(const std::vector<PF_Vector>& s);
    PF_VectorSolutions(std::initializer_list<PF_Vector> const& l);
    PF_VectorSolutions(int num);

    ~PF_VectorSolutions()=default;

    void alloc(size_t num);
    void clear();
    /**
     * @brief get range safe method of member access
     * @param i member index
     * @return indexed member, or invalid vector, if out of range
     */
    PF_Vector get(size_t i) const;
    const PF_Vector& at(size_t i) const;
    const PF_Vector&  operator [] (const size_t i) const;
    PF_Vector&  operator [] (const size_t i);
    size_t getNumber() const;
    size_t size() const;
    void resize(size_t n);
    bool hasValid() const;
    void set(size_t i, const PF_Vector& v);
    void push_back(const PF_Vector& v);
    void removeAt(const size_t i);
    PF_VectorSolutions& push_back(const PF_VectorSolutions& v);
    void setTangent(bool t);
    bool isTangent() const;
    PF_Vector getClosest(const PF_Vector& coord,
                         double* dist=nullptr, size_t* index=nullptr) const;
    double getClosestDistance(const PF_Vector& coord,
                              int counts = -1); //default to search all
    const std::vector<PF_Vector>& getVector() const;

    std::vector<PF_Vector>::const_iterator begin() const;
    std::vector<PF_Vector>::const_iterator end() const;
    std::vector<PF_Vector>::iterator begin();
    std::vector<PF_Vector>::iterator end();

    void rotate(double ang);
    void rotate(const PF_Vector& angleVector);
    void rotate(const PF_Vector& center, double ang);
    void rotate(const PF_Vector& center, const PF_Vector& angleVector);
    void move(const PF_Vector& vp);
    void scale(const PF_Vector& center, const PF_Vector& factor);
    void scale(const PF_Vector& factor);

    /** switch x,y for all vectors */
    PF_VectorSolutions flipXY(void) const;

    PF_VectorSolutions operator = (const PF_VectorSolutions& s);

//    friend std::ostream& operator << (std::ostream& os,
//                                      const PF_VectorSolutions& s);

private:
    std::vector<PF_Vector> vector;
    bool tangent;
};
#endif // PF_VECTOR_H
