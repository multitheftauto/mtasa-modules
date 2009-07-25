/*********************************************************
*
*  Multi Theft Auto v1.0 - Module
*  
*  Module:       ml_irc
*  Url:          http://development.mtasa.com/index.php?title=Modules/SebasIRC
*  Project page: http://multitheftauto-modules.googlecode.com/
*  Developers:   Sebas Lamers <baslamers@home.nl>
*
*********************************************************/

class CThreadData;

#ifndef __CTHREADDATA_H
#define __CTHREADDATA_H

#include "CThread.h"

class CThreadData
{
public:
						CThreadData			( void );
						~CThreadData		( void );

	bool				bAbortThread;
	ThreadMutex			MutexPrimary;		// primary mutex for suspend/resume operations
	ThreadMutex			MutexLogical;		// logical mutex for proper CThreadData sync
};

#endif
