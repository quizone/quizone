//---------------------------------------------------------------------------

#pragma hdrstop

#include "ScriptControl.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

namespace{

bool SetCommand	 		 (TCommand *Command, void *Object);
bool LoadScriptCommand	 (TCommand *Command, void *Object);
bool ReLoadScriptCommand (TCommand *Command, void *Object);
bool RunScriptCommand	 (TCommand *Command, void *Object);
bool StopScriptCommand	 (TCommand *Command, void *Object);
bool PauseScriptCommand	 (TCommand *Command, void *Object);
bool StopAllScriptCommand(TCommand *Command, void *Object);

//--------------------------------------------------------------


bool SetCommand(TCommand *Command, void *Object)
{

int Num;
CHECK_PARAM_NUM(Num,2)
double value=0;

/*
if (Command->Param(1)->IsString()==true)
	{
	TCommand Cmd(Command->Param(1)->GetAsString());
	if (Cmd.GetCommandName()=="GetSlot") {
	 value = ;
		}
	 else
		{
		if (!Command->GetResult())
		Command->InsertResult(CommandErrorCodeAndMessage(COMMAND::COMMAND_DOES_NOT_EXIST));
		return false;
		}
	}
else
*/
	value = Command->Param(1)->GetAsNumber();

TVars::const_iterator iter = TCommand::Vars->find(Command->Param(0)->GetAsString());
	if (iter == TCommand::Vars->end())
		{
		TCommand::Vars->insert(std::make_pair(Command->Param(0)->GetAsString(),value));
		}
	else
		TCommand::Vars->operator [](Command->Param(0)->GetAsString()) = value;
RETURN_SUCCESS
}

//--------------------------------------------------------------

bool ReLoadScriptCommand (TCommand *Command, void *Object)
{
int ScriptID=0;
TScripts** Scripts = (TScripts**)Object;
int Num;
CHECK_PARAM_NUM(Num,1)
READ_VAR_FROM_PARAM(0,ScriptID)
CHECK_VAR_RANGE(ScriptID,0,TScripts::ScriptCount-1)
if (Scripts[ScriptID]->IsLoaded())
	{
	int AutoRun=0;
	READ_VAR_FROM_PARAM(1,AutoRun)
	ostringstream ss,aa;
	ss << ScriptID;
	aa << AutoRun;
	string str = "LoadScript,\"" + Scripts[ScriptID]->GetFileName() +"\"," + ss.str() + "," + aa.str();
	Command->UpdateCommand(str);
	return LoadScriptCommand(Command,Object);
	}
Command->InsertResult(COMMAND::FILE_CANNOT_OPEN,"Script not loaded");
return false;
}

//---------------------------------------------------------------------------

bool LoadScriptCommand(TCommand *Command,void *Object)
{
int ScriptID=0;
TScripts** Scripts = (TScripts**)Object;
int Num;
CHECK_PARAM_NUM(Num,1)
READ_VAR_FROM_PARAM(1,ScriptID)
CHECK_VAR_RANGE(ScriptID,0,TScripts::ScriptCount-1)


Scripts[ScriptID]->Stop();

FILE *fp;
fp = fopen(Command->Param(0)->GetAsString().c_str(),"rt");
if (!fp)
	{
	Command->InsertResult(CommandErrorCodeAndMessage(COMMAND::FILE_CANNOT_OPEN));
	return false;
	}
Scripts[ScriptID]->Commands->ClearQueue();
Scripts[ScriptID]->SetFileName(Command->Param(0)->GetAsString());
Scripts[ScriptID]->SetSlot(ScriptID);

char fstr[1024];
char* exp;
TCommand* TempCommand = new TCommand("Temp");
double delay = 0;

while (fgets(fstr,1024,fp))
	{
	exp=fstr;
	while(isspace(*exp)) ++exp;
	if(strchr("#", *exp)) continue; //comment
	TempCommand->UpdateCommand((string)fstr);
	int Code; string Msg;
	if (TempCommand->GetResult(&Code,&Msg))
		{
	   //	Logging.AddLog(Msg,TempCommand->GetCommandString());
		continue;
		}
	if (TempCommand->GetCommandName()=="Delay")
		{
		if (TempCommand->GetParamCount()>0)
			{
			delay = delay + TempCommand->Param(0)->GetAsNumber();
			}
		continue;
		}
	if (TempCommand->GetCommandName()=="Offset")
		{
		if (TempCommand->GetParamCount()>0)
			{
			delay = TempCommand->Param(0)->GetAsNumber();
			}
		continue;
		}
	Scripts[ScriptID]->Commands->CreateCommand(TempCommand->GetCommandString(),delay*1000,COMMAND::SOURCE_SCRIPT);
	continue;
	}
Scripts[ScriptID]->Commands->CreateCommand("End",delay*1000,COMMAND::SOURCE_SCRIPT);
fclose(fp);

int AutoRun=0;
READ_VAR_FROM_PARAM(2,AutoRun)
if (AutoRun!=0)
	{
	Scripts[ScriptID]->Start();
	}
RETURN_SUCCESS
}


//---------------------------------------------------------------------------

bool RunScriptCommand(TCommand *Command,void *Object)
{
int ScriptID=0;
TScripts** Scripts = (TScripts**)Object;

READ_VAR_FROM_PARAM(0,ScriptID)
CHECK_VAR_RANGE(ScriptID,0,TScripts::ScriptCount-1)

if (Scripts[ScriptID]->Start())
	{
	RETURN_SUCCESS
	}
Command->InsertResult(COMMAND::OUT_OF_RANGE,"Script Already Running");
return false;
}

//---------------------------------------------------------------------------

bool StopScriptCommand(TCommand *Command,void *Object)
{
int ScriptID=0;
TScripts** Scripts = (TScripts**)Object;

READ_VAR_FROM_PARAM(0,ScriptID)
CHECK_VAR_RANGE(ScriptID,0,TScripts::ScriptCount-1)

if (Scripts[ScriptID]->Stop())
	{
	RETURN_SUCCESS
	}
Command->InsertResult(COMMAND::OUT_OF_RANGE,"Script Already Stopped");
return false;
}

//---------------------------------------------------------------------------

bool StopAllScriptCommand(TCommand *Command,void *Object)
{
int ScriptID=0;
TScripts** Scripts = (TScripts**)Object;

READ_VAR_FROM_PARAM(0,ScriptID)
CHECK_VAR_RANGE(ScriptID,0,TScripts::ScriptCount-1)

for (int i = 0; i < TScripts::ScriptCount; i++) {
	Scripts[i]->Stop();
	}
RETURN_SUCCESS
}

//---------------------------------------------------------------------------


bool PauseScriptCommand(TCommand *Command,void *Object)
{
int ScriptID=0;
TScripts** Scripts = (TScripts**)Object;

READ_VAR_FROM_PARAM(0,ScriptID)
CHECK_VAR_RANGE(ScriptID,0,TScripts::ScriptCount-1)

if (Scripts[ScriptID]->Pause())
	{
	RETURN_SUCCESS
	}
Command->InsertResult(COMMAND::OUT_OF_RANGE,"Script Already Paused");
return false;
}


};

//end of namespace
//---------------------------------------------------------------------------

__fastcall TScriptControl::TScriptControl(bool CreateSuspended,TCommandQueue* Commands)
: TCommandExecuter(CreateSuspended,Commands)
{
mapCommands.insert(std::make_pair("Set", &SetCommand));
mapCommands.insert(std::make_pair("LoadScript", &LoadScriptCommand));
mapCommands.insert(std::make_pair("ReLoadScript", &ReLoadScriptCommand));
mapCommands.insert(std::make_pair("RunScript", &RunScriptCommand));
mapCommands.insert(std::make_pair("StopScript", &StopScriptCommand));
mapCommands.insert(std::make_pair("PauseScript", &PauseScriptCommand));
mapCommands.insert(std::make_pair("StopAllScript", &StopAllScriptCommand));
detectme=3;
}

//---------------------------------------------------------------------------

bool TScriptControl::InitializeScripts(int NumberOfScripts,TCommandExecuter* Reciever)
{
if (TScripts::ScriptCount!=0) return false; // already initialized
if ((NumberOfScripts<1) && (NumberOfScripts>MAX_SCRIPT)) return false; // out of range
TScripts::ScriptCount = NumberOfScripts;
TScripts** Scripts = new TScripts*[TScripts::ScriptCount];
Object = (TScripts**)Scripts;

TCommandQueue** ScriptsCommands = new TCommandQueue*[TScripts::ScriptCount];
for (int i=0; i < TScripts::ScriptCount; i++) {
   ScriptsCommands[i] = new TCommandQueue;
   Scripts[i] = new TScripts(false,ScriptsCommands[i]);
   Scripts[i]->Reciever = Reciever;
   Scripts[i]->Stop();
	}
return true;
}

//---------------------------------------------------------------------------

__fastcall TScriptControl::~TScriptControl()
{
Terminate();
TScripts** Scripts = (TScripts**)Object;
for (int i=0; i < TScripts::ScriptCount; i++) {
   Scripts[i]->Terminate();
   TCommandQueue *Com = Scripts[i]->Commands;
   delete Scripts[i];
   delete Com;
	}
delete Scripts;
}

//---------------------------------------------------------------------------

