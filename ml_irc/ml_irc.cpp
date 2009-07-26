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

#include "ml_irc.h"

#ifndef WIN32
#include <string.h>
//#define strncpy strcpy
#endif

MTAEXPORT bool InitModule ( ILuaModuleManager *pManager, char *szModuleName, char *szAuthor, float *fVersion )
{
	pModuleManager = pManager;
	strncpy(szModuleName, MODULE_NAME, MAX_INFO_LENGTH);
	strncpy(szAuthor, MODULE_AUTHOR, MAX_INFO_LENGTH);
	(*fVersion) = MODULE_VERSION;

	return true;
}


MTAEXPORT void RegisterFunctions ( lua_State * luaVM )
{
	if ( pModuleManager && luaVM )
	{
		// Functions
		pModuleManager->RegisterFunction ( luaVM, "ircConnect", CFunctions::ircConnect );
		pModuleManager->RegisterFunction ( luaVM, "ircDisconnect", CFunctions::ircDisconnect );
		pModuleManager->RegisterFunction ( luaVM, "ircJoin", CFunctions::ircJoin );
		pModuleManager->RegisterFunction ( luaVM, "ircRaw", CFunctions::ircRaw );
		pModuleManager->RegisterFunction ( luaVM, "ircPart", CFunctions::ircPart );
		pModuleManager->RegisterFunction ( luaVM, "ircSay", CFunctions::ircSay );
		pModuleManager->RegisterFunction ( luaVM, "ircChangeNick", CFunctions::ircChangeNick );
		pModuleManager->RegisterFunction ( luaVM, "ircShowDebug", CFunctions::ircShowDebug );
		pModuleManager->RegisterFunction ( luaVM, "ircSetMode", CFunctions::ircSetMode );
		pModuleManager->RegisterFunction ( luaVM, "ircSetChannelMode", CFunctions::ircSetChannelMode );
		//pModuleManager->RegisterFunction ( luaVM, "ircIsConnected", CFunctions::ircIsConnected );

		// Events
		CFunctions::AddEvent( luaVM, "onIRCRaw" );
	}
}


MTAEXPORT bool DoPulse ( void )
{
	return true;
}

MTAEXPORT bool ShutdownModule ( void )
{
	CFunctions::sendRaw("QUIT :Module unload.");
	CFunctions::sendConsole("Shutting down..");
	return true;
}
