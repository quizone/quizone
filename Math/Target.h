//---------------------------------------------------------------------------

#ifndef TargetH
#define TargetH

#include "AxisMotion.h"

#define MAXMOTION 300

//---------------------------------------------------------------------------

struct TMotions
{
unsigned long InitialTick;
TAxisMotion Motion;
};

class TTarget
{
private:
//unsigned long InitialTick;
//unsigned long NextMotionInitialTicks;
//TAxisMotion Motion;
//TAxisMotion NextMotion;
TMotions Motions[MAXMOTION];
unsigned long CurMotion;
unsigned long UptoMotion;
bool IsCircle;
int CircularType;

public:
void CreateTargetBySpeed(double DestinationSpeed, double Acc, unsigned long Ticks,bool IsCircle=false,unsigned long NextMotionTicks=0,int CircularType=CIRCULAR360);
double CreateTargetBySpeedPosDur(double Destination, double DestinationSpeed, double Duration, double MaxAcc, double MaxSpeed, unsigned long Ticks,bool IsCircle=false,unsigned long NextMotionTicks=0,int CircularType=CIRCULAR360,bool InfiniteSpeed = false);
void CreateTargetBySpeedMaxDuration(double DestinationSpeed, double MaxDuration, double DesiredAcc, unsigned long Ticks, bool IsCircle=false,unsigned long NextMotionTicks=0,int CircularType=CIRCULAR360);
void CreateImmediateTarget(double Destination, double DestinationSpeed, unsigned long Ticks,bool IsCircle=false,unsigned long NextMotionTicks=0,int CircularType=CIRCULAR360);
double CreateTargetByDuration(double Destination, double Duration, double DesiredAcc, unsigned long Ticks,bool IsCircle=false,unsigned long NextMotionTicks=0,int CircularType=CIRCULAR360);
void CreateTargetByDurationMaxSpeed(double Destination, double Duration, double Acc, double MaxSpeed,unsigned long Ticks,bool IsCircle,unsigned long NextMotionTicks=0,int CircularType=CIRCULAR360);
void CreateTargetByAcceleration(double Destination, double Acc, unsigned long Ticks,bool IsCircle=false,unsigned long NextMotionTicks=0,int CircularType=CIRCULAR360);
void CreateTargetByAccelerationMaxSpeed(double Destination, double Acc, double MaxSpeed, unsigned long Ticks, bool IsCircle = false,unsigned long NextMotionTicks=0,int CircularType=CIRCULAR360);
//void CreateTargetBySpeedTarget(double DestinationSpeed, double Destination, double Acc, unsigned long Ticks,bool IsCircle=false,unsigned long NextMotionTicks=0,int CircularType=CIRCULAR360);

double GetLocation(unsigned long Ticks);
double GetSpeed(unsigned long Ticks);
TTarget(double InitialDestination=0);
double GetDestination(unsigned long Ticks);
double GetDestinationSpeed();
};


#endif
