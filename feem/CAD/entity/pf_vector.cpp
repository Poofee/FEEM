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

PF_Vector PF_Vector::polar(double rho, double theta)
{
    return {rho*cos(theta),rho*sin(theta),0};
}

double PF_Vector::distanceTo(const PF_Vector &v) const
{
    if(!valid || !v.valid){
        return 1e10;
    }else{
        return (*this-v).magnitude();
    }
}

double PF_Vector::angle() const
{
    return fmod(M_PI + remainder(atan2(y,x) - M_PI, M_PI*2), M_PI*2);
}

double PF_Vector::angleTo(const PF_Vector &v) const
{
    if (!valid || !v.valid) return 0.0;
    return (v-(*this)).angle();
}

double PF_Vector::angleBetween(const PF_Vector &v1, const PF_Vector &v2) const
{
    if (!valid || !v1.valid || !v2.valid) return 0.0;
    PF_Vector const vStart(v1 - (*this));
    PF_Vector const vEnd(v2 - (*this));
    double a = atan2( vStart.x*vEnd.y-vStart.y*vEnd.x, vStart.x*vEnd.x+vStart.y*vEnd.y);
    return fmod(M_PI + remainder(a - M_PI, M_PI*2), M_PI*2);
}

double PF_Vector::magnitude() const
{
    double ret = 0.;
    if(valid){
        ret = hypot(hypot(x, y), z);
    }
    return ret;
}

double PF_Vector::squared() const
{
    if (valid)
        return x*x + y*y + z*z;
    return PF_MAXDOUBLE;
}

/**
 * @brief 返回距离的平方。
 *
 * @param v1
 * @return double
 */
double PF_Vector::squaredTo(const PF_Vector &v1) const
{
    if (valid && v1.valid) {
        return  (*this - v1).squared();
    }
    return PF_MAXDOUBLE;
}

PF_Vector PF_Vector::move(const PF_Vector &offset)
{
    *this += offset;
    return *this;
}

PF_Vector PF_Vector::rotate(double angle)
{
    rotate(PF_Vector{angle});
    return *this;
}

PF_Vector PF_Vector::rotate(const PF_Vector &angleVector)
{
    double x0 = x * angleVector.x - y * angleVector.y;
    y = x * angleVector.y + y * angleVector.x;
    x = x0;

    return *this;
}

PF_Vector PF_Vector::rotate(const PF_Vector &center, double angle)
{
    *this = center + (*this-center).rotate(angle);
    return *this;
}

PF_Vector PF_Vector::rotate(const PF_Vector &center, const PF_Vector &angleVector)
{
    *this = center + (*this-center).rotate(angleVector);
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

PF_Vector PF_Vector::scale(const PF_Vector &factor) const
{
    return {x*factor.x, y*factor.y};
}

PF_Vector PF_Vector::scale(const PF_Vector &center, const PF_Vector &factor)
{
    *this = center + (*this-center).scale(factor);
    return *this;
}

PF_Vector PF_Vector::mirror(const PF_Vector &axisPoint1, const PF_Vector &axisPoint2)
{
    PF_Vector direction(axisPoint2-axisPoint1);
    double a= direction.squared();
    PF_Vector ret(false);
    if(a<PF_TOLERANCE2) {
        return ret;
    }
    ret= axisPoint1 + direction* dotP(*this - axisPoint1,direction)/a; //projection point
    *this = ret + ret - *this;
    return *this;
}

double PF_Vector::dotP(const PF_Vector &v1) const
{
    return x*v1.x+y*v1.y;
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

PF_Vector PF_Vector::crossP(const PF_Vector &v1, const PF_Vector &v2)
{
    return {v1.y*v2.z - v1.z*v2.y,
                v1.z*v2.x - v1.x*v2.z,
                v1.x*v2.y - v1.y*v2.x};
}

double PF_Vector::dotP(const PF_Vector &v1, const PF_Vector &v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

PF_Vector PF_Vector::flipXY() const
{
    return {y,x};
}

QString PF_Vector::toString() const
{
    return QString("(%1,%2)").arg(x).arg(y);
}

bool PF_Vector::operator !=(bool valid) const
{
    return this->valid != valid;
}

PF_Vector::operator bool() const
{
    return valid;
}

//-------------------------------------------
//
//
//            PF_VectorSolutions
//
//-------------------------------------------
/**
 * Constructor for no solution.
 */
PF_VectorSolutions::PF_VectorSolutions()
    :vector(0)
    ,tangent(false)
{
}

PF_VectorSolutions::PF_VectorSolutions(const std::vector<PF_Vector>& l)
    : vector( l.begin(), l.end())
    , tangent(false)
{
}

/**
 * Constructor for num solutions.
 */
PF_VectorSolutions::PF_VectorSolutions(int num)
    : vector(num, PF_Vector(false))
    , tangent(false)
{
}

PF_VectorSolutions::PF_VectorSolutions(std::initializer_list<PF_Vector> const& l)
    : vector(l)
    , tangent(false)
{
}


/*!
 \brief 调整大小

 \param num
*/
void PF_VectorSolutions::alloc(size_t num) {
    if(num <= vector.size()){
        vector.resize(num);
    }else{
        const std::vector<PF_Vector> v(num - vector.size());
        vector.insert(vector.end(), v.begin(), v.end());
    }
}

PF_Vector PF_VectorSolutions::get(size_t i) const
{
    if(i<vector.size())
        return vector.at(i);
    return {};
}

const PF_Vector&  PF_VectorSolutions::operator [] (const size_t i) const
{
    return vector[i];
}

PF_Vector&  PF_VectorSolutions::operator [] (const size_t i)
{
    return vector[i];
}

size_t PF_VectorSolutions::size() const
{
    return vector.size();
}
/**
 * Deletes vector array and resets everything.
 */
void PF_VectorSolutions::clear() {
    vector.clear();
    tangent = false;
}

/**
 * @return vector solution number i or an invalid vector if there
 * are less solutions.
 */
const PF_Vector& PF_VectorSolutions::at(size_t i) const {
    return vector.at(i);
}

/**
 * @return Number of solutions available.
 */
size_t PF_VectorSolutions::getNumber() const {
    return vector.size();
}

/**
 * @retval true There's at least one valid solution.
 * @retval false There's no valid solution.
 */
bool PF_VectorSolutions::hasValid() const {
    for(const PF_Vector& v: vector)
        if (v.valid)  return true;

    return false;
}

void PF_VectorSolutions::resize(size_t n){
    vector.resize(n);
}

const std::vector<PF_Vector>& PF_VectorSolutions::getVector() const {
    return vector;
}

std::vector<PF_Vector>::const_iterator PF_VectorSolutions::begin() const
{
    return vector.begin();
}

std::vector<PF_Vector>::const_iterator PF_VectorSolutions::end() const
{
    return vector.end();
}

std::vector<PF_Vector>::iterator PF_VectorSolutions::begin()
{
    return vector.begin();
}

std::vector<PF_Vector>::iterator PF_VectorSolutions::end()
{
    return vector.end();
}

void PF_VectorSolutions::push_back(const PF_Vector& v) {
    vector.push_back(v);
}

void PF_VectorSolutions::removeAt(const size_t i){
    if (vector.size()> i)
        vector.erase(vector.begin()+i);
}

PF_VectorSolutions& PF_VectorSolutions::push_back(const PF_VectorSolutions& v) {
    vector.insert(vector.end(), v.begin(), v.end());
    return *this;
}

/**
 * Sets the solution i to the given vector.
 * If i is greater than the current number of solutions available,
 * nothing happens.
 */
void PF_VectorSolutions::set(size_t i, const PF_Vector& v) {
    if (i<vector.size()) {
        vector[i] = v;
    }else{
        //            RS_DEBUG->print(RS_Debug::D_ERROR, "set member in vector in PF_VectorSolutions: out of range, %d to size of %d", i,vector.size());
        for(size_t j=vector.size();j<=i;++j)
            vector.push_back(v);
    }
}

/**
 * Sets the tangent flag.
 */
void PF_VectorSolutions::setTangent(bool t) {
    tangent = t;
}

/**
 * @return true if at least one of the solutions is a double solution
 * (tangent).
 */
bool PF_VectorSolutions::isTangent() const {
    return tangent;
}

/**
 * Rotates all vectors around (0,0) by the given angle.
 */
void PF_VectorSolutions::rotate(double ang) {
    PF_Vector angleVector(ang);
    for (auto& vp: vector) {
        if (vp.valid) {
            vp.rotate(angleVector);
        }
    }
}

/**
 * Rotates all vectors around (0,0) by the given angleVector.
 */
void PF_VectorSolutions::rotate(const PF_Vector& angleVector) {
    for (auto& vp: vector) {
        if (vp.valid) {
            vp.rotate(angleVector);
        }
    }
}

/**
 * Rotates all vectors around the given center by the given angle.
 */
void PF_VectorSolutions::rotate(const PF_Vector& center, double ang) {
    const PF_Vector angleVector(ang);
    for (auto& vp: vector) {
        if (vp.valid) {
            vp.rotate(center,angleVector);
        }
    }
}

void PF_VectorSolutions::rotate(const PF_Vector& center, const PF_Vector& angleVector) {
    for (auto& vp: vector) {
        if (vp.valid) {
            vp.rotate(center, angleVector);
        }
    }
}

/**
 * Move all vectors around the given center by the given vector.
 */
void PF_VectorSolutions::move(const PF_Vector& vp) {
    for (PF_Vector& v: vector) {
        if (v.valid) {
            v.move(vp);
        }
    }
}

/**
 * Scales all vectors by the given factors with the given center.
 */
void PF_VectorSolutions::scale(const PF_Vector& center, const PF_Vector& factor) {
    for (auto& vp: vector) {
        if (vp.valid) {
            vp.scale(center, factor);
        }
    }
}

void PF_VectorSolutions::scale( const PF_Vector& factor) {
    for (auto& vp: vector) {
        if (vp.valid) {
            vp.scale(factor);
        }
    }
}

/**
 * @return vector solution which is the closest to the given coordinate.
 * dist will contain the distance if it doesn't point to NULL (default).
 */
PF_Vector PF_VectorSolutions::getClosest(const PF_Vector& coord,
                                         double* dist, size_t* index) const {

    double curDist{0.};
    double minDist = PF_MAXDOUBLE;
    PF_Vector closestPoint{false};
    int pos(0);

    for (size_t i=0; i<vector.size(); i++) {
        if (vector[i].valid) {
            curDist = (coord - vector[i]).squared();

            if (curDist<minDist) {
                closestPoint = vector[i];
                minDist = curDist;
                pos=i;
            }
        }
    }
    if (dist) {
        *dist = sqrt(minDist);
    }
    if (index) {
        *index = pos;
    }
    return closestPoint;
}

/**
  *@ return the closest distance from the first counts rs_vectors
  *@coord, distance to this point
  *@counts, only consider this many points within solution
  */
double PF_VectorSolutions::getClosestDistance(const PF_Vector& coord,
                                              int counts)
{
    double ret=PF_MAXDOUBLE*PF_MAXDOUBLE;
    int i=vector.size();
    if (counts < i && counts >= 0) i=counts;
    std::for_each(vector.begin(), vector.begin() + i,
                  [&ret, &coord](PF_Vector const& vp) {
        if(vp.valid) {
            double d=(coord - vp).squared();
            if(d<ret) ret=d;
        }
    }
    );

    return sqrt(ret);
}

/** switch x,y for all vectors */
PF_VectorSolutions PF_VectorSolutions::flipXY(void) const
{
    PF_VectorSolutions ret;
    for(const auto& vp: vector)
        ret.push_back(vp.flipXY());
    return ret;
}

PF_VectorSolutions PF_VectorSolutions::operator = (const PF_VectorSolutions& s) {
    setTangent(s.isTangent());
    vector=s.vector;

    return *this;
}

//std::ostream& operator << (std::ostream& os,
//                           const PF_VectorSolutions& s) {
//    for (const PF_Vector& vp: s){
//        os << "(" << vp << ")\n";
//    }
//    os << " tangent: " << (int)s.isTangent() << "\n";
//    return os;
//}
