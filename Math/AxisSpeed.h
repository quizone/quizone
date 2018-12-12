//---------------------------------------------------------------------------

#ifndef AxisSpeedH
#define AxisSpeedH

#include "AxisMotion.h"

//---------------------------------------------------------------------------

class TAxisSpeed
{
private:
TAxisMotion Motion;
int Mode;
public:
double CalcSpeed(double Xi, double Vi, double Xt, double Vt, double Acc, double Vmax, double Tres,bool IsCircle = true,double TIME_MULTIPLIER=40);
};

extern int jj;



#endif
