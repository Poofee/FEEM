#ifndef PF_VECTOR_H
#define PF_VECTOR_H

#include <vector>
#include <iosfwd>

class PF_Vector
{
public:
    PF_Vector()=default;
    PF_Vector(double vx, double vy, double vz=0.0);
    explicit PF_Vector(double angle);
    explicit PF_Vector(bool valid);
    ~PF_Vector()=default;

    explicit operator bool() const;

    void set(double angle);// set to unit vector by the direction of angle
    void set(double vx, double vy, double vz=0.0);
    void setPolar(double radius, double angle);

    double distanceTo(const PF_Vector & v) const;
    double angle() const;
    double angleTo(const PF_Vector & v) const;
    double angleBetween(const PF_Vector& v1, const PF_Vector& v2) const;
    double magnitude() const;

    bool isInWindow(const PF_Vector& firstCorner, const PF_Vector& secondCorner) const;

    PF_Vector move(const PF_Vector& offset);
    PF_Vector rotate(double angle);
    PF_Vector rotate(const PF_Vector& angleVector);
    PF_Vector rotate(const PF_Vector &center, double angle);

    PF_Vector scale(double factor);
    PF_Vector scale(const PF_Vector& factor);

    PF_Vector mirror(const PF_Vector& axisPoint1, const PF_Vector& axisPoint2);

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

    PF_Vector flipXY(void) const;
public:
    double x = 0.;
    double y = 0.;
    double z = 0.;
    bool valid = false;
};

#endif // PF_VECTOR_H
