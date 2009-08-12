/*********************************************************
*
*  Multi Theft Auto v1.0 - Module
*  
*  Module:       ml_irc
*  Url:          http://development.mtasa.com/index.php?title=Modules/SebasIRC
*  Project page: http://multitheftauto-modules.googlecode.com/
*  Developers:   Sebas Lamers <sebasdevelopment@gmx.com>
*
*********************************************************/

// INTERFACE for Lua dynamic modules

#ifndef __ILUAMODULEMANAGER_H
#define __ILUAMODULEMANAGER_H

#define MAX_INFO_LENGTH 128

extern "C"
{
    #include "include/lua.h"
    #include "include/lualib.h"
    #include "include/lauxlib.h"
}

class ILuaModuleManager
{
public:
	virtual void			ErrorPrintf			( const char* szFormat, ... ) = 0;
	virtual void			DebugPrintf			( lua_State * luaVM, const char* szFormat, ... ) = 0;
	virtual void			Printf				( const char* szFormat, ... ) = 0;
	virtual bool			RegisterFunction	( lua_State * luaVM, const char *szFunctionName, lua_CFunction Func ) = 0;
	virtual const char*		GetResourceName		( lua_State * luaVM ) = 0;
	virtual unsigned long	GetResourceMetaCRC	( lua_State * luaVM ) = 0;
	virtual unsigned long	GetResourceFileCRC	( lua_State * luaVM, const char* szFile ) = 0;
};

#endif
