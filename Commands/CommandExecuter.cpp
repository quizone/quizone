//---------------------------------------------------------------------------

#pragma hdrstop

#include "CommandExecuter.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

__fastcall TCommandExecuter::TCommandExecuter(bool CreateSuspended,TCommandQueue* Commands)
	: TThread(CreateSuspended)
{
Terminated = false;
this->Commands = Commands;
IsPaused = false;
IsStopped = false;
detectme=0;
}
//---------------------------------------------------------------------------
void __fastcall TCommandExecuter::Execute()
{
TCommand* Command;
RunningTicks = 0;
PausingTicks = 0;
CALCTICKS()
StartTicks = GETTICKS()
PreviousTicks = StartTicks;
while(!Terminated)
	{
	unsigned long CurrentTicks;
	if (IsStopped==true)
		{Sleep(50);continue;}
	if (IsPaused==true)
		{
		CALCTICKS()
		unsigned long CurrentTicks =  GETTICKS()
		PausingTicks = PausingTicks + (CurrentTicks - PreviousTicks);
		PreviousTicks = CurrentTicks;
		Sleep(50);
		continue;
		}
	CALCTICKS()

	CurrentTicks =  GETTICKS()
	RunningTicks = RunningTicks + (CurrentTicks - PreviousTicks) - PausingTicks;
	PreviousTicks = CurrentTicks;
	if (Commands->GetCommand(Command,RunningTicks) == false)
			{
			HandleIdle();
			LastCycleTicks = CurrentTicks;
			Sleep (5);
			continue;
			} // wait for next command
	TmapCommands::const_iterator iter = mapCommands.find(Command->GetCommandName());
	if (iter == mapCommands.end())
		{
		int i=detectme ;
		if (HandleCommand(Command)==false)
			{
			int i=detectme ;
			if (!Command->GetResult())
				Command->InsertResult(CommandErrorCodeAndMessage(COMMAND::COMMAND_DOES_NOT_EXIST));
			}
		}
	else
		{
		if (PreCommandHandler(Command))
			{
			(*iter->second)(Command,Object);
			PostCommandHandler(Command);
			}
		}
	LastCycleTicks = CurrentTicks;
	continue;
	}
 Terminated = true;
}

//---------------------------------------------------------------------------

void TCommandExecuter::Terminate()
{
 Terminated = true;
}

//---------------------------------------------------------------------------

bool TCommandExecuter::Stop()
{
 if (IsStopped == true) return false;
 IsStopped = true;
 IsPaused = false;
 return true;
}

//---------------------------------------------------------------------------

bool TCommandExecuter::Pause()
{
 if (IsStopped == true) return false;
 if (IsPaused == true) return false;
 IsPaused = true;
 return true;
}

//---------------------------------------------------------------------------

bool TCommandExecuter::Start()
{
CALCTICKS()
 if (IsStopped==false)
  {
  if (IsPaused == false)
	  {
	  return false;
	  }
  StartTicks = GETTICKS()
  PreviousTicks = StartTicks;
  RunningTicks = 0;
  IsPaused = false;
  return true;
  }
 Commands->RestartExecution();
 IsStopped = false;
 IsPaused = false;

 StartTicks = GETTICKS()
 PreviousTicks = StartTicks;
 PausingTicks = 0;
 RunningTicks = 0;
 return true;
}

