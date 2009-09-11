/*********************************************************
*
*  Multi Theft Auto v1.0 - Module
*  
*  Module:       ml_irc
*  Url:          http://development.mtasa.com/index.php?title=Modules/SebasIRC
*  Project page: http://multitheftauto-modules.googlecode.com/
*  Developers:   Sebas <sebasdevelopment@gmx.com>
*
*********************************************************/

#include "ml_irc.h"
#include "string.h"
#include "CIrc.h"

ILuaModuleManager10 *pModuleManager = NULL;

MTAEXPORT bool InitModule(ILuaModuleManager10 *pManager, char *szModuleName, char *szAuthor, float *fVersion)
{
	pModuleManager = pManager;
	strncpy(szModuleName, MODULE_NAME, MAX_INFO_LENGTH);
	strncpy(szAuthor, MODULE_AUTHOR, MAX_INFO_LENGTH);
	(*fVersion) = MODULE_VERSION;

	return true;
}


MTAEXPORT void RegisterFunctions(lua_State * luaVM)
{
	if(pModuleManager && luaVM)
	{
		// Functions
		pModuleManager->RegisterFunction(luaVM, "ircConnect", CFunctions::ircConnect);
		pModuleManager->RegisterFunction(luaVM, "ircDisconnect", CFunctions::ircDisconnect);
		pModuleManager->RegisterFunction(luaVM, "ircJoin", CFunctions::ircJoin);
		pModuleManager->RegisterFunction(luaVM, "ircRaw", CFunctions::ircRaw);
		pModuleManager->RegisterFunction(luaVM, "ircPart", CFunctions::ircPart);
		pModuleManager->RegisterFunction(luaVM, "ircSay", CFunctions::ircSay);
		pModuleManager->RegisterFunction(luaVM, "ircNotice", CFunctions::ircNotice);
		//pModuleManager->RegisterFunction(luaVM, "ircChangeNick", CFunctions::ircChangeNick);
		pModuleManager->RegisterFunction(luaVM, "ircSetMode", CFunctions::ircSetMode);
		//pModuleManager->RegisterFunction(luaVM, "ircSetChannelMode", CFunctions::ircSetChannelMode);
		//pModuleManager->RegisterFunction(luaVM, "ircIsConnected", CFunctions::ircIsConnected);

		// Events
		CFunctions::AddEvent(luaVM, "onIRCRaw");
	}
}


MTAEXPORT bool DoPulse(void)
{
	return true;
}

MTAEXPORT bool ShutdownModule(void)
{
	CIrc::sendRaw("QUIT :Module unload.");
	CIrc::CloseSocket();
	return true;
}
