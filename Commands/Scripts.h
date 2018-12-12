//---------------------------------------------------------------------------
#ifndef ScriptsH
#define ScriptsH
//---------------------------------------------------------------------------

#include "CommandExecuter.h"

#define MAX_SCRIPT 30

class TScripts : public TCommandExecuter
{
private:
string FileName;
int Slot;
bool f_IsLoaded;
bool HandleCommand(TCommand* Command);

public:
TCommandExecuter* Reciever;
static int ScriptCount;
__fastcall TScripts (bool CreateSuspended,TCommandQueue* Commands);
string GetFileName() {return FileName;};
bool IsLoaded () {return f_IsLoaded;};
void SetFileName(string str) {FileName = str; f_IsLoaded=true; };
void SetSlot(int Slot) {this->Slot=Slot;};
int GetSlot() {return Slot;};
bool ForwardCommand(string CommandLine);
bool PreCommandHandler(TCommand * Command);
};

#endif
