/****************************************************************
*        Multi Theft Auto : San Andreas - Socket module.
*        Info: Win32/Linux Sockets
*        Developers: Gamesnert & x86
****************************************************************/

#ifndef __MTA_SOCK_H
#define __MTA_SOCK_H

//#include "CFunctions.h"
#include "ml_base.h"

#ifdef WIN32
	#define WIN32_MEAN_AND_LEAN

	#include <process.h>
	#include <winsock2.h>
	#include <windows.h>
	#include <string>
	#include <vector>
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

using namespace std;

#define SOCK_RECV_LIMIT 16384

class Socket
{
public:
	Socket            (lua_State *luaVM, string host, unsigned short port);
	~Socket           ();

	bool isConnected  ();
    bool isConnecting ();
	bool sendData     (string data);
	bool VerifyIP     (string host);

	void* getUserdata ();

    void doPulse      ();

private:
#ifdef WIN32
    HANDLE             m_thread;
	SOCKET             m_sock;
	SOCKADDR_IN        m_addr;
#else
    pthread_t          m_thread;
	int                m_sock;
    struct sockaddr_in m_addr;
#endif

	void* m_userdata;

	bool m_connected;
	bool m_connecting;
};

#endif
