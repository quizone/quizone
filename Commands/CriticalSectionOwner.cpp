//---------------------------------------------------------------------------

#pragma hdrstop

#include "CriticalSectionOwner.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

PrivateCriticalSection InitializeCriticalSection()
{
PrivateCriticalSection mutex;

/*
#ifdef __ANDROID_API__
int res = pthread_mutex_init(&mutex, 0);
#endif

#ifdef _WIN32
*/
mutex = new TCriticalSection;
//#endif

return mutex;
}

//----------------

void DestroyCriticalSection(PrivateCriticalSection mutex)
{
/*
#ifdef __ANDROID_API__
pthread_mutex_destroy(&mutex);
#endif

#ifdef _WIN32
*/
if (mutex) delete mutex;
//#endif
}

TCriticalSectionOwner::TCriticalSectionOwner(PrivateCriticalSection mutex)
{
	mutexlock = mutex;
	Lock();
}

//-----------------

TCriticalSectionOwner::~TCriticalSectionOwner()
{
	Unlock();
}

//-----------------

void TCriticalSectionOwner::Lock()
{
/*
	#ifdef __ANDROID_API__
	if (pthread_mutex_lock(&mutexlock))
		{
		int i=0;
		}
	#endif

	#ifdef WIN32
*/
	mutexlock->Acquire();
//	#endif

}

//-----------------

void TCriticalSectionOwner::Unlock()
{
/*
	#ifdef __ANDROID_API__
	pthread_mutex_unlock(&mutexlock);
	#endif

	#ifdef WIN32
*/
	mutexlock->Release();
//	#endif
}
