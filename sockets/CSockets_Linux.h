/****************************************************************
*        Multi Theft Auto : San Andreas - Socket module.
*        Info: Linux Sockets
*        Developers: x86 <sebasdevelopment@gmx.com>
****************************************************************/

#ifndef WIN32

#ifndef __MTA_SOCK_H
#define __MTA_SOCK_H

#include "ml_base.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <netdb.h>
#include <cstdlib>
#include <sys/select.h>

using namespace std;

#define SOCK_RECV_LIMIT 16384

class Socket {
public:
//	Socket            ();
	Socket            (lua_State *luaVM, string host, unsigned short port);
	~Socket           ();

	bool isConnected  ();
    bool isConnecting ();
	bool sendData     (string data);

	void* getUserdata ();

    void doPulse      ();

private:
    pthread_t          m_thread;
	int                m_sock;
    struct sockaddr_in m_addr;

	void* m_userdata;

	bool m_connected;
	bool m_connecting;

	bool Socket::VerifyIP(sockaddr_in* sockAddr, string host);
};

#endif

#endif
