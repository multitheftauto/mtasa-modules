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

// Includes
#include "CFunctions.h"
#include "extra/CLuaArguments.h"
#include <time.h>
#include <process.h>
#include <winsock.h>
#include <string>

// Namespace
using namespace std;

// Vars
long SocketState;
SOCKET Socket;
SOCKADDR_IN Addr;
bool showDebugText = true;

/*
	------
	Lua functions
*/

int CFunctions::ircConnect ( lua_State* luaVM )
{
    if(luaVM)
    {
        if(lua_type(luaVM, 1) == LUA_TSTRING && lua_type(luaVM, 2) == LUA_TNUMBER && lua_type(luaVM, 3) == LUA_TSTRING && lua_type(luaVM, 4) == LUA_TSTRING)
        {
			string luairc = lua_tostring(luaVM, 1);
			unsigned short luaport = static_cast < unsigned short > ( atoi ( lua_tostring ( luaVM, 2 ) ) );
			string luanickname = lua_tostring(luaVM, 3);
			string luachannel = lua_tostring(luaVM, 4);

			if(!connectToIRC(luairc, luaport))
			{
				sendConsole("Cannot connect!");
				lua_pushboolean(luaVM, false);
				return 1;
			}

			sendRaw("NICK " + luanickname + "\r\n");
			sendRaw("USER MTABot Bot 127.0.0.1 :IRCBot by Sebas\r\n");
			sendRaw("JOIN " + luachannel);

			_beginthread(messageThread, 0, 0); // This needs a rewrite, this uses 50% from your CPU :\
			//CreateThread ( NULL, 0, messageThread, this, 0, NULL );

			lua_pushboolean(luaVM, true);
			return 1;
		}
	}
    lua_pushboolean(luaVM, false);
    return 0;
}

int CFunctions::ircDisconnect ( lua_State* luaVM )
{
    if(luaVM)
    {
		sendRaw("QUIT :MTABot");
		CloseSocket();
		lua_pushboolean(luaVM, true);
		return 1;
	}
    lua_pushboolean(luaVM, false);
    return 0;
}

int CFunctions::ircJoin ( lua_State* luaVM )
{
    if(luaVM)
    {
		if(lua_type(luaVM, 1) == LUA_TSTRING)
		{
			string tchannel = lua_tostring(luaVM, 1);
			sendRaw("JOIN " + tchannel);
			lua_pushboolean(luaVM, true);
			return 1;
		}
	}
    lua_pushboolean(luaVM, false);
    return 0;
}

int CFunctions::ircRaw ( lua_State* luaVM )
{
    if(luaVM)
    {
		if(lua_type(luaVM, 1) == LUA_TSTRING)
		{
			string rawtext = lua_tostring ( luaVM, 1 );
			sendRaw(rawtext);
			lua_pushboolean(luaVM, true);
			return 1;
		}
	}
    lua_pushboolean(luaVM, false);
    return 0;
}

int CFunctions::ircSay ( lua_State* luaVM )
{
    if(luaVM)
    {
		if(lua_type(luaVM, 1) == LUA_TSTRING && lua_type(luaVM, 2) == LUA_TSTRING)
		{
			string luachannel = lua_tostring ( luaVM, 1 );
			string luatext = lua_tostring ( luaVM, 2 );

			sendRaw("PRIVMSG " + luachannel + " :" + luatext);
			lua_pushboolean(luaVM, true);
			return 1;
		}
	}
    lua_pushboolean(luaVM, false);
    return 0;
}

int CFunctions::ircShowDebug ( lua_State* luaVM )
{
    if(luaVM)
    {
		if(lua_type(luaVM, 1) == LUA_TBOOLEAN)
		{
			bool newdebug = lua_toboolean ( luaVM, 1 );
			showDebugText = newdebug;

			lua_pushboolean(luaVM, true);
			return 1;
		}
	}
    lua_pushboolean(luaVM, false);
    return 0;
}

int CFunctions::ircPart ( lua_State* luaVM )
{
    if(luaVM)
    {
		if(lua_type(luaVM, 1) == LUA_TSTRING)
		{
			string part = lua_tostring(luaVM, 1);
			sendRaw("PART " + part);

			lua_pushboolean(luaVM, true);
			return 1;
		}
	}
    lua_pushboolean(luaVM, false);
    return 0;
}

int CFunctions::ircChangeNick ( lua_State* luaVM )
{
    if(luaVM)
    {
		if(lua_type(luaVM, 1) == LUA_TSTRING)
		{
			string newnick = lua_tostring(luaVM, 1);
			sendRaw("NICK " + newnick);

			lua_pushboolean(luaVM, true);
			return 1;
		}
	}
    lua_pushboolean(luaVM, false);
    return 0;
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

int CFunctions::startWinSocket()
{
    WSADATA WSA;
    return WSAStartup(MAKEWORD(2, 0), &WSA);
}

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
    SocketState = startWinSocket();
    if(SocketState != 0)
    {
        return false;
    }
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
    closesocket(Socket);
    WSACleanup();
    if(showDebugText)
    {
        CFunctions::sendConsole("Socket closed.");
	}
    return;
}

void CFunctions::messageThread(void *ok)
{
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
				onDataReceived(buf);
            }
        }
    }
}

void CFunctions::onDataReceived(char* msg)
{
	if(strncmp(msg, "PING", 4) == 0)
	{
		sendRaw("PONG :REPLY");

		if(showDebugText)
		{
			CFunctions::sendConsole("PONG!");
		}
	}

	if(showDebugText)
	{
		CFunctions::sendConsole(msg);
	}
	return;
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