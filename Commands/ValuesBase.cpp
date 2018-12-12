//---------------------------------------------------------------------------

#pragma hdrstop

#include "ValuesBase.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

#define UpdateMap(MapName,ParamName) MapName [CommandCode]=ParamName;

TValues::TValues()
{
TheMap = new StringMaps;
}

//---------------------------------------------------------------------------

TValues::~TValues()
{
delete TheMap;
}

//---------------------------------------------------------------------------

bool TValues::WriteToFile(char* filename)
{
FILE *fp = fopen(filename,"w");
if (fp==0)
		{
		return false;
		}
StringMaps::const_iterator iter = TheMap->begin();
while (iter != TheMap->end())
	{
	string str1 = iter->first;
	string str2 = iter->second;
	string str = str1 + " " + str2 + "\n";
	fputs(str.c_str(),fp);
	iter++;
	}
fclose (fp);
}

//---------------------------------------------------------------------------

bool TValues::ReadFromFile(char* filename)
{
FILE *fp = fopen(filename,"rt");
if (fp==0)
		{
		return false;
		}
int length = 255;
char str[260];
while (fgets(str,length+1,fp))
{
	   char CommandCode[256];
	   char Param[256];
	   CommandCode[0] = '\0';
	   int pos = 0;
// Get command code
	   int i=0;
	   while ((str[i]==' ') || (str[i]==',') || (str[i]=='=') || (str[i]==9) && (i<length))
			i++; // remove first space
	   while ((str[i]!=' ') && (str[i]!='\n') && (str[i]!=',') && (str[i]!='=') && (str[i]!=9) && (i<length))
		{
		 CommandCode[pos] = toupper(str[i]);
		 CommandCode[pos+1] = '\0';
		 i++;
		 pos++;
		}
if (CommandCode[0] == '#') continue;     // comment
if (CommandCode[0] == 0) continue;
  // Parameter

		 pos = 0;
		 Param[0] = '\0';
		 while ((str[i]==' ') || (str[i]==',') || (str[i]=='=') || (str[i]==9) && (i<length))
				i++; // remove spaces
		 while ((str[i]!=',') && (str[i]!='\n') && (str[i]!='\0') && (str[i]!='=') && (str[i]!=9) && (i<length))
				{
				 Param[pos] = str[i];
				 Param[pos+1] = '\0';
				 i++;
				 pos++;
				}
WriteAsString(CommandCode,Param);
}
fclose(fp);
return true;
}

//---------------------------------------------------------------------------

bool TValues::IsExist(char* VarName)
{
StringMaps::const_iterator iter = TheMap->find(VarName);
	return !(iter == TheMap->end());
}

//---------------------------------------------------------------------------

std::string TValues::ReadAsString(char* VarName)
{
	StringMaps::const_iterator iter = TheMap->find(VarName);
	if (iter == TheMap->end())
		{
		return "";
		}
	else
		return (std::string)iter->second;
}

//---------------------------------------------------------------------------

double TValues::ReadAsNumber(char* VarName)
{
   return atof(ReadAsString(VarName).c_str());
}

//---------------------------------------------------------------------------

bool TValues::WriteAsString(char* VarName, std::string Value)
{
	StringMaps::const_iterator iter = TheMap->find(VarName);
	if (iter == TheMap->end())
		return false;
	else
		{
		TheMap->operator [](VarName) = Value;
		return true;
		}
}

//---------------------------------------------------------------------------

bool TValues::WriteAsNumber(char* VarName, double Value)
{
	StringMaps::const_iterator iter = TheMap->find(VarName);
	if (iter == TheMap->end())
		return false;
	else
		{
		std::ostringstream strs;
		strs << Value;
		std::string str = strs.str();
		TheMap->operator [](VarName) = str;
		return true;
		}
}


