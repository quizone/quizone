//---------------------------------------------------------------------------
#ifndef ValuesBaseH
#define ValuesBaseH
//---------------------------------------------------------------------------

#include <iostream>    // should be at the very top
#include <sstream>
#include <fstream>

#include <string>   // should be included along map
#include <map>
#include <stdlib.h>
#include <stdio.h>

using namespace std;
typedef std::map<std::string,std::string> StringMaps;

class TValues{   // This is an abstract class. Make inherit classes

protected:
StringMaps *TheMap;

public:
TValues();
~TValues();
virtual bool ReadFromFile(char* filename);
virtual bool WriteToFile(char* filename);
bool IsExist(char* VarName);
std::string ReadAsString(char* VarName);
double ReadAsNumber(char* VarName);
bool WriteAsString(char* VarName, std::string Value);
bool WriteAsNumber(char* VarName, double Value);
};

//---------------------------------------------------------------------------
#endif
