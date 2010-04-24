/*********************************************************
*
*  Multi Theft Auto: San Andreas - Deathmatch
*
*  ml_base, External lua add-on module
*
*  Copyright © 2003-2008 MTA.  All Rights Reserved.
*
*  Grand Theft Auto is © 2002-2003 Rockstar North
*
*  THE FOLLOWING SOURCES ARE PART OF THE MULTI THEFT
*  AUTO SOFTWARE DEVELOPMENT KIT AND ARE RELEASED AS
*  OPEN SOURCE FILES. THESE FILES MAY BE USED AS LONG
*  AS THE DEVELOPER AGREES TO THE LICENSE THAT IS
*  PROVIDED WITH THIS PACKAGE.
*
*********************************************************/

#include "CFunctions.h"
#include "extra/CLuaArguments.h"

lua_State* gLuaVM;
Socket sockets [64];

int CFunctions::sockOpen(lua_State *luaVM)
{
	if (luaVM)
	{
		if (lua_type(luaVM, 1) == LUA_TSTRING && lua_type(luaVM, 2) == LUA_TNUMBER)
		{
			const char* host    = lua_tostring(luaVM, 1);
			unsigned short port = static_cast<unsigned short>(atoi(lua_tostring(luaVM, 2)));

			Socket socket(luaVM, host, port);

			if (!socket.isConnected())
			{
				lua_pushboolean(luaVM, false);
				return 1;
			}

			void* userdata = socket.getUserdata();
			
			for (unsigned int i = 0; i < 64; ++i)
			{
				if (!sockets[i].getUserdata())
				{
					sockets[i] = socket;
					break;
				}
			}

			CFunctions::triggerEvent("onSockOpened",userdata,"nil","nil");
			lua_pushlightuserdata(luaVM,userdata);
			return 1;
		}
	}

	lua_pushboolean(luaVM, false);
	return 1;
}

int CFunctions::sockWrite(lua_State *luaVM)
{
	if (luaVM)
	{
		if (lua_type(luaVM, 1) == LUA_TLIGHTUSERDATA && lua_type(luaVM, 2) == LUA_TSTRING)
		{
			void* userdata   = lua_touserdata(luaVM, 1);
			const char* data = lua_tostring(luaVM, 2);

			bool socketFound = false;
			Socket theSocket;

			for (unsigned int i = 0; i < 64; ++i)
			{
				Socket socket = sockets[i];

				if (socket.getUserdata() == userdata)
				{
					theSocket = socket;
					socketFound = true;
					break;
				}
			}

			if (socketFound)
			{
				lua_pushboolean(luaVM, theSocket.sendData(data));
				return 1;
			}
		}
	}

	lua_pushboolean(luaVM, false);
	return 1;
}

int CFunctions::sockClose(lua_State *luaVM)
{
	/*if (luaVM)
	{
		if (lua_type(luaVM, 1) == LUA_TLIGHTUSERDATA)
		{
			void* userdata   = lua_touserdata(luaVM, 1);

			bool socketFound = false;
			Socket theSocket;

			for (unsigned int i = 0; i < 64; ++i)
			{
				Socket socket = sockets[i];

				if (socket.getUserdata() == userdata)
				{
					theSocket = socket;
					socketFound = true;
					sockets[i] = NULL;
					break;
				}
			}

			if (socketFound)
			{
				lua_pushboolean(luaVM, theSocket.destroy());
				return 1;
			}
		}
	}*/ 

	lua_pushboolean(luaVM, false);
	return 1;
}

int CFunctions::saveLuaData(lua_State *luaVM)
{
    gLuaVM = luaVM;
    return 1;
}

int CFunctions::addEvent(lua_State* luaVM, const char* szEventName)
{
    CLuaArguments args;
    args.PushString(szEventName);
    args.PushBoolean(true);
    args.Call(luaVM, "addEvent");
    return 1;
}

int CFunctions::triggerEvent(char* eventName, void* userdata, char* arg1, char* arg2)
{
    CLuaArguments args;
    args.PushString(eventName);
    lua_getglobal(gLuaVM, "root");
    CLuaArgument RootElement(gLuaVM, -1);
    args.PushUserData(RootElement.GetLightUserData());
	args.PushUserData(userdata);
	if (arg1 != "nil") {
		args.PushString(arg1);
	}
	if (arg2 != "nil") {
		args.PushString(arg2);
	}
    args.Call(gLuaVM, "triggerEvent");
    return 1;
}

void CFunctions::debugPrint(char* text)
{
#ifdef _DEBUG
	printf(text);
#endif
}