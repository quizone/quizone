//---------------------------------------------------------------------------

#ifndef ScriptControlH
#define ScriptControlH
//---------------------------------------------------------------------------


#include "Scripts.h"

//---------------------------------------------------------------------------

class TScriptControl : public TCommandExecuter
{
private:
public:
__fastcall TScriptControl(bool CreateSuspended,TCommandQueue* Commands);
__fastcall TScriptControl::~TScriptControl();
bool InitializeScripts(int NumberOfScripts,TCommandExecuter* Reciever);
};

//---------------------------------------------------------------------------
#endif
