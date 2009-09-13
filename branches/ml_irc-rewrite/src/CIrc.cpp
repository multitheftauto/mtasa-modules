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

#include "CIrc.h"
#include "CFunctions.h"

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

using namespace std;

#ifdef WIN32
SOCKET ircSocket;
SOCKADDR_IN Addr;
HANDLE WinThread;
#else
pthread_t UnixThread;
int ircSocket;
struct sockaddr_in Addr;
#endif

// -------------------------------------------------------------
// IRC functions
// -------------------------------------------------------------

bool CIrc::connectToIRC(std::string server, int port, std::string nickname)
{
#ifdef WIN32
// WINDOWS:
    if(startWinSocket() != 0) // Startup (Needed for Win32)
    {
        return false;
    }

    ircSocket = socket(PF_INET, SOCK_STREAM, 0); // Create Socket
    if(ircSocket == -1)
    {
        return false;
    }
	
    memset(&Addr, 0, sizeof(SOCKADDR_IN));
    Addr.sin_family = AF_INET;
    Addr.sin_port = htons(port);
    if(GetAddr(server) == -1) // Grab IP-Adres
    {
        return false;
    }

	if (connect(ircSocket, (struct sockaddr *)&Addr, sizeof(Addr)) != 0) // Connect
	{
		return false;
	}
	
	WinThread = (HANDLE)_beginthread(messageThread, 0, NULL);
	//WinThread = CreateThread(NULL, 0, messageThread, NULL, 0, NULL);

#else

// LINUX:
    Addr.sin_family = AF_INET;
    ircSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(ircSocket == -1) // Create socket.
    {
        return false;
    }

    Addr.sin_family = AF_INET;
	memset(&Addr, 0, sizeof(Addr));
    Addr.sin_port = htons(port);
    if(GetAddr(server) == -1) // Grab IP-Adres
    {
        return false;
    }
	
    if(connect(ircSocket, (SOCKADDR*)&Addr, sizeof(SOCKADDR)) == -1) // Connect
    {
        return false;
    }

	pthread_create(&UnixThread, 0, messageThread, NULL);
#endif

	// Send data to IRC.
	sendRaw("USER MTABot Bot * :IRCBot for MTA:SA 1.0\r\n");
	sendRaw("NICK " + nickname + "\r\n");

    return true;
}

#ifdef WIN32
int CIrc::startWinSocket()
{
    WSADATA WSA;
    return WSAStartup(MAKEWORD(2, 0), &WSA);
}
#endif

long CIrc::GetAddr(string hostname)
{
    unsigned long IP;
    hostent* he;
    IP = inet_addr(hostname.c_str());

    if(IP != INADDR_NONE)
    {
        Addr.sin_addr.s_addr = IP;
        return 0;
    }else{
        he = gethostbyname(hostname.c_str());
        if(he == NULL)
		{
            return -1;
		}else{
            memcpy(&(Addr.sin_addr), he->h_addr_list[0], 4);
		}
        return 0;
    }
}

int CIrc::CloseSocket()
{
#ifdef WIN32
	CloseHandle(WinThread);
	//shutdown(ircSocket, true);
    //closesocket(ircSocket);
    //WSACleanup();
	WinThread = NULL;
	ircSocket = NULL;
#else
	// pthread_exit();
	//UnixThread = NULL;
#endif
    return 1;
}

#ifdef WIN32
// DWORD WINAPI CIrc::messageThread(void* x) // Neede for createThread
void CIrc::messageThread(void* x)
#else
void *CIrc::messageThread(void* x)
#endif
{
    while(true)
    {
#ifdef WIN32
if(WinThread != NULL)
#else
if(UnixThread != NULL)
#endif
{ // Thread exists check
		char buf[1024];
        int i = recv(ircSocket, buf, 1024, 0);
        if(i > 0)
        {
			buf[i] = '\0';
			char part[512];
			for(i = 0; i < (int)strlen(buf); i++)
			{
				if(buf[i] == '\n')
				{
					onDataReceived(part);
					memset(&part, 0, sizeof(part));
#ifdef WIN32
Sleep(50);
#else
usleep(50);
#endif
				}else
				if(buf[i] != '\r')
				{
					part[strlen(part)] = buf[i];
				}
			}
        }
}else{
#ifdef WIN32
Sleep(50);
#else
usleep(50);
#endif	
}// END Thread exists check
    }
}


int CIrc::onDataReceived(char* msg)
{
	if(strncmp(msg, "PING", 4) == 0) // Check for "PING"
	{
		msg[1] = 'O';
		sendRaw("PONG :REPLY");
	}
	if(search(msg, "ERROR :Closing Link:") != -1)
	{
		CloseSocket(); // Connection was lost with IRC!
	}

	CFunctions::triggerRawEvent(msg);
	return 1;
}

int CIrc::search(char *string, char *substring)
{
   int i = 0, j = 0, yes = 0, index;
   while(string[i] != '\0')
   {
       if(string[i] == substring[j])
       {
            index=i;

            for (j=1; substring[j]; j++)
            {
                i++;
                if(string[i] == substring[j])
				{
					 yes = 1;
				}else{
					yes = 0;
					break;
				}
			}
			if(yes == 1)
			{
				return index;
			}
		}
		i++;
		j = 0;
     }
   return -1;
}

int CIrc::sendRaw(string text)
{
	string SendMsg = text + "\r\n";
	send(ircSocket, SendMsg.c_str(), SendMsg.length(), 0);
	return 1;
}
