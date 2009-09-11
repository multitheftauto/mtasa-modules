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

#include "CFunctions.h"
#include "CIrc.h"
#include "extra/CLuaArguments.h"
#include <time.h>
#include <string.h>
#include <stdio.h>

using namespace std;

// Vars
string botname;
//string server;
lua_State* gLuaVM;

/*
	------
	Lua functions
*/

// bool ircConnect(string host, int port, string nickname)
int CFunctions::ircConnect(lua_State* luaVM)
{
    if(luaVM)
    {
        if(lua_type(luaVM, 1) == LUA_TSTRING && lua_type(luaVM, 2) == LUA_TNUMBER && lua_type(luaVM, 3) == LUA_TSTRING)
        {
			// Defines
			string luairc = lua_tostring(luaVM, 1);
			unsigned short luaport = static_cast<unsigned short>(atoi(lua_tostring(luaVM, 2)));
			string luanickname = lua_tostring(luaVM, 3);

			//server = luairc;
			botname = luanickname;

			if (!CIrc::connectToIRC(luairc, luaport, luanickname))
			{
				lua_pushboolean(luaVM, false);
				return 1;
			}
			gLuaVM = luaVM;

			lua_pushboolean(luaVM, true);
			return 1;
		}
	}
    lua_pushboolean(luaVM, false);
    return 1;
}

// bool ircDisconnect([string quitMessage = "MTABot"])
int CFunctions::ircDisconnect(lua_State* luaVM)
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

		CIrc::sendRaw("QUIT :" + quitMessage);
		CIrc::CloseSocket();

		lua_pushboolean(luaVM, true);
		return 1;
	}
    lua_pushboolean(luaVM, false);
    return 1;
}

// bool ircJoin(string channel [, string password])
int CFunctions::ircJoin(lua_State* luaVM)
{
    if(luaVM)
    {
		if(lua_type(luaVM, 1) == LUA_TSTRING)
		{
			string tchannel = lua_tostring(luaVM, 1);

			if(lua_type(luaVM, 2) == LUA_TSTRING)
			{
				string channelPassword = lua_tostring(luaVM, 2);
				CIrc::sendRaw("JOIN " + tchannel + " " + channelPassword);
			}else{
				CIrc::sendRaw("JOIN " + tchannel);
			}

			lua_pushboolean(luaVM, true);
			return 1;
		}
	}
    lua_pushboolean(luaVM, false);
    return 1;
}

// bool ircRaw(string data)
int CFunctions::ircRaw(lua_State* luaVM)
{
    if(luaVM)
    {
		if(lua_type(luaVM, 1) == LUA_TSTRING)
		{
			CIrc::sendRaw(lua_tostring(luaVM, 1));
			lua_pushboolean(luaVM, true);
			return 1;
		}
	}
    lua_pushboolean(luaVM, false);
    return 1;
}

// bool ircSay(string target, string message)
int CFunctions::ircSay(lua_State* luaVM)
{
    if(luaVM)
    {
		if(lua_type(luaVM, 1) == LUA_TSTRING && lua_type(luaVM, 2) == LUA_TSTRING)
		{
			string luatarget = lua_tostring(luaVM, 1);
			string luatext = lua_tostring(luaVM, 2);

			CIrc::sendRaw("PRIVMSG " + luatarget + " :" + luatext);

			lua_pushboolean(luaVM, true);
			return 1;
		}
	}
    lua_pushboolean(luaVM, false);
    return 1;
}

// bool ircNotice(string target, string message)
int CFunctions::ircNotice(lua_State* luaVM)
{
    if(luaVM)
    {
		if(lua_type(luaVM, 1) == LUA_TSTRING && lua_type(luaVM, 2) == LUA_TSTRING)
		{
			string luatarget = lua_tostring(luaVM, 1);
			string luatext = lua_tostring(luaVM, 2);

			CIrc::sendRaw("NOTICE " + luatarget + " :" + luatext);

			lua_pushboolean(luaVM, true);
			return 1;
		}
	}
    lua_pushboolean(luaVM, false);
    return 1;
}

// bool ircPart(string channel [, string partReason])
int CFunctions::ircPart(lua_State* luaVM)
{
    if(luaVM)
    {
		if(lua_type(luaVM, 1) == LUA_TSTRING)
		{
			string part = lua_tostring(luaVM, 1);
			string partReason = "";

			if (lua_type(luaVM, 2) == LUA_TSTRING)
			{
				partReason = lua_tostring(luaVM, 2);
			}
			CIrc::sendRaw("PART " + part + " :" + partReason);

			lua_pushboolean(luaVM, true);
			return 1;
		}
	}
    lua_pushboolean(luaVM, false);
    return 1;
}

/*
// bool ircChangeNick(string newnick)
int CFunctions::ircChangeNick(lua_State* luaVM)
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
*/

// bool ircSetMode(string mode)
int CFunctions::ircSetMode(lua_State* luaVM)
{
    if(luaVM)
    {
		if(lua_type(luaVM, 1) == LUA_TSTRING)
		{
			string newmode = lua_tostring(luaVM, 1);
			CIrc::sendRaw("MODE " + botname + " " + newmode);

			lua_pushboolean(luaVM, true);
			return 1;
		}
	}
    lua_pushboolean(luaVM, false);
    return 1;
}

/*
// bool ircSetChannelMode(string channel, string mode)
// (also ircSetChannelMode("#bla", "+o Sebas")
int CFunctions::ircSetChannelMode(lua_State* luaVM)
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
int CFunctions::ircIsConnected(lua_State* luaVM)
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
*/

/*
	-----------------------------------------------------------
	Other funcs
*/

int CFunctions::AddEvent(lua_State* luaVM, const char* szEventName)
{
	CLuaArguments args;
	args.PushString(szEventName);
	args.PushBoolean(true);
	args.Call(luaVM, "addEvent");
	return 1;
}

int CFunctions::triggerRawEvent(char* msg)
{
	CLuaArguments args;
	args.PushString("onIRCRaw");
	lua_getglobal(gLuaVM, "root");
	CLuaArgument RootElement(gLuaVM, -1);
	args.PushUserData(RootElement.GetLightUserData());
	args.PushString(msg);
	args.Call(gLuaVM, "triggerEvent");
	return 1;
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
            {
                i++;

                if(string[i]==substring[j])
				{
					 yes=1;
				}else{
					yes=0;
					break;
				}
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
	printf("[%02d:%02d:%02d] IRCModule: %s\n", lt->tm_hour, lt->tm_min, lt->tm_sec, text);
	return 1;
}