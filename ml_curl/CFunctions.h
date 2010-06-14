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

class CFunctions;

#ifndef __CFUNCTIONS_H
#define __CFUNCTIONS_H

#include "ml_system.h"
#include "Mtacurl.h"

#include "include/ILuaModuleManager.h"
extern ILuaModuleManager10 *pModuleManager;

#include "Mtacurls.h"

#define LUACURL_OPTIONP_UPVALUE(L, INDEX) ((CURLoption *) lua_touserdata(L, lua_upvalueindex(INDEX)))

class Mtacurl;

class CFunctions
{
public:
	static int curl_init			( lua_State* luaVM );
	static int curl_close			( lua_State* luaVM );
	static int curl_setopt			( lua_State* luaVM );

	static void createCurlGlobal	( lua_State* luaVM, const char* name, int value );
	static void saveLuaData			( lua_State* luaVM );
	static void addEvent			( lua_State* luaVM, const char* szEventName );
	static void triggerEvent		( const string& eventName, void* userdata, const string& arg1 = "" );
};
#endif
