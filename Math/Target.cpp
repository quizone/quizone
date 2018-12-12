//---------------------------------------------------------------------------

#pragma hdrstop
#include "Target.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

void TTarget::CreateImmediateTarget(double Destination, double DestinationSpeed, unsigned long Ticks,bool IsCircle,unsigned long NextMotionTicks,int CircularType)
{
if (DestinationSpeed>100000)
        int i=1;
this->IsCircle= IsCircle;
this->CircularType = CircularType;
UptoMotion++; //if (UptoMotion>MAXMOTION) UptoMotion = 0;
Motions[UptoMotion%MAXMOTION].InitialTick = Ticks + NextMotionTicks;
Motions[UptoMotion%MAXMOTION].Motion.SetImmediateTarget(Destination, DestinationSpeed, IsCircle,CircularType);

int Count = 0;
for (unsigned long i=CurMotion;i<UptoMotion;i++)
        {
        if (Motions[(i+1)%MAXMOTION].InitialTick<=Ticks) Count++;
        else break;
        }
CurMotion = CurMotion + Count;
}

//---------------------------------------------------------------------------

void TTarget::CreateTargetBySpeed(double DestinationSpeed, double Acc, unsigned long Ticks, bool IsCircle,unsigned long NextMotionTicks,int CircularType)
{
double CurSpeed = GetSpeed(Ticks + NextMotionTicks);
double CurLoc = GetLocation(Ticks + NextMotionTicks);
UptoMotion++;
Motions[UptoMotion%MAXMOTION].InitialTick = Ticks + NextMotionTicks;
this->IsCircle= IsCircle;
this->CircularType = CircularType;

Motions[UptoMotion%MAXMOTION].Motion.SetTargetSpeed(CurLoc,CurSpeed,DestinationSpeed,Acc,IsCircle,CircularType);

int Count = 0;
for (unsigned long i=CurMotion;i<UptoMotion;i++)
        {
        if (Motions[(i+1)%MAXMOTION].InitialTick<=Ticks) Count++;
        else break;
        }
CurMotion = CurMotion + Count;

}

//---------------------------------------------------------------------------

void TTarget::CreateTargetBySpeedMaxDuration(double DestinationSpeed, double MaxDuration, double DesiredAcc, unsigned long Ticks, bool IsCircle,unsigned long NextMotionTicks,int CircularType)
{
double CurSpeed = GetSpeed(Ticks + NextMotionTicks);
double CurLoc = GetLocation(Ticks + NextMotionTicks);

UptoMotion++;
Motions[UptoMotion%MAXMOTION].InitialTick = Ticks + NextMotionTicks;
this->IsCircle= IsCircle;
this->CircularType = CircularType;

Motions[UptoMotion%MAXMOTION].Motion.SetTargetSpeed(CurLoc,CurSpeed,DestinationSpeed,DesiredAcc,IsCircle,CircularType);
double Dur = Motions[UptoMotion%MAXMOTION].Motion.GetTa() + Motions[UptoMotion%MAXMOTION].Motion.GetTs() + Motions[UptoMotion%MAXMOTION].Motion.GetTd();
if (Dur>MaxDuration)
        {
        double NewAcc = fabs(CurSpeed - DestinationSpeed) / MaxDuration;
        Motions[UptoMotion%MAXMOTION].Motion.SetTargetSpeed(CurLoc,CurSpeed,DestinationSpeed,NewAcc,IsCircle,CircularType);
        Dur = Motions[UptoMotion%MAXMOTION].Motion.GetTa() + Motions[UptoMotion%MAXMOTION].Motion.GetTs() + Motions[UptoMotion%MAXMOTION].Motion.GetTd();
        }

int Count = 0;
for (unsigned long i=CurMotion;i<UptoMotion;i++)
        {
        if (Motions[(i+1)%MAXMOTION].InitialTick<=Ticks) Count++;
        else break;
        }
CurMotion = CurMotion + Count;

}

//---------------------------------------------------------------------------

void TTarget::CreateTargetByAcceleration(double Destination, double Acc, unsigned long Ticks, bool IsCircle,unsigned long NextMotionTicks,int CircularType)
{
double CurSpeed = GetSpeed(Ticks + NextMotionTicks);
double CurLoc = GetLocation(Ticks + NextMotionTicks);
this->IsCircle= IsCircle;
this->CircularType = CircularType;
UptoMotion++;
Motions[UptoMotion%MAXMOTION].InitialTick = Ticks + NextMotionTicks;

Motions[UptoMotion%MAXMOTION].Motion.SetTarget(CurLoc,CurSpeed,Destination,0, Acc, 1,IsCircle, true,CircularType);

int Count = 0;
for (unsigned long i=CurMotion;i<UptoMotion;i++)
        {
        if (Motions[(i+1)%MAXMOTION].InitialTick<=Ticks) Count++;
        else break;
        }
CurMotion = CurMotion + Count;

}
//---------------------------------------------------------------------------

double TTarget::CreateTargetBySpeedPosDur(double Destination, double DestinationSpeed, double Duration, double MaxAcc, double MaxSpeed, unsigned long Ticks,bool IsCircle,unsigned long NextMotionTicks,int CircularType, bool InfiniteSpeed)
{
double CurSpeed = GetSpeed(Ticks + NextMotionTicks);
double CurLoc = GetLocation(Ticks + NextMotionTicks);
UptoMotion++;
Motions[UptoMotion%MAXMOTION].InitialTick = Ticks + NextMotionTicks;
this->IsCircle= IsCircle;
this->CircularType = CircularType;

double s = Motions[UptoMotion%MAXMOTION].Motion.SetTargetPositionSpeedDuration( CurLoc,CurSpeed,Destination,DestinationSpeed,Duration,MaxAcc,MaxSpeed,IsCircle,InfiniteSpeed,CircularType);

int Count = 0;
for (unsigned long i=CurMotion;i<UptoMotion;i++)
        {
        if (Motions[(i+1)%MAXMOTION].InitialTick<=Ticks) Count++;
        else break;
        }
CurMotion = CurMotion + Count;
return s;
}

//---------------------------------------------------------------------------



void TTarget::CreateTargetByAccelerationMaxSpeed(double Destination, double Acc, double MaxSpeed, unsigned long Ticks, bool IsCircle,unsigned long NextMotionTicks,int CircularType)
{
double CurSpeed = GetSpeed(Ticks + NextMotionTicks);
double CurLoc = GetLocation(Ticks + NextMotionTicks);
this->IsCircle= IsCircle;
this->CircularType = CircularType;
UptoMotion++;
Motions[UptoMotion%MAXMOTION].InitialTick = Ticks + NextMotionTicks;

Motions[UptoMotion%MAXMOTION].Motion.SetTarget(CurLoc,CurSpeed,Destination,0, Acc, MaxSpeed,IsCircle, false,CircularType);

int Count = 0;
for (unsigned long i=CurMotion;i<UptoMotion;i++)
        {
        if (Motions[(i+1)%MAXMOTION].InitialTick<=Ticks) Count++;
        else break;
        }
CurMotion = CurMotion + Count;

}

//---------------------------------------------------------------------------

double TTarget::CreateTargetByDuration(double Destination, double Duration, double DesiredAcc, unsigned long Ticks,bool IsCircle,unsigned long NextMotionTicks,int CircularType)
{
double CurSpeed = GetSpeed(Ticks + NextMotionTicks);
double CurLoc = GetLocation(Ticks + NextMotionTicks);
UptoMotion++;
this->IsCircle= IsCircle;
this->CircularType = CircularType;
Motions[UptoMotion%MAXMOTION].InitialTick = Ticks + NextMotionTicks;
double ResAcc = DesiredAcc;
if (Duration<EPSILON)
        Motions[UptoMotion%MAXMOTION].Motion.SetImmediateTarget(Destination,0,IsCircle,CircularType);
else
        ResAcc = Motions[UptoMotion%MAXMOTION].Motion.SetTargetDuration (CurLoc,CurSpeed,Destination,0, Duration, DesiredAcc, IsCircle,CircularType);

int Count = 0;
for (unsigned long i=CurMotion;i<UptoMotion;i++)
        {
        if (Motions[(i+1)%MAXMOTION].InitialTick<=Ticks) Count++;
        else break;
        }
CurMotion = CurMotion + Count;
return ResAcc;
}

//---------------------------------------------------------------------------

void TTarget::CreateTargetByDurationMaxSpeed(double Destination, double Duration, double Acc, double MaxSpeed,unsigned long Ticks,bool IsCircle,unsigned long NextMotionTicks,int CircularType)
{
double CurSpeed = GetSpeed(Ticks + NextMotionTicks);
double CurLoc = GetLocation(Ticks + NextMotionTicks);
UptoMotion++;
this->IsCircle= IsCircle;
this->CircularType = CircularType;
Motions[UptoMotion%MAXMOTION].InitialTick = Ticks + NextMotionTicks;

double Dur = Motions[UptoMotion%MAXMOTION].Motion.SetTargetDuration (CurLoc,CurSpeed,Destination,0, Duration, Acc, IsCircle,CircularType);
if ((Dur > Duration + EPSILON) || (fabs(Motions[UptoMotion%MAXMOTION].Motion.GetAcc()) > Acc + EPSILON))
        Motions[UptoMotion%MAXMOTION].Motion.SetTarget(CurLoc,CurSpeed,Destination,0, Acc, MaxSpeed,IsCircle, false,CircularType);

int Count = 0;
for (unsigned long i=CurMotion;i<UptoMotion;i++)
        {
        if (Motions[(i+1)%MAXMOTION].InitialTick<=Ticks) Count++;
        else break;
        }
CurMotion = CurMotion + Count;
}

//---------------------------------------------------------------------------

double TTarget::GetLocation(unsigned long Ticks)
{
unsigned long Def = 0;
unsigned long index=CurMotion;
for (unsigned long i=CurMotion;i<UptoMotion;i++)
        {
        if (Motions[(i+1)%MAXMOTION].InitialTick <= Ticks)
                {
                index++;
             //   if (Motions[(i+1)%MAXMOTION].InitialTick >= Ticks)
             //           {
             //           index = i;
             //           break;
             //           }
                }
                else break;
        }

unsigned long jj = Motions[index%MAXMOTION].InitialTick;
if (Ticks<Motions[index%MAXMOTION].InitialTick)
        Def = 0;
else
        Def = Ticks - Motions[index%MAXMOTION].InitialTick;
double CalcTime = (Def)/1000.0;
if (CalcTime<0) CalcTime = 0;
double CurLoc =  Motions[index%MAXMOTION].Motion.GetPosition(CalcTime);
if (IsCircle == true)
        {
        if (CircularType==CIRCULAR360)
                return _geo_degmod(CurLoc);
        else
                return _geo_degmod2(CurLoc);
        }
else return CurLoc;
}

//---------------------------------------------------------------------------

double TTarget::GetSpeed(unsigned long Ticks)
{
unsigned long Def = 0;
unsigned long index=CurMotion;
for (unsigned long i=CurMotion;i<UptoMotion;i++)
        {
        if (Motions[i%MAXMOTION].InitialTick <= Ticks)
                {
                index++;
              //  if (Motions[(i+1)%MAXMOTION].InitialTick >= Ticks)
              //          {
              //          index = i;
              //          break;
              //          }
                }
                else break;
        }

if (Ticks<Motions[index%MAXMOTION].InitialTick)
        Def = 0;
else
        Def = Ticks - Motions[index%MAXMOTION].InitialTick;
double CalcTime = (Def)/1000.0;
if (CalcTime<0) CalcTime = 0;
double CurSpeed = Motions[index%MAXMOTION].Motion.GetSpeed(CalcTime);
return CurSpeed;
}

//---------------------------------------------------------------------------

TTarget::TTarget(double InitialDestination)
{
CurMotion = 0;
UptoMotion = 0;
//InitialTick = 0;
Motions[0].InitialTick = 0;
this->CircularType = CircularType;
Motions[0].Motion.SetTarget(0,0,InitialDestination,0, 1, 1,false,true);
//NextMotion.SetTarget(0,0,InitialDestination,0, 1, 1,false,true);
}

//---------------------------------------------------------------------------

double TTarget::GetDestination(unsigned long Ticks)
{
double TotalT = Motions[UptoMotion%MAXMOTION].Motion.GetTa()+Motions[UptoMotion%MAXMOTION].Motion.GetTs()+Motions[UptoMotion%MAXMOTION].Motion.GetTd();
double OffsetT = (Ticks-Motions[CurMotion%MAXMOTION].InitialTick)/1000.0;
double CalcT = TotalT;
if (OffsetT > TotalT)
        CalcT = OffsetT;
return Motions[UptoMotion%MAXMOTION].Motion.GetPosition(CalcT);
}

//---------------------------------------------------------------------------

double TTarget::GetDestinationSpeed()
{
return Motions[UptoMotion%MAXMOTION].Motion.GetSpeed(Motions[UptoMotion%MAXMOTION].Motion.GetTa()+Motions[UptoMotion%MAXMOTION].Motion.GetTs()+Motions[UptoMotion%MAXMOTION].Motion.GetTd());
}


