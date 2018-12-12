//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "DBAccess"
#pragma link "MyAccess"
#pragma link "MemDS"
#pragma resource "*.fmx"
TForm1 *Form1;


//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
Form1->FullScreen = true;
Form1->Top = 0;
Form1->Left=0;
PanelIsPressed = false;
InsertLog = false;
//Form1->Width = 3000;
//Form1->Height = 1030;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormCreate(TObject *Sender)
{
//IdFTP1->Connect();
//if (IdFTP1->Connected()) {
  //	IdFTP1->ChangeDir("test");
  //	TStream* str;
  //	IdFTP1->Put("autorun.msc","22.txt");
	//IdFTP1->Get("1.php",str);
 //	}

Panel1->Visible = false;


CMD = new TCommandQueue;
MC = new TMainControl(false,CMD);

TCommandQueue* UICommands = new TCommandQueue;
MC->UICommands = UICommands;
Images = new TImage*[255];
Labels = new TLabel*[255];
ItemLabels = new TLabel*[255];

MainLabel = new TLabel(Form1);
MotionsX = new TAxisMotion[255];
MotionsY = new TAxisMotion[255];
LastX = new double[255];
LastY = new double[255];
for (int i=0; i < 255; i++) {
	Images[i] = new TImage(Form1);
	Labels[i] = new TLabel(Form1);
	ItemLabels[i] = new TLabel(Form1);
	Images[i]->Parent = Form1;
	Labels[i]->Parent = Form1;
    ItemLabels[i]->Parent = Form1;
	MotionsX[i].SetImmediateTarget(0,0,false);
	MotionsY[i].SetImmediateTarget(0,0,false);
	}
MainLabel->Parent = Form1;
MC->Commands->CreateCommand("Script.LoadScript,\"autorun.msc\",0,1");

Image1->Bitmap->LoadFromFile("wall2.jpg");
Image1->Position->X = 0;
Image1->Position->Y = 0;

Image1->Size->Width = Form1->Width;
Image1->Size->Height = Form1->Height;

//for (int i=0; i < 100; i++) {
//  Images[i]->Bitmap->LoadFromFile("c:\\Quizone\\Quizes\\ThaiKings\\1.png");	
//}

//Image1->RotationAngle = 45;
//Image1->Scale->X = 0.5;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
TotalWidth = 1680; //3840;
TQuiz* Quiz = (TQuiz*)MC->QuizControl->GetObject();
for (int i=0; i < Quiz->NumItems; i++) {
		if (Quiz->Items[i].Reload) {
			string str = Quiz->Path;
			str = str + Quiz->Items[i].FileName;
		  //  Images[i]->Bitmap->Clear(0);
		 //	Images[i]->Bitmap->FreeHandle();
		//	Images[i]->Bitmap->Resize(100,100);
		try{
			Images[i]->Bitmap->LoadFromFile(str.c_str());
			Quiz->Items[i].Reload = false;
			}
			catch(...)
				{
					int t=1;
				}

			}
		}

static TCommand *Command=NULL;
int Code;
string Msg;

	while (MC->ReportCommands->GetResult(Command))
	 {
	if (Command->GetResult(&Code,&Msg))
		{
		if (Command->GetResult(&Code,&Msg))
			{
			if (InsertLog)
			{
			//if (Code!=COMMAND::SUCCESS) {
			//Memo1->Lines->Add(Msg.c_str());
			//}
			Memo2->Lines->Add(Msg.c_str());
			Memo2->Lines->Add(Command->GetCommandString().c_str());
			}
			}
		}
	 }

while (MC->UICommands->GetCommand(Command))
	{
	if (Command->GetCommandName()=="Msg") {
		if (Command->GetParamCount()>0) {
		  //	if (Command->Param(0)->IsNumber())
		  //	   ShowMessage((AnsiString)Command->Param(0)->GetAsNumber());
		  //	else
			   ShowMessage((AnsiString)Command->Param(0)->GetAsString().c_str());
			Command->InsertResult(COMMAND::SUCCESS,COMMAND::CommandErrorString[COMMAND::SUCCESS]);
			}
		else
		Command->InsertResult(COMMAND::TOO_FEW_PARAMETERS,COMMAND::CommandErrorString[COMMAND::TOO_FEW_PARAMETERS]);
		}
	}



for (int i=Quiz->NumItems; i < 255; i++) {
	Images[i]->Opacity = 0; // Bitmap->Clear(0);
	Labels[i]->Opacity = 0;
	ItemLabels[i]->Opacity = 0;
	}

for (int i=0; i < Quiz->NumItems; i++) {

	double SpeedX = MotionsX[i].GetSpeed(Timer1->Interval/1000.0);
	double SpeedY = MotionsY[i].GetSpeed(Timer1->Interval/1000.0);

	LastX[i] = MotionsX[i].GetPosition(Timer1->Interval/1000.0);
	LastY[i] = MotionsY[i].GetPosition(Timer1->Interval/1000.0);

	MotionsX[i].SetTargetDuration(LastX[i],0,Quiz->Items[i].PosX,SpeedX,Quiz->TransitionTime,Quiz->Acc,false);
	MotionsY[i].SetTargetDuration(LastY[i],0,Quiz->Items[i].PosY,SpeedY,Quiz->TransitionTime,Quiz->Acc,false);

	double X = MotionsX[i].GetPosition(Timer1->Interval/1000.0);
	double Y = MotionsY[i].GetPosition(Timer1->Interval/1000.0);


	Images[i]->Position->X = X * (TotalWidth-150);
	Images[i]->Position->Y = Y * Form1->Height;

	Images[i]->Opacity = Quiz->Items[i].Image.Opacity.GetCurrent();

//	if (Images[i]->Position->X <400) Images[i]->Position->X += 400;

	//if (Images[i]->Position->X > TotalWidth-250) Images[i]->Opacity = (TotalWidth- Images[i]->Position->X -150) /100.0;
	//else if (Images[i]->Position->X < 100)
	//	Images[i]->Opacity = (Images[i]->Position->X) /100.0;
	//else Images[i]->Opacity = 1;

	//if (Quiz->Items[i].PosX * (TotalWidth-150) > TotalWidth-250) Images[i]->Opacity = (TotalWidth- Quiz->Items[i].PosX * (TotalWidth-150) -150) /100.0;
	//else if (Quiz->Items[i].PosX * (TotalWidth-150) < 400) Images[i]->Opacity = 0;
	//	Images[i]->Opacity = (Quiz->Items[i].PosX)* (TotalWidth-150) /100.0;
	//else Images[i]->Opacity = 1;

	//Images[i]->Opacity = Images[i]->Opacity * Quiz->Items[i].Opacity;

	//Images[i]->Scale->X = Images[i]->Scale->X * Quiz->Items[i].Scale;
	//Images[i]->Scale->Y = Images[i]->Scale->Y * Quiz->Items[i].Scale;

	Images[i]->Position->Y = Y * Form1->Height;
	Images[i]->Scale->X = Quiz->Items[i].SizeX * Quiz->Items[i].Scale;
	Images[i]->Scale->Y = Quiz->Items[i].SizeY * Quiz->Items[i].Scale;

	Labels[i]->Position->X = Images[i]->Position->X + (Images[i]->Width*Images[i]->Scale->X) /2.0;
	Labels[i]->Position->Y = Images[i]->Position->Y + (Images[i]->Height*Images[i]->Scale->Y)/3.0;

	ItemLabels[i]->Position->X = Images[i]->Position->X + Quiz->Items[i].Label.PosX.GetCurrent();
	ItemLabels[i]->Position->Y = Images[i]->Position->Y + Quiz->Items[i].Label.PosY.GetCurrent();

	Labels[i]->Scale->X = 1.5* Images[i]->Scale->X;
	Labels[i]->Scale->Y = 1.5* Images[i]->Scale->Y;

	ItemLabels[i]->Scale->X = Quiz->Items[i].Label.ScaleX.GetCurrent();
	ItemLabels[i]->Scale->Y = Quiz->Items[i].Label.ScaleY.GetCurrent();

	ItemLabels[i]->Text = Quiz->Items[i].Label.Caption.c_str();

	//Label1->StyledSettings = Label1->StyledSettings << TStyledSetting::FontColor;
	//Label1->FontColor = claCrimson;
	//Label1->StyledSettings-> Settings->StyledSettings = Settings->StyledSettings >> TStyledSetting::ssFontColor;
	//Label1->TextSettings->FontColor = 0xff5511;


	Labels[i]->StyledSettings = Labels[i]->StyledSettings >> TStyledSetting::ssFontColor;
	Labels[i]->TextSettings->FontColor = 0xffee8822;

	ItemLabels[i]->StyledSettings = ItemLabels[i]->StyledSettings >> TStyledSetting::ssFontColor;
	ItemLabels[i]->TextSettings->FontColor = Quiz->Items[i].Label.Color;

	//if (Quiz->Items[i].Label.Color.c_str()!="") ItemLabels[i]->TextSettings->FontColor = StrToInt(Quiz->Items[i].Label.Color.c_str());
	//ItemLabels[i]->TextSettings->FontColor = 0xffee8822;
	ItemLabels[i]->Opacity = Quiz->Items[i].Label.Opacity.GetCurrent();

	//Labels[i]->FontColor = 0xaa5511;
	Labels[i]->Text = i+1;

	if (Quiz->Items[i].AddNumber == true) {
					 Labels[i]->Opacity = 0.8 * Images[i]->Opacity ;
		}
	else
		{
			Labels[i]->Opacity = 0.0;
		}
MainLabel->Opacity = Quiz->LabelOpacity;
MainLabel->Text = Quiz->Label.c_str();
MainLabel->Scale->X = 10;
MainLabel->Scale->Y = 10;
MainLabel->StyledSettings = MainLabel->StyledSettings >> TStyledSetting::ssFontColor;
MainLabel->TextSettings->FontColor = 0xffffffff;
//int t =MainLabel->Canvas->Width;
 //t =MainLabel->Canvas->TextWidth();
int t1 =MainLabel->Canvas->TextWidth(MainLabel->Text);
int t2 =MainLabel->Canvas->TextHeight(MainLabel->Text);

MainLabel->Position->X = Form1->Width/2 - t1*10/2;
MainLabel->Position->Y = Form1->Height/2 - t2*10/2;


//ItemLabels[i]->Position->X = 500;
//ItemLabels[i]->Position->Y = 100;
//ItemLabels[1]->Scale->X = 10;//ItemLabels[i]->Scale->X; //1 * Quiz->Items[i].Label.ScaleX.GetCurrent(); ;//ItemLabels[i]->Scale->X;
//ItemLabels[1]->Scale->Y = 10;//ItemLabels[i]->Scale->Y; //1;//ItemLabels[i]->Scale->X; //10;//ItemLabels[i]->Scale->X;
//ItemLabels[i]->Scale->X = 10;
//ItemLabels[1]->Opacity = 1;
//ItemLabels[1]->Text = "asdfadsf";
//ItemLabels[i]->TextSettings->FontColor = 0xffee8822;




	}
}

//---------------------------------------------------------------------------

void __fastcall TForm1::FormKeyDown(TObject *Sender, WORD &Key, System::WideChar &KeyChar,
		  TShiftState Shift)
{
static double SizeX = 1;
static int ScriptId = 1;
if (Key==39) {
	MC->Commands->CreateCommand("Script.StopScript,RotateQuizId");
	MC->Commands->CreateCommand("Script.RunScript,RotateQuizId");




  //	ScriptId++;
  //	char str[255];
  //	sprintf(str,"Script.RunScript,%d",ScriptId);
  //	MC->Commands->CreateCommand(str);
}
if (Key==37) {
  //	ScriptId--;
  //	char str[255];
  //	sprintf(str,"Script.RunScript,%d",ScriptId);
  //	MC->Commands->CreateCommand(str);
}

if (KeyChar=='a')
	{
	SizeX = SizeX + 0.1;
	char str[255];
	sprintf(str,"LoadImage,0,\"Egypt.png\",%f,%f",SizeX,SizeX);
	MC->QuizCommands->CreateCommand(str);
	}
if (KeyChar=='q')
	{
	SizeX = SizeX - 0.1;
	char str[255];
	sprintf(str,"LoadImage,0,\"Egypt.png\",%f,%f",SizeX,SizeX);
	MC->QuizCommands->CreateCommand(str);
	}
if (KeyChar=='r')
	{
	MC->QuizCommands->CreateCommand("Quiz.Randomize");
	}
if (Key==27) // escape
	{
	MC->QuizControl->Stop();
	MC->QueryControl->Stop();
	MC->ScriptControl->Stop();
	MC->Stop();
	Sleep(2500);
	Form1->Close();
	Application-> Terminate();
	}


}

//---------------------------------------------------------------------------



void __fastcall TForm1::IdUDPServer1UDPRead(TIdUDPListenerThread *AThread, const TIdBytes AData,
          TIdSocketHandle *ABinding)
{
ProcessString(AData,COMMAND::SOURCE_UDP);
}
//---------------------------------------------------------------------------

void TForm1::ProcessString(TIdBytes AData, int Source)
{
string Str;
bool IsHeader = false;
int i=0;
int CommandStart;
int CommandEnd;
while (i < AData.get_length()-1)
	{
	Str = "";
	CommandStart=i;
	CommandEnd = i-1;
	if ((AData[i]==0x02) && (AData[i+1]==',')) {
		IsHeader=true;
		CommandStart = i+2;
		int j=CommandStart;
		while (j <AData.get_length()-1) {
			if ((AData[j]==0x03) && (AData[j+1]==0x00)) {
				CommandEnd = j;
				break;
				}
			j++;
			}
		if (CommandEnd<CommandStart) {
				return;		// wrong tailer
			}
		}
	else   {
		IsHeader = false;
		}
		if (IsHeader==true) {
			for (int k=CommandStart;k<CommandEnd;k++)
				Str = Str + char(AData[k]);
			}
		else{
			for (int k=i;k<AData.get_length();k++)
				Str = Str + char(AData[k]);
			}

		CMD->CreateCommand(Str,0,COMMAND::SOURCE_UDP);
		if (IsHeader==true)
			i=i+2+CommandEnd;
		else
			break;
	}

}

void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
Application-> Terminate();
return;

	MC->QuizControl->Stop();
	MC->QueryControl->Stop();
	MC->ScriptControl->Stop();
	MC->Stop();
	Sleep(1000);
	Form1->Close();
	Application-> Terminate();
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Image1DblClick(TObject *Sender)
{
Panel1->Visible = true;
Panel1->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Image1Click(TObject *Sender)
{
Panel1->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Panel1MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
		  float X, float Y)
{
PanelIsPressed = true;
PanelStartX = X;
PanelStartY = Y;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Panel1MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
		  float X, float Y)
{
PanelIsPressed = false;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Panel1MouseMove(TObject *Sender, TShiftState Shift, float X,
		  float Y)
{
if (PanelIsPressed) {
	Panel1->Position->X = Panel1->Position->X - (PanelStartX -X);
	Panel1->Position->Y = Panel1->Position->Y - (PanelStartY -Y);
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Image1MouseMove(TObject *Sender, TShiftState Shift, float X,
		  float Y)
{
if (PanelIsPressed) {
	Panel1->Position->X = (X - PanelStartX );
	Panel1->Position->Y = (Y - PanelStartY);
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Image1MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          float X, float Y)
{
PanelIsPressed = false;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::CheckBox3Change(TObject *Sender)
{
InsertLog = CheckBox3->IsChecked;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Edit1KeyDown(TObject *Sender, WORD &Key, System::WideChar &KeyChar,
		  TShiftState Shift)
{
if (Key==13) {
	//char str[255];
	//sprintf(str,"Script.RunScript,%d",ScriptId);
   //	UnicodeString stt = Edit1->Text;
   //stt.
   //	AnsiString str = (AnsiString)stt;
	//string strr = stt. .c_str();
	MC->Commands->CreateCommand((AnsiString(Edit1->Text)).c_str());
	}
}
//---------------------------------------------------------------------------

