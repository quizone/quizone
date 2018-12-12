//---------------------------------------------------------------------------

#ifndef MainControlH
#define MainControlH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
//---------------------------------------------------------------------------

#include "ScriptControl.h"
#include "QuizControl.h"
#include "AxisMotion.h"
#include "QueryControl.h"
#include "MusicControl.h"

class TMainControl : public TCommandExecuter
{
private:
	virtual bool HandleCommand(TCommand* Command);
	virtual void HandleIdle();
	unsigned long SendTicks;

public:
	__fastcall TMainControl(bool CreateSuspended,TCommandQueue* Commands);
	__fastcall ~TMainControl();
	void UpdateVar(string VarName,double VarValue);

	TCommandQueue* ReportCommands;
	TCommandQueue* UICommands;
	TCommandQueue* QuizCommands;
	TQuizControl* QuizControl;

	TCommandQueue* QueryCommands;
	TQueryControl* QueryControl;

	TCommandQueue* MusicCommands;
	TMusicControl* MusicControl;

	 TScriptControl* ScriptControl;
	 TCommandQueue* ScriptCommands;

};

//---------------------------------------------------------------------------
#endif

