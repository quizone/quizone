//---------------------------------------------------------------------------

#ifndef QueryControlH
#define QueryControlH
//---------------------------------------------------------------------------

#include "CommandExecuter.h"

#include "MySqlModule.h"

//---------------------------------------------------------------------------

class TQuery
{
public:
TQuery();
string host;
string username;
string password;
string query;
};

//---------------------------------------------------------------------------

class TQueryControl : public TCommandExecuter
{
private:


public:
__fastcall TQueryControl(bool CreateSuspended,TCommandQueue* Commands,TQuery *Query);
__fastcall ~TQueryControl();
};

#endif
