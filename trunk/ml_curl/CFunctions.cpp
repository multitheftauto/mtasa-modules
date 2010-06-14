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

int CFunctions::curl_func_register( lua_State *luaVM )
{
	if(luaVM)
	{
		const char* func = lua_tostring(luaVM, lua_upvalueindex(0));
		pModuleManager->DebugPrintf(luaVM, "The function '%s' is registered, but not working.", func);
		lua_pushnil(luaVM);
	}
	lua_pushnil(luaVM);
	return 1;
}

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
				CURLoption option = options_opt[(int)lua_tonumber( luaVM, 2 )];
				CURLcode code;

				switch(lua_type( luaVM, 3))
				{
					case LUA_TBOOLEAN:
						// call boolean setopt
						code = pMtacurl->setopt_boolean(option, lua_toboolean(luaVM, 3));
					break;

					case LUA_TNUMBER:
						// call number setopt
						code = pMtacurl->setopt_number(option, lua_tonumber(luaVM, 3));
					break;

					case LUA_TSTRING:
						// call string setopt
						code = pMtacurl->setopt_string(option, lua_tostring(luaVM, 3));
					break;
				}

				if( code )
				{
					lua_pushnumber( luaVM, code_str[code] );
					return 1;
				}
			}
		}
	}
	lua_pushboolean(luaVM, false);
	return 1;
}

int CFunctions::curl_cleanup( lua_State* luaVM )
{
	if( luaVM )
	{
		if( lua_type( luaVM, 1 ) == LUA_TLIGHTUSERDATA )
		{
			Mtacurl* pMtacurl = mtacurls->Get( lua_touserdata( luaVM, 1 ) );

			if(pMtacurl != NULL)
			{
				pMtacurl->cleanup();
				lua_pushboolean( luaVM, true );
				return 1;
			}
		}
	}

	lua_pushboolean( luaVM, false );
	return 1;
}

int CFunctions::curl_escape( lua_State* luaVM )
{
	if( luaVM )
	{
		if( lua_type( luaVM, 1 ) == LUA_TLIGHTUSERDATA &&
			lua_type( luaVM, 2 ) == LUA_TSTRING )
		{
			Mtacurl* pMtacurl = mtacurls->Get( lua_touserdata( luaVM, 1) );

			if( pMtacurl != NULL )
			{
				const char* url = lua_tostring( luaVM, 2 );
				const char* escape_url = pMtacurl->escape( url );

				lua_pushstring( luaVM, escape_url );
				return 1;
			}
		}
	}

	lua_pushnil( luaVM );
	return 1;
}

int CFunctions::curl_perform( lua_State* luaVM )
{
	if( luaVM )
	{
		if( lua_type( luaVM, 1 ) == LUA_TLIGHTUSERDATA )
		{
			Mtacurl* pMtacurl = mtacurls->Get( lua_touserdata( luaVM, 1) );

			if( pMtacurl != NULL )
			{
				CURLcode code = pMtacurl->perform();

				lua_pushnumber( luaVM, code_str[code] );
				return 1;
			}
		}
	}

	lua_pushboolean( luaVM, false );
	return 1;
}

int CFunctions::curl_send( lua_State* luaVM )
{
	if( luaVM )
	{
		if( lua_type( luaVM, 1 ) == LUA_TLIGHTUSERDATA )
		{
			Mtacurl* pMtacurl = mtacurls->Get( lua_touserdata( luaVM, 1 ) );

			if( pMtacurl != NULL )
			{
				CURLcode code = pMtacurl->send();

				lua_pushnumber( luaVM, code_str[code] );
				return 1;
			}
		}
	}

	lua_pushboolean( luaVM, false );
	return 1;
}

int CFunctions::curl_strerror( lua_State* luaVM )
{
	if( luaVM )
	{
		if( lua_type( luaVM, 1 ) == LUA_TLIGHTUSERDATA &&
			lua_type( luaVM, 2 ) == LUA_TSTRING )
		{
			Mtacurl* pMtacurl = mtacurls->Get( lua_touserdata( luaVM, 1 ) );

			if( pMtacurl != NULL )
			{
				lua_pushstring(luaVM, pMtacurl->strerror( code_opt[(int)lua_tonumber( luaVM, 2 )] ) );
				return 1;
			}
		}
	}

	lua_pushnil( luaVM );
	return 1;
}

int num_options = 0;
void CFunctions::registerCurlOption( lua_State* luaVM, const char* name, CURLoption* value )
{
	++ num_options;
	// lua_pushnumber(luaVM, value);
	options_str[*value] = num_options;
	options_opt[num_options] = *value;

	lua_pushnumber(luaVM, num_options);
	lua_setglobal(luaVM, name);
}

int num_code = 0;
void CFunctions::registerCurlCode( lua_State* luaVM, const char* name, CURLcode* value )
{
	++ num_code;

	code_str[*value] = num_code;
	code_opt[num_code] = *value;

	lua_pushnumber(luaVM, num_code);
	lua_setglobal(luaVM, name);
}

int num_info = 0;
void CFunctions::registerCurlInfo( lua_State* luaVM, const char* name, CURLINFO* value )
{
	++ num_info;

	info_str[*value] = num_info;
	info_opt[num_info] = *value;

	lua_pushnumber(luaVM, num_info);
	lua_setglobal(luaVM, name);
}

void CFunctions::registerCurlGlobal( lua_State* luaVM, const char* name, int value )
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