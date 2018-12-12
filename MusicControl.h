//---------------------------------------------------------------------------

#ifndef MusicControlH
#define MusicControlH
//---------------------------------------------------------------------------

#include "CommandExecuter.h"

#include "MySqlModule.h"

//---------------------------------------------------------------------------

class TMusic
{
public:
TMusic();
string filename;
string path;
int id;
};

//---------------------------------------------------------------------------

class TMusicControl : public TCommandExecuter
{
private:


public:
__fastcall TMusicControl(bool CreateSuspended,TCommandQueue* Commands,TMusic *Music);
__fastcall ~TMusicControl();
};


#endif
