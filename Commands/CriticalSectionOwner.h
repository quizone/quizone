//---------------------------------------------------------------------------

#ifndef CriticalSectionOwnerH
#define CriticalSectionOwnerH
//---------------------------------------------------------------------------

/*
#ifndef _WIN32
#include <pthread.h>
typedef pthread_mutex_t PrivateCriticalSection;
#endif


#ifdef _WIN32
*/

#include <System.SyncObjs.hpp>

typedef TCriticalSection* PrivateCriticalSection;
//#endif

//----------------

PrivateCriticalSection InitializeCriticalSection();
void DestroyCriticalSection(PrivateCriticalSection mutex);

//----------------

class TCriticalSectionOwner
{
private:
	PrivateCriticalSection mutexlock;
	void Lock();
	void Unlock();

public:
	TCriticalSectionOwner(PrivateCriticalSection mutexlock);
	virtual ~TCriticalSectionOwner();


};

#endif
