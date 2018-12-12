//---------------------------------------------------------------------------

#pragma hdrstop

#include "commandqueue.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)


TCommandQueue::TCommandQueue(int Length)
{
CQes = InitializeCriticalSection();
TCriticalSectionOwner CriticalSectionOwner(CQes);
MaxQueueLength=MAX_COMMANDS_QUEUE;
 head = 0;
 tail = 0;
 resultTail = 0;
 processTail = 0;

if ((Length<MaxQueueLength) && (Length>1))
	 MaxQueueLength = Length;
 MaxQueueLength = MAX_COMMANDS_QUEUE;
 Commands = new TCommand*[MaxQueueLength];
 for (int i=0; i < MaxQueueLength; i++) {
	Commands[i] = NULL;
	}
 ClearQueue();
}

//---------------------------------------------------------------------------

TCommandQueue::~TCommandQueue()
{
delete [] Commands;
DestroyCriticalSection(CQes);
}

//---------------------------------------------------------------------------

void TCommandQueue::ClearQueue()
{
TCriticalSectionOwner CriticalSectionOwner(CQes);
int id=tail;
while (id<head)
	{
	id = increase(id);
	if (Commands[id]) Commands[id]->DecreaseRef();
	}
 head = 0;
 tail = 0;
 resultTail = 0;
 processTail = 0;
}

//---------------------------------------------------------------------------

void TCommandQueue::RestartExecution()
{
TCriticalSectionOwner CriticalSectionOwner(CQes);
//ClearResults();
processTail = tail;
//resultTail = tail;
}

//---------------------------------------------------------------------------

void TCommandQueue::ClearResults()
{
TCriticalSectionOwner CriticalSectionOwner(CQes);
int id=tail;
while (id<head)
	{
	id = increase(id);
	if (Commands[id]) Commands[id]->RemoveResult();
	}
return;
}

//---------------------------------------------------------------------------

int TCommandQueue::increase(int index)
{
TCriticalSectionOwner CriticalSectionOwner(CQes);
	index++;
	if (index>=MaxQueueLength)
	   index  = 0;
  return index;
}

//---------------------------------------------------------------------------

int TCommandQueue::decrease(int index)
{
TCriticalSectionOwner CriticalSectionOwner(CQes);
	index--;
	if (index>=MaxQueueLength)
	   index  = 0;
 return index;
}

//---------------------------------------------------------------------------

void TCommandQueue::AddCommand()
{
TCriticalSectionOwner CriticalSectionOwner(CQes);
t=0;
head = increase (head);
if (tail==head)
		{
		if (Commands[tail])
			Commands[tail]->DecreaseRef(); // loosing the very last command
		tail=increase(tail);
		}
	if (resultTail==head)  resultTail=increase(resultTail);  // loosing the very last result
	if (processTail==head)
			processTail=increase(processTail);  // loosing the very last processing command leaving it unprocessed
}
//---------------------------------------------------------------------------

bool TCommandQueue::CreateCommand(string CommandString, int Source, unsigned long mmDelay)
{
TCriticalSectionOwner CriticalSectionOwner(CQes);
AddCommand();
Commands[head] = new TCommand(CommandString,Source,mmDelay);
  if (Commands[head]->GetResult()) {
	return false;
	}
return true;
}

//---------------------------------------------------------------------------

bool TCommandQueue::ForwardCommand(TCommand *Command)
{
TCriticalSectionOwner CriticalSectionOwner(CQes);
t=0;
AddCommand();
Commands[head] = Command;
Command->IncrementRef();
return true;
}

//---------------------------------------------------------------------------

bool TCommandQueue::GetCommand(TCommand* &Command,unsigned long TimeSinceStart)
{
TCriticalSectionOwner CriticalSectionOwner(CQes);
if (processTail==head) return false;
processTail = increase(processTail);
if (Commands[processTail])
	{
	if (TimeSinceStart >Commands[processTail]->GetDelay())
		{
		Command = Commands[processTail];
		return true;
		}
	processTail = decrease(processTail);
	return false;
	}
return false;
}

//---------------------------------------------------------------------------

bool TCommandQueue::GetResult(TCommand* &Command)
{
TCriticalSectionOwner CriticalSectionOwner(CQes);
t=0;
if (resultTail==head) return false;
resultTail = increase(resultTail);
if (Commands[resultTail])
	{
	if (Commands[resultTail]->GetResult())
		{
		Command = Commands[resultTail];
		return true;
		}
	resultTail = decrease(resultTail);
	return false;
	}
return false;
}

//---------------------------------------------------------------------------


