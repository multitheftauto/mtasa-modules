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

#include "curl/curl.h"

#define LUACURL_OPTIONP_UPVALUE(L, INDEX) ((CURLoption *) lua_touserdata(L, lua_upvalueindex(INDEX)))
#define LUACURL_CODEP_UPVALUE(L, INDEX) ((CURLcode *) lua_touserdata(L, lua_upvalueindex(INDEX)))

class Mtacurl;

class CFunctions
{
public:
	static int curl_func_register	( lua_State* luaVM );
	static int curl_init			( lua_State* luaVM );
	static int curl_close			( lua_State* luaVM );
	static int curl_setopt			( lua_State* luaVM );
	static int curl_cleanup			( lua_State* luaVM );
	static int curl_send			( lua_State* luaVM );
	static int curl_perform			( lua_State* luaVM );
	static int curl_escape			( lua_State* luaVM );
	static int curl_strerror		( lua_State* luaVM );

	static void registerCurlOption	( lua_State* luaVM, const char* name, CURLoption *value );
	static void registerCurlCode	( lua_State* luaVM, const char* name, CURLcode *value );
	static void registerCurlInfo	( lua_State* luaVM, const char* name, CURLINFO *value );
	static void registerCurlGlobal	( lua_State* luaVM, const char* name, int value );
	static void saveLuaData			( lua_State* luaVM );
	static void addEvent			( lua_State* luaVM, const char* szEventName );
	static void triggerEvent		( const string& eventName, void* userdata, const string& arg1 = "" );

private:
	static valarray<int> options_str();
	static valarray<CURLoption> options_opt();

	static valarray<int> code_str;
	static valarray<CURLcode> code_opt;

	static valarray<int> info_str;
	static valarray<CURLINFO> info_opt;
};
#endif
