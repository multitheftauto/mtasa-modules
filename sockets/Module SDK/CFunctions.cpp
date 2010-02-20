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

#ifndef __MTA_SOCK_FUNC_H
#define __MTA_SOCK_FUNC_H

#include "CFunctions.h"
#include "extra/CLuaArguments.h"
#include "ml_base.h"

int CFunctions::socketConnect(lua_State *luaVM)
{
	if (luaVM)
	{
		if (lua_type(luaVM, 1) == LUA_TSTRING && lua_type(luaVM, 2) == LUA_TNUMBER)
		{
			const char* host    = lua_tostring(luaVM, 1);
			unsigned short port = static_cast<unsigned short>(atoi(lua_tostring(luaVM, 2)));

			Socket socket(host, port);

			if (!socket.isConnected())
			{
				lua_pushboolean(luaVM, false);
				return 1;
			}

//			lua_pushlightuserdata(luaVM, socket);
			lua_pushboolean(luaVM, true);
			return 1;
		}
	}

	lua_pushboolean(luaVM, false);
	return 1;
}

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

#endif