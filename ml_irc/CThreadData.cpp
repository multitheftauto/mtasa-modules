/*********************************************************
*
*  Multi Theft Auto v1.0 - Module
*  
*  Module:       ml_irc
*  Url:          http://development.mtasa.com/index.php?title=Modules/SebasIRC
*  Project page: http://multitheftauto-modules.googlecode.com/
*  Developers:   Sebas Lamers <sebasdevelopment@gmx.com>
*
*********************************************************/

#include "CThreadData.h"

CThreadData::CThreadData ( void )
{
	bAbortThread = false;

	// Initialize the mutexes
	#ifdef WIN32	// Win32 threads
		InitializeCriticalSection ( &MutexPrimary );
		InitializeCriticalSection ( &MutexLogical );
	#else			// POSIX threads
		pthread_mutex_init ( &MutexPrimary, NULL );
		pthread_mutex_init ( &MutexLogical, NULL );
	#endif
}

CThreadData::~CThreadData ( void )
{
	#ifdef WIN32
		DeleteCriticalSection ( &MutexPrimary );
		DeleteCriticalSection ( &MutexLogical );
	#else
	#endif
}
