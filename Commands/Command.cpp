//---------------------------------------------------------------------------

#pragma hdrstop

#include "Command.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

TVars* TCommand::Vars = NULL;
TStringVars* TCommand::StringVars = NULL;


double TParameters::GetAsNumber()
{
TAnalyzer Analyzer(TCommand::Vars);
return Analyzer.sniff(Value.c_str());
}

//-------------------------------------------------

bool TParameters::IsValid()
{
if (ParamType!=Tokentypes::NUMBER) return true;
TAnalyzer Analyzer(TCommand::Vars);
Analyzer.sniff(Value.c_str());
if (Analyzer.GetLastError()!=0)
	return false;
return true;
}

//-------------------------------------------------

string TParameters::GetAsString(bool convert)
{
if (convert==false) {
   return Value;
}
if (IsString()) return Value;
if (IsEmpty()) return "";
TAnalyzer Analyzer(TCommand::StringVars);
return Analyzer.GetStringVarValue(Value.c_str());
	TStringVars::const_iterator iter = TCommand::StringVars->find(Value);
	if (iter == TCommand::StringVars->end())
		{
		//serror(COMMAND::VARIABLE_DOES_NOT_EXIST);
		return Value;
		}
	else
		{
		return (string)iter->second;
		}

}

//-------------------------------------------------
 /*
TParameters::TParameters(TVars * Vars)
{
this->Vars = Vars;
}
	  */
//-------------------------------------------------

TCommand::TCommand(string CommandString, unsigned long mmDelay, int Source)
{
 CommandName = "";
 CommandClass = "";
 NumberOfParams = 0;
 Params = NULL;
 Expression = NULL;
 ErrorCode = COMMAND::SUCCESS;
 LinkCount = 1;
 UpdateCommand(CommandString,mmDelay,Source);
}

//-------------------------------------------------

void TCommand::UpdateCommand(string CommandString, unsigned long mmDelay, int Source)
{
 if (!Expression)
		delete [] Expression;
 Expression = new char[CommandString.size()+1];
 Command = CommandString;
 this->Source = Source;
 this->CommandExecuteDelay = mmDelay;
 RemoveResult();
// ftime(tmb);
 if (!Parse())
	{InsertResult(ErrorCode,COMMAND::CommandErrorString[ErrorCode]); }
}

//-------------------------------------------------

//TDateTime Time;

void TCommand::InsertResult(int Code, string Msg)
{
 ResultCode = Code;
 ResultMessage = Msg;
 IsCompleted = true;
}

//-------------------------------------------------

bool TCommand::GetResult(int *Code, string *Msg)
{
if (IsCompleted==false)
	return false;
if (Code)
	*Code = ResultCode;
if (Msg)
	*Msg = ResultMessage;
return true;
}

//-------------------------------------------------

void TCommand::DecreaseRef()
{
LinkCount--;
if (LinkCount<1) {
	delete this;
	}
}

//-------------------------------------------------

void TCommand::IncrementRef()
{
LinkCount++;
}

//-------------------------------------------------
TCommand::~TCommand()
{
if (Params!=NULL)
		delete [] Params;
if (Expression!=NULL)
		delete [] Expression;
}

//-------------------------------------------------

void TCommand::RemoveResult()
{
IsCompleted = false;
ResultCode = 0;
ResultMessage = "";
}

//-------------------------------------------------
bool TCommand::Parse()
{
char token[1024];
char *pExpresion;     // Points to the expresion
CommandName = "";
CommandClass = "";
NumberOfParams = 0;
Params = NULL;
ErrorCode = COMMAND::SUCCESS;

if (Command=="") { ErrorCode = COMMAND::EMPTY_EXPRESSION;
		return false;}

for (int i=0; i < Command.size(); i++) {
	   Expression[i] = Command[i];
	}
Expression[Command.size()] = 0;
pExpresion = Expression;

while(isspace(*pExpresion)) ++pExpresion;      // delete white spaces
if(!isalpha(*pExpresion)) { ErrorCode = COMMAND::NO_FUNCTION;
		return false;}     // first is illigal sign

char *bucket;
bool finished=false;
bool OpenQuote=false;
bool IsEscape=false;
int Type=0;
TParameters TempParams[MAX_PARAM_COUNT];
NumberOfParams = 0;

bucket = token;
*bucket = '\0';
while (!finished)
	{
	while ((!isdelim(*pExpresion)) && (*pExpresion!='.'))  *bucket++ =*pExpresion++;
	if ((*pExpresion =='\r' ) || (*pExpresion =='\n') ||  (*pExpresion =='\0' )) finished = true;
	if (*pExpresion =='.' )
		 { *bucket = '\0'; CommandClass = (string)token; bucket = token; *bucket = '\0'; pExpresion++;}
	else
		 { *bucket = '\0'; CommandName = (string)token; bucket = token; *bucket = '\0'; finished =true;}
	}
finished = false;
if ((*pExpresion !='\r' ) &&  (*pExpresion !='\n') && (*pExpresion !='\0' )) pExpresion++;
bucket = token;
*bucket = '\0';
Type=0;
while (!finished) {
	while (!isdelim(*pExpresion)) *bucket++ =*pExpresion++;
	if ((*pExpresion =='\r' ) || (*pExpresion =='\n') || (*pExpresion =='\0' )) finished = true;
	if ((*pExpresion=='\"' ) && (OpenQuote==false))
		 { IsEscape=false; OpenQuote = true; pExpresion++; continue;}
	if ((*pExpresion!='\"' ) && (*pExpresion!='\\') && (OpenQuote==true))  { IsEscape=false; *bucket++ =*pExpresion++; continue;}
	if ((*pExpresion=='\"' ) && (OpenQuote==true))  { if (IsEscape==true) {IsEscape=false; *bucket++ =*pExpresion++; continue;}  Type = 1; OpenQuote=false; pExpresion++; continue;}
	if ((*pExpresion=='\\' ) && (OpenQuote==true))
		{ IsEscape=true; pExpresion++; continue;}
	*bucket = '\0';
	if ((!token) && (finished==true)) break;
	TempParams[NumberOfParams].Value = (string)token;
	if (Type==1) { TempParams[NumberOfParams].ParamType = Tokentypes::STRING;}
	else TempParams[NumberOfParams].ParamType = Tokentypes::NUMBER;
	pExpresion++;
	NumberOfParams++;
	bucket = token;
	*bucket = '\0';
	Type=0;
	if (NumberOfParams>=MAX_PARAM_COUNT) {ErrorCode = COMMAND::TOO_MANY_PARAMETERS; break;}
	}

if (NumberOfParams) Params = new TParameters[NumberOfParams];
for (int i = 0; i < NumberOfParams; i++)
	{
	if (TempParams[i].Value=="")
			TempParams[i].ParamType= Tokentypes::EMPTY;
	Params[i] = TempParams[i];
	}
  return !ErrorCode;
}
//-------------------------------------

int TCommand::isdelim(char c)
{
if(strchr(",", c) || c == 9 || c == '\r' || c == '\n' || c == '\"' || c=='\\' || c == 0)
return 1;
return 0;
}


