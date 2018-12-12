//---------------------------------------------------------------------------
#ifndef commandqueueH
#define commandqueueH
//---------------------------------------------------------------------------

#include <system.hpp>
//#include <SysUtils.hpp>
#include "Command.h"
#include <iostream>
#include <sstream>
#include <string>
#include "CriticalSectionOwner.h"

#define MAX_COMMANDS_QUEUE 300

class TCommandQueue
{
	private:
		PrivateCriticalSection CQes;
		TCommand** Commands;
		int head;
		int tail;
		int resultTail;
		int processTail;

		int increase(int index);
		int decrease(int index);
		int MaxQueueLength;

	   void ClearResults();
	   void AddCommand();

	   int t;
	public:
		TCommandQueue(int Length=MAX_COMMANDS_QUEUE);
		~TCommandQueue();
		bool CreateCommand(string CommandString, int Source=0, unsigned long mmDelay = 0 );
		bool ForwardCommand(TCommand* Command);

		bool GetCommand(TCommand* &Command,unsigned long TimeSinceStart = 0xffffffff);   // Get the next command in the queue. return true if success.  TimeSinceStar in ms, applies when Delay is filled.
		bool GetResult(TCommand* &Command);                     // return true if a newly resulted command exists, otherwise false
		void ClearQueue();
		void RestartExecution();

   //		friend ostream& operator<<(ostream& os, const TCommandQueue& cq);
	  //	friend istream& operator>>(istream& os, TCommandQueue& cq);
	//	friend istream& operator>>(istream& os, TCommandQueue& cq);


		//friend TCommandQueue& operator<<(ostream& os, const TCommandQueue& cq);
};

 /*
ostream& operator<<(ostream& os, const TCommandQueue& cq)
{
   //	os << cq
   //	os << dt.mo << '/' << dt.da << '/' << dt.yr;
	return os;
}

istream& operator>>(istream& is, TCommandQueue& cq)
{
 //	int buff_size = 255;
 //	char *line = new char [buff_size];
 //	std::istringstream iss;
 //	while (is.getline(line, buff_size) != NULL)
		{
 //		cq.AddCommand(string(line));
	   //	iss << line;
	   //	std::istringstream iss(line);
	//iss >> verts[i].x >> verts[i].y >> verts[i].z;
		}
	return is;
}
*/

#endif


