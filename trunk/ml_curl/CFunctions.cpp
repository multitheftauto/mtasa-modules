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
			size_t length;
			const char* url = luaL_checklstring(luaVM, 1, &length);

			Mtacurl* pMtacurl = mtacurls->Create( luaVM, url, length );
			if( pMtacurl )
			{
				lua_pushlightuserdata( luaVM, pMtacurl->GetUserData());
				return 1;
			}
		}
		else {
			Mtacurl* pMtacurl = mtacurls->Create( luaVM, NULL, 0 );
			if( pMtacurl )
			{
				lua_pushlightuserdata(luaVM, pMtacurl->GetUserData());
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
			lua_type( luaVM, 2 ) == LUA_TLIGHTUSERDATA )
		{
			Mtacurl* pMtacurl = mtacurls->Get( lua_touserdata( luaVM, 1 ) );

			if( pMtacurl != NULL )
			{				
				CURLoption* option = (CURLoption*)lua_touserdata( luaVM, 2); // options_opt[(int)lua_tonumber( luaVM, 2 )];
				CURLcode code;

				switch(lua_type( luaVM, 3))
				{
					case LUA_TBOOLEAN:
						// call boolean setopt
						code = pMtacurl->setopt_boolean((CURLoption&)option, lua_toboolean(luaVM, 3));
					break;

					case LUA_TNUMBER:
						// call number setopt
						code = pMtacurl->setopt_number((CURLoption&)option, lua_tonumber(luaVM, 3));
					break;

					case LUA_TSTRING:
						// call string setopt
						code = pMtacurl->setopt_string((CURLoption&)option, lua_tostring(luaVM, 3));
					break;

				}

				if( code )
				{
					// lua_pushnumber( luaVM, code_str[code] );
					lua_pushlightuserdata( luaVM, (void*)code);
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
				size_t length = 0;
				// const char* url = lua_tostring( luaVM, 2 );
				const char* url = luaL_checklstring( luaVM, 2, &length );
				char* escape_url = pMtacurl->escape( url, length );

				lua_pushstring( luaVM, escape_url );
				curl_free(escape_url);
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

				luaL_opt(luaVM, luaL_checktable, 2, lua_newtable(luaVM));


				lua_getfield(luaVM, 2, "writefunction");
				int writefunction = lua_isfunction(luaVM, -1)?1:0;
				if (writefunction)
					pMtacurl->setup_writefunction(luaVM);
				lua_pop(luaVM, 1);

				lua_getfield(luaVM, 2, "readfunction");
				int readfunction = lua_isfunction(luaVM, -1)?1:0;
				if (readfunction)
					pMtacurl->setup_readfunction(luaVM);
				lua_pop(luaVM, 1);

				lua_getfield(luaVM, 2, "headerfunction");
				int headerfunction = lua_isfunction(luaVM, -1)?1:0;
				if (headerfunction)
					pMtacurl->setup_headerfunction(luaVM);
				lua_pop(luaVM, 1);


				CURLcode code = pMtacurl->perform();

				if(writefunction)
					pMtacurl->clear_writefunction( luaVM );
				if(readfunction)
					pMtacurl->clear_readfunction( luaVM );
				if(headerfunction)
					pMtacurl->clear_headerfunction( luaVM );

				lua_pushlightuserdata(luaVM, (void*)code);
				// lua_pushstring(luaVM, (const char*)pMtacurl->getResult());
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

				lua_pushlightuserdata(luaVM, (void*)code);
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
			lua_type( luaVM, 2 ) == LUA_TLIGHTUSERDATA )
		{
			Mtacurl* pMtacurl = mtacurls->Get( lua_touserdata( luaVM, 1 ) );

			if( pMtacurl != NULL )
			{
				CURLcode* code = (CURLcode*)lua_touserdata(luaVM, 2);
				lua_pushstring(luaVM, pMtacurl->strerror( (CURLcode&)code ) );
				return 1;
			}
		}
	}

	lua_pushnil( luaVM );
	return 1;
}

void CFunctions::registerLuaGlobal( lua_State* luaVM, const char* name, void* value )
{
	lua_pushlightuserdata(luaVM, value);
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