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

int CFunctions::socketConnect(lua_State *luaVM)
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

			lua_pushlightuserdata(luaVM, socket.getUserdata());
//			lua_pushboolean(luaVM, true);
			return 1;
		}
	}

	lua_pushboolean(luaVM, false);
	return 1;
}

/*int CFunctions::socketSendData(lua_State *luaVM)
{
	if (luaVM)
	{
		if (lua_type(luaVM, 1) == LUA_TLIGHTUSERDATA && lua_type(luaVM, 2) == LUA_TSTRING)
		{
			void* userdata   = lua_touserdata(luaVM, 1);
			const char* data = lua_tostring(luaVM, 2);

			bool socketFound = false;
			Socket theSocket;

			for (unsigned int i = 0; i < sockets.size(); ++i)
			{
				Socket socket = *sockets[i];

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
}*/

void CFunctions::addEvent(lua_State* luaVM, const char* eventName)
{
	CLuaArguments args;
	args.PushString(eventName);
	args.PushBoolean(false);
	args.Call(luaVM, "addEvent");
}

void CFunctions::debugPrint(char* text)
{
#ifdef _DEBUG
	printf(text);
#endif
}