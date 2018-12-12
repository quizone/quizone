//---------------------------------------------------------------------------


#pragma hdrstop

#include "AxisSpeed.h"

//#define TIME_MULTIPLIER 40

int jj;
//---------------------------------------------------------------------------

#pragma package(smart_init)

double TAxisSpeed::CalcSpeed(double Xi, double Vi, double Xt, double Vt, double Acc, double Vmax, double Tres, bool IsCircle,double TIME_MULTIPLIER)
{
if (Tres<EPSILON) return Vi;
if (IsCircle == true)
        {
        Xt = _geo_degmod(Xt);
        Xi = _geo_degmod(Xi);
        }
Motion.SetTarget(Xi,Vi,Xt - (2*Tres*Vt),Vt,Acc,Vmax,IsCircle,false);
double Mnoj =  1 + (fabs(Vt) +1 ) * (fabs(Vi) +1 );   //fabs((TIME_MULTIPLIER-40)*0.1*Vt*Vi) +
if (Mnoj>TIME_MULTIPLIER) Mnoj = TIME_MULTIPLIER;
double Vmod = Vi;
double t;
if (Mnoj*Tres < Motion.GetTa()+Motion.GetTs()+Motion.GetTd())
        {
        if ((Motion.GetTa()>0.6*Motion.GetTd()) || ((Motion.GetTa()>0.3*Motion.GetTd()) && (Mode!=4)))
                {
                Vmod = Motion.GetSpeed(Motion.GetTa());
                t = Motion.GetTa();
                Mode = 1;
                }
        else
                {
                Vmod = Motion.GetSpeed(Mnoj*Tres);
                t = Mnoj*Tres;
                Mode = 4;
                }
        }
else
        {
        if ((Motion.GetTd()>Motion.GetTa()+EPSILON) && (2*Motion.GetTa()+Motion.GetTs() > Tres+EPSILON))
                {
                Vmod = Motion.GetSpeed(2*Motion.GetTa()+Motion.GetTs());
                t = 2*Motion.GetTa()+Motion.GetTs();
                Mode = 2;
                }
        else
                {
                if (Motion.GetTa()>2*Motion.GetTd())
                        {
                        Vmod = Motion.GetSpeed(Motion.GetTa());
                        t = Motion.GetTa();
                        }
                else
                        {
                        Vmod = Motion.GetSpeed(Motion.GetTa()+Motion.GetTs()+Motion.GetTd());
                        t = Motion.GetTa()+Motion.GetTs()+Motion.GetTd();
                        }
                Mode = 3;
                }
        }
if (Mode==1) jj=Mnoj; else jj=0;
double TheAcc = Acc;
double dist = _geo_degmod2(Xi-Xt) * _geo_degmod2(Xi-Xt) + (Vi-Vt) * (Vi-Vt);

if (t>EPSILON)
        TheAcc = (Vmod - Vi) / t;
if ((Mode == 1) || (Mode == 4))
        {
      // Vmod = Vi + TheAcc * Tres;
       Vmod = Vi + TheAcc * Tres * pow(dist,0.2);;

        }

if (Vmod > Vi + (Tres*Acc))
        Vmod = Vi + (Tres)*Acc;
if (Vmod < Vi - (Tres*Acc))
        Vmod = Vi - (Tres)*Acc;

if (Vmod<-Vmax) Vmod = -Vmax;
if (Vmod>Vmax) Vmod = Vmax;

return Vmod;
}
