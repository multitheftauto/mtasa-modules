/*********************************************************
*
*  Multi Theft Auto v1.0 - Module
*  
*  Module:       ml_irc
*  Url:          http://development.mtasa.com/index.php?title=Modules/SebasIRC
*  Project page: http://multitheftauto-modules.googlecode.com/
*  Developers:   Sebas Lamers <baslamers@home.nl>
*
*********************************************************/

class CFunctions;

#ifndef __CFUNCTIONS_H
	#define __CFUNCTIONS_H

	#include "include/ILuaModuleManager.h"
	#include <string>
	using namespace std;

	extern ILuaModuleManager *pModuleManager;
	class CFunctions
	{
		public:
			static int		ircConnect			(lua_State* luaVM);
			static int		ircDisconnect		(lua_State* luaVM);
			static int		ircJoin				(lua_State* luaVM);
			static int		ircRaw				(lua_State* luaVM);
			static int		ircSay				(lua_State* luaVM);
			static int		ircPart				(lua_State* luaVM);
			static int		ircChangeNick		(lua_State* luaVM);
			static int		ircShowDebug		(lua_State* luaVM);
			static int		ircSetMode			(lua_State* luaVM);
			static int		ircSetChannelMode	(lua_State* luaVM);
			static int      ircIsConnected		(lua_State* luaVM);

			// Sockets
#ifdef WIN32
			static int		startWinSocket      ();
#endif
			static long		GetAddr             (string hostname);
			static bool		connectToIRC        (string server, int Port);
			static void		CloseSocket         ();
#ifdef WIN32
			static void		messageThread       (void* x);
#else
			static void*		messageThread	     (void *ok);
#endif
			static void		onDataReceived      (char* msg);
			static void		sendRaw             (string rawtext);
			static int		sendConsole         (char* text);
			static int		search				(char* string, char* substring);

			static void		AddEvent			( lua_State* luaVM, const char* szEventName );
	};
#endif
