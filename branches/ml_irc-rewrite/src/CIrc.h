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

class CIrc;

#ifndef __CIRC_H
#define __CIRC_H

#include <stdio.h>
#include <string>
#ifdef WIN32
#include <process.h>
#include <winsock.h>
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <netdb.h>
#include <cstdlib>
#include <sys/select.h>
#endif

// ---------------------------------------------------------------
class CIrc
{
	public:
#ifdef WIN32
		static int		startWinSocket      ();
#endif
		static long		GetAddr             (std::string hostname);
		static bool		connectToIRC        (std::string server, int port, std::string nickname);
		static int		CloseSocket         ();
#ifdef WIN32
		static DWORD WINAPI		messageThread       (void* x);
#else
		static void*	messageThread	    (void* x);
#endif
		static int		onDataReceived      (char* msg);
		static int		sendRaw				(std::string text);
		static int		search				(char* string, char* substring);

// ---------------------------------------------------------------
	//private:

};
#endif
