/****************************************************************
*        Multi Theft Auto : San Andreas - Socket module.
*        Info: Windows Sockets
*        Developers: Gamesnert
****************************************************************/

#ifdef WIN32

#ifndef __MTA_SOCK_H
#define __MTA_SOCK_H

//#include "CFunctions.h"
#include "ml_base.h"

#define WIN32_MEAN_AND_LEAN

#include <process.h>
#include <winsock2.h>
#include <windows.h>
#include <string>
#include <vector>

using namespace std;

#define SOCK_RECV_LIMIT 16384

class Socket {
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
    HANDLE             m_thread;
	SOCKET             m_sock;
	SOCKADDR_IN        m_addr;

	void* m_userdata;

	bool m_connected;
	bool m_connecting;
};

#endif

#endif
