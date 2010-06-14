/*********************************************************
*
*  Multi Theft Auto: San Andreas
*
*  Curl module for mta.
*
*  Developers:
*  Alexander de Jong <alexander.t.dejong@gmail.com>
*
*********************************************************/
#include <stdlib.h>
#include "CFunctions.h"
#include "extra/CLuaArguments.h"

lua_State* gLuaVM;
extern Mtacurls* mtacurls;

int CFunctions::curl_init( lua_State *luaVM )
{
	if(luaVM)
	{
		if( lua_type( luaVM, 1 ) == LUA_TSTRING )
		{
			const char* url = lua_tostring(luaVM, 1);

			Mtacurl* pMtacurl = mtacurls->Create( luaVM, url );
			if( pMtacurl )
			{
				lua_pushlightuserdata( luaVM, pMtacurl->GetUserData());
				return 1;
			}
		}
	}

	lua_pushboolean( luaVM, false );
	return 1;
}

int CFunctions::curl_close( lua_State* luaVM )
{
	if(luaVM)
	{
		if( lua_type( luaVM, 1 ) == LUA_TLIGHTUSERDATA )
		{
			Mtacurl* pMtacurl = mtacurls->Get( lua_touserdata( luaVM, 1 ) );
			if( pMtacurl != NULL )
			{
				pMtacurl->MakeAwaitDestruction();

				lua_pushboolean( luaVM, true );
				return 1;
			}
		}
	}

	lua_pushboolean( luaVM, false );
	return 1;
}

int CFunctions::curl_setopt( lua_State* luaVM )
{
	if(luaVM)
	{
		if( lua_type( luaVM, 1 ) == LUA_TLIGHTUSERDATA &&
			lua_type( luaVM, 2 ) == LUA_TNUMBER )
		{
			Mtacurl* pMtacurl = mtacurls->Get( lua_touserdata( luaVM, 1 ) );
			if( pMtacurl != NULL )
			{
				CURLoption *option = LUACURL_OPTIONP_UPVALUE(luaVM, 2);

				switch(lua_type( luaVM, 3))
				{
				case LUA_TBOOLEAN:
					// call boolean setopt
					pMtacurl->setopt_boolean(*option, lua_toboolean(luaVM, 3));
				break;

				case LUA_TNUMBER:
					// call number setopt
					pMtacurl->setopt_number(*option, lua_tonumber(luaVM, 3));
				break;

				case LUA_TSTRING:
					// call string setopt
					pMtacurl->setopt_string(*option, lua_tostring(luaVM, 3));
				break;
				}
			}
		}
	}
	lua_pushboolean(luaVM, false);
	return 1;
}

void CFunctions::createCurlGlobal( lua_State* luaVM, const char* name, int value )
{
	lua_pushnumber(luaVM, value);
	lua_setglobal(luaVM, name);
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

	args.PushUserData(RootElement.GetLightUserData());
	args.PushUserData(userdata);

	if (arg1.length() > 0)
		args.PushString(arg1.c_str());

	args.Call(gLuaVM, "triggerEvent");
}