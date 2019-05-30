#include "pf_material.h"

#include <stdio.h>
#include <QDebug>

PF_Material::PF_Material()
{

}

CMaterialProp::CMaterialProp()
{
    BlockName=QObject::tr("New Material");
    mu_x=1.;
    mu_y=1.;			// permeabilities, relative
    H_c=0.;				// magnetization, A/m
    Jsrc.im=0;				// applied current density, MA/m^2
    Jsrc.re=0.;
    Cduct=0.;		    // conductivity of the material, MS/m
    Lam_d=0.;			// lamination thickness, mm
    Theta_hn=0.;			// hysteresis angle, degrees
    Theta_hx=0.;			// hysteresis angle, degrees
    Theta_hy=0.;			// hysteresis angle, degrees
    Theta_m=0.;			// magnetization direction, degrees;
    LamFill=1.;			// lamination fill factor;
    LamType=0;			// type of lamination;
    WireD=0;			// strand diameter, mm
    NStrands=0;			// number of strands per wire

    BHpoints=0;
}

CMaterialProp::~CMaterialProp()
{
    qDebug()<<Q_FUNC_INFO;
    if(BHpoints>0) free(BHdata);
}

void CMaterialProp::StripBHData(QString &b, QString &h)
{
//    int i,k;
//    char *buff,*nptr,*endptr;
//    double z;
//    CArray <double, double> B;
//    CArray <double, double> H;

//    if (BHpoints>0) free(BHdata);
//    B.RemoveAll();
//    H.RemoveAll();

//    k=b.GetLength()+10;
//    buff=(char *)calloc(k,sizeof(char));
//    strcpy(buff,b);
//    nptr=buff;
//    while (sscanf(nptr,"%lf",&z)!=EOF){
//        z=strtod(nptr,&endptr );
//        if(nptr==endptr) nptr++; //catch special case
//        else nptr=endptr;
//        if(B.GetSize()>0){ // enforce monotonicity
//            if (z<=B[B.GetSize()-1])
//                break;
//        }
//        else if(z!=0) B.Add(0);
//        B.Add(z);
//    }
//    free(buff);

//    k=h.GetLength()+10;
//    buff=(char *)calloc(k,sizeof(char));
//    strcpy(buff,h);
//    nptr=buff;
//    while (sscanf(nptr,"%lf",&z)!=EOF){
//        z=strtod(nptr,&endptr );
//        if(nptr==endptr) nptr++;
//        else nptr=endptr;
//        if(H.GetSize()>0){
//            if (z<=H[H.GetSize()-1])
//                break;
//        }
//        else if(z!=0) H.Add(0);
//        H.Add(z);
//    }

//    k=B.GetSize();
//    if (H.GetSize()<k) k=H.GetSize();

//    if (k>1){
//        BHpoints=k;
//        {
//            BHdata=(CComplex *)calloc(k,sizeof(CComplex));
//            for(i=0;i<k;i++) BHdata[i].Set(B[i],H[i]);
//        }
//    }
//    else BHpoints=0;
//    free(buff);

    return;
}

void CMaterialProp::BHDataToCString(QString &b, QString &h)
{
//    int i;
//    char c[80];

//    b.Empty();
//    h.Empty();

//    for(i=0;i<BHpoints;i++){
//        sprintf(c,"%f%c%c",BHdata[i].re,0x0D,0x0A);
//        b+=c;
//        sprintf(c,"%f%c%c",BHdata[i].im,0x0D,0x0A);
//        h+=c;
//    }

//    b.AnsiToOem();
    //    h.AnsiToOem();
}

