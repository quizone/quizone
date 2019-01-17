//---------------------------------------------------------------------------

#ifndef QuizControlH
#define QuizControlH
//---------------------------------------------------------------------------

#include "CommandExecuter.h"

//---------------------------------------------------------------------------

void PrepareRandomValues(int min, int max);
bool GetRandomValue();


class TLinearMotion
{
public:
TLinearMotion(double InitialValue=0);
void Set(double Target, double Rate=1000000);
double Update(unsigned long duration);
double GetCurrent() {return Current;};
private:
double Current;
double Target;
unsigned long StartTime;
double Rate;
};

//---------------------------------------------------------------------------

class TVisualObject
{
public:
TVisualObject() : Opacity(1)  {ScaleX.Set(1); ScaleY.Set(1);};
TLinearMotion Opacity;
TLinearMotion PosX;
TLinearMotion PosY;
TLinearMotion ScaleX;
TLinearMotion ScaleY;
string Caption;
unsigned long Color;
};

//---------------------------------------------------------------------------

class TItem
{
public:
TItem() {Scale = 1; Reload = false; AddNumber=true;};
string FileName;
double SizeX;
double SizeY;
double PosX;
double PosY;
TVisualObject Image;
TVisualObject Label;
double Scale;
bool Reload;
bool AddNumber;
};

//---------------------------------------------------------------------------

class TQuiz
{
public:
TQuiz();
int NumItems;
TItem* Items;
int* Sequence;
int Model;
double Rate;
unsigned long InitialTime;
double Acc;
double TransitionTime;
string Path;
string Label;
double LabelOpacity;

double LabelTargetOpacity;
double LabelOpacityStartingTime;
double LabelOpacityChangeRate;
};

//---------------------------------------------------------------------------

class TQuizControl : public TCommandExecuter
{
private:


public:
__fastcall TQuizControl(bool CreateSuspended,TCommandQueue* Commands,TQuiz *Quiz);
void PostCommandHandler(TCommand * Command);
__fastcall ~TQuizControl();
void HandleIdle();
};

#endif
