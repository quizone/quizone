//---------------------------------------------------------------------------

#ifndef MySqlModuleH
#define MySqlModuleH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include "DBAccess.hpp"
#include "MemDS.hpp"
#include "MyAccess.hpp"
#include <Data.DB.hpp>
#include <FMX.Media.hpp>
#include <FMX.Types.hpp>
#include <System.Net.HttpClient.hpp>
#include <System.Net.HttpClientComponent.hpp>
#include <System.Net.URLClient.hpp>
//---------------------------------------------------------------------------
class TDataModule2 : public TDataModule
{
__published:	// IDE-managed Components
	TMyCommand *MyCommand1;
	TMyQuery *MyQuery1;
	TMyTable *MyTable1;
	TMediaPlayer *MediaPlayer1;
	TMediaPlayer *MediaPlayer2;
	TNetHTTPClient *NetHTTPClient1;
	void __fastcall NetHTTPClient1RequestError(TObject * const Sender, const UnicodeString AError);

private:	// User declarations
public:		// User declarations
	__fastcall TDataModule2(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TDataModule2 *DataModule2;
//---------------------------------------------------------------------------
#endif
