/*********************************************************
*
*  Multi Theft Auto: San Andreas - Deathmatch
*
*  ml_sockets, External lua add-on module
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
extern Sockets* sockets;

int CFunctions::sockOpen( lua_State *luaVM )
{
	if( luaVM )
	{
		if( lua_type( luaVM, 1 ) == LUA_TSTRING && lua_type( luaVM, 2 ) == LUA_TNUMBER )
		{
			const char* host = lua_tostring( luaVM, 1 );
			unsigned short port = static_cast < unsigned short > ( lua_tonumber( luaVM, 2 ) );

			// Check for SSL
			bool ssl = false;
			if( lua_type( luaVM, 3 ) == LUA_TBOOLEAN )
				ssl = lua_toboolean( luaVM, 3 ) == 1;

			Socket* pSocket = sockets->Create( luaVM, host, port, ssl );
			if( pSocket )
			{
				lua_pushlightuserdata( luaVM, pSocket->GetUserData( ) );
				return 1;
			}
		}
	}

	lua_pushboolean( luaVM, false );
	return 1;
}

int CFunctions::sockWrite( lua_State *luaVM )
{
	if( luaVM )
	{
		if( lua_type( luaVM, 1 ) == LUA_TLIGHTUSERDATA && lua_type( luaVM, 2 ) == LUA_TSTRING )
		{
			Socket* pSocket = sockets->Get( lua_touserdata( luaVM, 1 ) );
			if( pSocket != NULL )
			{
				lua_pushboolean( luaVM, pSocket->SendData( lua_tostring( luaVM, 2 ) ) );
				return 1;
			}
		}
	}

	lua_pushboolean(luaVM, false);
	return 1;
}

int CFunctions::sockClose(lua_State *luaVM)
{
	if( luaVM )
	{
		if( lua_type( luaVM, 1 ) == LUA_TLIGHTUSERDATA )
		{
			Socket* pSocket = sockets->Get( lua_touserdata( luaVM, 1 ) );
			if( pSocket != NULL )
			{
				pSocket->MakeAwaitDestruction( );
				
				lua_pushboolean( luaVM, true );
				return 1;
			}
		}
	}
	
	lua_pushboolean( luaVM, false );
	return 1;
}

void CFunctions::saveLuaData( lua_State *luaVM )
{
	gLuaVM = luaVM;
}

void CFunctions::addEvent(lua_State* luaVM, const char* szEventName)
{
	CLuaArguments args;
	args.PushString(szEventName);
	args.PushBoolean(true);
	args.Call(luaVM, "addEvent");
}

void CFunctions::triggerEvent(const string& eventName, void* userdata, const string& arg1)
{
	CLuaArguments args;
	args.PushString(eventName.c_str());

	lua_getglobal(gLuaVM, "root");
	CLuaArgument RootElement(gLuaVM, -1);

	args.PushUserData(RootElement.GetLightUserData()); // source
	args.PushUserData(userdata);

	if (arg1.length() > 0)
		args.PushString(arg1.c_str());

	args.Call(gLuaVM, "triggerEvent");
}
