#ifndef PF_MATERIAL_H
#define PF_MATERIAL_H

#include <QString>

class PF_Material
{
public:
    PF_Material();
};
class CComplex{
public:
    double im;
    double re;
};

class CMaterialProp
{
public:

    CMaterialProp();
    ~CMaterialProp();
    void StripBHData(QString &b, QString &h);
    void BHDataToCString(QString &b, QString &h);

    QString BlockName;
    double mu_x,mu_y;		// permeabilities, relative

    int    BHpoints;		// number of B-H datapoints;
    CComplex *BHdata;		    // array of B-H pairs;

    int    LamType;			// flag that tells how block is laminated;
    //	0 = not laminated or laminated in plane;
    //  1 = laminated in the x-direction;
    //  2 = laminated in the y-direction;
    double LamFill;			// lamination fill factor, dimensionless;
    double Theta_m;			// direction of the magnetization, degrees
    double H_c;				// magnetization, A/m
    CComplex Jsrc;			// applied current density, MA/m^2
    double Cduct;		    // conductivity of the material, MS/m
    double Lam_d;			// lamination thickness, mm
    double Theta_hn;		// max hysteresis angle, degrees, for nonlinear problems
    double Theta_hx;		// hysteresis angle, degrees, x-direction
    double Theta_hy;		// and y-direction, for anisotropic linear problems.
    int    NStrands;		// number of strands per wire
    double WireD;			// strand diameter, mm

    void GetSlopes();
    void GetSlopes(double omega);
    CComplex GetH(double B);
    CComplex GetdHdB(double B);
    CComplex Get_dvB2(double B);
    CComplex Get_v(double B);
    void GetBHProps(double B, CComplex &v, CComplex &dv);
    void GetBHProps(double B, double &v, double &dv);
    CComplex LaminatedBH(double omega, int i);

private:
};

#endif // PF_MATERIAL_H
