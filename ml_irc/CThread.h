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

#ifndef __CTHREAD_H
#define __CTHREAD_H

#ifdef WIN32			// Win32 threads
	#define _WIN32_WINNT 0x400
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>

	typedef HANDLE				ThreadHandle;
	typedef CRITICAL_SECTION	ThreadMutex;
#else					// POSIX threads
	#include <stdio.h>
	#include <pthread.h>

	typedef pthread_t			ThreadHandle;
	typedef pthread_mutex_t		ThreadMutex;
#endif

#include "CThreadData.h"

// Base thread class
class CThread
{
public:
                        CThread         ( void );
				virtual	~CThread        ( void );

    bool                Start           ( CThreadData *pData );
    void                Stop            ( void );

	static bool			TryLock			( ThreadMutex * Mutex );
	static void			Lock			( ThreadMutex * Mutex );
	static void			Unlock			( ThreadMutex * Mutex );

protected:
    int                 Run             ( CThreadData* arg );

    virtual int			Execute         ( CThreadData* pData ) = 0;

    CThreadData*		Arg             ( void ) const;
    void				Arg             ( CThreadData* pData );

#ifdef WIN32	// Win32 threads
    static DWORD WINAPI EntryPoint      ( void* );
#else			// POSIX threads
	static void*		EntryPoint		( void* );
#endif

private:
    void*               m_pArg;
	CThreadData*		m_pThreadData;
	ThreadHandle		m_hThread;
};

#endif
