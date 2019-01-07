#include "pf_vector.h"

#include <iostream>
#include <cmath>
#include <algorithm>


PF_Vector::PF_Vector(double vx, double vy, double vz)
    :x(vx)
    ,y(vy)
    ,z(vz)
    ,valid(true)
{

}
/**构造一个单位向量**/
PF_Vector::PF_Vector(double angle)
    :x(cos(angle))
    ,y(sin(angle))
    ,valid(true)
{

}

PF_Vector::PF_Vector(bool valid)
    :valid(valid)
{

}

/**按照角度设为一个单位向量**/
void PF_Vector::set(double angle)
{
    x = cos(angle);
    y = sin(angle);
    z = 0.;
    valid = true;
}

void PF_Vector::set(double vx, double vy, double vz)
{
    x = vx;
    y = vy;
    z = vz;
    valid = true;
}

void PF_Vector::setPolar(double radius, double angle)
{
    x = radius * cos(angle);
    y = radius * sin(angle);
    z = 0.0;
    valid = true;
}

double PF_Vector::distanceTo(const PF_Vector &v) const
{
    if(!valid || !v.valid){
        return 1e10;
    }else{
        return (*this-v).magnitude();
    }
}

double PF_Vector::magnitude() const
{
    double ret = 0.;
    if(valid){
        ret = hypot(hypot(x, y), z);
    }
    return ret;
}

PF_Vector PF_Vector::move(const PF_Vector &offset)
{
    *this += offset;
	return *this;
}

PF_Vector PF_Vector::rotate(const PF_Vector &angleVector)
{
	return *this;
}

PF_Vector PF_Vector::rotate(const PF_Vector &center, double angle)
{
	return *this;
}

PF_Vector PF_Vector::scale(double factor)
{
    x *= factor;
    y *= factor;
    return *this;
}

PF_Vector PF_Vector::scale(const PF_Vector &factor)
{
    x *= factor.x;
    y *= factor.y;
    return *this;
}

PF_Vector PF_Vector::mirror(const PF_Vector &axisPoint1, const PF_Vector &axisPoint2)
{
	return *this;
}

PF_Vector PF_Vector::operator +(const PF_Vector &v) const
{
    return {x+v.x,y+v.y,z+v.z};
}

PF_Vector PF_Vector::operator +(double d) const
{
    return {x+d,y+d,z+d};
}

PF_Vector PF_Vector::operator -(const PF_Vector &v) const
{
    return {x-v.x,y-v.y,z-v.z};
}

PF_Vector PF_Vector::operator -(double d) const
{
    return {x-d,y-d,z-d};
}

PF_Vector PF_Vector::operator *(const PF_Vector &v) const
{
    return {x*v.x,y*v.y,z*v.z};
}

PF_Vector PF_Vector::operator /(const PF_Vector &v) const
{
    if(fabs(v.x) > 1.0e-10 && fabs(v.y)>1.0e-10)
        return {x/v.x,y/v.y,std::isnormal(v.z)?z/v.z:z};
    return *this;
}

PF_Vector PF_Vector::operator *(double s) const
{
    return {x*s,y*s,z*s};
}

PF_Vector PF_Vector::operator /(double s) const
{
    if(fabs(s) > 1.0e-10)
        return {x/s,y/s,z/s};
    return *this;
}

PF_Vector PF_Vector::operator +=(const PF_Vector &v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}

PF_Vector PF_Vector::operator -=(const PF_Vector &v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}

PF_Vector PF_Vector::operator *=(const PF_Vector &v)
{
    x *= v.x;
    y *= v.y;
    z *= v.z;
    return *this;
}

PF_Vector PF_Vector::operator /=(const PF_Vector &v)
{
    if(fabs(v.x) > 1.0e-10 && fabs(v.y) > 1.0e-10){
        x /= v.x;
        y /= v.y;
        if(std::isnormal(v.z))
            z /= v.z;
    }
    return *this;
}

PF_Vector PF_Vector::operator *=(double s)
{
    if(fabs(s) > 1.0e-10){
        x *= s;
        y *= s;
        z *= s;
    }
    return *this;
}

PF_Vector PF_Vector::operator /=(double s)
{
    if(fabs(s) > 1.0e-10){
        x /= s;
        y /= s;
        z /= s;
    }
    return *this;
}

bool PF_Vector::operator ==(const PF_Vector &v) const
{
    return (x==v.x && y==v.y && z==v.z && valid && v.valid);
}

bool PF_Vector::operator ==(bool valid) const
{
    return this->valid == valid;
}

PF_Vector PF_Vector::minimum(const PF_Vector &v1, const PF_Vector &v2)
{
    if(!v1)
        return v2;
    if(!v2)
        return v1;
    return {std::min(v1.x, v2.x),
                std::min(v1.y, v2.y),
                std::min(v1.z, v2.z)

    };
}

PF_Vector PF_Vector::maximum(const PF_Vector &v1, const PF_Vector &v2)
{
    if(!v1)
        return v2;
    if(!v2)
        return v1;
    return {std::max(v1.x, v2.x),
                std::max(v1.y, v2.y),
                std::max(v1.z, v2.z)

    };
}

PF_Vector PF_Vector::flipXY() const
{
    return {y,x};
}

bool PF_Vector::operator !=(bool valid) const
{
    return this->valid != valid;
}

PF_Vector::operator bool() const
{
    return valid;
}
