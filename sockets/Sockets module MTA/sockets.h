#ifndef __MTA_SOCK_H
#define __MTA_SOCK_H

#define WIN32_MEAN_AND_LEAN

#include <process.h>
#include <winsock2.h>
#include <windows.h>
#include <string>

#include "../Module SDK/ml_base.h"

using namespace std;

class Socket {
public:
	Socket(string host, unsigned short port);
	~Socket();

	bool isConnected();

private:
	SOCKET      m_sock;
	SOCKADDR_IN m_addr;

	bool m_connected;

	bool Socket::VerifyIP(sockaddr_in* sockAddr, string host);
};

#endif