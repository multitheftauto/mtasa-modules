#include "sockets.h"

const char HEAD_REQUEST_PART1[] =
{
	"HEAD / HTTP/1.1\r\n"
	"Host: "
};

const char HEAD_REQUEST_PART2[] =
{
	"\r\n"
	"User-agent: MTASockModule\r\n"
	"Connection: close\r\n"
	"\r\n"
};

Socket::Socket(string host, unsigned short port)
{
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

	CFunctions::debugPrint("Sending data... ");

	if (send(m_sock, HEAD_REQUEST_PART1, sizeof(HEAD_REQUEST_PART1)-1, 0)==SOCKET_ERROR)
	{
		CFunctions::debugPrint("FAILED\n");
		return;
	}

	if (send(m_sock, host.c_str(), lstrlen(host.c_str()), 0)==SOCKET_ERROR)
	{
		CFunctions::debugPrint("FAILED\n");
		return;
	}

	if (send(m_sock, HEAD_REQUEST_PART2, sizeof(HEAD_REQUEST_PART2)-1, 0)==SOCKET_ERROR)
	{
		CFunctions::debugPrint("FAILED\n");
		return;
	}

	CFunctions::debugPrint("SUCCESS\n");

	char tempBuffer[128];

	CFunctions::debugPrint("Outputting feedback... \n\n");

	while (true)
	{
		int retval;
		retval = recv(m_sock, tempBuffer, sizeof(tempBuffer)-1, 0);
		if (retval == 0)
		{
			CFunctions::debugPrint("All done!\n");
			break;
		}
		else if (retval == SOCKET_ERROR)
		{
			CFunctions::debugPrint("SOCKET ERROR\n");
			return;
		}
		else
		{
			tempBuffer[retval] = 0;
			CFunctions::debugPrint(tempBuffer);
		}
	}

	m_connected = true;
}

Socket::~Socket()
{

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