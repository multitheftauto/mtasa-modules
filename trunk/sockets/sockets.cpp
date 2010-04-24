#include "sockets.h"

Socket::Socket()
{
	CFunctions::debugPrint("Bummer, I don't want this... :(\n");
}

Socket::Socket(lua_State *luaVM, string host, unsigned short port)
{
//	CFunctions::debugPrint("Creating data...\n");
//	m_threadData = new CThreadData;
//	CFunctions::debugPrint("Created data...\n");
//	m_thread->Start(m_threadData);
//	CFunctions::debugPrint("Started thread...\n");

	m_connected = false;

	sockaddr_in sockAddr = {0};
	
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port   = htons(port);
	
	CFunctions::debugPrint("Verifying IP... ");

	if (!VerifyIP(&sockAddr, host.c_str()))
	{
		CFunctions::debugPrint("FAILED\n");
		return;
	}

	CFunctions::debugPrint("SUCCESS\n");

	CFunctions::debugPrint("Creating socket... ");

	m_sock = socket(PF_INET, SOCK_STREAM, 0);
	
	if (m_sock == INVALID_SOCKET)
	{
		CFunctions::debugPrint("FAILED\n");
		return;
	}

	CFunctions::debugPrint("SUCCESS\n");
	
	CFunctions::debugPrint("Connecting... ");
	
	if (connect(m_sock, reinterpret_cast<sockaddr*>(&sockAddr), sizeof(sockAddr)) != 0)
	{
		CFunctions::debugPrint("FAILED\n");
		return;
	}

	CFunctions::debugPrint("SUCCESS\n");

	m_connected = true;
	m_userdata = lua_newuserdata(luaVM, 128);
}

Socket::~Socket()
{
//	m_thread->Stop();
//	delete m_threadData;

	if (m_connected)
		closesocket(m_sock);
}

bool Socket::isConnected()
{
	return m_connected;
}

bool Socket::VerifyIP(sockaddr_in* sockAddr, string host)
{
	hostent* Hostent;
	unsigned int IP = inet_addr(host.c_str());

	if (IP != INADDR_NONE)
	{
		sockAddr->sin_addr.S_un.S_addr = IP;
		return true;
	}
	else
	{
		Hostent = gethostbyname(host.c_str());
		if (Hostent == NULL)
		{
			return false;
		}
		else
		{
			memcpy(&(sockAddr->sin_addr), Hostent->h_addr_list[0], 4);
			return true;
		}
	}
}

bool Socket::sendData(string data)
{
	CFunctions::debugPrint("Sending data... ");

	if (send(m_sock, data.c_str(), data.length(), 0)==SOCKET_ERROR)
	{
		CFunctions::debugPrint("FAILED\n");
		return false;
	}
	else
	{
		CFunctions::debugPrint("SUCCESS\n");
		return true;
	}
}

void* Socket::getUserdata()
{
	return m_userdata;
}