/*********************************************************
*
*  Multi Theft Auto v1.0 - Module
*  
*  Module:       ml_irc
*  Url:          http://development.mtasa.com/index.php?title=Modules/SebasIRC
*  Project page: http://multitheftauto-modules.googlecode.com/
*  Developers:   Sebas Lamers <sebasdevelopment@gmx.com>
*                "MaVe" <mave1337@gmail.com>
*
*********************************************************/

// Includes
#include "CFunctions.h"
#include "extra/CLuaArguments.h"
#include <time.h>

#ifdef WIN32
#include <process.h>
#include <winsock.h>
#else
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <cstdlib>
#include <stdio.h>

#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
typedef int SOCKET;
typedef sockaddr_in SOCKADDR_IN;
typedef hostent HOSTENT;
typedef sockaddr SOCKADDR;
typedef timeval TIMEVAL;
#define SOCKET_ERROR -1
#define INVALID_SOCKET -1
#define Sleep sleep
#define closesocket close
#define sprintf_s sprintf

pthread_t t;
#endif

#include <string.h>

// Namespace
using namespace std;

// Vars
long SocketState;
SOCKET Socket;
SOCKADDR_IN Addr;
bool showDebugText = false;
string botname;
string server;
lua_State* gLuaVM;

/*
	------
	Lua functions
*/

// bool ircConnect(string host, int port, string nickname)
int CFunctions::ircConnect ( lua_State* luaVM )
{
    if (luaVM)
    {
        if (lua_type(luaVM, 1) == LUA_TSTRING && lua_type(luaVM, 2) == LUA_TNUMBER && lua_type(luaVM, 3) == LUA_TSTRING)
        {
			string luairc = lua_tostring(luaVM, 1);
			unsigned short luaport = static_cast < unsigned short > ( atoi ( lua_tostring ( luaVM, 2 ) ) );
			string luanickname = lua_tostring(luaVM, 3);

			server = luairc;
			if (!connectToIRC(server, luaport))
			{
				lua_pushboolean(luaVM, false);
				return 1;
			}
			
			gLuaVM = luaVM;
#ifdef WIN32
			_beginthread(messageThread, 0, NULL);
#else
			//pthread_create(&t, NULL, &messageThread, NULL);
			pthread_create(&t, 0, messageThread, NULL);
			//pthread_join(t, NULL);
#endif
			botname = luanickname;
			sendRaw("USER MTABot Bot localhost :IRCBot by Sebas\r\n");
			sendRaw("NICK " + luanickname + "\r\n");

			lua_pushboolean(luaVM, true);
			return 1;
		}
	}
    lua_pushboolean(luaVM, false);
    return 1;
}

// bool ircDisconnect([string quitMessage])
int CFunctions::ircDisconnect ( lua_State* luaVM )
{
    if(luaVM)
    {
		string quitMessage;

		if(lua_type(luaVM, 1) == LUA_TSTRING)
		{
			quitMessage = lua_tostring(luaVM, 1);
		}
		else
			quitMessage = "MTABot";

		sendRaw("QUIT :" + quitMessage);

		//CloseSocket();
		lua_pushboolean(luaVM, true);
		return 1;
	}
    lua_pushboolean(luaVM, false);
    return 1;
}

// bool ircJoin(string channel [, string password])
int CFunctions::ircJoin ( lua_State* luaVM )
{
    if(luaVM)
    {
		if(lua_type(luaVM, 1) == LUA_TSTRING)
		{
			string tchannel = lua_tostring(luaVM, 1);

			if(lua_type(luaVM, 2) == LUA_TSTRING)
			{
				string channelPassword = lua_tostring(luaVM, 2);
				sendRaw("JOIN " + tchannel + " " + channelPassword);
			}
			else
				sendRaw("JOIN " + tchannel);
			
			lua_pushboolean(luaVM, true);
			return 1;
		}
	}
    lua_pushboolean(luaVM, false);
    return 1;
}

// bool ircRaw(string data)
int CFunctions::ircRaw ( lua_State* luaVM )
{
    if(luaVM)
    {
		if(lua_type(luaVM, 1) == LUA_TSTRING)
		{
			sendRaw(lua_tostring ( luaVM, 1 ));
			lua_pushboolean(luaVM, true);
			return 1;
		}
	}
    lua_pushboolean(luaVM, false);
    return 1;
}

// bool ircSay(string target, string message)
int CFunctions::ircSay ( lua_State* luaVM )
{
    if(luaVM)
    {
		if(lua_type(luaVM, 1) == LUA_TSTRING && lua_type(luaVM, 2) == LUA_TSTRING)
		{
			string luatarget = lua_tostring ( luaVM, 1 );
			string luatext = lua_tostring ( luaVM, 2 );

			sendRaw("PRIVMSG " + luatarget + " :" + luatext);
			lua_pushboolean(luaVM, true);
			return 1;
		}
	}
    lua_pushboolean(luaVM, false);
    return 1;
}

// bool ircShowDebug(bool show)
int CFunctions::ircShowDebug ( lua_State* luaVM )
{
    if(luaVM)
    {
		if(lua_type(luaVM, 1) == LUA_TBOOLEAN)
		{
			showDebugText = ((lua_toboolean(luaVM, 1)) ? true : false);
			lua_pushboolean(luaVM, true);
			return 1;
		}
	}
    lua_pushboolean(luaVM, false);
    return 1;
}

// bool ircPart(string channel [, string partReason])
int CFunctions::ircPart ( lua_State* luaVM )
{
    if(luaVM)
    {
		if(lua_type(luaVM, 1) == LUA_TSTRING)
		{
			string part = lua_tostring(luaVM, 1);
			string partReason = "";
			if (lua_type(luaVM, 2) == LUA_TSTRING)
				partReason = lua_tostring(luaVM, 2);

			sendRaw("PART " + part + " :" + partReason);

			lua_pushboolean(luaVM, true);
			return 1;
		}
	}
    lua_pushboolean(luaVM, false);
    return 1;
}

// bool ircChangeNick(string newnick)
int CFunctions::ircChangeNick ( lua_State* luaVM )
{
    if(luaVM)
    {
		if(lua_type(luaVM, 1) == LUA_TSTRING)
		{
			string newnick = lua_tostring(luaVM, 1);
			sendRaw("NICK " + newnick);

			char buf[1024];
			int i = recv(Socket, buf, 1024, 0);
			if(i > 0)
			{
				if(search(buf, " :Nickname already in use.") != -1)
				{
					lua_pushboolean(luaVM, false);
					return 1;
				}else
				if(search(buf, " :Erronous Nickname: Illegal") != -1)
				{
					lua_pushboolean(luaVM, false);
					return 1;
				}else
				if(search(buf, " NICK :") != -1)
				{
					botname = newnick;
					lua_pushboolean(luaVM, true);
					return 1;
				}
			}

			lua_pushboolean(luaVM, true);
			return 1;
		}
	}
    lua_pushboolean(luaVM, false);
    return 1;
}

// bool ircSetMode(string mode)
int CFunctions::ircSetMode ( lua_State* luaVM )
{
    if(luaVM)
    {
		if(lua_type(luaVM, 1) == LUA_TSTRING)
		{
			string newmode = lua_tostring(luaVM, 1);
			sendRaw("MODE " + botname + " " + newmode);

			lua_pushboolean(luaVM, true);
			return 1;
		}
	}
    lua_pushboolean(luaVM, false);
    return 1;
}

// bool ircSetChannelMode(string channel, string mode)
// (also ircSetChannelMode("#bla", "+o Sebas")
int CFunctions::ircSetChannelMode ( lua_State* luaVM )
{
    if(luaVM)
    {
		if(lua_type(luaVM, 1) == LUA_TSTRING && lua_type(luaVM, 2) == LUA_TSTRING)
		{
			string channel = lua_tostring(luaVM, 1);
			string newmode = lua_tostring(luaVM, 2);
			sendRaw("MODE " + channel + " " + newmode);

			char buf[1024];
			int i = recv(Socket, buf, 1024, 0);
			if(i > 0)
			{
				if(search(buf, " :You're not channel owner") != -1)
				{
					lua_pushboolean(luaVM, false);
					return 1;
				}
				lua_pushboolean(luaVM, true);
				return 1;
			}
		}
	}
    lua_pushboolean(luaVM, false);
    return 1;
}

// bool ircIsConnected()
int CFunctions::ircIsConnected ( lua_State* luaVM )
{
    if(luaVM)
    {
		sendRaw("PING " + server);

		char buf[1024];
        int i = recv(Socket, buf, 1024, 0);
        if(i > 0)
        {
			if(search(buf, "PONG") != -1)
			{
				lua_pushboolean(luaVM, true);
				return 1;
			}
		}
	}
    lua_pushboolean(luaVM, false);
    return 1;
}

/*
	-----------------------------------------------------------
	Other funcs
*/

void CFunctions::sendRaw(string text)
{
	string SendMsg = text + "\r\n";
	SocketState = send(Socket, SendMsg.c_str(), SendMsg.length(), 0);
	return;
}

#ifdef WIN32
int CFunctions::startWinSocket()
{
    WSADATA WSA;
    return WSAStartup(MAKEWORD(2, 0), &WSA);
}
#endif

long CFunctions::GetAddr(string hostname)
{
    unsigned long IP;
    HOSTENT* he;
    IP = inet_addr(hostname.c_str());

    if(IP != INADDR_NONE)
    {
        Addr.sin_addr.s_addr = IP;
        return 0;
    }else{
        he = gethostbyname(hostname.c_str());
        if(he == NULL)
		{
            return SOCKET_ERROR;
		}else{
            memcpy(&(Addr.sin_addr), he->h_addr_list[0], 4);
		}
        return 0;
    }
}

bool CFunctions::connectToIRC(string server, int port)
{
#ifdef WIN32
    SocketState = startWinSocket();
    if(SocketState != 0)
    {
        return false;
    }
#endif
    Socket = socket(PF_INET, SOCK_STREAM, 0);
    if(Socket == INVALID_SOCKET)
    {
        return false;
    }
    memset(&Addr, 0, sizeof(SOCKADDR_IN));
    Addr.sin_family = AF_INET;
    Addr.sin_port = htons(port);
    SocketState = GetAddr(server);
    if(SocketState == SOCKET_ERROR)
    {
        return false;
    }
    SocketState = connect(Socket, (SOCKADDR*)&Addr, sizeof(SOCKADDR));
    if(SocketState == SOCKET_ERROR)
    {
        return false;
    }
    return true;
}

void CFunctions::CloseSocket()
{
	//shutdown(Socket, true);
	closesocket(Socket); // This makes a crash. (MaVe: Why using it then? :p)
    //WSACleanup();

    if (showDebugText)
        CFunctions::sendConsole("Socket closed.");
    return;
}

#ifdef WIN32
void CFunctions::messageThread(void* x)
#else
void *CFunctions::messageThread(void* x)
#endif
{
	//int y = *(int*)(x); // y has 100 now
	fd_set fdSetRead;
	TIMEVAL timeout;

	while(SocketState != SOCKET_ERROR)
    {
        FD_ZERO(&fdSetRead); 
        FD_SET(Socket, &fdSetRead); 
        timeout.tv_sec = 0; 
        timeout.tv_usec = 0; 
        while((SocketState = select(0, &fdSetRead, NULL, NULL, &timeout)) > 0)
        {
			char buf[1024];
            int i = recv(Socket, buf, 1024, 0);
            if(i > 0)
            {
				buf[i] = '\0';
				char part[512];
				for (i = 0; i < (int)strlen(buf); i++)
				{
					if (buf[i] == '\n')
					{
						onDataReceived(part);
						memset(&part, 0, sizeof(part));
					}
					else if (buf[i] != '\r')
					{
						part[strlen(part)] = buf[i];
					}
				}
            }
			Sleep(50); // prevent the bot from eating your CPU
        }
    }
}

void CFunctions::AddEvent ( lua_State* luaVM, const char* szEventName )
{
	CLuaArguments args;
	args.PushString(szEventName);
	args.PushBoolean(true);
	args.Call(luaVM, "addEvent");
}

void CFunctions::onDataReceived(char* msg)
{
	if (showDebugText)
	{
		CFunctions::sendConsole(msg);
	}

	// Check for "PING"
	if(strncmp(msg, "PING", 4) == 0)
	{
		// Send "PONG" back
		msg[1] = 'O';
		sendRaw(msg);
		if (showDebugText)
			sendConsole("Ping received, ponged back.");
		return;
	}

	CLuaArguments args;
	args.PushString("onIRCRaw");
	lua_getglobal(gLuaVM, "root");
	CLuaArgument RootElement(gLuaVM, -1);
	args.PushUserData(RootElement.GetLightUserData());
	args.PushString(msg);
	args.Call(gLuaVM, "triggerEvent");

	return;
}

int CFunctions::search(char *string, char *substring)
{
   int i=0, j=0, yes=0, index;

   while(string[i]!='\0')
   {
       if(string[i]==substring[j])
       {
            index=i;

            for (j=1;substring[j];j++)
            //match characters until
            //the end of substring
            {
                i++;

                if(string[i]==substring[j])
					 yes=1;
				else
					yes=0;
					break;
			}
			if(yes==1) return index;
		}
		i++;
		j=0;
     }
   return -1;
}

int CFunctions::sendConsole(char* text)
{
	time_t t = time(0);
	struct tm* lt = localtime(&t);
	char time_str[15];
	sprintf_s(time_str, "%02d:%02d:%02d", lt->tm_hour, lt->tm_min, lt->tm_sec);
	printf("[%s] IRCModule: %s\n", time_str, text);
	return 1;
}
